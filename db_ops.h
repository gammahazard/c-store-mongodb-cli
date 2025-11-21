#ifndef DB_OPS_H
#define DB_OPS_H

#include <mongoc/mongoc.h>

// telling main.c that these functions exist somewhere else
// so it doesnt crash when compiling
void op_set(mongoc_collection_t *collection, const char *key, const char *value);
void op_get(mongoc_collection_t *collection, const char *key);
void op_del(mongoc_collection_t *collection, const char *key);
void op_all(mongoc_collection_t *collection);

#endif