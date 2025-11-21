# C-Store: Modular MongoDB CLI

A lightweight, modular Command Line Interface (CLI) for MongoDB written in C. 
This project demonstrates low-level system programming concepts including manual memory management, microservice-style modularity, and BSON serialization.

## 🚀 Features

* **Modular Architecture**: Logic is split into separate operation modules (`ops_*.c`) rather than a monolithic file.
* **Live Database Integration**: Connects to a local MongoDB instance using the official C Driver (`libmongoc`).
* **Robust Error Handling**: Includes connection health checks on startup.
* **Interactive Shell**: A custom REPL (Read-Eval-Print Loop) to interact with the database.

## 🛠️ Prerequisites

To build this project, you need a Linux environment (WSL2 recommended) with the following installed:

1.  **GCC & Build Tools**
    ```bash
    sudo apt update
    sudo apt install build-essential pkg-config
    ```

2.  **MongoDB C Driver**
    ```bash
    sudo apt install libmongoc-dev libbson-dev
    ```

3.  **MongoDB Server**
    ```bash
    sudo apt install mongodb-org
    sudo systemctl start mongod
    ```

## ⚙️ Compilation

This project uses `pkg-config` to manage library linking. Run the following command to compile the interface and all modules:

```bash
gcc -o mongo_store main.c ops_set.c ops_get.c ops_del.c ops_all.c $(pkg-config --libs --cflags libmongoc-1.0)
```
## 🏃 Usage

1.  **Start the Database** (if not running):
    ```bash
    sudo systemctl start mongod
    ```

2.  **Run the Tool**:
    ```bash
    ./mongo_store
    ```

3.  **Commands**:
    ```text
    local-db> SET user gammahazard
    saved: { user: gammahazard }

    local-db> GET user
    { ... "key": "user", "value": "gammahazard" }

    local-db> DEL user
    deleted user

    local-db> ALL
    (Lists all entries)
    ```

## 📂 File Structure

* `main.c` - The Entry Point (UI & Routing Logic).
* `db_ops.h` - Header file defining the function prototypes.
* `ops_set.c` - Implementation for inserting data.
* `ops_get.c` - Implementation for querying data.
* `ops_del.c` - Implementation for deleting data.
* `ops_all.c` - Implementation for listing collections.

## 🔧 Troubleshooting

**Error: Connection Failed!**
If you see the "Connection Failed" banner upon startup:
1.  Check if MongoDB is running: `sudo systemctl status mongod`
2.  If it is stopped, run: `sudo systemctl start mongod`

**Error: db_ops.h: No such file or directory**
Ensure all files are in the same directory and you are running the `gcc` command from inside that folder.