#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml2/include/libxml/xmlmemory.h>
#include <libxml2/include/libxml/parser.h>

//o array que vai receber a data
char array[8][50];

static void parse(const char *nomedoc)
{
    xmlDocPtr data = xmlParseFile(nomedoc);
    xmlNodePtr cur = xmlDocGetRootElement(data);
    xmlChar *conteudo;

    if (cur == NULL) {
        printf("documento vazio \n");
        xmlFreeDoc(data);
        return;
    }

        if (data == NULL) {
        printf("O parse nao foi feito \n");
        return;
    }

    int i=0;
    for (cur = cur->children; cur != NULL; cur = cur->next) {
        if (cur->type != XML_ELEMENT_NODE) continue;
        conteudo = xmlNodeGetContent(cur);
        strncpy(array[i], conteudo, 50);
        i++;
        printf("Conteudo lido de <page>: %s\n", conteudo);
        xmlFree(conteudo);
    }
    xmlFreeDoc(data);
}

int main(int argc, char *argv[]){
	parse("datateste");
}
