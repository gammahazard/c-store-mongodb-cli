#include <stdio.h>
#include "db_ops.h"

void op_get(mongoc_collection_t *collection, const char *key) {
    // query = { key: "user_input" }
    bson_t *query = BCON_NEW("key", BCON_UTF8(key));
    
    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);
    const bson_t *doc;
    int found = 0;

    while (mongoc_cursor_next(cursor, &doc)) {
        char *str = bson_as_canonical_extended_json(doc, NULL);
        printf("%s\n", str);
        bson_free(str);
        found = 1;
    }

    if (!found) printf("key not found dude\n");

    bson_destroy(query);
    mongoc_cursor_destroy(cursor);
}