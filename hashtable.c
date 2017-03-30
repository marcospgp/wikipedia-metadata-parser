#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

#include "hashtable.h"

// TODO - guardar char *title ao receber antes de ser apagado

TAD_istruct initHashtables(TAD_istruct qs) {

    GHashTable *users = g_hash_table_new(NULL, NULL);
    GHashTable *articles = g_hash_table_new(NULL, NULL);
}
