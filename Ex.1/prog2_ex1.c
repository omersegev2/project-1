#include "prog2_ex1.h"

void prog2_report_error_message(records_result res)
{
    if (res == 0)
    return;
    fprintf(stderr, "Error: ");
    switch (res)
    {
    case OUT_OF_MEMORY:
        fprintf(stderr, "Out Of Memory");
        break;
    case INVALID_ARGUMENTS:
        fprintf(stderr, "Invalid Arguments");
        break;
    case CANNOT_OPEN_FILE:
        fprintf(stderr, "Cannot Open File");
        break;
    case INVALID_YEAR:
        fprintf(stderr, "Invalid Year");
        break;
    case INVALID_NUM_OF_TRACKS:
        fprintf(stderr, "Invalid Number Of Tracks");
        break;
    case INVALID_CATEGORY:
        fprintf(stderr, "Invalid Category");
        break;
    case RECORD_ALREADY_EXISTS:
        fprintf(stderr, "Record Already Exists");
        break;
    case RECORD_DOESNT_EXIST:
        fprintf(stderr, "Record Doesn't Exist");
        break;
    case NO_RECORDS:
        fprintf(stderr, "No Records");
        break;
    case INVALID_TRACK_LENGTH:
        fprintf(stderr, "Invalid Track Length");
        break;
    case INVALID_TRACK_POSITION:
        fprintf(stderr, "Invalid Track Position");
        break;
    case TRACK_ALREADY_EXISTS:
        fprintf(stderr, "Track Already Exists");
        break;
    case TRACK_OVERFLOW:
        fprintf(stderr, "Track Overflow");
        break;
    case NO_TRACKS:
        fprintf(stderr, "No Tracks");
        break;
    case TRACK_DOESNT_EXIST:
        fprintf(stderr, "Track Doesn't Exist");
        break;
    default:
        fprintf(stderr, "Unknown Error");
        break;
    }
    fprintf(stderr, "\n");
}

void prog2_report_record(FILE *out, const char *name, unsigned int year, unsigned int num_of_tracks, record_category category)
{
    const char *cat_str = NULL;

    if (!out || !name)
        return;

    switch (category)
    {
    case ROCK:
        cat_str = "Rock";
        break;
    case POP:
        cat_str = "Pop";
        break;
    case JAZZ:
        cat_str = "Jazz";
        break;
    case BLUES:
        cat_str = "Blues";
        break;
    case HIP_HOP:
        cat_str = "Hip Hop";
        break;
    case COUNTRY:
        cat_str = "Country";
        break;
    case CLASSIC:
        cat_str = "Classic";
        break;
    default:
        cat_str = "Unknown Category";
        break;
    }
    fprintf(out, "%s RECORD: %s from year %u contains %u tracks\n", cat_str, name, year, num_of_tracks);
}


void prog2_report_track(FILE *out, const char *name, unsigned int position, unsigned int length)
{
    if (!out || !name)
        return;

    fprintf(out, "TRACK %03u: <%24s> %03u seconds long\n", position, name, length);
}
