#include "shell.h"

/**
 * create_date - converts tm struct integers into strings
 * Return: today's date in Weekday Month Date format
 */
char *create_date(void)
{
	struct tm *myTime;
	time_t currentTime;
	char *month, *weekday, *day;
	char *WMD = NULL;

	time(&currentTime);
	myTime = localtime(&currentTime);

	weekday = its_weekday(myTime->tm_wday);
	month = its_month(myTime->tm_mon);
	day = convert_number(myTime->tm_mday - 1, 10, 0);

	WMD = _strdup(weekday);
	_strcat(WMD, " ");
	_strcat(WMD, month);
	_strcat(WMD, " ");
	if (_strlen(day) < 2)
		_strcat(WMD, "0");
	_strcat(WMD, day);

	return (WMD);
}

/**
 * create_time - converts tm struct integers into strings
 * @seconds: flag for printing seconds or not
 * Return: current time
 */
char *create_time(int seconds)
{
	struct tm *myTime;
	time_t currentTime;
	char *_time = NULL, *hour, *minute, *second;

	time(&currentTime);
	myTime = localtime(&currentTime);

	hour = convert_number(myTime->tm_hour + 1, 10, 0);
	minute = convert_number(myTime->tm_min, 10, 0);
	second = convert_number(myTime->tm_sec, 10, 0);

	if (_strlen(hour) < 2)
	{
		_time = _strdup("0");
		_strcat(_time, hour);
	}
	else
		_time = _strdup(hour);

	_strcat(_time, ":");
	_strcat(_time, minute);
	if (seconds)
	{
		_strcat(_time, ":");
		_strcat(_time, second);
	}
	return (_time);
}
/**
 * its_weekday - converts int from tm struct to string
 * @weekday_int: weekday in ints
 * Return: weekday string
 */
char *its_weekday(int weekday_int)
{
	char *weekdays[7];
	char *weekday_string;

	weekdays[0] = "Sunday";
	weekdays[1] = "Monday";
	weekdays[2] = "Tuesday";
	weekdays[3] = "Wednesday";
	weekdays[4] = "Thursday";
	weekdays[5] = "Friday";
	weekdays[6] = "Saturday";

	weekday_string = weekdays[weekday_int];
	return (weekday_string);
}

/**
 * its_month - converts ints from tm struct to string
 * @month_int: month in integer
 * Return: month string
 */
char *its_month(int month_int)
{
	char *months[12];
	char *month_string;

	months[0] = "January";
	months[1] = "February";
	months[2] = "March",
	months[3] = "April";
	months[4] = "May";
	months[5] = "June";
	months[6] = "July",
	months[7] = "August";
	months[8] = "September";
	months[9] = "October";
	months[10] = "November",
	months[11] = "December";

	month_string = months[month_int];
	return (month_string);
}
