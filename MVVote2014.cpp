// MVVote2014.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

//#include <iostream>
//#include <fstream>

//using namespace std;

#define MAX_LINE_LEN 1000
#define MAX_FIELD_LEN 50

typedef struct s_voter_data
{
    char voterid[MAX_FIELD_LEN];
    char last_name[MAX_FIELD_LEN];
    char first_name[MAX_FIELD_LEN];
    char street[MAX_FIELD_LEN];
    char zip[MAX_FIELD_LEN];
    char housenum[MAX_FIELD_LEN];
    char streetname[MAX_FIELD_LEN];
    char streetsuffix[MAX_FIELD_LEN];
    char phone[MAX_FIELD_LEN];
    char party[MAX_FIELD_LEN];
    char status[MAX_FIELD_LEN];
    char precinct[8];
    char precinctsuffix[6];
    char absentee[2];
    char gen11062012[2];
    char pri06052012[2];
    char gen11022010[2];
    char pri06082010[2];
    char mvrecall02022010[2];
    int  electioncount;
} voter_data;

int strtrim(char *instring, size_t strsize);
int write_outrec(voter_data *voter_ptr, FILE *outfile);
int write_outhdr(FILE *outfile);

int main(int argc, char* argv[])
{
    FILE *voterfile;
    FILE *electionsfile;
    FILE *outfile;

    voter_data voter_info;
    char voter_headers[MAX_LINE_LEN];
    char hist_headers[MAX_LINE_LEN];

    size_t line_size;
    size_t bufsize = MAX_LINE_LEN;
    char *line_ptr = (char *)malloc(MAX_LINE_LEN);
    char *hline_ptr = (char *)malloc(MAX_LINE_LEN);
    char delim_char;
    char *curpos;
    int curtabpos;
    int fieldlen;
    char *fieldptr;
    char infield[MAX_FIELD_LEN];

    unsigned int voteid;
    unsigned int histvoteid;
    char election_name[MAX_FIELD_LEN];
    char vote_type[MAX_FIELD_LEN];
    char voted[MAX_FIELD_LEN];
    bool first_read = true;
    size_t histline_size = -1;
    char *election_ptr;

    voterfile = fopen("./MVIE_04012014_sorted.csv", "r");
    electionsfile = fopen("./MVIE_Hist_04012014_sorted.csv", "r");

    outfile = fopen("./elec_out.csv", "w");
    write_outhdr(outfile);

    // read past header line at top of file
    line_size = getline(&line_ptr, &bufsize, voterfile);
    line_size = getline(&line_ptr, &bufsize, electionsfile);

    while ( ( (line_size = getline(&line_ptr, &bufsize, voterfile) ) != -1) )
    {
        curpos = line_ptr;
        curtabpos = 0;
        fieldlen = 0;
        fieldptr = infield;

        while ( curtabpos <= 44 )
        {
            // copy character to temp area until tab delimiter found
            while ( ( ( delim_char = (*fieldptr++ = *curpos++) ) != '\t') &&
                      ( delim_char != '\0') &&
                      ( delim_char != '\n') &&
                      (++fieldlen <= MAX_FIELD_LEN ));

            if ( (delim_char == '\t') || (delim_char == '\n') )
            {
                *--fieldptr = '\0'; //replace tab with null
            }

            switch (curtabpos)
            {
                case 0:
                    strncpy(voter_info.voterid, infield, sizeof(voter_info.voterid));
                    break;

                case 4:
                    strncpy(voter_info.last_name, infield, sizeof(voter_info.last_name));
                    break;

                case 5:
                    strncpy(voter_info.first_name, infield, sizeof(voter_info.first_name));
                    break;

                case 9:
                    strncpy(voter_info.street, infield, sizeof(voter_info.street));
                    break;

                case 12:
                    strncpy(voter_info.zip, infield, sizeof(voter_info.zip));
                    break;

                case 13:
                    strncpy(voter_info.housenum, infield, sizeof(voter_info.housenum));
                    break;

                case 16:
                    strncpy(voter_info.streetname, infield, sizeof(voter_info.streetname));
                    break;

                case 17:
                    strncpy(voter_info.streetsuffix, infield, sizeof(voter_info.streetsuffix));
                    break;

                case 25:
                    strncpy(voter_info.phone, infield, sizeof(voter_info.phone));
                    break;

                case 38:
                    strncpy(voter_info.party, infield, sizeof(voter_info.party));
                    strtrim(voter_info.party, sizeof(voter_info.party));
                    break;

                case 39:
                    strncpy(voter_info.status, infield, sizeof(voter_info.status));
                    break;

                case 42:
                    strncpy(voter_info.precinct, infield, sizeof(voter_info.precinct));
                    break;

                case 43:
                    strncpy(voter_info.precinctsuffix, infield, sizeof(voter_info.precinctsuffix));
                    break;

            }

            curtabpos++;
            fieldlen = 0;
            fieldptr = infield;
        }

        voteid = atoi(voter_info.voterid);
        strcpy (voter_info.mvrecall02022010, "N");
        strcpy (voter_info.pri06082010, "N");
        strcpy (voter_info.gen11022010, "N");
        strcpy (voter_info.pri06052012, "N");
        strcpy (voter_info.gen11062012, "N");
        voter_info.electioncount = 0;

        if ( !first_read && (histline_size == -1) )
        {
            voter_info.electioncount = 0;
        }
        // iterate through history records for this voter
        // scan sorted history file for relevant elections
        while ( (first_read || (histline_size != -1) ) )
        {
            // scan sorted history file for relevant elections

            if ( first_read )
            {
                histline_size = getline(&hline_ptr, &bufsize, electionsfile);
                if ( !histline_size )
                {
                    return 20;  // history file empty
                }
                first_read = false;
            }

            // process the next history record
            curpos = hline_ptr;
            curtabpos = 0;
            fieldlen = 0;
            fieldptr = infield;

            while ( curtabpos <= 9 )
            {
                // copy character to temp area until tab delimiter found
                while ( ( ( delim_char = (*fieldptr++ = *curpos++) ) != '\t') &&
                          ( delim_char != '\0') &&
                          ( delim_char != '\n') &&
                          (++fieldlen <= MAX_FIELD_LEN ));

                if ( (delim_char == '\t') || (delim_char == '\n') )
                {
                    *--fieldptr = '\0'; //replace tab with null
                }

                if ( curtabpos == 0 )
                {
                    histvoteid = atoi(infield); 
                }

                // process matching history record
                if ( histvoteid > voteid )
                {
                    break; // curtabpos <= 9
                }

                switch (curtabpos)
                {
                    case 1:
                        strncpy(election_name, infield, sizeof(election_name));
                        strtrim(election_name, sizeof(election_name));
                        break;

                    case 6:
                        strncpy(vote_type, infield, sizeof(vote_type));
                        strtrim(vote_type, sizeof(vote_type));
                        break;

                    case 9:
                        strncpy(voted, infield, sizeof(voted));
                        break;
                }

                curtabpos++;
                fieldlen = 0;
                fieldptr = infield;
            }

            // process matching history record
            if ( histvoteid == voteid )
            {

                // classify the election record
                if ( !strcmp(election_name, "MVRCL10") )
                {
                    election_ptr = voter_info.mvrecall02022010;
                }
                else if ( !strcmp(election_name, "Pri2010") )
                {
                    election_ptr = voter_info.pri06082010;
                }
                else if ( !strcmp(election_name, "Gen2010") )
                {
                    election_ptr = voter_info.gen11022010;
                }
                else if ( !strcmp(election_name, "Pri2012") )
                {
                    election_ptr = voter_info.pri06052012;
                }
                else if ( !strcmp(election_name, "Gen2012") )
                {
                    election_ptr = voter_info.gen11062012;
                }
                else
                {
                    election_ptr = NULL;
                }

                if ( election_ptr )
                {
                    if ( !strcmp(vote_type, "Voted at Polling Place") )
                    {
                        strcpy(election_ptr, "Y");
                        voter_info.electioncount++;
                    }
                    else if ( !strcmp(vote_type, "Voted by Absentee Ballot")     ||
                              !strcmp(vote_type, "Voted by Mail Ballot")         || 
                              !strcmp(vote_type, "Voted by Vote-by-Mail Ballot")   )
                    {
                        strcpy(election_ptr, "A");
                        voter_info.electioncount++;
                    }
                    else if ( !strcmp(vote_type, "Provisional Voter") )
                    {
                        strcpy(election_ptr, "P");
                        voter_info.electioncount++;
                    }
                    else if ( !strcmp(vote_type, "Voted a Fail-Safe Ballot") )
                    {
                        strcpy(election_ptr, "F");
                        voter_info.electioncount++;
                    }
                    else
                    {
                        strcpy(election_ptr, "N");
                    }
                }

                // read the next history record
                histline_size = getline(&hline_ptr, &bufsize, electionsfile);
            }
            else if ( histvoteid > voteid )
            {
                write_outrec(&voter_info, outfile);
                strcpy (voter_info.mvrecall02022010, "N");
                strcpy (voter_info.pri06082010, "N");
                strcpy (voter_info.gen11022010, "N");
                strcpy (voter_info.pri06052012, "N");
                strcpy (voter_info.gen11062012, "N");
                voter_info.electioncount = 0;
                break; // (first_read || histline_size)
            }
            else
            {
                // read the next history record - no votor record for this history record
                histline_size = getline(&hline_ptr, &bufsize, electionsfile);
                
            }
        }
    }

    // check for to see if last voter record found must be written
    write_outrec(&voter_info, outfile);
    fclose(outfile);

    fclose(electionsfile);
    fclose(voterfile);
    return 0;
}


int strtrim(char *instring, size_t strsize)
{
    if ( (instring == NULL) || (*instring == '\0') )
    {
        return 0;
    }

    char *holdstr = (char *)malloc(strsize); 

    if ( holdstr == NULL )
    {
        return -1;
    }

    char *curposhold = holdstr;
    char *curposin = instring;

    strcpy(holdstr, instring);

    // skip leading spaces
    while ( *curposhold == ' ' )
    {
        curposhold++;
    }

    // copy string until null
    while ( (*curposin++ = *curposhold++) );
    curposin--; // back off to null from post increment
    curposin--; // now back up to last character before null

    // backup to non space
    while ( *curposin == ' ' )
    {
        curposin--;
    }

    *++curposin = '\0';

    free(holdstr);
    return(strlen(instring));
}

int write_outhdr(FILE *outfile)
{
	char outbuf[MAX_LINE_LEN];

    snprintf(outbuf, MAX_LINE_LEN,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s\n",
                                   "VoterID", //voter_ptr->voterid,
                                   "LastName", // voter_ptr->last_name,
                                   "FirstName", //voter_ptr->first_name,
                                   "HouseNum", //voter_ptr->housenum,
                                   "StreetName", //voter_ptr->streetname, voter_ptr->streetsuffix,
                                   "ZipCode", //voter_ptr->zip,
                                   "Phone", // voter_ptr->phone,
                                   "Party", //voter_ptr->party,
                                   "Status", //voter_ptr->status,
                                   "Precinct", //voter_ptr->precinct, voter_ptr->precinctsuffix,
                                   "Elections"); //voter_ptr->mvrecall02022010,
                                   //voter_ptr->pri06082010,
                                   //voter_ptr->gen11022010,
                                   //voter_ptr->pri06052012,
                                   //voter_ptr->gen11062012);
    fwrite(outbuf, strlen(outbuf), 1, outfile);

    return 0;
}

int write_outrec(voter_data *voter_ptr, FILE *outfile)
{
	char outbuf[MAX_LINE_LEN];

    if ( (voter_ptr->electioncount >= 4) &&
         ( (!strcmp(voter_ptr->party, "Republican") || (!strcmp(voter_ptr->party, "No Party Preference"))) ) )
    {
        // write output record
        snprintf(outbuf, MAX_LINE_LEN,"%s,%s,%s,%s,%s %s,%s,%s,%s,%s,%s%s,%s%s%s%s%s\n",
                                       voter_ptr->voterid,
                                       voter_ptr->last_name,
                                       voter_ptr->first_name,
                                       voter_ptr->housenum,
                                       voter_ptr->streetname, voter_ptr->streetsuffix,
                                       voter_ptr->zip,
                                       voter_ptr->phone,
                                       voter_ptr->party,
                                       voter_ptr->status,
                                       voter_ptr->precinct, voter_ptr->precinctsuffix,
                                       voter_ptr->mvrecall02022010,
                                       voter_ptr->pri06082010,
                                       voter_ptr->gen11022010,
                                       voter_ptr->pri06052012,
                                       voter_ptr->gen11062012);
        fwrite(outbuf, strlen(outbuf), 1, outfile);
        return 0;
    }

    return 1;
}
