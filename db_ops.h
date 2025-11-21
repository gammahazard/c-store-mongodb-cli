#ifndef DB_OPS_H
// This is called an include guard. It checks if DB_OPS_H hasn't been defined yet.
// If it hasn't, the compiler includes the code below.
#define DB_OPS_H

// If the code above was included, this line defines DB_OPS_H.
// This prevents the compiler from processing this file more than once if it's
// accidentally included multiple times in the project (which prevents errors).

#include <mongoc/mongoc.h> // We need to include the MongoDB driver header here
                         // because we use mongoc_collection_t in our function prototypes.

// The prototypes below tell the compiler: "Hey, these functions exist, 
// they're defined in a different .c file, but they will be available later when linking."

// telling main.c that these functions exist somewhere else
// so it doesnt crash when compiling

// Prototype for the SET operation: takes the collection handle, a key, and a value.
void op_set(mongoc_collection_t *collection, const char *key, const char *value);

// Prototype for the GET operation: takes the collection handle and the key to look up.
void op_get(mongoc_collection_t *collection, const char *key);

// op_del now requires both key AND value, was deleting full key data before
// Prototype for the DEL operation: now strictly requires the collection, key, AND value.
void op_del(mongoc_collection_t *collection, const char *key, const char *value);

// Prototype for the ALL operation: takes the collection handle and prints everything.
void op_all(mongoc_collection_t *collection);

#endif
// The #endif marks the end of the code protected by the #ifndef at the top.