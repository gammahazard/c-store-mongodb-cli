#include <stdio.h>
#include "db_ops.h"

void op_all(mongoc_collection_t *collection) {
    // empty query {} matches everything
    bson_t *query = bson_new();
    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);
    const bson_t *doc;

    printf("--- all entries ---\n");
    while (mongoc_cursor_next(cursor, &doc)) {
        char *str = bson_as_canonical_extended_json(doc, NULL);
        printf("%s\n", str);
        bson_free(str);
    }
    printf("-------------------\n");

    bson_destroy(query);
    mongoc_cursor_destroy(cursor);
}