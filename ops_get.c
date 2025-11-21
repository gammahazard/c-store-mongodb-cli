#include <stdio.h> // standard C stuff for printing output
#include "db_ops.h" // gotta include our header file

// this function looks up documents based on a key and prints whatever it finds
void op_get(mongoc_collection_t *collection, const char *key) {
    // --- Build the Query ---
    // we build the BSON query document that tells MongoDB what to look for.
    // the query looks like: { "key": "whatever_the_user_typed" }
    bson_t *query = BCON_NEW("key", BCON_UTF8(key));
    
    // --- Execute the Find Operation ---
    // we tell the driver to find documents matching our query. 
    // the result isn't the data itself yet, it's a pointer to a cursor.
    mongoc_cursor_t *cursor = mongoc_collection_find_with_opts(collection, query, NULL, NULL);
    
    // this pointer will temporarily hold the document found by the cursor
    const bson_t *doc;
    
    // a simple flag to track if we actually found anything or not
    int found = 0;

    // --- Loop Through Results ---
    // mongoc_cursor_next moves the cursor to the next document; the loop runs as long as it finds docs.
    while (mongoc_cursor_next(cursor, &doc)) {
        // the doc is in BSON format, so we convert it into a readable JSON string for printing.
        char *str = bson_as_canonical_extended_json(doc, NULL);
        
        // print the resulting document string
        printf("%s\n", str);
        
        // we have to free the memory that bson_as_canonical_extended_json allocated for 'str'
        bson_free(str);
        
        // mark that we definitely found at least one result
        found = 1;
    }

    // --- Report Missing Key ---
    // if the 'found' flag is still 0 after the loop, nothing matched the query
    if (!found) printf("key not found dude\n");

    // --- Cleanup ---
    // always clean up the BSON query document memory
    bson_destroy(query);
    // and destroy the cursor
    mongoc_cursor_destroy(cursor);
}