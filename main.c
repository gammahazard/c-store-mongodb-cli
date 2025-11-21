#include <stdio.h>
#include <string.h>
#include <mongoc/mongoc.h> // The official MongoDB C Driver library
#include "db_ops.h"        // Our custom database operation declarations

// This function just prints a nice startup message and the available commands 
void print_banner(const char *uri) {
    printf("\n==================================================\n");
    printf("  C-STORE (MongoDB CLI Implementation)            \n");
    printf("==================================================\n");
    printf("Status: Connected to %s\n", uri); // Show the user what URI we connected to
    printf("--------------------------------------------------\n");
    printf(" AVAILABLE COMMANDS:\n");
    printf("  1. SET <key> <value>   -> Save data\n");
    // *** UPDATED USAGE MESSAGE for DEL ***
    printf("  2. GET <key>           -> Retrieve data\n");
    printf("  3. DEL <key> <value>   -> Delete specific key/value pair\n");
    printf("  4. ALL                 -> List everything\n");
    printf("  5. EXIT                -> Close program\n");
    printf("--------------------------------------------------\n\n");
}

int main() {
    mongoc_client_t *client; // Pointer for our main MongoDB client connection
    mongoc_collection_t *collection; // Pointer for the specific collection we're working with
    char input_line[200]; // A buffer to store the command line input
    bson_error_t error; // A struct to catch any errors from the MongoDB C driver

    // 1. INITIALIZATION AND CONNECTION SETUP
    const char *uri_string = "mongodb://localhost:27017"; // Standard local MongoDB address
    mongoc_init(); // Crucial: Initialize the MongoDB C driver
    client = mongoc_client_new(uri_string); // Attempt to connect
    
    // Safety check: if client is NULL, we couldn't even create the client object
    if (!client) return 1;

    // CONNECTION CHECK
    // Send a simple 'ping' command to the 'admin' database to confirm the server is running
    bson_t *ping = BCON_NEW("ping", BCON_INT32(1)); 
    bson_t reply;
    if (!mongoc_client_command_simple(client, "admin", ping, NULL, &reply, &error)) {
        // If the command fails, the server is likely down
        printf("\n[CRITICAL ERROR] Connection Failed!\n");
        printf("Reason: %s\n", error.message);
        printf("TIP: Run 'sudo systemctl start mongod'\n\n"); // Helpful tip for the user
        return 1; // Exit with an error code
    }
    bson_destroy(ping);
    bson_destroy(&reply);
    
    // Get a handle to our working collection: database 'c_portfolio', collection 'demo'
    collection = mongoc_client_get_collection(client, "c_portfolio", "demo");
    print_banner(uri_string); // Show the user the main menu

    // 4. MAIN INTERACTIVE LOOP
    while (1) {
        printf("c-store> "); // The command prompt
        
        // Read input line from the console (stdin)
        if (fgets(input_line, sizeof(input_line), stdin) == NULL) break; 
        
        // Remove the trailing newline character that fgets leaves
        input_line[strcspn(input_line, "\n")] = 0; 
        
        // Skip to the next prompt if the user just pressed Enter
        if (strlen(input_line) == 0) continue; 

        // Get the first token (the command)
        char *command = strtok(input_line, " "); 
        if (command == NULL) continue; 

        // --- COMMAND ROUTING ---
        
        if (strcmp(command, "SET") == 0) {
            char *key = strtok(NULL, " "); // Get the key
            char *value = strtok(NULL, " "); // Get the value
            if (key && value) {
                op_set(collection, key, value); // Call the SET database operation
            } else {
                printf("[!] usage: SET <key> <value>\n"); // Error if key/value is missing
            }
        } 
        else if (strcmp(command, "GET") == 0) {
            char *key = strtok(NULL, " "); // Get the key
            if (key) op_get(collection, key); // Call the GET database operation
            else printf("[!] usage: GET <key>\n"); // Error if key is missing
        }
        
        // --- STRICT DELETE (Requires key AND value) ---
        else if (strcmp(command, "DEL") == 0) {
            char *key = strtok(NULL, " ");   // Get the key
            char *value = strtok(NULL, " "); // Get the value
            
            // Check if both key AND value are present on the same line
            if (key && value) {
                // Pass BOTH the key and the value to the op_del function
                op_del(collection, key, value); 
            } else {
                // If either is missing, show the strict usage rule
                printf("[!] usage: DEL <key> <value>\n"); 
            }
        }
        else if (strcmp(command, "ALL") == 0) {
            op_all(collection); // List everything in the collection
        }
        else if (strcmp(command, "HELP") == 0) {
            print_banner(uri_string); // Re-print the help menu
        }
        else if (strcmp(command, "EXIT") == 0) {
            printf("goodbye!\n");
            break; // Exit the main loop
        }
        else {
            printf("[!] unknown command.\n"); // Handle any unrecognized input
        }
    }

    // --- CLEANUP ---
    // Always free resources associated with the MongoDB driver
    mongoc_collection_destroy(collection); 
    mongoc_client_destroy(client); 
    mongoc_cleanup(); // Clean up the global driver environment

    return 0; 
}