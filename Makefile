# Name of the program you want to build
TARGET = mongo_store

# The compiler
CC = gcc

# The flags (includes your pkg-config)
CFLAGS = -Wall $(shell pkg-config --cflags libmongoc-1.0)
LIBS = $(shell pkg-config --libs libmongoc-1.0)

# The list of files (your command inputs)
SRCS = main.c ops_set.c ops_get.c ops_del.c ops_all.c

# The Build Rule
# This effectively runs: gcc -o mongo_store main.c ... $(pkg-config ...)
all:
	$(CC) -o $(TARGET) $(SRCS) $(CFLAGS) $(LIBS)