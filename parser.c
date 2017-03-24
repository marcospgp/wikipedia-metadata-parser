#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libxml2/include/libxml/xmlmemory.h>
#include <libxml2/include/libxml/parser.h>

//o array que vai receber a data
// o array tem um tamanho limitado, vai ser imprevisivel o comportamento após 50 caracteres
char* array[20];

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
    for (cur = cur->children; (cur != NULL) && (cur->type != XML_ELEMENT_NODE); cur = cur->next, i++) {
        	conteudo = xmlNodeGetContent(cur);
        	strcpy(array[i], conteudo);
        	printf("Conteudo no array na posição %d de <page>: %s\n", i, array[i]);
        	//printf("Conteudo lido de <page>: %s\n", conteudo);
        	xmlFree(conteudo);
    }
    xmlFreeDoc(data);

}

int main(int argc, char *argv[]){
	// insere-se o nome do ficheiro à frente do ./parser
	parse(argv[1]);
}
