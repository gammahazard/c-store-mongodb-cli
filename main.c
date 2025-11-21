#include <stdio.h>
#include <string.h>
#include <mongoc/mongoc.h>
#include "db_ops.h"

void print_banner(const char *uri) {
    printf("\n==================================================\n");
    printf("   C-STORE (MongoDB CLI Implementation)           \n");
    printf("==================================================\n");
    printf("Status: Connected to %s\n", uri);
    printf("--------------------------------------------------\n");
    printf(" AVAILABLE COMMANDS:\n");
    printf("  1. SET <key> <value>   -> Save data\n");
    printf("  2. GET <key>           -> Retrieve data\n");
    printf("  3. DEL <key>           -> Delete data\n");
    printf("  4. ALL                 -> List everything\n");
    printf("  5. EXIT                -> Close program\n");
    printf("--------------------------------------------------\n\n");
}

int main() {
    mongoc_client_t *client;
    mongoc_collection_t *collection;
    char input_line[200];
    bson_error_t error;

    // 1. CONNECT LOCAL
    const char *uri_string = "mongodb://localhost:27017";
    mongoc_init();
    client = mongoc_client_new(uri_string);
    
    if (!client) return 1;

    // CONNECTION CHECK
    bson_t *ping = BCON_NEW("ping", BCON_INT32(1));
    bson_t reply;
    if (!mongoc_client_command_simple(client, "admin", ping, NULL, &reply, &error)) {
        printf("\n[CRITICAL ERROR] Connection Failed!\n");
        printf("Reason: %s\n", error.message);
        printf("TIP: Run 'sudo systemctl start mongod'\n\n");
        return 1;
    }
    bson_destroy(ping);
    bson_destroy(&reply);

    collection = mongoc_client_get_collection(client, "c_portfolio", "demo");
    print_banner(uri_string);

    // 4. MAIN LOOP
    while (1) {
        printf("c-store> ");
        
        if (fgets(input_line, sizeof(input_line), stdin) == NULL) break;
        input_line[strcspn(input_line, "\n")] = 0;
        if (strlen(input_line) == 0) continue;

        char *command = strtok(input_line, " ");
        if (command == NULL) continue;

        // --- ROUTING ---
        
        if (strcmp(command, "SET") == 0) {
            char *key = strtok(NULL, " ");
            char *value = strtok(NULL, " ");
            if (key && value) {
                op_set(collection, key, value);
            } else {
                printf("[!] usage: SET <key> <value>\n");
            }
        } 
        else if (strcmp(command, "GET") == 0) {
            char *key = strtok(NULL, " ");
            if (key) op_get(collection, key);
            else printf("[!] usage: GET <key>\n");
        }
        // --- INTERACTIVE DELETE ---
        else if (strcmp(command, "DEL") == 0) {
            char *key = strtok(NULL, " ");
            
            // If they didn't type a key, ASK them for it
            if (key == NULL) {
                printf("  > specify key to delete: ");
                
                // Re-use the input buffer to catch the answer
                if (fgets(input_line, sizeof(input_line), stdin)) {
                    input_line[strcspn(input_line, "\n")] = 0; // clean newline
                    key = input_line; // point key to the new input
                }
            }

            // Now check if we have a key (either typed originally or answered)
            if (key && strlen(key) > 0) {
                op_del(collection, key);
            } else {
                printf("  > delete cancelled.\n");
            }
        }
        else if (strcmp(command, "ALL") == 0) {
            op_all(collection);
        }
        else if (strcmp(command, "HELP") == 0) {
            print_banner(uri_string);
        }
        else if (strcmp(command, "EXIT") == 0) {
            printf("goodbye!\n");
            break;
        }
        else {
            printf("[!] unknown command.\n");
        }
    }

    // cleanup
    mongoc_collection_destroy(collection);
    mongoc_client_destroy(client);
    mongoc_cleanup();

    return 0;
}