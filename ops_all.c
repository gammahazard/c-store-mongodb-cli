#include <stdio.h> // gotta include this for printf
#include "db_ops.h" // and the header with the function declarations

// this function just prints every single document in the collection, no filter
void op_all(mongoc_collection_t *collection) {
    // --- Setup the Query ---
    // we need a query BSON object, but since we want *everything*, we just make an empty one {}.
    bson_t *query = bson_new();
    
    // --- Execute the Find ---
    // we tell the driver to find everything that matches that empty query.
    // the result comes back as a cursor, which we can iterate over.
    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);
    // this pointer will point to the current document as we loop through the cursor
    const bson_t *doc;

    // --- Loop and Print Results ---
    printf("--- all entries ---\n");
    // mongoc_cursor_next moves to the next document; the loop stops when there are no more docs left.
    while (mongoc_cursor_next(cursor, &doc)) {
        // documents are stored as BSON (Binary JSON), so we convert it into a human-readable JSON string.
        char *str = bson_as_canonical_extended_json(doc, NULL);
        
        // print the resulting string (the whole document)
        printf("%s\n", str);
        
        // bson_as_canonical_extended_json allocated memory for 'str', so we gotta free it right away.
        bson_free(str);
    }
    printf("-------------------\n");

    // --- Cleanup ---
    // always clean up the memory used by the query BSON object
    bson_destroy(query);
    // and destroy the cursor, which frees up resources associated with the database connection
    mongoc_cursor_destroy(cursor);
}