#include "script.h"


sqlite3 *db;
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

            char fullPath[32768];
            snprintf(fullPath, sizeof(fullPath), "%s\\%s", folders->source_path, entry->d_name);

            // Get file attributes
            DWORD attrs = GetFileAttributes(fullPath);
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

int *get_destination_id() {
    /*
    sqlite3_stmt *stmt;
    const char *all_destination_query = "SELECT folder_id FROM destination";

    if (sqlite3_prepare_v2(db, all_destination_query, -1, &stmt, NULL) != SQLITE_OK) {
        return NULL;
    }

    int count = 0;
    int *res = (int *)malloc(sizeof(int));

    if (res == NULL) {
        return NULL;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int destination_id = sqlite3_column_int(stmt, 0);
        printf("Fetched destination id: %d\n", destination_id);
        res[count]= destination_id;

        count++;
        printf("\n%d",count);
        res = (int *)realloc(res, count * sizeof(int));
    }

    sqlite3_finalize(stmt);
    return res;
*/
}

int write_entry(char** source, int source_count, char* destination, filterPair* filters, int filter_count) {
    connect_db();

    int* source_ids = (int*)malloc(source_count * sizeof(int));
    int* filter_ids = (int*)malloc(filter_count * sizeof(int));

    if (!source_ids || !filter_ids) {
        free(source_ids);
        free(filter_ids);
        return -1;
    }

    sqlite3_stmt* stmt = NULL;
    int destination_id, error_id;

    if (sqlite3_prepare_v2(db, "INSERT INTO destination(folder_path) VALUES(?)", -1, &stmt, NULL) != SQLITE_OK) {
        error_id = -2;
        goto cleanup;
    }
    if (sqlite3_bind_text(stmt, 1, destination, -1, SQLITE_TRANSIENT) != SQLITE_OK) {
        error_id = -3;
        goto cleanup;
    }
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        error_id = -4;
        goto cleanup;
    }
    sqlite3_finalize(stmt);
    stmt = NULL;


    destination_id = sqlite3_last_insert_rowid(db); //

    // sources
    for (int i = 0; i < source_count; i++) {
        if (sqlite3_prepare_v2(db, "INSERT INTO source(folder_path) VALUES(?)", -1, &stmt, NULL) != SQLITE_OK) {
            error_id = -5;
            goto cleanup;
        }
        if (sqlite3_bind_text(stmt, 1, source[i], -1, SQLITE_TRANSIENT) != SQLITE_OK) {
            error_id = -6;
            goto cleanup;
        }
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            error_id = -7;
            goto cleanup;
        }
        source_ids[i] = sqlite3_last_insert_rowid(db);
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    // filters
    for (int i = 0; i < filter_count; i++) {
        if (sqlite3_prepare_v2(db, "INSERT INTO filters(filter, filter_type) VALUES(?, ?)", -1, &stmt, NULL) != SQLITE_OK) {
            error_id = -8;
            goto cleanup;
        }
        if (sqlite3_bind_text(stmt, 1, filters[i].filter, -1, SQLITE_TRANSIENT) != SQLITE_OK) {
            error_id = -9;
            goto cleanup;
        }
        if (sqlite3_bind_text(stmt, 2, filters[i].type, -1, SQLITE_TRANSIENT) != SQLITE_OK) {
            error_id = -10;
            goto cleanup;
        }
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            error_id = -11;
            goto cleanup;
        }
        filter_ids[i] = sqlite3_last_insert_rowid(db);
        sqlite3_finalize(stmt);
        stmt = NULL;
    }
    for (int i = 0; i < source_count; i++) {
        for (int j = 0; j < filter_count; j++) {
            if (sqlite3_prepare_v2(db, "INSERT INTO link(filter_id, source_folder_id, destination_folder_id) VALUES(?, ?, ?)", -1, &stmt, NULL) != SQLITE_OK) {
                error_id = -12;
                goto cleanup;
            }
            if (sqlite3_bind_int(stmt, 1, filter_ids[j]) != SQLITE_OK ||
                sqlite3_bind_int(stmt, 2, source_ids[i]) != SQLITE_OK ||
                sqlite3_bind_int(stmt, 3, destination_id) != SQLITE_OK) {
                error_id = -13;
                goto cleanup;
            }
            if (sqlite3_step(stmt) != SQLITE_DONE) {
                error_id = -14;
                goto cleanup;
            }
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
    }

    error_id = 0;

cleanup:
    if (stmt) sqlite3_finalize(stmt);
    free(source_ids);
    free(filter_ids);

    return error_id;
}



char **get_destination() {
    sqlite3_stmt *stmt;
    const char *all_destination_query = "SELECT folder_path FROM destination";

    if (sqlite3_prepare_v2(db, all_destination_query, -1, &stmt, NULL) != SQLITE_OK) {
        return NULL;
    }

    int capacity = 10;
    int count = 0;
    char **res = (char **)malloc(capacity * sizeof(char *));  // Allocate for 'capacity' pointers

    if (res == NULL) {
        return NULL;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *destination_path = (const char *)sqlite3_column_text(stmt, 0);

        if (count >= capacity) {
            capacity *= 2;
            res = (char **)realloc(res, capacity * sizeof(char *));
            if (res == NULL) {
                return NULL;
            }
        }

        // Allocate memory for the string and copy it
        res[count] = (char *)malloc(strlen(destination_path) + 1);  // +1 for the null terminator
        if (res[count] == NULL) {
            return NULL;
        }

        strcpy(res[count], destination_path);  // Copy the string into allocated memory
        printf("Storing destination %d: %s\n", count + 1, res[count]);  // Debugging the result
        count++;
    }

    sqlite3_finalize(stmt);

    if (count < capacity) {
        res = (char **)realloc(res, count * sizeof(char *));
    }


    return res;
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
// int main(){
//     connect_db();

//     int *destinations = get_destination_id();

//     int i =0;
//     while (destinations[i] != 0) {
//         printf("\nDestination %d: %s\n", i + 1, destinations[i]);
//         i++;
//     }
//     main_script();
//     return 0;
// }

/*
gcc -o script script.c sqlite3.c -IC:\MinGW\include
*/
