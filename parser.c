#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include "articles.h"
#include "users.h"

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

static TAD_istruct parsePage(TAD_istruct qs, xmlDocPtr doc, xmlNodePtr cur) {

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

		TODO - Ignorar contributor caso apenas tenha <ip> e não <username> + <id>

		TODO - Trocar os nodegetcontent pela função usada no tutorial

		TODO - ADCIONAR IF PARA VERIFICAR SE VALORES NÃO HÁ NULLS OU -1, para passar ao módulo ARTICLES
	Nota: Em algumas entradas dos backups, o campo <contributor> apenas contém a tag <ip>.
	Nestes casos, deve-se ignorar este autor para os resultados da interrogação.
	*/

	//printf("parser.c - Sending page data to users.c\n");

	qs = onPageUsers(qs, revisionContributorId, revisionContributorUsername, articleId, revisionId);

	/*
		-> no módulo, vai ter de chamar a função (da HASH) de procura por ID
			-> se encontrar, chama a função (da HASH) que aumenta o numOfContributions
			-> se não encontrar, chama a função (da HASH) que insere o ID+username+counter=1
	*/

	//printf("parser.c - Sending page data to articles.c\n");

	qs = onPageArticles(qs, articleId, title, revisionText, revisionId, revisionParentId, revisionTimestamp);

	/*
		-> chama a função de procura (da HASH) ID do artigo
			-> se não encontrar, chama a func insere (da HASH) articleID+title+
																sizeOfArticle(revisionText) (função do modulo ARTICLES)+
																numOfWords(revisionText) (funcao do modulo ARTICLES)+
																revisionId
			-> se encontrar, tem de fazer update do título
	*/

	/*
	//printf("Page:\n");
	//printf("title: %s\n", title);
	//printf("articleId: %ld\n", articleId);
	//printf("revisionId: %ld\n", revisionId);
	//printf("revisionParentId: %ld\n", revisionParentId);
	//printf("revisionTimestamp: %s\n", revisionTimestamp);
	//printf("revisionContributorId: %ld\n", revisionContributorId);
	//printf("revisionContributorUsername: %s\n", revisionContributorUsername);
	//printf("revisionText: %s\n", revisionText);
	//printf("--------------------\n");
	*/

	return qs;

	xmlFree(title);
	xmlFree(revisionTimestamp);
	xmlFree(revisionContributorUsername);
	xmlFree(revisionText);
}

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

	// Loop principal - itera pelas páginas

	cur = cur->children;

	while (cur != NULL) {

		if (!xmlStrcmp(cur->name, (const xmlChar *)"page")) {

			qs = parsePage(qs, doc, cur->children);
		}

		cur = cur->next;
	}

	return qs;

	// Libertar a memória
	xmlFreeDoc(doc);
}
