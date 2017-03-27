#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include "parser.h"

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

	// Loop principal

	cur = cur->children;

	while (cur != null) {

		if ((!xmlStrcmp(cur->name, (const xmlChar *)"storyinfo"))){
			parseStory (doc, cur);
		}

		cur = cur->next;
	}

	// Libertar a memória
	xmlFreeDoc(doc);
}

/*
//o array que vai receber os dados
// o array tem um tamanho limitado, vai ser imprevisivel o comportamento após 50 caracteres
static char array[20][50];

void parse(const char *nomedoc) {

	xmlDocPtr data = xmlParseFile(nomedoc);
	xmlNodePtr cur = xmlDocGetRootElement(data);
	xmlChar *conteudo;

	if (data == NULL) {
		printf("O parse nao foi feito \n");
		return;
	}

	if (cur == NULL) {
		printf("documento vazio \n");
		xmlFreeDoc(data);
		return;
	}

	int i = 0;

	for (cur = cur->children; cur != NULL; cur = cur->next) {
		if (cur->type != XML_ELEMENT_NODE) continue;
		conteudo = xmlNodeGetContent(cur);
		strcpy(array[i], conteudo);
		printf("Conteudo no array na posição %d de <page>: %s\n", i, array[i]);
		i++;
		// printf("Conteudo lido de <page>: %s\n", conteudo);
		xmlFree(conteudo);
	}

	xmlFreeDoc(data);
}
*/
