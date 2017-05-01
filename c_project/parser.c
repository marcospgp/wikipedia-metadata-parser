#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include "articles.h"
#include "users.h"

#include "parser.h"

/**
 * @brief Função que faz o parse das revisions do XML.
 *
 * Verifica os campos id, parentid, timestamp, text e contributor (e neste, username e id),
 * e atualiza os apontadores que recebeu de parsePage.
 *
 * @see parsePage()
 *
 * @param doc O documento.
 * @param cur O local atual do documento.
 * @param revisionId O apontador para o ID da revisao.
 * @param revisionParentId O apontador para o ID da revisao pai.
 * @param revisionTimestamp O apontador para a timestamp da revisao.
 * @param revisionContributorId O apontador para o ID do contribuidor da revisao.
 * @param revisionContributorUsername O apontador para o username do contribuidor.
 */
static void parseRevision(
	xmlDocPtr doc,
	xmlNodePtr cur,
	long *revisionId,
	long *revisionParentId,
	char **revisionTimestamp,
	long *revisionContributorId,
	char **revisionContributorUsername,
	char **revisionText
) {

	char *temp, *longEndPtr; // Necessário para strtol().

	while (cur != NULL) {

		if (cur->type == XML_ELEMENT_NODE) {

			// Revision ID.
			if (!xmlStrcmp(cur->name, (const xmlChar *) "id")) {

				temp = (char*) xmlNodeGetContent(cur);
				*revisionId = strtol(temp, &longEndPtr, 10);
				xmlFree(temp);
			}

			// Revision Parent ID.
			if (!xmlStrcmp(cur->name, (const xmlChar *) "parentid")) {

				temp = (char*) xmlNodeGetContent(cur);
				*revisionParentId = strtol(temp, &longEndPtr, 10);
				xmlFree(temp);
			}

			// Revision Timestamp.
			if (!xmlStrcmp(cur->name, (const xmlChar *) "timestamp")) {
				*revisionTimestamp = (char*) xmlNodeGetContent(cur);
			}

			// Revision Text.
			if (!xmlStrcmp(cur->name, (const xmlChar *) "text")) {
				*revisionText = (char*) xmlNodeGetContent(cur);
			}

			// Revision Contributor.
			if (!xmlStrcmp(cur->name, (const xmlChar *) "contributor")) {

				xmlNodePtr current = cur->children;

				while (current != NULL) {

					if (current->type == XML_ELEMENT_NODE) {

						// Username.
						if (!xmlStrcmp(current->name, (const xmlChar *) "username")) {
							*revisionContributorUsername = (char*) xmlNodeGetContent(current);
						}

						// ID.
						if (!xmlStrcmp(current->name, (const xmlChar *) "id")) {
							temp = (char*) xmlNodeGetContent(current);
							*revisionContributorId = strtol(temp, &longEndPtr, 10);
							xmlFree(temp);
						}
					}

					current = current->next;
				}
			}
		}

		cur = cur->next;
	}
}

/**
 * @brief Função que faz o parse das pages do XML.
 *
 * Verifica os campos title, id, revision e chama a parseRevision.
 *
 * @see parseRevision()
 *
 * @param qs A estrutura geral do programa.
 * @param doc O documento.
 * @param cur O local atual do documento.
 * @return A estrutura atualizada.
 */
static TAD_istruct parsePage(TAD_istruct qs, xmlDocPtr doc, xmlNodePtr cur) {

	char *title = NULL, *revisionContributorUsername = NULL, *revisionText = NULL, *revisionTimestamp = NULL;
	long articleId = -1, revisionId = -1, revisionParentId = -1, revisionContributorId = -1;

	char *temp, *longEndPtr; // Necessário para strtol().

	while (cur != NULL) {

		if (cur->type == XML_ELEMENT_NODE) {

			// Title.
			if (!xmlStrcmp(cur->name, (const xmlChar *) "title")) {
				title = (char*) xmlNodeGetContent(cur);
			}

			// ID.
			if (!xmlStrcmp(cur->name, (const xmlChar *) "id")) {

				temp = (char*) xmlNodeGetContent(cur);
				articleId = strtol(temp, &longEndPtr, 10);
				xmlFree(temp);
			}

			// Revision.
			if (!xmlStrcmp(cur->name, (const xmlChar *) "revision")) {
				parseRevision(
					doc,
					cur->children,
					&revisionId,
					&revisionParentId,
					&revisionTimestamp,
					&revisionContributorId,
					&revisionContributorUsername,
					&revisionText
				);
			}
		}

		cur = cur->next;
	}


	// Ignorar contributor caso apenas tenha <ip> e não <username> + <id>
	if (revisionContributorId != -1) {
		// Funçaõ responsável por organizar e passar dados. Presente em users.c.
		qs = onPageUsers(qs, revisionContributorId, revisionContributorUsername, articleId, revisionId);
	}

	// Função responsável por organizar e passar dados. Presente em articles.c.
	qs = onPageArticles(qs, articleId, title, revisionText, revisionId, revisionParentId, revisionTimestamp);


	// Libertar memória.
	xmlFree(title);
	xmlFree(revisionTimestamp);
	xmlFree(revisionContributorUsername);
	xmlFree(revisionText);


	return qs;
}

/**
 * @brief Função que carrega e distribui as pages.
 *
 * Verifica o correto carregamento do ficheiro e obtém a root,
 * percorrendo após isso as pages e dando-as à funão parsePage para esta recolher dados.
 *
 * @see parsePage()
 *
 * @param qs A estrutura geral do programa.
 * @param docname O nome do documento a carregar.
 * @return A estrutura carregada.
 */
TAD_istruct parseWikiData(TAD_istruct qs, const char *docname) {

	xmlDocPtr doc;
	xmlNodePtr cur;

	doc = xmlParseFile(docname);

	if (doc == NULL ) {
		fprintf(stderr,"Document not parsed successfully. \n");
		return NULL;
	}

	cur = xmlDocGetRootElement(doc);

	if (cur == NULL) {
		fprintf(stderr,"Empty document\n");
		xmlFreeDoc(doc);
		return NULL;
	}

	if (xmlStrcmp(cur->name, (const xmlChar *) "mediawiki")) {
		fprintf(stderr,"Document of the wrong type, root node != mediawiki");
		xmlFreeDoc(doc);
		return NULL;
	}

	// Loop principal - itera pelas páginas.

	cur = cur->children;

	while (cur != NULL) {

		if (!xmlStrcmp(cur->name, (const xmlChar *)"page")) {

			qs = parsePage(qs, doc, cur->children);
		}

		cur = cur->next;
	}

	return qs;

	// Libertar a memória.
	xmlFreeDoc(doc);
}
