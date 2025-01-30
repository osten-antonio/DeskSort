#include <stdio.h>
#include "sqlite3.h"
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <windows.h>
#include <stdio.h>

sqlite3 *db;

typedef struct {
    char *destination_path;
    char *source_path;
} folderPair;

int connect_db(){
    int rc = sqlite3_open("entries.db", &db);
    if(rc){
        return rc;
    }
    return 0;
}
// argv[0] = filter, argv[1] = filter_type
int process(void *data, int argc, char **argv, char **azColName) {

    folderPair *folders = (folderPair *)data;

    DIR *dir = opendir(folders->source_path);
    if(folders->source_path[strlen(folders->source_path)-1] != '\\'){
        // Ensures that path is in right format \ or no \ //
        folders->source_path[strlen(folders->source_path)] = '\\';
        folders->source_path[strlen(folders->source_path)+1] = '\0';
    }

    if(!dir){
        return 0;
    }
    if(!strcmp(argv[1],"prefix")){
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {

            char fullPath[PATH_MAX];
            snprintf(fullPath, sizeof(fullPath), "%s\\%s", folders->source_path, entry->d_name);

            // Get file attributes
            DWORD attrs = GetFileAttributes((wchar_t*)fullPath);
            if((attrs & INVALID_FILE_ATTRIBUTES) ||
                (attrs & FILE_ATTRIBUTE_DIRECTORY) ||
                (attrs & FILE_ATTRIBUTE_REPARSE_POINT)){
                /*
                 * Skips file if it is a folder, symbolic or an invalid file
                 * & (bitwise and) is used to check if a flag is set in the attributes, or when working with bitmasks
                 * the constant are bitmasks, eg if attrs = 10111 and the bitmask is 00111 the result is 00111
                 * if the resulting is non zero, then the attribute is set
                 */
                continue;
            }
            if (!strcmp(entry->d_name, ".")|| !strcmp(entry->d_name, "..")) {
                // Skips current and parent directory (. & ..)
                continue;
            }

            if(!strncmp(entry->d_name, argv[0], strlen(argv[0]))){
                // move
            }
            printf("%s\n", entry->d_name);
        }



    }else if(!strcmp(argv[1],"suffix")){

    }else if(!strcmp(argv[1],"containing")){

    }else if(!strcmp(argv[1],"extension")){

    }

    // Check for filter type
    // Scan through all of the source file (data), with the appropiate cehcks (prefix, suffix, contain)
    // If it matches perform move
    return 1;
}

int destination(int id, char *path){
    sqlite3_stmt *stmt;
    char destination_query[256];
    snprintf(destination_query, sizeof(destination_query),
             "SELECT destination.folder_path, destination.folder_id "
             "FROM destination "
             "INNER JOIN link ON link.destination_folder_id = destination.folder_id "
             "WHERE link.source_folder_id = %d;", id);
    if(sqlite3_prepare_v2(db,destination_query, -1, &stmt, NULL) != SQLITE_OK){
        fprintf(stderr, "Failed to load destination for %s: %s\n", path,sqlite3_errmsg(db));
        return 0;
    }
    while(sqlite3_step(stmt) == SQLITE_ROW){
        char filter_query[256];
        char *destination_path = (char *)sqlite3_column_text(stmt, 0);
        folderPair data = { destination_path, path };

        snprintf(filter_query, sizeof(filter_query),
                 "SELECT filters.filter, filters.filter_type "
                 "FROM filters INNER JOIN "
                 "link ON link.filter_id = filters.filter_id "
                 "WHERE link.source_folder_id = %d AND link.destination_folder_id = %d",
                 id,sqlite3_column_int(stmt,1));
        int filters = sqlite3_exec(db,filter_query,process,&data,0);
        if(filters != SQLITE_OK){
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        } else{
            printf("done");
        }
    }
    return 0;
}

int main_script(){
    sqlite3_stmt *stmt;
    const char *source_query = "SELECT * from source";
    if (sqlite3_prepare_v2(db, source_query, -1, &stmt, NULL) != SQLITE_OK) {
        fprintf(stderr, "Failed to load sources: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        char *path = (char *)sqlite3_column_text(stmt, 1);
        destination(id, path);
        printf("ID: %d, Path: %s\n", id, path);

    }
    sqlite3_finalize(stmt);
    return 0;

}
int main(){
    connect_db();
    main_script();
    return 0;
}

/*
gcc -o script script.c sqlite3.c -IC:\MinGW\include
*/
