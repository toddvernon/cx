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

// Forward declaration
class CxSheetCell;


//-------------------------------------------------------------------------------------------------
//
// CxSheetInputParseResult
//
// Result from parseAndClassify() - contains all information needed to create a cell
// and apply formatting attributes.
//
//-------------------------------------------------------------------------------------------------

struct CxSheetInputParseResult {
    int success;                    // 1 if parsed, 0 if error (with intent)
    int cellType;                   // 0=EMPTY, 1=TEXT, 2=DOUBLE, 3=FORMULA
    double doubleValue;             // valid when cellType == DOUBLE
    CxString textValue;             // valid when cellType == TEXT
    CxString dateFormat;            // e.g., "mm/dd/yyyy" (empty if not date)
    int hasCurrency;                // input had $
    int hasPercent;                 // input had %
    int hasThousands;               // input had commas
    int decimalDigits;              // digits after decimal point (-1 = no decimal)
    CxString errorMsg;              // non-empty on failure
};


//-------------------------------------------------------------------------------------------------
//
// Input intent constants (returned by detectInputIntent)
//
//-------------------------------------------------------------------------------------------------

enum {
    INTENT_NONE = 0,
    INTENT_DATE = (1 << 0),         // has / or - between numbers
    INTENT_CURRENCY = (1 << 1),     // starts with $
    INTENT_PERCENT = (1 << 2),      // ends with %
    INTENT_NUMBER = (1 << 3)        // all chars are numeric-like
};


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
    //   - decimalDigits: digits after decimal point (-1 if no decimal)
    //
    // On failure (if errorMsg is non-NULL):
    //   - errorMsg: descriptive error message
    //---------------------------------------------------------------------------------------------
    static int tryParseNumber(CxString input, double *value,
                              int *hasCurrency, int *hasPercent, int *hasThousands,
                              int *decimalDigits,
                              CxString *errorMsg = NULL);


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
    //
    // On failure (if errorMsg is non-NULL):
    //   - errorMsg: descriptive error message
    //---------------------------------------------------------------------------------------------
    static int tryParseDate(CxString input, double *serialDate, CxString *dateFormat,
                            CxString *errorMsg = NULL);


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


    //---------------------------------------------------------------------------------------------
    // detectInputIntent
    //
    // Analyze input string to determine what the user likely intended.
    // Returns: bitmask of INTENT_DATE, INTENT_CURRENCY, INTENT_PERCENT, INTENT_NUMBER
    //
    // This is used for error handling: if the input looks like a date but fails to parse,
    // we show an error instead of silently falling back to text.
    //---------------------------------------------------------------------------------------------
    static int detectInputIntent(CxString input);


    //---------------------------------------------------------------------------------------------
    // parseAndClassify
    //
    // Single entry point for input parsing. Orchestrates date/number/text detection.
    // Tries: date -> number -> text, with intent-based error handling.
    //
    // Returns: CxSheetInputParseResult with cell type, value, and format attributes.
    // On failure (success=0), errorMsg describes the problem.
    //---------------------------------------------------------------------------------------------
    static CxSheetInputParseResult parseAndClassify(CxString input);


    //---------------------------------------------------------------------------------------------
    // applyParsingAttributes
    //
    // Apply format attributes from parse result to a cell's appAttributes.
    // Sets: dateFormat, currency, percent, thousands as appropriate.
    //---------------------------------------------------------------------------------------------
    static void applyParsingAttributes(CxSheetCell *cell,
                                       const CxSheetInputParseResult &result);

};


#endif
