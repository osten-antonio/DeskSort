// script.h

#ifndef SCRIPT_H
#define SCRIPT_H

#include <stdio.h>
#include "sqlite3.h"
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <windows.h>



typedef struct {
    char *destination_path;
    char *source_path;
} folderPair;

typedef struct{
    char* filter;
    char* type;
} filterPair;

// C++ linkage specification
#ifdef __cplusplus
extern "C" {
#endif

int connect_db();
int process(void *data, int argc, char **argv, char **azColName);
int* get_destination_id();
int write_entry(char** source, int source_count, char* destination, filterPair* filters, int filter_count);
char** get_destination();
int destination(int id, char *path);
int main_script();
void test();

#ifdef __cplusplus
}
#endif

#endif // SCRIPT_H
