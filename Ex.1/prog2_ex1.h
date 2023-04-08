#ifndef __PROG2_EX1_H__
#define __PROG2_EX1_H__

#include <stdio.h>

#define MAX_LEN 256
#define MAX_RECORDS 100

/*
 * This file contains the function prototypes for the functions you need to
 * implement for the first exercise of programming2 course.
 * You should not change this file.
 *
 * Author:  David Avdat
 */

/******************************************************************************
 * enum records_result_e
 */ 
typedef enum records_result_e {
    SUCCESS,
    INVALID_ARGUMENTS,
    CANNOT_OPEN_FILE,
    INVALID_YEAR,
    INVALID_NUM_OF_TRACKS,
    INVALID_CATEGORY,
    RECORD_ALREADY_EXISTS,
    RECORD_DOESNT_EXIST,
    NO_RECORDS,
    INVALID_TRACK_LENGTH,
    INVALID_TRACK_POSITION,
    TRACK_ALREADY_EXISTS,
    TRACK_OVERFLOW,
    NO_TRACKS,
    TRACK_DOESNT_EXIST,
    OUT_OF_MEMORY,
} records_result;

/******************************************************************************
 * enum record_category_e
 */
typedef enum record_category_e {
    ROCK,
    POP,
    JAZZ,
    BLUES,
    HIP_HOP,
    COUNTRY,
    CLASSIC, 
    ALL_CATEGORIES,
} record_category;

/******************************************************************************
 * Function: prog2_report_error_message
 * Description: This function prints an error message to the standard error
 *             output.
 * Parameters:
 *     result - the error code
 * Returns: Nothing.
*/
void prog2_report_error_message(records_result result);


/******************************************************************************
 * Function: prog2_report_record
 * Description: This function prints record details to a given output stream.
 * Parameters:
 *    out - the output stream
 *    name - the record name
 *    year - the record year
 *    num_of_tracks - the number of tracks
 *    category - the record category
 * Returns: Nothing.
 */ 
 void prog2_report_record(FILE *out, const char *name, unsigned int year, unsigned int num_of_tracks_added, record_category category);


/******************************************************************************
 * Function: prog2_report_track
 * Description: This function prints track details to a given output stream.
 * Parameters:
 *   out - the output stream
 *   name - the track name
 *   position - the track position
 *   duration - the track length
 * Returns: Nothing.
 */
void prog2_report_track(FILE *out, const char *name, unsigned int position, unsigned int duration);

#endif /* __PROG2_EX1_H__ */
