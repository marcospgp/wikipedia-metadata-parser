#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include "parser.h"

void parsePage(xmlDocPtr doc, xmlNodePtr cur) {

	printf("Parsing a page! Cur: %s\n",  cur->name);

	cur = cur->children;

	while(cur != NULL) {

		if (cur->type == XML_ELEMENT_NODE) {

			printf("%s\n", cur->name);

			// Elemento "revision"
			if (!xmlStrcmp(cur->name, (const xmlChar *) "revision")) {
				parsePage(doc, cur);
			}
		}

		cur = cur->next;
	}
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

		if ((!xmlStrcmp(cur->name, (const xmlChar *)"page"))){

			parsePage(doc, cur);
		}

		cur = cur->next;
	}

	// Libertar a memória
	xmlFreeDoc(doc);
}
