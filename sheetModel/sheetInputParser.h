//-------------------------------------------------------------------------------------------------
//
//  sheetInputParser.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  Post-commit input parsing for spreadsheet cells.
//  Parses user input strings to determine type (number, date, text) and extract
//  values with implied formatting (currency, percent, thousands, date format).
//
//-------------------------------------------------------------------------------------------------

#ifndef _CX_SHEET_INPUT_PARSER_H_
#define _CX_SHEET_INPUT_PARSER_H_

#include <cx/base/string.h>


//-------------------------------------------------------------------------------------------------
//
// CxSheetInputParser
//
// Static utility class for parsing spreadsheet cell input.
// Determines cell type and extracts values + format hints from user input.
//
//-------------------------------------------------------------------------------------------------

class CxSheetInputParser {

  public:

    //---------------------------------------------------------------------------------------------
    // tryParseNumber
    //
    // Attempt to parse input as a number with optional formatting.
    // Recognizes: plain numbers, currency ($), percent (%), thousands separators (,)
    //
    // Returns: 1 if successfully parsed as number, 0 otherwise
    //
    // On success:
    //   - value: the numeric value (percent is divided by 100)
    //   - hasCurrency: 1 if input had $ prefix
    //   - hasPercent: 1 if input had % suffix
    //   - hasThousands: 1 if input had comma separators
    //---------------------------------------------------------------------------------------------
    static int tryParseNumber(CxString input, double *value,
                              int *hasCurrency, int *hasPercent, int *hasThousands);


    //---------------------------------------------------------------------------------------------
    // tryParseDate
    //
    // Attempt to parse input as a date in various formats.
    // Recognizes:
    //   mm/dd/yyyy    (10/20/2026)
    //   yyyy-mm-dd    (2026-10-20) ISO format
    //   mm-dd-yyyy    (10-20-2026)
    //
    // Returns: 1 if successfully parsed as date, 0 otherwise
    //
    // On success:
    //   - serialDate: Excel-compatible serial date number
    //   - dateFormat: the format string to use for display (e.g., "mm/dd/yyyy")
    //---------------------------------------------------------------------------------------------
    static int tryParseDate(CxString input, double *serialDate, CxString *dateFormat);


    //---------------------------------------------------------------------------------------------
    // dateToSerial
    //
    // Convert year/month/day to Excel-compatible serial date.
    // Serial 1 = January 1, 1900.
    //---------------------------------------------------------------------------------------------
    static double dateToSerial(int year, int month, int day);


    //---------------------------------------------------------------------------------------------
    // serialToComponents
    //
    // Convert Excel serial date back to year/month/day components.
    //---------------------------------------------------------------------------------------------
    static void serialToComponents(double serial, int *year, int *month, int *day);


    //---------------------------------------------------------------------------------------------
    // formatDate
    //
    // Format a serial date according to the given format string.
    // Supported formats: "mm/dd/yyyy", "yyyy-mm-dd", "mm-dd-yyyy"
    //---------------------------------------------------------------------------------------------
    static CxString formatDate(double serial, CxString format);

};


#endif
