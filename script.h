// script.h

#ifndef SCRIPT_H
#define SCRIPT_H

#include <stdio.h>
#include "sqlite3.h"
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <windows.h>
#include <stdbool.h>



typedef struct {
    char *destination_path;
    char *source_path;
} folderPair;

typedef struct{
    char* filter;
    char* type;

} filterPair;

typedef struct {
    char** source;
    int source_count;
    char* destination;
    filterPair* filters;
    int filter_count;
} entry;




// C++ linkage specification
#ifdef __cplusplus
extern "C" {
#endif

int connect_db();
int* get_destination_ids();
int write_entry(entry* entry_arg, bool from_update);
int update_entry(entry* entry_arg, entry* prev_entry);
char** get_destination(int* arr_size);
int delete_entry(entry* prev_entry);
filterPair *get_filters(char* destination_folder, int* size);
char** get_sources(char* destination_folder, int* size);
int destination(int id, char *path);
int main_script();
void test();

#ifdef __cplusplus
}
#endif

#endif // SCRIPT_H
