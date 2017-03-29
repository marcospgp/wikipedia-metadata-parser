#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include "parser.h"

void parseRevision(
	xmlDocPtr doc,
	xmlNodePtr cur,
	long *revisionId,
	long *revisionParentId,
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

void parsePage(xmlDocPtr doc, xmlNodePtr cur) {

	char *title, *revisionContributorUsername, *revisionText;
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
					&revisionContributorId,
					&revisionContributorUsername,
					&revisionText
				);
			}
		}

		cur = cur->next;
	}

	/*
	printf("Page:\n");
	printf("title: %s\n", title);
	printf("articleId: %ld\n", articleId);
	printf("revisionId: %ld\n", revisionId);
	printf("revisionParentId: %ld\n", revisionParentId);
	printf("revisionContributorId: %ld\n", revisionContributorId);
	printf("revisionContributorUsername: %s\n", revisionContributorUsername);
	printf("revisionText: %s\n", revisionText);
	printf("--------------------\n");
	*/

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
