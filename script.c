#include "script.h"

#define BUFFER_SIZE 4096

sqlite3 *db;
int connect_db(){
    int rc = sqlite3_open("./data/entries.db", &db);
    if(rc){
        return rc;
    }
    return 0;
}

int process(folderPair destination_data,char* filter,const char* filter_type) {
    if(destination_data.source_path[strlen(destination_data.source_path)-1] != '/'){
        // Ensures that path is in right format \ or no \ //
        destination_data.source_path[strlen(destination_data.source_path)] = '/';
        destination_data.source_path[strlen(destination_data.source_path)+1] = '\0';
    }
    DIR *dir = opendir(destination_data.source_path);
    if(!dir){
        return -1; // Source cannot be opened
    }
    printf("%s",filter_type);
    //fflush(stdout);
    if(!strcmp(filter_type,"Prefix")){
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            printf("Cehcking: %s\n",entry->d_name);
            char fullPath[32768];
            snprintf(fullPath, sizeof(fullPath), "%s\\%s", destination_data.source_path, entry->d_name);
            fullPath[sizeof(fullPath) - 1] = '\0';

            FILE* f = fopen(fullPath, "r");
            if (f) { // File exists
                fclose(f);
            } else {
                continue;
            }
            // // Get file attributes
            // DWORD attrs = GetFileAttributes(fullPath);
            // if((attrs == INVALID_FILE_ATTRIBUTES) ||
            //     (attrs & FILE_ATTRIBUTE_DIRECTORY) ||
            //     (attrs & FILE_ATTRIBUTE_REPARSE_POINT)){
            //     /*
            //      * Skips file if it is a folder, symbolic or an invalid file
            //      * & (bitwise and) is used to check if a flag is set in the attributes, or when working with bitmasks
            //      * the constant are bitmasks, eg if attrs = 10111 and the bitmask is 00111 the result is 00111
            //      * if the resulting is non zero, then the attribute is set
            //      * == Is used to test for error or other stuff
            //      */
            //     // printf("a: %s\n",entry->d_name);
            //     continue;
            // }
            if (!strcmp(entry->d_name, ".")|| !strcmp(entry->d_name, "..")) {
                // Skips current and parent directory (. & ..)
                // printf("b: %s\n",entry->d_name);
                continue;
            }

            printf("Cehcking: %s\n",entry->d_name);
            if(!strncmp(entry->d_name, filter, strlen(filter))){
                // move
                // printf("%s\n",entry->d_name);
                char destination_file_path[32768];

                // Copies destination path, with format of destination paht,entry->d_name(directory_name or file name)
                snprintf(destination_file_path, sizeof(destination_file_path), "%s/%s", destination_data.destination_path, entry->d_name);
                destination_file_path[sizeof(destination_file_path) - 1] = '\0';

                // Check for duplicate, appends count to file name
                int count = strlen(entry->d_name);
                char cur_file[count + 1];

                strcpy(cur_file, entry->d_name);
                while(count>0 && entry->d_name[count-1]!='.'){
                    count--;
                }
                if(strstr(entry->d_name, ".")){
                    char extension[strlen(entry->d_name)-count + 1];
                    strncpy(extension,entry->d_name+count,strlen(entry->d_name)-count);
                    extension[strlen(entry->d_name)-count]='\0';
                    cur_file[count-1] = '\0'; //Cuts the file name to not include hte extension

                    count = 1;
                    // printf("Extension:%s\n",extension);
                    // printf("Cur_file:%s\n",cur_file);
                    while(!access(destination_file_path,F_OK)){
                        snprintf(destination_file_path, sizeof(destination_file_path), "%s/%s[%d].%s",
                                 destination_data.destination_path, cur_file, count,extension);
                        count++;
                    }
                    // printf("Duplicated file: %s\n",destination_file_path);
                    // printf("Destination path: %s\n",destination_file_path);
                }else{
                    count = 1;
                    while(!access(destination_file_path,F_OK)){
                        snprintf(destination_file_path, sizeof(destination_file_path), "%s/%s[%d]",
                                 destination_data.destination_path, cur_file, count);
                        count++;
                    }
                }

                FILE *source_file = fopen(fullPath, "rb");
                if(source_file == NULL){
                    return -2; // Source file cannot be opened
                }

                FILE *destination_file = fopen(destination_file_path, "wb");
                if(destination_file == NULL){
                    fclose(source_file);
                    return -3;
                }

                unsigned char buffer[BUFFER_SIZE]; // Unsigend char because dealing wiht raw bytes, no negatiges
                size_t bytes; // size t represents the size of any object in the memory
                // printf("Destination path: %s\n",destination_file_path);
                while((bytes = fread(buffer, 1, BUFFER_SIZE, source_file)) > 0){
                    fwrite(buffer, 1, bytes, destination_file); // Write from buffer, exactly bytes byte, 1 byte at a time at destination_file
                }

                fclose(source_file);
                fclose(destination_file);
                remove(fullPath);
                }
        }



    }else if(!strcmp(filter_type,"Suffix")){
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
                printf("Cehcking: %s\n",entry->d_name);
                char fullPath[32768];
                snprintf(fullPath, sizeof(fullPath), "%s%s", destination_data.source_path, entry->d_name);

                FILE* f = fopen(fullPath, "r");
                if (f) { // File exists
                    fclose(f);
                } else {
                    continue;
                }

                if (!strcmp(entry->d_name, ".")|| !strcmp(entry->d_name, "..")) {
                    // Skips current and parent directory (. & ..)
                    // printf("b: %s\n",entry->d_name);
                    continue;
                }
                int count = strlen(entry->d_name);
                char cur_file[count + 1];

                strcpy(cur_file, entry->d_name);
                while(count>0 && entry->d_name[count-1]!='.'){
                    count--;
                }
                if(count < strlen(filter)-1){
                    continue; // Current file name is shorter than the filter
                }
                cur_file[count-1] = '\0'; //Cuts the file name to not include hte extension

                // printf("%s\n",cur_file);
                if(!strncmp(&cur_file[strlen(cur_file)-strlen(filter)], filter, strlen(filter))){
                    // move
                    // printf("Moving %s\n",entry->d_name);
                    char destination_file_path[32768];

                    snprintf(destination_file_path, sizeof(destination_file_path), "%s/%s", destination_data.destination_path, entry->d_name);
                    destination_file_path[sizeof(destination_file_path) - 1] = '\0';

                    // Check for duplicate, appends count to file name
                    int count = strlen(entry->d_name);
                    char cur_file[count + 1];

                    strcpy(cur_file, entry->d_name);
                    while(count>0 && entry->d_name[count-1]!='.'){
                        count--;
                    }
                    if(strstr(entry->d_name, ".")){
                        char extension[strlen(entry->d_name)-count + 1];
                        strncpy(extension,entry->d_name+count,strlen(entry->d_name)-count);
                        extension[strlen(entry->d_name)-count]='\0';
                        cur_file[count-1] = '\0'; //Cuts the file name to not include hte extension

                        count = 1;
                        // printf("Extension:%s\n",extension);
                        // printf("Cur_file:%s\n",cur_file);
                    while(!access(destination_file_path,F_OK)){
                        snprintf(destination_file_path, sizeof(destination_file_path), "%s\\%s[%d].%s",
                                 destination_data.destination_path, cur_file, count,extension);
                        count++;
                    }
                    // printf("Duplicated file: %s\n",destination_file_path);
                    // printf("Destination path: %s\n",destination_file_path);
                }else{
                    while(!access(destination_file_path,F_OK)){
                        snprintf(destination_file_path, sizeof(destination_file_path), "%s\\%s[%d]",
                                destination_data.destination_path, cur_file, count);
                        count++;
                    }
                }


                FILE *source_file = fopen(fullPath, "rb");
                if(source_file == NULL){
                    return -2; // Source file cannot be opened
                }

                FILE *destination_file = fopen(destination_file_path, "wb");
                if(destination_file == NULL){
                    fclose(source_file);
                    return -3;
                }
                unsigned char buffer[BUFFER_SIZE];
                size_t bytes;
                // printf("Destination path: %s\n",destination_file_path);
                while((bytes = fread(buffer, 1, BUFFER_SIZE, source_file)) > 0){
                    fwrite(buffer, 1, bytes, destination_file);
                }

                fclose(source_file);
                fclose(destination_file);
                remove(fullPath);

            }
        }

    }else if(!strcmp(filter_type,"Containing")){
        struct dirent *entry;

        while ((entry = readdir(dir)) != NULL) {
            if(entry->d_name!=NULL){
                char fullPath[32768];
                // printf("Cehcking: %s\n",entry->d_name);
                snprintf(fullPath, sizeof(fullPath), "%s%s", destination_data.source_path, entry->d_name);

                FILE* f = fopen(fullPath, "r");
                if (f) { // File exists
                    fclose(f);
                } else {
                    continue;
                }

                if (!strcmp(entry->d_name, ".")|| !strcmp(entry->d_name, "..")) {
                    // Skips current and parent directory (. & ..)
                    // printf("b: %s\n",entry->d_name);
                    continue;
                }

                int count = strlen(entry->d_name);
                char cur_file[count + 1];

                strcpy(cur_file, entry->d_name);
                while(count>0 && entry->d_name[count-1]!='.'){
                    count--;
                }
                if(count < strlen(filter)-1){
                    continue; // Current file name is shorter than the filter
                }
                cur_file[count-1] = '\0'; //Cuts the file name to not include hte extension

                if(strstr(cur_file,filter)){ // wtf is this function, dont use sliding window lol
                    // printf("Moving %s\n",entry->d_name);
                    char destination_file_path[32768];

                    snprintf(destination_file_path, sizeof(destination_file_path), "%s/%s", destination_data.destination_path, entry->d_name);
                    destination_file_path[sizeof(destination_file_path) - 1] = '\0';

                    // Check for duplicate, appends count to file name
                    int count = strlen(entry->d_name);
                    char cur_file[count + 1];

                    strcpy(cur_file, entry->d_name);
                    while(count>0 && entry->d_name[count-1]!='.'){
                        count--;
                    }
                    if(strstr(entry->d_name, ".")){
                        char extension[strlen(entry->d_name)-count + 1];
                        strncpy(extension,entry->d_name+count,strlen(entry->d_name)-count);
                        extension[strlen(entry->d_name)-count]='\0';
                        cur_file[count-1] = '\0'; //Cuts the file name to not include hte extension

                        count = 1;
                        // printf("Extension:%s\n",extension);
                        // printf("Cur_file:%s\n",cur_file);
                        while(!access(destination_file_path,F_OK)){
                            snprintf(destination_file_path, sizeof(destination_file_path), "%s/%s[%d].%s",
                                     destination_data.destination_path, cur_file, count,extension);
                            count++;
                        }
                        // printf("Duplicated file: %s\n",destination_file_path);
                        // printf("Destination path: %s\n",destination_file_path);
                    }else{
                        while(!access(destination_file_path,F_OK)){
                            snprintf(destination_file_path, sizeof(destination_file_path), "%s/%s[%d]",
                                     destination_data.destination_path, cur_file, count);
                            count++;
                        }
                    }

                    // printf("Destination path: %s\n",destination_file_path);
                    // printf("Full: %s\n",fullPath);
                    FILE *source_file = fopen(fullPath, "rb");
                    if(source_file == NULL){
                        return -2; // Source file cannot be opened
                    }

                    FILE *destination_file = fopen(destination_file_path, "wb");
                    if(destination_file == NULL){
                        fclose(source_file);
                        return -3;
                    }

                    unsigned char buffer[BUFFER_SIZE];
                    size_t bytes;
                    // printf("Destination path: %s\n",destination_file_path);
                    while((bytes = fread(buffer, 1, BUFFER_SIZE, source_file)) > 0){
                        fwrite(buffer, 1, bytes, destination_file);
                    }

                    fclose(source_file);
                    fclose(destination_file);
                    remove(fullPath);
                }else{
                    continue;
                }
            }
        }
    }else if(!strcmp(filter_type,"Extension")){

        int length = strlen(filter);
        if (length == 0) {
            return 10;  // Return 10 if the string is empty
        }
        while(filter[0]=='.'){ // Shifts filter to the left until no . in front of filter
            if (length == 0) return 10;
            for(int i = 0;i<length-1;i++){
                filter[i]=filter[i+1];
            }
            filter[length-1] = '\0';
            length = strlen(filter);
            if (length == 0) {
                return 10;  // Return 10 if the string is empty
            }
        }
        while(filter[length-1]=='.'){
            filter[length-1] = '\0'; // Removes trailing . by making the positiion of the . a null pointer
            length = strlen(filter);
        }
        // printf("filter: %s\n",filter);
        struct dirent *entry;
        while ((entry = readdir(dir)) != NULL) {
            // printf("Check: %s\n",entry->d_name);
            if(entry->d_name!=NULL){
                char fullPath[32768];
                snprintf(fullPath, sizeof(fullPath), "%s%s", destination_data.source_path, entry->d_name);
                fullPath[sizeof(fullPath) - 1] = '\0';

                FILE* f = fopen(fullPath, "r");
                if (f) { // File exists
                    fclose(f);
                } else {
                    continue;
                }

                if (!strcmp(entry->d_name, ".")|| !strcmp(entry->d_name, "..")) {
                    continue;
                }

                if(!strcmp(&entry->d_name[strlen(entry->d_name)-strlen(filter)],filter)){
                    char destination_file_path[32768];

                    snprintf(destination_file_path, sizeof(destination_file_path), "%s/%s", destination_data.destination_path, entry->d_name);
                    destination_file_path[sizeof(destination_file_path) - 1] = '\0';
                    // Check for duplicate, appends count to file name
                    int count = strlen(entry->d_name);
                    char cur_file[count+1];

                    strcpy(cur_file, entry->d_name);
                    while(count>0 && entry->d_name[count-1]!='.'){
                        count--;
                    }
                    if(strstr(entry->d_name, ".")){
                        char extension[strlen(entry->d_name)-count+1];
                        strncpy(extension,entry->d_name+count,strlen(entry->d_name)-count);
                        extension[strlen(entry->d_name)-count]='\0';
                        cur_file[count-1] = '\0'; //Cuts the file name to not include hte extension

                        count = 1;
                        // printf("Extension:%s\n",extension);
                        // printf("Cur_file:%s\n",cur_file);
                        while(!access(destination_file_path,F_OK)){
                            snprintf(destination_file_path, sizeof(destination_file_path), "%s/%s[%d].%s",
                                     destination_data.destination_path, cur_file, count,extension);
                            count++;
                        }
                        // printf("Duplicated file: %s\n",destination_file_path);
                        // printf("Destination path: %s\n",destination_file_path);
                    }else{
                        while(!access(destination_file_path,F_OK)){
                            snprintf(destination_file_path, sizeof(destination_file_path), "%s/%s[%d]",
                                     destination_data.destination_path, cur_file, count);
                            count++;
                        }
                    }
                    FILE *source_file = fopen(fullPath, "rb");
                    if(source_file == NULL){
                        return -2; // Source file cannot be opened
                    }

                    FILE *destination_file = fopen(destination_file_path, "wb");
                    if(destination_file == NULL){
                        fclose(source_file);
                        return -3;
                    }

                    unsigned char buffer[BUFFER_SIZE];
                    size_t bytes;
                    printf("Destination path: %s\n",destination_file_path);
                    while((bytes = fread(buffer, 1, BUFFER_SIZE, source_file)) > 0){
                        fwrite(buffer, 1, bytes, destination_file);
                    }

                    fclose(source_file);
                    fclose(destination_file);
                    remove(fullPath);
                }
            }
        }
    }
    // Check for filter type
    // Scan through all of the source file (data), with the appropiate cehcks (prefix, suffix, contain)
    // If it matches perform move
    return 1;
}

int *get_destination_ids() {
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

int delete_entry(entry* prev_entry){
    int connect = connect_db();
    if(connect!=0){
        return -100;
    }

    sqlite3_exec(db, "PRAGMA foreign_keys = ON;", NULL, NULL, NULL);
    sqlite3_exec(db,"BEGIN TRANSACTION;",NULL,NULL,NULL);
    sqlite3_stmt* stmt;
    if(sqlite3_prepare_v2(db,"SELECT DISTINCT filter_id FROM link "
                               "INNER JOIN destination ON "
                               "link.destination_folder_id = destination.folder_id WHERE destination.folder_path = (?)",-1,
                           &stmt, NULL)!=SQLITE_OK){
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "ROLLBACK TRANSACTION;", NULL, NULL, NULL);
        sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, NULL);
        return -1;
    }
    if(sqlite3_bind_text(stmt,1,prev_entry->destination,-1,SQLITE_TRANSIENT)!=SQLITE_OK){
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "ROLLBACK TRANSACTION;", NULL, NULL, NULL);
        sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, NULL);
        return -2;
    }
    while(sqlite3_step(stmt) == SQLITE_ROW){
        sqlite3_stmt* stmt2;
        if(sqlite3_prepare_v2(db,"DELETE FROM filters WHERE filter_id = (?)",-1,&stmt2,NULL)!=SQLITE_OK){
            sqlite3_finalize(stmt);
            sqlite3_finalize(stmt2);
            sqlite3_exec(db, "ROLLBACK TRANSACTION;", NULL, NULL, NULL);
            sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, NULL);
            return -3;
        }
        if(sqlite3_bind_int(stmt2,1,sqlite3_column_int(stmt,0))!=SQLITE_OK){
            sqlite3_finalize(stmt);
            sqlite3_finalize(stmt2);
            sqlite3_exec(db, "ROLLBACK TRANSACTION;", NULL, NULL, NULL);
            sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, NULL);
            return -4;
        }
        if(sqlite3_step(stmt2)!=SQLITE_DONE){
            sqlite3_finalize(stmt);
            sqlite3_finalize(stmt2);
            sqlite3_exec(db, "ROLLBACK TRANSACTION;", NULL, NULL, NULL);
            sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, NULL);
            return -5;
        }
        sqlite3_finalize(stmt2);
        stmt2=NULL;
    }
    sqlite3_finalize(stmt);
    stmt=NULL;
    if(sqlite3_prepare_v2(db,"SELECT DISTINCT source_folder_id FROM link "
                               "INNER JOIN destination ON "
                               "link.destination_folder_id = destination.folder_id WHERE destination.folder_path = (?)",-1,
                           &stmt, NULL)!=SQLITE_OK){
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "ROLLBACK TRANSACTION;", NULL, NULL, NULL);
        sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, NULL);
        return -6;
    }
    if(sqlite3_bind_text(stmt,1,prev_entry->destination,-1,SQLITE_TRANSIENT)!=SQLITE_OK){
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "ROLLBACK TRANSACTION;", NULL, NULL, NULL);
        sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, NULL);
        return -7;
    }
    while(sqlite3_step(stmt) == SQLITE_ROW){
        sqlite3_stmt* stmt2;
        if(sqlite3_prepare_v2(db,"DELETE FROM source WHERE folder_id = (?)",-1,&stmt2,NULL)!=SQLITE_OK){
            sqlite3_finalize(stmt);
            sqlite3_finalize(stmt2);
            sqlite3_exec(db, "ROLLBACK TRANSACTION;", NULL, NULL, NULL);
            sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, NULL);
            return -8;
        }
        if(sqlite3_bind_int(stmt2,1,sqlite3_column_int(stmt,0))!=SQLITE_OK){
            sqlite3_finalize(stmt);
            sqlite3_finalize(stmt2);
            sqlite3_exec(db, "ROLLBACK TRANSACTION;", NULL, NULL, NULL);
            sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, NULL);
            return -9;
        }
        if(sqlite3_step(stmt2)!=SQLITE_DONE){
            sqlite3_finalize(stmt);
            sqlite3_finalize(stmt2);
            sqlite3_exec(db, "ROLLBACK TRANSACTION;", NULL, NULL, NULL);
            sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, NULL);
            return -10;
        }
        sqlite3_finalize(stmt2);
        stmt2=NULL;
    }
    sqlite3_finalize(stmt);
    stmt=NULL;
    if(sqlite3_prepare_v2(db,"DELETE FROM destination WHERE folder_path = (?)",-1,&stmt,NULL)!=SQLITE_OK){
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "ROLLBACK TRANSACTION;", NULL, NULL, NULL);
        sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, NULL);
        return -11;
    }
    if(sqlite3_bind_text(stmt,1,prev_entry->destination,-1,SQLITE_TRANSIENT)!=SQLITE_OK){
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "ROLLBACK TRANSACTION;", NULL, NULL, NULL);
        sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, NULL);
        return -12;
    }
    if(sqlite3_step(stmt)!=SQLITE_DONE){
        sqlite3_finalize(stmt);
        sqlite3_exec(db, "ROLLBACK TRANSACTION;", NULL, NULL, NULL);
        sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, NULL);
        return -13;
    }
    sqlite3_finalize(stmt);
    sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, NULL);
    return 0;
}

int write_entry(entry* entry_arg, bool from_update) {
    if(entry_arg->filter_count < 1){
        return -30;
    }
    if(entry_arg->source_count < 1){
        return -40;
    }
    if(entry_arg->destination==NULL || strcmp(entry_arg->destination,"")==0){
        return -50;
    }

    if(!from_update){
        sqlite3_exec(db,"BEGIN TRANSACTION;",NULL,NULL,NULL);
        int connect = connect_db();
        if(connect!=0){
            return -100;
        }
    }
    sqlite3_stmt* stmt;
    int destination_id, error_id;
    if(!from_update){
        if(sqlite3_prepare_v2(db,"SELECT COUNT(*) FROM destination WHERE folder_path = (?)",-1,&stmt,NULL)!=SQLITE_OK){
            return -10;
        }
        if(sqlite3_bind_text(stmt,1,entry_arg->destination,-1,SQLITE_TRANSIENT)!=SQLITE_OK){
            return -11;
        }
        if(sqlite3_step(stmt)!=SQLITE_ROW){
            return -12;
        }
        if(sqlite3_column_int(stmt,0)>0){
            return -200;
        }

        sqlite3_finalize(stmt);
        stmt=NULL;
    }
    int* source_ids = (int*)malloc(entry_arg->source_count * sizeof(int));
    int* filter_ids = (int*)malloc(entry_arg->filter_count * sizeof(int));

    if (!source_ids || !filter_ids) {
        free(source_ids);
        free(filter_ids);
        return -1;
    }





    if (sqlite3_prepare_v2(db, "SELECT folder_id FROM destination WHERE folder_path = ?", -1, &stmt, NULL) != SQLITE_OK) {
        error_id = -2;
        goto cleanup;
    }
    if (sqlite3_bind_text(stmt, 1, entry_arg->destination, -1, SQLITE_TRANSIENT) != SQLITE_OK) {
        error_id = -3;
        sqlite3_finalize(stmt);
        goto cleanup;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        destination_id = sqlite3_column_int(stmt, 0);
    } else {
        sqlite3_finalize(stmt);
        if (sqlite3_prepare_v2(db, "INSERT INTO destination(folder_path) VALUES(?)", -1, &stmt, NULL) != SQLITE_OK) {
            error_id = -4;
            goto cleanup;
        }
        if (sqlite3_bind_text(stmt, 1, entry_arg->destination, -1, SQLITE_TRANSIENT) != SQLITE_OK) {
            error_id = -5;
            sqlite3_finalize(stmt);
            goto cleanup;
        }
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            error_id = -6;
            sqlite3_finalize(stmt);
            goto cleanup;
        }
        destination_id = sqlite3_last_insert_rowid(db);
    }
    sqlite3_finalize(stmt);
    stmt = NULL;


    // sources
    for (int i = 0; i < entry_arg->source_count; i++) {
        if (sqlite3_prepare_v2(db, "INSERT INTO source(folder_path) VALUES(?)", -1, &stmt, NULL) != SQLITE_OK) {
            error_id = -5;
            goto cleanup;
        }
        if (sqlite3_bind_text(stmt, 1, entry_arg->source[i], -1, SQLITE_TRANSIENT) != SQLITE_OK) {
            error_id = -6;
            sqlite3_finalize(stmt);
            goto cleanup;
        }
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            error_id = -7;
            sqlite3_finalize(stmt);
            goto cleanup;
        }
        source_ids[i] = sqlite3_last_insert_rowid(db);
        sqlite3_finalize(stmt);
        stmt = NULL;
    }

    // filters
    for (int i = 0; i < entry_arg->filter_count; i++) {

        if (sqlite3_prepare_v2(db, "INSERT INTO filters(filter, filter_type) VALUES(?, ?)", -1, &stmt, NULL) != SQLITE_OK) {
            error_id = -8;
            goto cleanup;
        }
        if (sqlite3_bind_text(stmt, 1, entry_arg->filters[i].filter, -1, SQLITE_TRANSIENT) != SQLITE_OK) {
            error_id = -9;
            sqlite3_finalize(stmt);
            goto cleanup;
        }
        if (sqlite3_bind_text(stmt, 2, entry_arg->filters[i].type, -1, SQLITE_TRANSIENT) != SQLITE_OK) {
            error_id = -10;
            sqlite3_finalize(stmt);
            goto cleanup;
        }
        if (sqlite3_step(stmt) != SQLITE_DONE) {
            error_id = -11;
            sqlite3_finalize(stmt);
            goto cleanup;
        }
        filter_ids[i] = sqlite3_last_insert_rowid(db);
        sqlite3_finalize(stmt);
        stmt = NULL;
    }
    for (int i = 0; i < entry_arg->source_count; i++) {
        for (int j = 0; j < entry_arg->filter_count; j++) {
            if (sqlite3_prepare_v2(db, "INSERT INTO link(filter_id, source_folder_id, destination_folder_id) VALUES(?, ?, ?)", -1, &stmt, NULL) != SQLITE_OK) {
                error_id = -12;
                goto cleanup;
            }
            if (sqlite3_bind_int(stmt, 1, filter_ids[j]) != SQLITE_OK ||
                sqlite3_bind_int(stmt, 2, source_ids[i]) != SQLITE_OK ||
                sqlite3_bind_int(stmt, 3, destination_id) != SQLITE_OK) {
                error_id = -13;
                sqlite3_finalize(stmt);
                goto cleanup;
            }
            if (sqlite3_step(stmt) != SQLITE_DONE) {
                error_id = -14;
                sqlite3_finalize(stmt);
                goto cleanup;
            }
            sqlite3_finalize(stmt);
            stmt = NULL;
        }
    }

    error_id = 0;

cleanup:
    free(source_ids);
    free(filter_ids);
    if(error_id==0){
        sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, NULL);
    }else{
        sqlite3_exec(db, "ROLLBACK TRANSACTION;", NULL, NULL, NULL);
        sqlite3_exec(db, "END TRANSACTION;", NULL, NULL, NULL);
    }

    return error_id;
}



int update_entry(entry* entry_arg, entry* prev_entry){
    sqlite3_exec(db, "PRAGMA foreign_keys = ON;", NULL, NULL, NULL);
    sqlite3_exec(db,"BEGIN TRANSACTION;",NULL,NULL,NULL);
    if(entry_arg->filter_count < 1){
        return -30;
    }
    if(entry_arg->source_count < 1){
        return -40;
    }
    if(entry_arg->destination==NULL || strcmp(entry_arg->destination,"")==0){
        return -50;
    }
    sqlite3_stmt* stmt;
    // if(sqlite3_prepare_v2(db,"SELECT COUNT(*) FROM destination WHERE folder_path = (?)",-1,&stmt,NULL)!=SQLITE_OK){
    //     return -10;
    // }
    // printf("SQL: SELECT COUNT(*) FROM destination WHERE folder_path = '%s'\n", entry_arg->destination);
    // //fflush(stdout);
    // printf("%s",entry_arg->destination);
    // //fflush(stdout);
    // if(sqlite3_bind_text(stmt,1,entry_arg->destination,-1,SQLITE_TRANSIENT)!=SQLITE_OK){
    //     return -11;
    // }
    // if(sqlite3_step(stmt)!=SQLITE_ROW){
    //     return -12;
    // }
    // printf("SQL: '%d'\n", sqlite3_column_int(stmt,0));
    // //fflush(stdout);
    // if(sqlite3_column_int(stmt,0)>0){
    //     printf("SQL: '%d'\n", sqlite3_column_int(stmt,0));
    //     //fflush(stdout);
    //     return -200;
    // }
    if(sqlite3_prepare_v2(db,"UPDATE destination SET folder_path=(?) WHERE folder_path=(?) RETURNING folder_id",-1,&stmt,NULL)!=SQLITE_OK){
        sqlite3_finalize(stmt);
        return -11;
    }
    if(sqlite3_bind_text(stmt,1,entry_arg->destination,-1,SQLITE_TRANSIENT)!=SQLITE_OK || sqlite3_bind_text(stmt,2,prev_entry->destination,-1,SQLITE_TRANSIENT)!=SQLITE_OK){
        sqlite3_finalize(stmt);
        return -21;
    }
    if(sqlite3_step(stmt)!=SQLITE_ROW){
        sqlite3_finalize(stmt);
        return -13;
    }
    int destination_id = sqlite3_column_int(stmt,0);
    sqlite3_finalize(stmt);
    stmt = NULL;
    for(int i = 0; i < prev_entry->source_count ;i++){
        if (sqlite3_prepare_v2(db,
                               "DELETE FROM source "
                               "WHERE source.folder_path = (?) "
                               "AND source.folder_id IN ("
                               "    SELECT link.source_folder_id FROM link WHERE link.destination_folder_id = (?))",
                               -1, &stmt, NULL) != SQLITE_OK) {
            return -14;
        }
        if(sqlite3_bind_text(stmt,1,prev_entry->source[i],-1,SQLITE_TRANSIENT)!=SQLITE_OK){
            sqlite3_finalize(stmt);
            return -15;
        }
        if(sqlite3_bind_int(stmt,2,destination_id)!=SQLITE_OK){
            sqlite3_finalize(stmt);
            return -151;
        }
        if(sqlite3_step(stmt)!=SQLITE_DONE){
            sqlite3_finalize(stmt);
            return -16;
        }
        sqlite3_finalize(stmt);
    }

    for(int i = 0; i < prev_entry->filter_count ;i++){
        if (sqlite3_prepare_v2(db,
                               "DELETE FROM filters "
                               "WHERE filters.filter = (?) AND filters.filter_type = (?)"
                               "AND filters.filter_id IN ("
                               "    SELECT link.filter_id FROM link WHERE link.destination_folder_id = (?))",
                               -1, &stmt, NULL) != SQLITE_OK) {
            return -17;
        }

        if(sqlite3_bind_text(stmt,1,prev_entry->filters[i].filter,-1,SQLITE_TRANSIENT)!=SQLITE_OK){
            sqlite3_finalize(stmt);
            return -18;
        }
        if(sqlite3_bind_text(stmt,2,prev_entry->filters[i].type,-1,SQLITE_TRANSIENT)!=SQLITE_OK){
            sqlite3_finalize(stmt);
            return -19;
        }
        if(sqlite3_bind_int(stmt,3,destination_id)!=SQLITE_OK){
            sqlite3_finalize(stmt);
            return -110;
        }
        if(sqlite3_step(stmt)!=SQLITE_DONE){
            sqlite3_finalize(stmt);
            return -1101;
        }
        sqlite3_finalize(stmt);
    }
    return write_entry(entry_arg,true);
}


char **get_destination(int* arr_size) {
    if(connect_db()!=0){
        return NULL;
    }
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, "SELECT count(*) FROM destination", -1, &stmt, NULL) != SQLITE_OK) {
        return NULL;
    }
    if(sqlite3_step(stmt)!=SQLITE_ROW){
        sqlite3_finalize(stmt);
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

    char **res = (char **)malloc(size* sizeof(char *));

    if (res == NULL) {
        sqlite3_finalize(stmt);
        return NULL;
    }

    int count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        const char *destination_path = (const char *)sqlite3_column_text(stmt, 0);
        if (!destination_path) destination_path = "";
        res[count]=strdup(destination_path);
        if (res[count] == NULL) {
            for (int i = 0; i < count; i++) {
                free(res[i]);
            }
            free(res);
            return NULL;
        }

        count++;
    }
    if(count==0){
        sqlite3_finalize(stmt);
        free(res);
        return NULL;
    }
    if (count < size) {
        char** temp = (char**)realloc(res, count * sizeof(char*));
        if (temp) {
            res = temp;
        }
    }

    sqlite3_finalize(stmt);
    *arr_size = count;
    return res;
}

int get_destination_id(char* destination_folder){
    if(connect_db()!=0){
        return -100;
    }
    sqlite3_stmt *stmt;
    if(sqlite3_prepare_v2(db,"SELECT folder_id FROM destination WHERE folder_path =(?)",-1,&stmt,NULL)!=SQLITE_OK){ //ffs
        return -1;
    }
    if(sqlite3_bind_text(stmt,1,destination_folder,-1,SQLITE_TRANSIENT)!=SQLITE_OK){
        sqlite3_finalize(stmt);
        return -2;
    }
    if(sqlite3_step(stmt)!=SQLITE_ROW){
        sqlite3_finalize(stmt);
        return -3;
    }
    int destination_id = sqlite3_column_int(stmt,0);
    sqlite3_finalize(stmt);
    return destination_id;
}

char** get_sources(char* destination_folder, int* size){
    if(connect_db()!=0){
        return NULL;
    }


    sqlite3_stmt *stmt;
    int destination_id = get_destination_id(destination_folder);
    if(destination_id<1){
        return NULL;
    }
    if(sqlite3_prepare_v2(db,"SELECT count(*) FROM source INNER JOIN "
                               "link ON source.folder_id = link.source_folder_id WHERE "
                               "link.destination_folder_id = (?)",-1,&stmt,NULL)!=SQLITE_OK){
        return NULL;
    }
    if(sqlite3_bind_int(stmt,1,destination_id)!=SQLITE_OK){
        sqlite3_finalize(stmt);
        return NULL;
    }
    if(sqlite3_step(stmt)!=SQLITE_ROW){
        sqlite3_finalize(stmt);
        return NULL;
    }
    int folder_count = sqlite3_column_int(stmt,0);
    sqlite3_finalize(stmt);
    stmt = NULL;

    if(folder_count==0){
        return NULL;
    }
    char** res = (char**)malloc(folder_count*sizeof(char*));

    if(sqlite3_prepare_v2(db,"SELECT DISTINCT source.folder_path FROM source INNER JOIN "
                               "link ON source.folder_id = link.source_folder_id WHERE "
                               "link.destination_folder_id = (?)",-1,&stmt,NULL)!=SQLITE_OK){
        free(res);
        return NULL;
    }
    if(sqlite3_bind_int(stmt,1,destination_id)!=SQLITE_OK){
        free(res);
        sqlite3_finalize(stmt);
        return NULL;
    }
    int i=0;
    while(sqlite3_step(stmt)==SQLITE_ROW){
        const char* cur = (const char*)sqlite3_column_text(stmt,0);
        if(cur==NULL){
            continue;
        }
        else res[i] = strdup(cur);
        i++;
    }
    if(i==0){
        sqlite3_finalize(stmt);
        free(res);
        return NULL;
    }
    if (i < folder_count) {
        char** temp = (char**)realloc(res, i * sizeof(char*));
        if (temp) {
            res = temp;
        }
    }
    *size = i;
    sqlite3_finalize(stmt);
    return res;
}

filterPair* get_filters(char* destination_folder, int* size){
    if(connect_db()!=0){
        return NULL;
    }
    sqlite3_stmt *stmt;
    int destination_id = get_destination_id(destination_folder);
    if(destination_id<1){
        return NULL;
    }

    if(sqlite3_prepare_v2(db,"SELECT count(*) FROM filters INNER JOIN "
                               "link ON filters.filter_id = link.filter_id WHERE "
                               "link.destination_folder_id = (?)",-1,&stmt,NULL)!=SQLITE_OK){
        return NULL;
    }
    if(sqlite3_bind_int(stmt,1,destination_id)!=SQLITE_OK){
        sqlite3_finalize(stmt);
        return NULL;
    }
    if(sqlite3_step(stmt)!=SQLITE_ROW){
        sqlite3_finalize(stmt);
        return NULL;
    }
    int filters_count=sqlite3_column_int(stmt,0);
    sqlite3_finalize(stmt);
    stmt=NULL;


    filterPair* res = (filterPair*)malloc(filters_count*sizeof(filterPair));

    if(sqlite3_prepare_v2(db,"SELECT DISTINCT  filters.filter,filters.filter_type FROM filters INNER JOIN "
                               "link ON filters.filter_id = link.filter_id WHERE "
                               "link.destination_folder_id = (?)",-1,&stmt,NULL)!=SQLITE_OK){
        free(res);
        return NULL;
    }
    if(sqlite3_bind_int(stmt,1,destination_id)!=SQLITE_OK){
        sqlite3_finalize(stmt);
        free(res);
        return NULL;
    }
    int i = 0;
    while(sqlite3_step(stmt)==SQLITE_ROW){
        filterPair res_part;
        char *filter=(char*)sqlite3_column_text(stmt,0);
        char *type=(char*)sqlite3_column_text(stmt,1);
        if(filter==NULL || type==NULL){
            continue;
        }
        res_part.filter = strdup(filter);
        res_part.type=strdup(type);
        res[i]=res_part;
        i++;
    }
    if(i==0){
        sqlite3_finalize(stmt);
        free(res);
        return NULL;
    }
    if (i < filters_count) {
        filterPair* temp = (filterPair*)realloc(res, i * sizeof(filterPair));
        if (temp) {
            res = temp;
        }
    }
    *size = i;
    sqlite3_finalize(stmt);
    return res;
}

int destination(int id, char *path){
    // if(connect_db()==0){
    //     return 0;
    // }
    printf("path: %s\n",path);
    //fflush(stdout);
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
                                   "link.filter_id=filters.filter_id WHERE link.source_folder_id = (?) AND "
                                   "link.destination_folder_id = (?)",-1,&stmt2,NULL)!=SQLITE_OK){
            fprintf(stderr, "SQLite prepare error: %s\n", sqlite3_errmsg(db));
            //fflush(stderr);
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
            printf("%s",filter_value);
            //fflush(stdout);
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
        printf("%d",destination(id, path));
        printf("ID: %d, Path: %s\n", id, path);
        //fflush(stdout);
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
