#include <stdio.h>
#include "db_ops.h"

void op_del(mongoc_collection_t *collection, const char *key) {
    bson_error_t error;
    bson_t *query = BCON_NEW("key", BCON_UTF8(key));
    
    if (!mongoc_collection_delete_one(collection, query, NULL, NULL, &error)) {
        printf("delete failed: %s\n", error.message);
    } else {
        printf("deleted %s\n", key);
    }
    bson_destroy(query);
}