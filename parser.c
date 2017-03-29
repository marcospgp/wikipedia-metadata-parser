#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/tree.h>
#include <libxml/parser.h>
#include "parser.h"

void parseRevision(xmlDocPtr doc, xmlNodePtr cur) {

	while(cur != NULL) {

		if (cur->type == XML_ELEMENT_NODE) {

			if (!xmlStrcmp(cur->name, (const xmlChar *) "revision")) {
				parseRevision(doc, cur->children);
			}
		}

		cur = cur->next;
	}
}

void parsePage(xmlDocPtr doc, xmlNodePtr cur) {

	char *title, *contributorName, *revisionText;
	long articleId, revisionId, revisionParentId, revisionContributorId;


	while(cur != NULL) {

		if (cur->type == XML_ELEMENT_NODE) {

			// Title
			if (!xmlStrcmp(cur->name, (const xmlChar *) "title")) {
				title = (char*) cur->content;
			}

			// Revision
			if (!xmlStrcmp(cur->name, (const xmlChar *) "revision")) {
				parseRevision(doc, cur->children);
			}

			// Revision
			if (!xmlStrcmp(cur->name, (const xmlChar *) "revision")) {
				parseRevision(doc, cur->children);
			}
		}

		cur = cur->next;
	}
}

void parse(const char *docname) {

	xmlDocPtr doc;
	xmlNodePtr cur;

	doc = xmlReadFile(docname, NULL);

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
