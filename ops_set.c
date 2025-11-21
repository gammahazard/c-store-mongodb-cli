#include <stdio.h>
#include "db_ops.h"

void op_set(mongoc_collection_t *collection, const char *key, const char *value) {
    bson_error_t error;
    
    // create the document { key: value }
    bson_t *doc = BCON_NEW(
        "key", BCON_UTF8(key),
        "value", BCON_UTF8(value)
    );

    if (!mongoc_collection_insert_one(collection, doc, NULL, NULL, &error)) {
        printf("error: %s\n", error.message);
    } else {
        printf("saved: { %s: %s }\n", key, value);
    }
    bson_destroy(doc);
}