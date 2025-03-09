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

int process(folderPair destination_data,const char* filter,const char* filter_type) {

    DIR *dir = opendir(destination_data.source_path);
    if(destination_data.source_path[strlen(destination_data.source_path)-1] != '\\'){
        // Ensures that path is in right format \ or no \ //
        destination_data.source_path[strlen(destination_data.source_path)] = '\\';
        destination_data.source_path[strlen(destination_data.source_path)+1] = '\0';
    }

    if(!dir){
        return 0;
    }
    if(!strcmp(filter_type,"prefix")){
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {

            char fullPath[32768];
            snprintf(fullPath, sizeof(fullPath), "%s\\%s", destination_data.source_path, entry->d_name);

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

            if(!strncmp(entry->d_name, filter, strlen(filter))){
                // move
            }
            printf("%s\n", entry->d_name);
        }



    }else if(!strcmp(filter_type,"suffix")){

    }else if(!strcmp(filter_type,"containing")){

    }else if(!strcmp(filter_type,"extension")){

    }

    // Check for filter type
    // Scan through all of the source file (data), with the appropiate cehcks (prefix, suffix, contain)
    // If it matches perform move
    return 1;
}

int *get_destination_id() {
    if(connect_db()==0){
        return 0;
    }
    sqlite3_stmt *stmt;


    if(sqlite3_prepare_v2(db,"SELECT COUNT(*) FROM destination",-1,&stmt,NULL)!=SQLITE_OK){
        return NULL;
    }

    int destination_count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        destination_count = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    stmt = NULL;

    if(destination_count==0){
        return NULL;
    }


    int count = 0;
    int *res = (int *)malloc(destination_count*sizeof(int)+1);

    if (res == NULL) {
        return NULL;
    }
    if (sqlite3_prepare_v2(db, "SELECT folder_id FROM destination", -1, &stmt, NULL) != SQLITE_OK) {
        free(res);
        return NULL;
    }


    while (sqlite3_step(stmt) == SQLITE_ROW) {
        res[count++]= sqlite3_column_int(stmt, 0);
    }

    res[destination_count+1]=-1;
    sqlite3_finalize(stmt);
    return res;
}

int write_entry(char** source, int source_count, char* destination, filterPair* filters, int filter_count) {
    if(connect_db()==0){
        return 0;
    }

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
    if(connect_db()==0){
        return 0;
    }
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, "SELECT count(*) FROM destination", -1, &stmt, NULL) != SQLITE_OK) {
        return NULL;
    }
    if(sqlite3_step(stmt)!=SQLITE_ROW){
        return NULL;
    }
    int size = sqlite3_column_int(stmt,0);
    sqlite3_finalize(stmt);
    stmt=NULL;

    if(size==0){
        return NULL;
    }

    if (sqlite3_prepare_v2(db, "SELECT folder_path FROM destination", -1, &stmt, NULL) != SQLITE_OK) {
        return NULL;
    }

    int count = 0;
    char **res = (char **)malloc(size* sizeof(char *));

    if (res == NULL) {
        return NULL;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *destination_path = (const char *)sqlite3_column_text(stmt, 0);
        if (!destination_path) destination_path = "";
        res[count]=strdup(destination_path);
        if (res[count] == NULL) {
            for (int i = 0; i < size; i++) {
                free(res[i]);
            }
            free(res);
            return NULL;
        }

        count++;
    }

    sqlite3_finalize(stmt);

    return res;
}


int destination(int id, char *path){
    if(connect_db()==0){
        return 0;
    }
    sqlite3_stmt *stmt;
    const char* destination_query="SELECT destination.folder_path, destination.folder_id "
                              "FROM destination "
                              "INNER JOIN link ON link.destination_folder_id = destination.folder_id "
                              "WHERE link.source_folder_id = (?);";

    if(sqlite3_prepare_v2(db,destination_query, -1, &stmt, NULL) != SQLITE_OK){
        return -1;
    }
    if(sqlite3_bind_int(stmt,1,id)!=SQLITE_OK){
        sqlite3_finalize(stmt);
        return -2;
    }
    while(sqlite3_step(stmt) == SQLITE_ROW){
        char *destination_path = (char *)sqlite3_column_text(stmt, 0);
        if (!destination_path) destination_path = "";
        folderPair data = { destination_path, path };
        sqlite3_stmt *stmt2;

        if(sqlite3_prepare_v2(db,"SELECT filters.filter, filters.filter_type FROM filters INNER JOIN link ON "
                                   "link.filter_id=filters.filter_id WHERE link.source_folder_id = (?) AND"
                                   "link.destination_folder_id = (?)",-1,&stmt2,NULL)!=SQLITE_OK){
            sqlite3_finalize(stmt);
            return -3;
        }
        if(sqlite3_bind_int(stmt2,1,id)!=SQLITE_OK){
            sqlite3_finalize(stmt2);
            sqlite3_finalize(stmt);
            return -4;
        }
        if(sqlite3_bind_int(stmt2,2,sqlite3_column_int(stmt,1))!=SQLITE_OK){
            sqlite3_finalize(stmt2);
            sqlite3_finalize(stmt);
            return -5;
        }
        while(sqlite3_step(stmt2)==SQLITE_ROW){
            const char *filter_value = (const char *)sqlite3_column_text(stmt2, 0);
            const char *filter_type = (const char *)sqlite3_column_text(stmt2, 1);

            if (!filter_value) filter_value = "";
            if (!filter_type) filter_type = "";

            process(data, filter_value, filter_type);
        }
        sqlite3_finalize(stmt2);
    }
    sqlite3_finalize(stmt);
    return 0;
}

int main_script(){
    sqlite3_stmt *stmt;
    const char *source_query = "SELECT * from source";
    if (sqlite3_prepare_v2(db, source_query, -1, &stmt, NULL) != SQLITE_OK) {
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
