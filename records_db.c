#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "prog2_ex1.h"

#define MIN_YEAR 1900

typedef enum e_bool {false, true} bool;

typedef struct {
    char* record_name;
    char* track_name;
    unsigned int length;
    unsigned int position;
} track;

typedef struct {
    char *name;
    unsigned int year;
    unsigned int max_num_of_tracks;
    unsigned int curr_num_of_tracks;
    track **tracks;
    record_category category;
} record;

typedef struct {
    record *records[MAX_RECORDS];
    unsigned int num_of_records;
} records_db;


// void readRecordsFromFile(FILE* fp_in, FILE* fp_out, records_db *R);
void loadRecords(FILE* fp_in, FILE* fp_out, records_db *R);
void loadRecordsFromFile(FILE* fp_in, FILE* fp_out, records_db *R);
void freeAndExit(FILE* in, FILE* out, records_db *R);
void freeAllRecords(records_db *R);
void sortRecordsByName(records_db *R);
records_result addRecord(records_db *R, char name[],int year, int max_num_of_tracks, char category[]);
records_result addTrack(records_db *R, char record_name[], char track_name[], int track_len, int position);
records_result removeRecordByName(records_db *R, char name[]);
records_result reportRecords(records_db *R, FILE* fp_out, char category[]);
records_result reportTracks(records_db *R, FILE* fp_out, char record_name[]);
records_result reportContainingTrack(records_db *R, FILE* fp_out, char track_name[]);
record *findRecordByName(records_db *R, char* name);
record *findRecordByCategory(records_db *R, record_category category);
bool findTrackByName(record *record, char* name);
record_category stringToCategory(char category[]);


int main(int argc, char *argv[]){
    
    char flag[3];
    FILE* in = stdin;
    FILE* out = stdout;
    records_db R = {{ NULL }, 0};

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i")==0) {
          strcpy(flag, "-i");
          if(!argv[i+1]){
            prog2_report_error_message(INVALID_ARGUMENTS);
            freeAndExit(in, out, &R);
          }
          continue;
        } else if (strcmp(argv[i], "-o")==0){
          strcpy(flag, "-o");
          if(!argv[i+1]){
            prog2_report_error_message(INVALID_ARGUMENTS);
            freeAndExit(in, out, &R);
          }
          continue;
        } 
        if (strcmp(flag,"-i")==0){
            in = fopen(argv[i], "r");
            if (in==NULL){
                prog2_report_error_message(CANNOT_OPEN_FILE);
                freeAndExit(in, out, &R);
            }
            strcpy(flag, "");
        } else if (strcmp(flag,"-o")==0){
            out = fopen(argv[i], "w");
            if (out==NULL){
                prog2_report_error_message(CANNOT_OPEN_FILE);
                freeAndExit(in, out, &R);
            } 
            strcpy(flag, "");
        } else {
            prog2_report_error_message(INVALID_ARGUMENTS);
            freeAndExit(in, out, &R);
        }
    }
    
    loadRecords(in,out,&R);
   
    freeAndExit(in,out, &R);
    return 0;
}

void loadRecords(FILE* fp_in, FILE* fp_out, records_db *R){
    records_result res = SUCCESS;

    char line[MAX_LEN];
    char record_name[MAX_LEN];
    char track_name[MAX_LEN];
    char category[MAX_LEN];
    unsigned int year = MIN_YEAR;
    unsigned int max_num_of_tracks = 0;
    unsigned int length = 0;
    unsigned int position = 0;

    while(fgets(line, MAX_LEN, fp_in)){
        char* token = strtok(line, " \n");

        if(token == NULL){
            continue;
        }

        if(strcmp(token, "Add")==0){
            token = strtok(NULL, " \r\n");
            if(strcmp(token,"Record")==0){
                token = strtok(NULL, " \r\n");
                strcpy(record_name,token);
                token = strtok(NULL, " \r\n");
                year = atoi(token);
                token = strtok(NULL, " \r\n");
                max_num_of_tracks = atoi(token);
                token = strtok(NULL, " \r\n");
                strcpy(category,token);
                res = addRecord(R, record_name, year, max_num_of_tracks, category);
            } else if(strcmp(token,"Track")==0){
                token = strtok(NULL, " \r\n");// To
                token = strtok(NULL, " \r\n");//Record
                token = strtok(NULL, " \r\n");
                strcpy(record_name,token);
                token = strtok(NULL, " \r\n");
                strcpy(track_name,token);
                token = strtok(NULL, " \r\n");
                length = atoi(token);
                token = strtok(NULL, " \r\n");

                if(token != NULL) {
                    position = atoi(token);
                } else {
                    position = 0;
                }

                res = addTrack(R, record_name, track_name, length, position);
            }
        } else if(strcmp(token,"Remove")==0){
            token = strtok(NULL, " \r\n");
            if(strcmp(token,"Record")==0){
                token = strtok(NULL, " \r\n");
                strcpy(record_name,token);
                res = removeRecordByName(R, record_name);
            }
        } else if(strcmp(token, "Report")==0){
            token = strtok(NULL, " \r\n");
            if(strcmp(token, "Records")==0){
                token = strtok(NULL, " \r\n");
                if(token != NULL){
                    strcpy(category, token);
                } else {
                    strcpy(category, "All-Categories");
                }
                res = reportRecords(R, fp_out, category);
            } else if(strcmp(token, "Tracks")==0){
                token = strtok(NULL, " \r\n");
                strcpy(record_name, token);
                res = reportTracks(R, fp_out, record_name);
            } else if(strcmp(token,"Containing")==0){
                token = strtok(NULL, " \r\n");//Records
                token = strtok(NULL, " \r\n");
                strcpy(track_name, token);
                res = reportContainingTrack(R, fp_out, track_name);
            }
        }
        if(res != SUCCESS){
            prog2_report_error_message(res);
            if (res == OUT_OF_MEMORY) freeAndExit(fp_in,fp_out,R);
        }
        res = SUCCESS;
    }
}

records_result addRecord(records_db *R, char name[],int year, int max_num_of_tracks,char category[]){
    
    record_category r_category;

    if(year < MIN_YEAR){
        return INVALID_YEAR;
    }

    if(max_num_of_tracks < 0){
        return INVALID_NUM_OF_TRACKS;
    }
    
    r_category = stringToCategory(category);
    if(r_category>ALL_CATEGORIES){
        return INVALID_CATEGORY;
    }
    
    if(findRecordByName(R, name)){
        return RECORD_ALREADY_EXISTS;
    }

    record *new_record = (record *)malloc(sizeof(record));
    if(new_record == NULL){
        return OUT_OF_MEMORY;
    }
    memset(new_record,0,sizeof(record));
    
    new_record->name = (char *)malloc(strlen(name)+1);
    if(new_record->name == NULL){
        free(new_record);
        return OUT_OF_MEMORY;
    }
    memset(new_record->name,0,strlen(name)+1);
 
    new_record->tracks = (track **)malloc(sizeof(track*) * max_num_of_tracks);
    if(new_record->tracks == NULL){
        free(new_record->name);
        free(new_record);
        return OUT_OF_MEMORY;
    }
    memset(new_record->tracks,0,sizeof(track*) * max_num_of_tracks);
    
    strcpy(new_record->name, name);
    new_record->max_num_of_tracks = max_num_of_tracks;
    new_record->curr_num_of_tracks = 0;
    new_record->year = year;
    new_record->category = r_category;
    
    R->records[R->num_of_records] = new_record;
    R->num_of_records++;
    
    return SUCCESS;
}

records_result addTrack(records_db *R, char record_name[], char track_name[], int track_len, int position){
    
    record *curr_rec = findRecordByName(R, record_name);
    track *new_track;

    if(!curr_rec){
        return RECORD_DOESNT_EXIST;
    }
    if(track_len < 0){
        return INVALID_TRACK_LENGTH;
    }
    if(curr_rec->curr_num_of_tracks < position || position < 0){
        return INVALID_TRACK_POSITION;
    }
    if(curr_rec->curr_num_of_tracks >= curr_rec->max_num_of_tracks){
        return TRACK_OVERFLOW;
    }
    if(findTrackByName(curr_rec, track_name)){
        return TRACK_ALREADY_EXISTS;
    }
    
    new_track = (track *)malloc(sizeof(track));
    if(new_track == NULL) {
        return OUT_OF_MEMORY;
    }
    memset(new_track,0,sizeof(track));
    
    new_track->track_name = (char *)malloc(strlen(track_name)+1);
    if(new_track->track_name == NULL) {
        free(new_track);
        return OUT_OF_MEMORY;
    }
    memset(new_track->track_name,0,strlen(track_name)+1);

    new_track->record_name = (char *)malloc(strlen(record_name)+1);
    if(new_track->record_name == NULL) {
        free(new_track->track_name);
        free(new_track);
        return OUT_OF_MEMORY;
    }
    memset(new_track->record_name,0,strlen(record_name)+1);

    strcpy(new_track->record_name, record_name);
    strcpy(new_track->track_name,track_name);
    new_track->length = track_len;
    new_track->position = position;

    for (int i = curr_rec->curr_num_of_tracks; i > position; i--) {
        curr_rec->tracks[i] = curr_rec->tracks[i-1];
        curr_rec->tracks[i]->position = i;
    }

    curr_rec->tracks[position] = new_track;
    curr_rec->curr_num_of_tracks++;

    return SUCCESS;
}

records_result removeRecordByName(records_db *R, char name[]){
    
    int record_index = -1;
    record *curr_rec;
   
    for (int i = 0; i < MAX_RECORDS; i++) {
        if (R->records[i] != NULL && strcmp(R->records[i]->name, name) == 0) {
            record_index = i;
            break;
        }
    }

    if(record_index == -1){
        return RECORD_DOESNT_EXIST; 
    }

    curr_rec = R->records[record_index];

    free(curr_rec->name);

    for (int i = 0; i < curr_rec->curr_num_of_tracks; i++) {
        if(curr_rec->tracks[i] != NULL){
            free(curr_rec->tracks[i]->record_name);
            free(curr_rec->tracks[i]->track_name);
            free(curr_rec->tracks[i]);
        }
    }
    free(curr_rec->tracks);
    free(curr_rec);
    R->num_of_records--;

    for (int i = record_index + 1; i < MAX_RECORDS && R->records[i] != NULL; i++) {
        R->records[i - 1] = R->records[i];
        R->records[i] = NULL;
    }

    return SUCCESS;
    


}

void freeAllRecords(records_db *R) {
    record *curr_rec;
    track *curr_track;
    for (int i = 0; i < MAX_RECORDS; i++) {
        curr_rec = R->records[i];
        if (curr_rec != NULL) {
            free(curr_rec->name);
            if(curr_rec->tracks != NULL){
                for (int j = 0; j < curr_rec->max_num_of_tracks; j++) {
                    curr_track = curr_rec->tracks[j];
                    if(curr_track != NULL){
                        free(curr_track->record_name);
                        free(curr_track->track_name);
                        free(curr_track);
                    } 
                }
                free(curr_rec->tracks);
            }
            free(curr_rec);
        }
    }
}

records_result reportRecords(records_db *R, FILE* fp_out, char category[]){

    record_category curr_category = stringToCategory(category);
    record *curr_rec;
    track *curr_track;
    unsigned int count = 0;

    if (R->num_of_records == 0){
        return NO_RECORDS;
    }
    
    if(curr_category == -1){
        return INVALID_CATEGORY;
    }

    sortRecordsByName(R);

    for (int i = 0; i < R->num_of_records; i++) {
        curr_rec = R->records[i];
        if(curr_category == ALL_CATEGORIES || curr_rec->category == curr_category){
            prog2_report_record(fp_out, curr_rec->name ,curr_rec->year, curr_rec->curr_num_of_tracks, curr_rec->category);
            for (int j = 0; j < curr_rec->curr_num_of_tracks; j++) {
                curr_track = curr_rec->tracks[j];
                prog2_report_track(fp_out, curr_track->track_name, curr_track->position, curr_track->length);
            }
            count++;
        }
    }

    if(count == 0){
        return NO_RECORDS;
    }

    return SUCCESS;
}

records_result reportTracks(records_db *R,FILE* fp_out, char record_name[]){

    record *curr_rec = findRecordByName(R,record_name);
    track *curr_track;

    if(curr_rec == NULL) return RECORD_DOESNT_EXIST;

    prog2_report_record(fp_out, curr_rec->name, curr_rec->year, curr_rec->curr_num_of_tracks, curr_rec->category);

    if(curr_rec->curr_num_of_tracks == 0) return NO_TRACKS;

    for (int i = 0; i < curr_rec->curr_num_of_tracks; i++) {
        curr_track = curr_rec->tracks[i];
        prog2_report_track(fp_out, curr_track->track_name, curr_track->position, curr_track->length);
    }
    
    return SUCCESS;
}

records_result reportContainingTrack(records_db *R, FILE* fp_out , char track_name[]){

    if(R->num_of_records == 0) return NO_RECORDS;
    
    record* curr_rec;
    unsigned int count = 0;
    
    for (int i = 0; i < R->num_of_records; i++){
        curr_rec = R->records[i];
        
        if(findTrackByName(curr_rec, track_name)){
            prog2_report_record(fp_out, curr_rec->name, curr_rec->year, curr_rec->curr_num_of_tracks, curr_rec->category);
            count++;
        } 
    }
    
    if(count==0) return TRACK_DOESNT_EXIST;

    return SUCCESS;
}

void freeAndExit(FILE* in, FILE* out, records_db *R){
    freeAllRecords(R);
    fclose(in);
    fclose(out);
    exit(EXIT_FAILURE);
}

record *findRecordByName(records_db *R, char* name) {
    for (int i = 0; i < R->num_of_records; i++) {
        if (R->records[i] != NULL && strcmp(R->records[i]->name,name)==0){
            return R->records[i];
        }
    }
    return NULL;
}

record *findRecordByCategory(records_db *R, record_category category){
    for (int i = 0; i < R->num_of_records; i++) {
        if(R->records[i] != NULL && R->records[i]->category == category){
            return R->records[i];
        }
    }
    return NULL;
}

bool findTrackByName(record *record, char* track_name){
    for (int i = 0; i < record->curr_num_of_tracks; i++) {
        if(record->tracks[i] != NULL && strcmp(record->tracks[i]->track_name, track_name)==0){
            return true;
        }
    }
    return false;  
}

record_category stringToCategory(char category[]){

    record_category r_category = -1;
    
    if(strcmp(category,"Rock")==0) r_category=ROCK;
    if(strcmp(category,"Pop")==0) r_category=POP;
    if(strcmp(category,"Jazz")==0) r_category=JAZZ;
    if(strcmp(category,"Blues")==0) r_category=BLUES;
    if(strcmp(category,"Hip-Hop")==0) r_category=HIP_HOP;
    if(strcmp(category,"Country")==0) r_category=COUNTRY;
    if(strcmp(category,"Classic")==0) r_category=CLASSIC;
    if(strcmp(category,"All-Categories")==0) r_category=ALL_CATEGORIES;

    return r_category;
}

void sortRecordsByName(records_db *R){
    int i, j;
    record *temp;
    for (i = 0; i < R->num_of_records; i++){
        for (j = 0; j < R->num_of_records - i - 1; j++){
            if(R->records[j] != NULL && R->records[j+1] != NULL){
                if(strcmp(R->records[j]->name,R->records[j+1]->name) > 0){
                    temp = R->records[j];
                    R->records[j] = R->records[j+1];
                    R->records[j+1] = temp;
                }
            }
        }  
    }
}

/*
// void loadRecords(FILE* fp_in, FILE* fp_out, records_db *R){

//     records_result res = SUCCESS;

//     char line[MAX_LEN];
//     char record_name[MAX_LEN];
//     char track_name[MAX_LEN];
//     char category[MAX_LEN];
//     unsigned int year = MIN_YEAR;
//     unsigned int max_num_of_tracks = 0;
//     unsigned int length = 0;
//     unsigned int position = 0;

//     while(fgets(line,MAX_LEN,fp_in)){
        
//         char* ptr = line;
//         while (*ptr && isspace(*ptr)) {
//             ptr++;
//         }
        
//         if(strncmp(ptr, "Add Record ", 11) == 0){
//             sscanf(ptr + 11," %s %u %u %s", record_name, &year, &max_num_of_tracks, category);
//             res = addRecord(R, record_name, year, max_num_of_tracks, category);

//         } else if(strncmp(ptr, "Add Track To Record", 19) == 0){
            
//             if(sscanf(ptr + 19," %s %s %u %u", record_name, track_name, &length, &position) == 3){
//                 position = 0;
//             }
//             res = addTrack(R, record_name, track_name, length, position);

//         } else if(strncmp(ptr, "Remove Record", 13) == 0){
//             sscanf(ptr+13," %s", record_name);
//             res = removeRecordByName(R,record_name);

//         } else if(strncmp(ptr, "Report Records", 14) == 0){
//             if(sscanf(ptr+14, "%s", category) != 1){
//                 strcpy(category,"All-Categories");
//             }
//             res = reportRecords(R, fp_out, category);
    
//         } else if(strncmp(ptr, "Report Tracks", 13) == 0){
//             sscanf(ptr+13, "%s", record_name);
//             res = reportTracks(R, fp_out, record_name);

//         } else if(strncmp(ptr, "Report Containing Records", 25) == 0){
//             sscanf(ptr+25, "%s", track_name);
//             res = reportContainingTrack(R, fp_out, track_name);
//         }

//         if(res != SUCCESS) {
//             prog2_report_error_message(res);
//             if(res == OUT_OF_MEMORY) freeAndExit(fp_in,fp_out, R);
//         }
//         res = SUCCESS;
//     }
// }
*/
