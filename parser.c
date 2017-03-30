#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include "parser.h"

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

	char *temp, *longEndPtr; // Necessário para strtol()

	while (cur != NULL) {

		if (cur->type == XML_ELEMENT_NODE) {

			// Revision ID
			if (!xmlStrcmp(cur->name, (const xmlChar *) "id")) {

				temp = (char*) xmlNodeGetContent(cur);
				*revisionId = strtol(temp, &longEndPtr, 10);
				xmlFree(temp);
			}

			// Revision Parent ID
			if (!xmlStrcmp(cur->name, (const xmlChar *) "parentid")) {

				temp = (char*) xmlNodeGetContent(cur);
				*revisionParentId = strtol(temp, &longEndPtr, 10);
				xmlFree(temp);
			}

			// Revision Timestamp
			if (!xmlStrcmp(cur->name, (const xmlChar *) "timestamp")) {
				*revisionTimestamp = (char*) xmlNodeGetContent(cur);
			}

			// Revision Text
			if (!xmlStrcmp(cur->name, (const xmlChar *) "text")) {
				*revisionText = (char*) xmlNodeGetContent(cur);
			}

			// Revision Contributor
			if (!xmlStrcmp(cur->name, (const xmlChar *) "contributor")) {

				xmlNodePtr current = cur->children;

				while (current != NULL) {

					if (current->type == XML_ELEMENT_NODE) {

						// Username
						if (!xmlStrcmp(current->name, (const xmlChar *) "username")) {
							*revisionContributorUsername = (char*) xmlNodeGetContent(current);
						}

						// ID
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

static void parsePage(xmlDocPtr doc, xmlNodePtr cur) {

	char *title, *revisionContributorUsername, *revisionText, *revisionTimestamp;
	long articleId, revisionId, revisionParentId, revisionContributorId;

	char *temp, *longEndPtr; // Necessário para strtol()

	while (cur != NULL) {

		if (cur->type == XML_ELEMENT_NODE) {

			// Title
			if (!xmlStrcmp(cur->name, (const xmlChar *) "title")) {
				title = (char*) xmlNodeGetContent(cur);
			}

			// ID
			if (!xmlStrcmp(cur->name, (const xmlChar *) "id")) {

				temp = (char*) xmlNodeGetContent(cur);
				articleId = strtol(temp, &longEndPtr, 10);
				xmlFree(temp);
			}

			// Revision
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

	/*
		TODO - ADCIONAR IF PARA VERIFICAR SE VALORES NÃO HÁ NULLS OU -1, para passar ao módulo ARTICLES
	Nota: Em algumas entradas dos backups, o campo <contributor> apenas contém a tag <ip>.
	Nestes casos, deve-se ignorar este autor para os resultados da interrogação.
	*/

	/*
	onPageUsers(revisionContributorId, revisionContributorUsername);
		-> no módulo, vai ter de chamar a função (da HASH) de procura por ID
			-> se encontrar, chama a função (da HASH) que aumenta o numOfContributions
			-> se não encontrar, chama a função (da HASH) que insere o ID+username+counter=1
	*/

	/*
	onPageArticles(articleId, title, revisionText, revisionId, revisionParentId);
		-> chama a função de procura (da HASH) ID do artigo
			-> se não encontrar, chama a func insere (da HASH) articleID+title+
																sizeOfArticle(revisionText) (função do modulo ARTICLES)+
																numOfWords(revisionText) (funcao do modulo ARTICLES)+
																revisionId
			-> se encontrar, tem de fazer update do título
	*/

	/*
	printf("Page:\n");
	printf("title: %s\n", title);
	printf("articleId: %ld\n", articleId);
	printf("revisionId: %ld\n", revisionId);
	printf("revisionParentId: %ld\n", revisionParentId);
	printf("revisionTimestamp: %s\n", revisionTimestamp);
	printf("revisionContributorId: %ld\n", revisionContributorId);
	printf("revisionContributorUsername: %s\n", revisionContributorUsername);
	printf("revisionText: %s\n", revisionText);
	printf("--------------------\n");
	*/

	xmlFree(title);
	xmlFree(revisionTimestamp);
	xmlFree(revisionContributorUsername);
	xmlFree(revisionText);
}

void parse(const char *docname) {

	xmlDocPtr doc;
	xmlNodePtr cur;

	doc = xmlParseFile(docname);

	if (doc == NULL ) {
		fprintf(stderr,"Document not parsed successfully. \n");
		return;
	}

	cur = xmlDocGetRootElement(doc);

	if (cur == NULL) {
		fprintf(stderr,"Empty document\n");
		xmlFreeDoc(doc);
		return;
	}

	if (xmlStrcmp(cur->name, (const xmlChar *) "mediawiki")) {
		fprintf(stderr,"Document of the wrong type, root node != mediawiki");
		xmlFreeDoc(doc);
		return;
	}

	// Loop principal - itera pelas páginas

	cur = cur->children;

	while (cur != NULL) {

		if (!xmlStrcmp(cur->name, (const xmlChar *)"page")) {

			parsePage(doc, cur->children);
		}

		cur = cur->next;
	}

	// Libertar a memória
	xmlFreeDoc(doc);
}
