#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "datetime.h"

const char *month_strings[12] = {
	"January", "February", "March", "April", "May", "June", "July",
	"August", "September", "October", "November", "December"
};

/* saves and displays the date and time on the system. */

typedef struct{
	int month;
	int day;
	int year;
} Date;

static Date date;

char output[128];
char time_string[64];

/* Sets date to system's actual date */
void InitDate(){
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	date.month = tm.tm_mon+1;
	date.day = tm.tm_mday;
	date.year = tm.tm_year+1900;
}

/* Sets date to custom user date */
int SetDate(int set_month, int set_day, int set_year){
	switch (set_month){
		case 1:
		case 3:
		case 5:
		case 7:
		case 8:
		case 10:
		case 12:
			/* 31 days in these months */
			if (set_day < 1 || set_day > 31){
				printf("\nERROR: There isn't a %d of %s!", 
					set_day, month_strings[set_month-1]);
				return -1;
			}
			break;
		case 4:
		case 6:
		case 9:
		case 11:
			/* 30 days in these months */
			if (set_day < 1 || set_day > 30){
				printf("\nERROR: There isn't a %d of %s!", 
					set_day, month_strings[set_month-1]);
				return -1;
			}
			break;
		case 2:
			/* lol february */
			if (set_day < 1 || set_day > 29){
				printf("\nERROR: There isn't a %d of %s!", 
					set_day, month_strings[set_month-1]);
			}
			if (set_day == 29){
				printf("\nThis program doesn't have a leap year handler, just this print statement.");
			}
			break;
		default:
			printf("\nERROR: Please enter a valid month.");
			return -1;
	}
	date.month 	= set_month;
	date.day 	= set_day;
	date.year 	= set_year;
	return 0;
}

/* Returns set date */
char* GetDate(char print_option){

	switch(print_option){

		case 'd':	// short date: 5/4/2009
			sprintf(output, "%d/%d/%d", 
				date.month, date.day, date.year);
			break;

		case 'D':	// long date: May 4, 2009
			sprintf(output, "%s %d, %d", 
				month_strings[date.month-1], date.day, date.year);
			break;

		case 'f':	// full (short time): May 4, 2009 1:45 PM
			sprintf(output, "%s %d, %d ",
				month_strings[date.month-1], date.day, date.year);
			strcat(output, GetTime('t'));
			break;

		case 'F':	// full (long time): May 4, 2009 1:45:30 PM
			sprintf(output, "%s %d, %d ",
				month_strings[date.month-1], date.day, date.year);
			strcat(output, GetTime('T'));
			break;

		case 'g':	// general (short time): 5/4/2009 1:45 PM
			sprintf(output, "%d/%d/%d ",
				date.month, date.day, date.year);
			strcat(output, GetTime('t'));
			break;

		case 'G':	// general (long time): 5/4/2009 1:45:30 PM
			sprintf(output, "%d/%d/%d ",
				date.month, date.day, date.year);
			strcat(output, GetTime('T'));
			break;

		case 'm':	// month/day: May 4
			sprintf(output, "%s %d", month_strings[date.month-1], date.day);
			break;

		case 's':	// sortable: 2009-05-04T13:45:30
			sprintf(output, "%.4d-%.2d-%.2dT", date.year, date.month, date.day);
			strcat(output, GetTime('S'));
			break;

		case 'y':	// year/month: May, 2009
			sprintf(output, "%s, %d", month_strings[date.month-1], date.year);
			break;
	}
	return output;
}

/* returns time */
char* GetTime(char print_option){

	time_t current_time;
	struct tm *local;

	current_time = time(NULL);
	local = localtime(&current_time);

	switch(print_option){
		case 't': // short time 1:45 PM
			strftime (time_string, sizeof (time_string), "%I:%M %p", local);
			break;
		case 'T': // long time 1:45:30 PM
			strftime (time_string, sizeof (time_string), "%I:%M:%S %p", local);
			break;
		case 'S': // sorted time 13:45:30
			strftime (time_string, sizeof (time_string), "%T", local);
			break;
	}
	return time_string;
}