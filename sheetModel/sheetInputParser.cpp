//-------------------------------------------------------------------------------------------------
//
//  sheetInputParser.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  Post-commit input parsing for spreadsheet cells.
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "sheetInputParser.h"


//-------------------------------------------------------------------------------------------------
// Date constants (Excel-compatible)
//-------------------------------------------------------------------------------------------------

// Days from Jan 1, 1900 to Unix epoch (Jan 1, 1970)
// Includes Excel's leap year bug (Feb 29, 1900 counted)
static const double EXCEL_EPOCH_OFFSET = 25569.0;
static const double SECONDS_PER_DAY = 86400.0;


//-------------------------------------------------------------------------------------------------
// Helper: check if character is a digit
//-------------------------------------------------------------------------------------------------
static int isDigit(char c)
{
    return (c >= '0' && c <= '9');
}


//-------------------------------------------------------------------------------------------------
// Helper: parse integer from string, advancing pointer
//-------------------------------------------------------------------------------------------------
static int parseIntAdvance(const char **p, int *value)
{
    if (!isDigit(**p)) {
        return 0;
    }
    *value = 0;
    while (isDigit(**p)) {
        *value = (*value * 10) + (**p - '0');
        (*p)++;
    }
    return 1;
}


//-------------------------------------------------------------------------------------------------
// CxSheetInputParser::tryParseNumber
//-------------------------------------------------------------------------------------------------
int
CxSheetInputParser::tryParseNumber(CxString input, double *value,
                                   int *hasCurrency, int *hasPercent, int *hasThousands)
{
    if (input.length() == 0) {
        return 0;
    }

    *hasCurrency = 0;
    *hasPercent = 0;
    *hasThousands = 0;

    const char *p = input.data();
    const char *end = p + input.length();

    // Skip leading whitespace
    while (p < end && *p == ' ') p++;

    // Check for currency prefix
    if (p < end && *p == '$') {
        *hasCurrency = 1;
        p++;
        // Skip space after $
        while (p < end && *p == ' ') p++;
    }

    // Check for negative sign
    int negative = 0;
    if (p < end && *p == '-') {
        negative = 1;
        p++;
    } else if (p < end && *p == '+') {
        p++;
    }

    // Must have at least one digit
    if (p >= end || (!isDigit(*p) && *p != '.')) {
        return 0;
    }

    // Parse the number, handling commas as thousands separators
    double result = 0.0;
    int hasDecimal = 0;
    double decimalPlace = 0.1;
    int digitCount = 0;

    while (p < end) {
        if (isDigit(*p)) {
            if (hasDecimal) {
                result = result + (*p - '0') * decimalPlace;
                decimalPlace *= 0.1;
            } else {
                result = result * 10 + (*p - '0');
            }
            digitCount++;
            p++;
        } else if (*p == ',') {
            // Comma is thousands separator (only valid before decimal)
            if (hasDecimal) {
                return 0;  // Comma after decimal is invalid
            }
            *hasThousands = 1;
            p++;
        } else if (*p == '.') {
            if (hasDecimal) {
                return 0;  // Multiple decimals invalid
            }
            hasDecimal = 1;
            p++;
        } else {
            break;
        }
    }

    if (digitCount == 0) {
        return 0;
    }

    // Skip trailing whitespace
    while (p < end && *p == ' ') p++;

    // Check for percent suffix
    if (p < end && *p == '%') {
        *hasPercent = 1;
        p++;
        // Skip trailing whitespace after %
        while (p < end && *p == ' ') p++;
    }

    // Must have consumed entire input
    if (p != end) {
        return 0;
    }

    // Apply negative
    if (negative) {
        result = -result;
    }

    // If percent, divide by 100
    if (*hasPercent) {
        result = result / 100.0;
    }

    *value = result;
    return 1;
}


//-------------------------------------------------------------------------------------------------
// CxSheetInputParser::dateToSerial
//-------------------------------------------------------------------------------------------------
double
CxSheetInputParser::dateToSerial(int year, int month, int day)
{
    // Validate basic ranges
    if (year < 1900 || year > 9999 || month < 1 || month > 12 || day < 1 || day > 31) {
        return 0.0;
    }

    struct tm timeinfo;
    memset(&timeinfo, 0, sizeof(timeinfo));
    timeinfo.tm_year = year - 1900;
    timeinfo.tm_mon = month - 1;
    timeinfo.tm_mday = day;
    timeinfo.tm_hour = 12;  // Use noon to avoid DST edge cases
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;
    timeinfo.tm_isdst = 0;

#if defined(_OSX_) || defined(_LINUX_)
    time_t unixTime = timegm(&timeinfo);
#else
    // Fallback: use mktime and adjust for timezone
    time_t unixTime = mktime(&timeinfo);
#endif

    if (unixTime == (time_t)-1) {
        return 0.0;
    }

    return (double)unixTime / SECONDS_PER_DAY + EXCEL_EPOCH_OFFSET;
}


//-------------------------------------------------------------------------------------------------
// CxSheetInputParser::serialToComponents
//-------------------------------------------------------------------------------------------------
void
CxSheetInputParser::serialToComponents(double serial, int *year, int *month, int *day)
{
    // Convert serial to Unix time
    time_t unixTime = (time_t)((serial - EXCEL_EPOCH_OFFSET) * SECONDS_PER_DAY);

    struct tm *timeinfo = gmtime(&unixTime);
    if (timeinfo == NULL) {
        *year = 1900;
        *month = 1;
        *day = 1;
        return;
    }

    *year = timeinfo->tm_year + 1900;
    *month = timeinfo->tm_mon + 1;
    *day = timeinfo->tm_mday;
}


//-------------------------------------------------------------------------------------------------
// CxSheetInputParser::tryParseDate
//-------------------------------------------------------------------------------------------------
int
CxSheetInputParser::tryParseDate(CxString input, double *serialDate, CxString *dateFormat)
{
    if (input.length() == 0) {
        return 0;
    }

    const char *p = input.data();
    const char *end = p + input.length();

    // Skip leading whitespace
    while (p < end && *p == ' ') p++;

    int n1 = 0, n2 = 0, n3 = 0;
    char sep1 = 0, sep2 = 0;

    // Parse first number
    if (!parseIntAdvance(&p, &n1)) {
        return 0;
    }

    // Parse first separator
    if (p >= end) {
        return 0;
    }
    sep1 = *p;
    if (sep1 != '/' && sep1 != '-') {
        return 0;
    }
    p++;

    // Parse second number
    if (!parseIntAdvance(&p, &n2)) {
        return 0;
    }

    // Parse second separator (must match first)
    if (p >= end) {
        return 0;
    }
    sep2 = *p;
    if (sep2 != sep1) {
        return 0;
    }
    p++;

    // Parse third number
    if (!parseIntAdvance(&p, &n3)) {
        return 0;
    }

    // Skip trailing whitespace
    while (p < end && *p == ' ') p++;

    // Must have consumed entire input
    if (p != end) {
        return 0;
    }

    // Determine format based on number sizes
    int year, month, day;

    if (n1 > 31) {
        // yyyy-mm-dd or yyyy/mm/dd (ISO format)
        year = n1;
        month = n2;
        day = n3;
        if (sep1 == '-') {
            *dateFormat = "yyyy-mm-dd";
        } else {
            *dateFormat = "yyyy/mm/dd";
        }
    } else if (n3 > 31) {
        // mm/dd/yyyy or mm-dd-yyyy (US format)
        month = n1;
        day = n2;
        year = n3;
        if (sep1 == '/') {
            *dateFormat = "mm/dd/yyyy";
        } else {
            *dateFormat = "mm-dd-yyyy";
        }
    } else {
        // Ambiguous - assume mm/dd/yy or mm-dd-yy with 2-digit year
        month = n1;
        day = n2;
        year = n3;
        // Expand 2-digit year
        if (year < 100) {
            year = (year < 30) ? 2000 + year : 1900 + year;
        }
        if (sep1 == '/') {
            *dateFormat = "mm/dd/yyyy";
        } else {
            *dateFormat = "mm-dd-yyyy";
        }
    }

    // Validate date components
    if (month < 1 || month > 12 || day < 1 || day > 31 || year < 1900 || year > 9999) {
        return 0;
    }

    // Convert to serial
    *serialDate = dateToSerial(year, month, day);

    if (*serialDate == 0.0) {
        return 0;
    }

    return 1;
}


//-------------------------------------------------------------------------------------------------
// CxSheetInputParser::formatDate
//-------------------------------------------------------------------------------------------------
CxString
CxSheetInputParser::formatDate(double serial, CxString format)
{
    int year, month, day;
    serialToComponents(serial, &year, &month, &day);

    char buf[32];

    if (format == "yyyy-mm-dd") {
        snprintf(buf, sizeof(buf), "%04d-%02d-%02d", year, month, day);
    } else if (format == "yyyy/mm/dd") {
        snprintf(buf, sizeof(buf), "%04d/%02d/%02d", year, month, day);
    } else if (format == "mm-dd-yyyy") {
        snprintf(buf, sizeof(buf), "%02d-%02d-%04d", month, day, year);
    } else {
        // Default: mm/dd/yyyy
        snprintf(buf, sizeof(buf), "%02d/%02d/%04d", month, day, year);
    }

    return CxString(buf);
}
