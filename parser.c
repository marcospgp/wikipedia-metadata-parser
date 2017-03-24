#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml2/include/libxml/xmlmemory.h>
#include <libxml2/include/libxml/parser.h>

//o array que vai receber a data
// o array tem um tamanho limitado, vai ser imprevisivel o comportamento após 50 caracteres
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
        strcpy(array[i], conteudo);
        i++;
        //printf("Conteudo lido de <page>: %s\n", conteudo);
        xmlFree(conteudo);
    }
    xmlFreeDoc(data);
}

	// so we can print all the info atferwards
	int j;
	for (j = 0; j < i; j++) {
		printf("Conteudo lido de <page>: %s\n", array[j]);
	}

int main(int argc, char *argv[]){
	// insere-se o nome do ficheiro à frente do ./parser
	parse(argv[1]);
}
