// --- In op_del.c ---
#include <stdio.h>    // Standard library for things like printing messages.
#include "db_ops.h"   // Our custom function declarations are in here.
#include <bson/bson.h> // We need the BSON library to work with MongoDB's data format and check the server's reply.

// This function removes a document from the collection.
// It's set up to require both the key and the value now, so we only delete
// the specific duplicate the user meant to target.
void op_del(mongoc_collection_t *collection, const char *key, const char *value) { 
    bson_error_t error;     // Holds any error info if the MongoDB driver fails.
    bson_t reply;           // This will receive the full BSON response from the server.
    bson_iter_t iter;       // A tool we use to look through the fields inside the 'reply' BSON document.
    int32_t deleted_count = 0; // Stores the number of documents the server actually removed.
    
    // --- Building the Strict Query ---
    // We construct a BSON document that serves as our strict filter.
    // The query must match the 'key' AND the 'value' to find a document.
    // Example BSON filter: { "key": "USER", "value": "admin1" }
    bson_t *query = BCON_NEW(
        "key", BCON_UTF8(key), // Match the document's 'key' field to our input key.
        "value", BCON_UTF8(value) // Match the document's 'value' field to our input value.
    );
    
    // Execute the delete operation. We pass '&reply' to get statistics back from the server.
    if (!mongoc_collection_delete_one(collection, query, NULL, &reply, &error)) {
        // If the driver itself runs into a critical error (connection issues, etc.).
        printf("[ERROR] Delete command failed: %s\n", error.message);
    } else {
        // The command ran successfully, now we need to verify the result count.
        
        // --- Inspecting the Reply ---
        // Look for the "deletedCount" field within the server's reply document.
        if (bson_iter_init_find_case(&iter, &reply, "deletedCount")) {
            // Extract the integer value of the count.
            deleted_count = bson_iter_int32(&iter);
        }
        
        // Check if the actual count is greater than zero.
        if (deleted_count > 0) {
            // Success: one or more documents were removed.
            printf("Deleted: { %s: %s }\n", key, value); 
        } else {
            // Failure: zero documents matched the key and value filter.
            printf("[INFO] No matching entry found for { %s: %s }.\n", key, value);
        }
    }
    
    // Cleanup: destroy BSON documents to prevent memory leaks.
    bson_destroy(query);
    bson_destroy(&reply);
}