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

static TAD_istruct parsePage(TAD_istruct qs, xmlDocPtr doc, xmlNodePtr cur) {

	char *title, *revisionContributorUsername = NULL, *revisionText = NULL, *revisionTimestamp = NULL;
	long articleId, revisionId = -1, revisionParentId = -1, revisionContributorId = -1;

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
