#include <stdio.h> // gotta have this for printing messages
#include "db_ops.h" // custom header for function declarations

// this function saves data. it's smart because it updates existing keys 
// instead of creating duplicates, which is called an upsert.
void op_set(mongoc_collection_t *collection, const char *key, const char *value) {
    bson_error_t error; // the standard place to catch errors from the MongoDB driver
    
    // --- Step 1: Define the Query (The Filter) ---
    // we build a BSON filter to tell MongoDB which document we're looking for.
    // this query is simple: "find the document where the 'key' field equals our input key"
    bson_t *query = BCON_NEW("key", BCON_UTF8(key));

    // --- Step 2: Define the Update (The Change) ---
    // we build another BSON document specifying *what* change to make.
    // the "$set" operator means "update or set these fields."
    // we only want to set the 'value' field to the new value.
    bson_t *update = BCON_NEW("$set", "{", 
        "value", BCON_UTF8(value)
    , "}");

    // --- Step 3: Set the Options (The Magic) ---
    // this BSON document holds extra options for the operation.
    // setting "upsert" to true is the key: 
    // - if the 'query' finds the document, it updates it.
    // - if the 'query' finds nothing, it inserts a new document using the key/value pair.
    bson_t *opts = BCON_NEW(
        "upsert", BCON_BOOL(true) 
    );

    // --- Execute the Upsert ---
    // we use update_one because it's the only way to do upserting. 
    // it tries to update the document found by 'query' with the changes in 'update'.
    if (!mongoc_collection_update_one(collection, query, update, opts, NULL, &error)) {
        // if the command fails entirely (connection, bad syntax, etc.)
        printf("error: %s\n", error.message);
    } else {
        // if it works, it either saved a new key or updated an old one
        printf("saved/updated: { %s: %s }\n", key, value);
    }
    
    // --- Cleanup ---
    // always gotta clean up the BSON documents created locally
    bson_destroy(query);
    bson_destroy(update);
    bson_destroy(opts);
}