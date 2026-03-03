//-------------------------------------------------------------------------------------------------
//
//  sheetFunctionDatabase.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxSheetFunctionDatabase Class Implementation
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <time.h>

#include "sheetFunctionDatabase.h"


//-------------------------------------------------------------------------
// Date helper functions
//
// Excel-style serial dates: 1 = January 1, 1900
// Time is represented as a fraction of a day (0.5 = noon)
//
// Note: Excel incorrectly treats 1900 as a leap year (bug from Lotus 1-2-3)
// We follow this convention for compatibility.
//-------------------------------------------------------------------------

// Days from Jan 1, 1900 to Unix epoch (Jan 1, 1970)
// This includes the Excel leap year bug (Feb 29, 1900 counted)
static const double EXCEL_EPOCH_OFFSET = 25569.0;
static const double SECONDS_PER_DAY = 86400.0;

// Convert Unix timestamp to Excel serial date
static double unixToSerial(time_t unixTime)
{
    return (double)unixTime / SECONDS_PER_DAY + EXCEL_EPOCH_OFFSET;
}

// Convert Excel serial date to Unix timestamp
static time_t serialToUnix(double serial)
{
    return (time_t)((serial - EXCEL_EPOCH_OFFSET) * SECONDS_PER_DAY);
}

// Create serial date from year, month, day
// Uses a direct calculation to avoid timezone issues
static double dateToSerial(int year, int month, int day)
{
    // Use timegm for UTC conversion (available on macOS/Linux)
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
    // Adjust for local timezone offset
    struct tm* gm = gmtime(&unixTime);
    struct tm* loc = localtime(&unixTime);
    if (gm && loc) {
        int offset = (loc->tm_hour - gm->tm_hour) * 3600;
        unixTime -= offset;
    }
#endif

    if (unixTime == (time_t)-1) {
        return 0.0;  // Invalid date
    }

    // Return integer serial (no time component)
    return floor(unixToSerial(unixTime));
}

// Extract year from serial date
static int serialToYear(double serial)
{
    time_t unixTime = serialToUnix(serial);
    struct tm* timeinfo = gmtime(&unixTime);
    if (timeinfo == 0) return 0;
    return timeinfo->tm_year + 1900;
}

// Extract month (1-12) from serial date
static int serialToMonth(double serial)
{
    time_t unixTime = serialToUnix(serial);
    struct tm* timeinfo = gmtime(&unixTime);
    if (timeinfo == 0) return 0;
    return timeinfo->tm_mon + 1;
}

// Extract day (1-31) from serial date
static int serialToDay(double serial)
{
    time_t unixTime = serialToUnix(serial);
    struct tm* timeinfo = gmtime(&unixTime);
    if (timeinfo == 0) return 0;
    return timeinfo->tm_mday;
}
#include "sheetCellRange.h"
#include "sheetModel.h"
#include "sheetCell.h"


//-------------------------------------------------------------------------
// CxSheetFunctionDatabase::CxSheetFunctionDatabase
//
// Default constructor
//-------------------------------------------------------------------------
CxSheetFunctionDatabase::CxSheetFunctionDatabase(void)
: sheetModel(0)
{
}


//-------------------------------------------------------------------------
// CxSheetFunctionDatabase::CxSheetFunctionDatabase
//
// Constructor with model reference
//-------------------------------------------------------------------------
CxSheetFunctionDatabase::CxSheetFunctionDatabase(CxSheetModel* model)
: sheetModel(model)
{
}


//-------------------------------------------------------------------------
// CxSheetFunctionDatabase::setModel
//
// Set the sheet model used for resolving cell values
//-------------------------------------------------------------------------
void
CxSheetFunctionDatabase::setModel(CxSheetModel* model)
{
    sheetModel = model;
}


//-------------------------------------------------------------------------
// CxSheetFunctionDatabase::setRangeList
//
// Set the list of ranges from the current expression
//-------------------------------------------------------------------------
void
CxSheetFunctionDatabase::setRangeList(CxSList<CxString> rangeList)
{
    currentRangeList = rangeList;
}


//-------------------------------------------------------------------------
// CxSheetFunctionDatabase::isRangeFunction
//
// Check if this is a function that accepts range arguments
// Function names are case-insensitive (SUM, Sum, sum all work)
//-------------------------------------------------------------------------
int
CxSheetFunctionDatabase::isRangeFunction(CxString name)
{
    if (name.equalsIgnoreCase("SUM"))     return 1;
    if (name.equalsIgnoreCase("AVERAGE")) return 1;
    if (name.equalsIgnoreCase("COUNT"))   return 1;
    if (name.equalsIgnoreCase("MIN"))     return 1;
    if (name.equalsIgnoreCase("MAX"))     return 1;
    if (name.equalsIgnoreCase("NPV"))     return 1;
    if (name.equalsIgnoreCase("IRR"))     return 1;
    return 0;
}


//-------------------------------------------------------------------------
// CxSheetFunctionDatabase::FunctionDefined
//
// Check if a function name is defined
// Function names are case-insensitive (SUM, Sum, sum all work)
//-------------------------------------------------------------------------
CxExpressionFunctionDatabase::returnCode
CxSheetFunctionDatabase::FunctionDefined(CxString name)
{
    if (name.equalsIgnoreCase("SUM"))     return FUNCTION_DEFINED;
    if (name.equalsIgnoreCase("AVERAGE")) return FUNCTION_DEFINED;
    if (name.equalsIgnoreCase("COUNT"))   return FUNCTION_DEFINED;
    if (name.equalsIgnoreCase("MIN"))     return FUNCTION_DEFINED;
    if (name.equalsIgnoreCase("MAX"))     return FUNCTION_DEFINED;
    if (name.equalsIgnoreCase("IF"))      return FUNCTION_DEFINED;
    if (name.equalsIgnoreCase("AND"))     return FUNCTION_DEFINED;
    if (name.equalsIgnoreCase("OR"))      return FUNCTION_DEFINED;
    if (name.equalsIgnoreCase("PMT"))     return FUNCTION_DEFINED;
    if (name.equalsIgnoreCase("FV"))      return FUNCTION_DEFINED;
    if (name.equalsIgnoreCase("PV"))      return FUNCTION_DEFINED;
    if (name.equalsIgnoreCase("NPV"))     return FUNCTION_DEFINED;
    if (name.equalsIgnoreCase("IRR"))     return FUNCTION_DEFINED;
    if (name.equalsIgnoreCase("NOW"))     return FUNCTION_DEFINED;
    if (name.equalsIgnoreCase("TODAY"))   return FUNCTION_DEFINED;
    if (name.equalsIgnoreCase("DATE"))    return FUNCTION_DEFINED;
    if (name.equalsIgnoreCase("YEAR"))    return FUNCTION_DEFINED;
    if (name.equalsIgnoreCase("MONTH"))   return FUNCTION_DEFINED;
    if (name.equalsIgnoreCase("DAY"))     return FUNCTION_DEFINED;

    return FUNCTION_UNDEFINED;
}


//-------------------------------------------------------------------------
// CxSheetFunctionDatabase::expandRange
//
// Expand a range string to a list of numeric values from the cells
//-------------------------------------------------------------------------
CxSList<double>
CxSheetFunctionDatabase::expandRange(CxString rangeStr)
{
    CxSList<double> values;

    if (sheetModel == 0) {
        return values;
    }

    CxSheetCellRange range(rangeStr);
    if (!range.isValid()) {
        return values;
    }

    unsigned long count = range.cellCount();
    for (unsigned long i = 0; i < count; i++) {
        CxSheetCellCoordinate coord = range.cellAt(i);
        CxSheetCell cell = sheetModel->getCell(coord);

        // Get the numeric value from the cell
        switch (cell.getType()) {
            case CxSheetCell::DOUBLE:
                values.append(cell.getDouble().value);
                break;

            case CxSheetCell::FORMULA:
                values.append(cell.getEvaluatedValue().value);
                break;

            case CxSheetCell::TEXT:
                // Text cells are treated as 0 in numeric contexts
                values.append(0.0);
                break;

            case CxSheetCell::EMPTY:
            default:
                // Empty cells are treated as 0
                values.append(0.0);
                break;
        }
    }

    return values;
}


//-------------------------------------------------------------------------
// CxSheetFunctionDatabase::sum
//
// Sum all values
//-------------------------------------------------------------------------
double
CxSheetFunctionDatabase::sum(CxSList<double>& values)
{
    double total = 0.0;
    for (int i = 0; i < values.entries(); i++) {
        total += values.at(i);
    }
    return total;
}


//-------------------------------------------------------------------------
// CxSheetFunctionDatabase::average
//
// Average of all values
//-------------------------------------------------------------------------
double
CxSheetFunctionDatabase::average(CxSList<double>& values)
{
    if (values.entries() == 0) {
        return 0.0;
    }
    return sum(values) / values.entries();
}


//-------------------------------------------------------------------------
// CxSheetFunctionDatabase::count
//
// Count of values (including zeros from empty cells)
//-------------------------------------------------------------------------
double
CxSheetFunctionDatabase::count(CxSList<double>& values)
{
    return (double)values.entries();
}


//-------------------------------------------------------------------------
// CxSheetFunctionDatabase::min
//
// Minimum value
//-------------------------------------------------------------------------
double
CxSheetFunctionDatabase::min(CxSList<double>& values)
{
    if (values.entries() == 0) {
        return 0.0;
    }

    double minVal = values.at(0);
    for (int i = 1; i < values.entries(); i++) {
        if (values.at(i) < minVal) {
            minVal = values.at(i);
        }
    }
    return minVal;
}


//-------------------------------------------------------------------------
// CxSheetFunctionDatabase::max
//
// Maximum value
//-------------------------------------------------------------------------
double
CxSheetFunctionDatabase::max(CxSList<double>& values)
{
    if (values.entries() == 0) {
        return 0.0;
    }

    double maxVal = values.at(0);
    for (int i = 1; i < values.entries(); i++) {
        if (values.at(i) > maxVal) {
            maxVal = values.at(i);
        }
    }
    return maxVal;
}


//-------------------------------------------------------------------------
// CxSheetFunctionDatabase::FunctionEvaluate
//
// Evaluate function with scalar arguments only
// For range-aware functions, this checks if any arguments might be ranges
// by looking at the currentRangeList
//-------------------------------------------------------------------------
CxExpressionFunctionDatabase::returnCode
CxSheetFunctionDatabase::FunctionEvaluate(
    CxString name,
    int numberOfArgs,
    double *args,
    double *result)
{
    // For functions that support ranges, check if we have ranges
    // in the current expression. If so, expand them.
    if (isRangeFunction(name) && currentRangeList.entries() > 0) {
        // Collect all values (from ranges and scalars)
        CxSList<double> allValues;

        int rangeIndex = 0;
        for (int i = 0; i < numberOfArgs; i++) {
            // Check if this argument was a range (value is 0.0 placeholder)
            // and we have more ranges to process
            if (rangeIndex < currentRangeList.entries() && args[i] == 0.0) {
                // This might be a range - expand it
                CxSList<double> rangeValues = expandRange(currentRangeList.at(rangeIndex));
                for (int j = 0; j < rangeValues.entries(); j++) {
                    allValues.append(rangeValues.at(j));
                }
                rangeIndex++;
            } else {
                // Scalar value
                allValues.append(args[i]);
            }
        }

        // Now compute the function
        if (name.equalsIgnoreCase("SUM")) {
            *result = sum(allValues);
            return FUNCTION_DEFINED;
        }
        if (name.equalsIgnoreCase("AVERAGE")) {
            *result = average(allValues);
            return FUNCTION_DEFINED;
        }
        if (name.equalsIgnoreCase("COUNT")) {
            *result = count(allValues);
            return FUNCTION_DEFINED;
        }
        if (name.equalsIgnoreCase("MIN")) {
            *result = min(allValues);
            return FUNCTION_DEFINED;
        }
        if (name.equalsIgnoreCase("MAX")) {
            *result = max(allValues);
            return FUNCTION_DEFINED;
        }
        if (name.equalsIgnoreCase("NPV")) {
            // First value is rate, rest are cash flows
            if (allValues.entries() < 2) {
                *result = 0.0;
                return FUNCTION_BAD_NUMBER_OF_ARGS;
            }
            double rate = allValues.at(0);
            double npv = 0.0;
            for (int i = 1; i < allValues.entries(); i++) {
                npv += allValues.at(i) / pow(1.0 + rate, i);
            }
            *result = npv;
            return FUNCTION_DEFINED;
        }
        if (name.equalsIgnoreCase("IRR")) {
            // All values are cash flows
            if (allValues.entries() < 2) {
                *result = 0.0;
                return FUNCTION_BAD_NUMBER_OF_ARGS;
            }

            // Newton-Raphson iteration to find IRR
            double rate = 0.1;  // Initial guess: 10%
            int maxIterations = 100;
            double tolerance = 0.0000001;

            for (int iter = 0; iter < maxIterations; iter++) {
                double npv = 0.0;
                double dnpv = 0.0;

                for (int i = 0; i < allValues.entries(); i++) {
                    double factor = pow(1.0 + rate, i);
                    npv += allValues.at(i) / factor;
                    if (i > 0) {
                        dnpv -= i * allValues.at(i) / (factor * (1.0 + rate));
                    }
                }

                if (dnpv == 0.0) {
                    break;
                }

                double newRate = rate - npv / dnpv;

                if (fabs(newRate - rate) < tolerance) {
                    *result = newRate;
                    return FUNCTION_DEFINED;
                }

                rate = newRate;

                if (rate < -1.0 || rate > 10.0) {
                    break;
                }
            }

            *result = rate;
            return FUNCTION_DEFINED;
        }
    }

    // No ranges or not a range function - just compute with scalar args
    if (name.equalsIgnoreCase("SUM")) {
        double total = 0.0;
        for (int i = 0; i < numberOfArgs; i++) {
            total += args[i];
        }
        *result = total;
        return FUNCTION_DEFINED;
    }

    if (name.equalsIgnoreCase("AVERAGE")) {
        if (numberOfArgs == 0) {
            *result = 0.0;
            return FUNCTION_DEFINED;
        }
        double total = 0.0;
        for (int i = 0; i < numberOfArgs; i++) {
            total += args[i];
        }
        *result = total / numberOfArgs;
        return FUNCTION_DEFINED;
    }

    if (name.equalsIgnoreCase("COUNT")) {
        *result = (double)numberOfArgs;
        return FUNCTION_DEFINED;
    }

    if (name.equalsIgnoreCase("MIN")) {
        if (numberOfArgs == 0) {
            *result = 0.0;
            return FUNCTION_DEFINED;
        }
        double minVal = args[0];
        for (int i = 1; i < numberOfArgs; i++) {
            if (args[i] < minVal) minVal = args[i];
        }
        *result = minVal;
        return FUNCTION_DEFINED;
    }

    if (name.equalsIgnoreCase("MAX")) {
        if (numberOfArgs == 0) {
            *result = 0.0;
            return FUNCTION_DEFINED;
        }
        double maxVal = args[0];
        for (int i = 1; i < numberOfArgs; i++) {
            if (args[i] > maxVal) maxVal = args[i];
        }
        *result = maxVal;
        return FUNCTION_DEFINED;
    }

    // IF(condition, true_value, false_value)
    // Returns true_value if condition is non-zero, false_value otherwise
    if (name.equalsIgnoreCase("IF")) {
        if (numberOfArgs != 3) {
            *result = 0.0;
            return FUNCTION_BAD_NUMBER_OF_ARGS;
        }
        // condition is args[0]: non-zero = true, zero = false
        if (args[0] != 0.0) {
            *result = args[1];  // true_value
        } else {
            *result = args[2];  // false_value
        }
        return FUNCTION_DEFINED;
    }

    // AND(args...)
    // Returns 1 if ALL arguments are non-zero, 0 otherwise
    if (name.equalsIgnoreCase("AND")) {
        if (numberOfArgs == 0) {
            *result = 1.0;  // AND with no args is true (identity)
            return FUNCTION_DEFINED;
        }
        for (int i = 0; i < numberOfArgs; i++) {
            if (args[i] == 0.0) {
                *result = 0.0;
                return FUNCTION_DEFINED;
            }
        }
        *result = 1.0;
        return FUNCTION_DEFINED;
    }

    // OR(args...)
    // Returns 1 if ANY argument is non-zero, 0 otherwise
    if (name.equalsIgnoreCase("OR")) {
        if (numberOfArgs == 0) {
            *result = 0.0;  // OR with no args is false (identity)
            return FUNCTION_DEFINED;
        }
        for (int i = 0; i < numberOfArgs; i++) {
            if (args[i] != 0.0) {
                *result = 1.0;
                return FUNCTION_DEFINED;
            }
        }
        *result = 0.0;
        return FUNCTION_DEFINED;
    }

    // PMT(rate, nper, pv)
    // Calculates the payment for a loan based on constant payments and interest rate
    // rate = interest rate per period
    // nper = number of periods
    // pv = present value (loan amount)
    // Formula: PMT = pv * rate * (1+rate)^nper / ((1+rate)^nper - 1)
    if (name.equalsIgnoreCase("PMT")) {
        if (numberOfArgs != 3) {
            *result = 0.0;
            return FUNCTION_BAD_NUMBER_OF_ARGS;
        }
        double rate = args[0];
        double nper = args[1];
        double pv = args[2];

        if (rate == 0.0) {
            // Special case: 0% interest = simple division
            *result = pv / nper;
        } else {
            double factor = pow(1.0 + rate, nper);
            *result = pv * rate * factor / (factor - 1.0);
        }
        return FUNCTION_DEFINED;
    }

    // FV(rate, nper, pmt)
    // Calculates the future value of an investment
    // rate = interest rate per period
    // nper = number of periods
    // pmt = payment per period
    // Formula: FV = pmt * ((1+rate)^nper - 1) / rate
    if (name.equalsIgnoreCase("FV")) {
        if (numberOfArgs != 3) {
            *result = 0.0;
            return FUNCTION_BAD_NUMBER_OF_ARGS;
        }
        double rate = args[0];
        double nper = args[1];
        double pmt = args[2];

        if (rate == 0.0) {
            // Special case: 0% interest
            *result = pmt * nper;
        } else {
            *result = pmt * (pow(1.0 + rate, nper) - 1.0) / rate;
        }
        return FUNCTION_DEFINED;
    }

    // PV(rate, nper, pmt)
    // Calculates the present value of an investment
    // rate = interest rate per period
    // nper = number of periods
    // pmt = payment per period
    // Formula: PV = pmt * (1 - (1+rate)^(-nper)) / rate
    if (name.equalsIgnoreCase("PV")) {
        if (numberOfArgs != 3) {
            *result = 0.0;
            return FUNCTION_BAD_NUMBER_OF_ARGS;
        }
        double rate = args[0];
        double nper = args[1];
        double pmt = args[2];

        if (rate == 0.0) {
            // Special case: 0% interest
            *result = pmt * nper;
        } else {
            *result = pmt * (1.0 - pow(1.0 + rate, -nper)) / rate;
        }
        return FUNCTION_DEFINED;
    }

    // NPV(rate, values...) - scalar version
    // For use when all arguments are scalars (no ranges)
    if (name.equalsIgnoreCase("NPV")) {
        if (numberOfArgs < 2) {
            *result = 0.0;
            return FUNCTION_BAD_NUMBER_OF_ARGS;
        }
        double rate = args[0];
        double npv = 0.0;
        for (int i = 1; i < numberOfArgs; i++) {
            npv += args[i] / pow(1.0 + rate, i);
        }
        *result = npv;
        return FUNCTION_DEFINED;
    }

    // IRR(values...) - scalar version
    // For use when all arguments are scalars (no ranges)
    if (name.equalsIgnoreCase("IRR")) {
        if (numberOfArgs < 2) {
            *result = 0.0;
            return FUNCTION_BAD_NUMBER_OF_ARGS;
        }

        // Newton-Raphson iteration to find IRR
        double rate = 0.1;  // Initial guess: 10%
        int maxIterations = 100;
        double tolerance = 0.0000001;

        for (int iter = 0; iter < maxIterations; iter++) {
            double npv = 0.0;
            double dnpv = 0.0;  // derivative of NPV with respect to rate

            for (int i = 0; i < numberOfArgs; i++) {
                double factor = pow(1.0 + rate, i);
                npv += args[i] / factor;
                if (i > 0) {
                    dnpv -= i * args[i] / (factor * (1.0 + rate));
                }
            }

            if (dnpv == 0.0) {
                break;  // Avoid division by zero
            }

            double newRate = rate - npv / dnpv;

            if (fabs(newRate - rate) < tolerance) {
                *result = newRate;
                return FUNCTION_DEFINED;
            }

            rate = newRate;

            // Guard against divergence
            if (rate < -1.0 || rate > 10.0) {
                break;
            }
        }

        // Return best estimate even if not fully converged
        *result = rate;
        return FUNCTION_DEFINED;
    }

    // NOW()
    // Returns current date and time as serial number
    // Fractional part represents time of day
    if (name.equalsIgnoreCase("NOW")) {
        if (numberOfArgs != 0) {
            *result = 0.0;
            return FUNCTION_BAD_NUMBER_OF_ARGS;
        }
        time_t now = time(0);
        *result = unixToSerial(now);
        return FUNCTION_DEFINED;
    }

    // TODAY()
    // Returns current date as serial number (no time component)
    if (name.equalsIgnoreCase("TODAY")) {
        if (numberOfArgs != 0) {
            *result = 0.0;
            return FUNCTION_BAD_NUMBER_OF_ARGS;
        }
        time_t now = time(0);
        *result = floor(unixToSerial(now));
        return FUNCTION_DEFINED;
    }

    // DATE(year, month, day)
    // Creates a serial date from components
    if (name.equalsIgnoreCase("DATE")) {
        if (numberOfArgs != 3) {
            *result = 0.0;
            return FUNCTION_BAD_NUMBER_OF_ARGS;
        }
        int year = (int)args[0];
        int month = (int)args[1];
        int day = (int)args[2];
        *result = dateToSerial(year, month, day);
        return FUNCTION_DEFINED;
    }

    // YEAR(serial_date)
    // Extracts year from serial date
    if (name.equalsIgnoreCase("YEAR")) {
        if (numberOfArgs != 1) {
            *result = 0.0;
            return FUNCTION_BAD_NUMBER_OF_ARGS;
        }
        *result = (double)serialToYear(args[0]);
        return FUNCTION_DEFINED;
    }

    // MONTH(serial_date)
    // Extracts month (1-12) from serial date
    if (name.equalsIgnoreCase("MONTH")) {
        if (numberOfArgs != 1) {
            *result = 0.0;
            return FUNCTION_BAD_NUMBER_OF_ARGS;
        }
        *result = (double)serialToMonth(args[0]);
        return FUNCTION_DEFINED;
    }

    // DAY(serial_date)
    // Extracts day (1-31) from serial date
    if (name.equalsIgnoreCase("DAY")) {
        if (numberOfArgs != 1) {
            *result = 0.0;
            return FUNCTION_BAD_NUMBER_OF_ARGS;
        }
        *result = (double)serialToDay(args[0]);
        return FUNCTION_DEFINED;
    }

    return FUNCTION_UNDEFINED;
}


//-------------------------------------------------------------------------
// CxSheetFunctionDatabase::FunctionEvaluateWithRanges
//
// Evaluate function with both scalar and range arguments
//-------------------------------------------------------------------------
CxExpressionFunctionDatabase::returnCode
CxSheetFunctionDatabase::FunctionEvaluateWithRanges(
    CxString name,
    int numberOfArgs,
    double *scalarArgs,
    CxString *rangeArgs,
    double *result)
{
    if (!isRangeFunction(name)) {
        // Not a range function - just use scalar values
        return FunctionEvaluate(name, numberOfArgs, scalarArgs, result);
    }

    // Collect all values from both ranges and scalars
    CxSList<double> allValues;

    for (int i = 0; i < numberOfArgs; i++) {
        if (rangeArgs[i].length() > 0) {
            // This is a range argument - expand it
            CxSList<double> rangeValues = expandRange(rangeArgs[i]);
            for (int j = 0; j < rangeValues.entries(); j++) {
                allValues.append(rangeValues.at(j));
            }
        } else {
            // Scalar value
            allValues.append(scalarArgs[i]);
        }
    }

    // Compute the function
    if (name.equalsIgnoreCase("SUM")) {
        *result = sum(allValues);
        return FUNCTION_DEFINED;
    }
    if (name.equalsIgnoreCase("AVERAGE")) {
        *result = average(allValues);
        return FUNCTION_DEFINED;
    }
    if (name.equalsIgnoreCase("COUNT")) {
        *result = count(allValues);
        return FUNCTION_DEFINED;
    }
    if (name.equalsIgnoreCase("MIN")) {
        *result = min(allValues);
        return FUNCTION_DEFINED;
    }
    if (name.equalsIgnoreCase("MAX")) {
        *result = max(allValues);
        return FUNCTION_DEFINED;
    }

    // NPV(rate, values...)
    // Calculates net present value of a series of cash flows
    // rate = discount rate per period
    // values = cash flows (can be range or individual values)
    // Formula: NPV = sum of (value_i / (1+rate)^i) for i = 1 to n
    // Note: First value is at period 1, not period 0
    if (name.equalsIgnoreCase("NPV")) {
        if (numberOfArgs < 2) {
            *result = 0.0;
            return FUNCTION_BAD_NUMBER_OF_ARGS;
        }

        // First argument is rate (must be scalar)
        double rate = scalarArgs[0];

        // Collect cash flow values from remaining arguments
        CxSList<double> cashFlows;
        for (int i = 1; i < numberOfArgs; i++) {
            if (rangeArgs[i].length() > 0) {
                CxSList<double> rangeValues = expandRange(rangeArgs[i]);
                for (int j = 0; j < rangeValues.entries(); j++) {
                    cashFlows.append(rangeValues.at(j));
                }
            } else {
                cashFlows.append(scalarArgs[i]);
            }
        }

        // Calculate NPV
        double npv = 0.0;
        for (int i = 0; i < cashFlows.entries(); i++) {
            npv += cashFlows.at(i) / pow(1.0 + rate, i + 1);
        }
        *result = npv;
        return FUNCTION_DEFINED;
    }

    // IRR(values...)
    // Calculates internal rate of return for a series of cash flows
    // Uses Newton-Raphson iteration to find rate where NPV = 0
    // First value is typically negative (initial investment)
    if (name.equalsIgnoreCase("IRR")) {
        if (numberOfArgs < 1) {
            *result = 0.0;
            return FUNCTION_BAD_NUMBER_OF_ARGS;
        }

        // Collect all cash flow values
        CxSList<double> cashFlows;
        for (int i = 0; i < numberOfArgs; i++) {
            if (rangeArgs[i].length() > 0) {
                CxSList<double> rangeValues = expandRange(rangeArgs[i]);
                for (int j = 0; j < rangeValues.entries(); j++) {
                    cashFlows.append(rangeValues.at(j));
                }
            } else {
                cashFlows.append(scalarArgs[i]);
            }
        }

        if (cashFlows.entries() < 2) {
            *result = 0.0;
            return FUNCTION_BAD_NUMBER_OF_ARGS;
        }

        // Newton-Raphson iteration to find IRR
        // NPV(r) = sum of (cf_i / (1+r)^i) for i = 0 to n
        // We want to find r where NPV(r) = 0
        double rate = 0.1;  // Initial guess: 10%
        int maxIterations = 100;
        double tolerance = 0.0000001;

        for (int iter = 0; iter < maxIterations; iter++) {
            double npv = 0.0;
            double dnpv = 0.0;  // derivative of NPV with respect to rate

            for (int i = 0; i < cashFlows.entries(); i++) {
                double factor = pow(1.0 + rate, i);
                npv += cashFlows.at(i) / factor;
                if (i > 0) {
                    dnpv -= i * cashFlows.at(i) / (factor * (1.0 + rate));
                }
            }

            if (dnpv == 0.0) {
                break;  // Avoid division by zero
            }

            double newRate = rate - npv / dnpv;

            if (fabs(newRate - rate) < tolerance) {
                *result = newRate;
                return FUNCTION_DEFINED;
            }

            rate = newRate;

            // Guard against divergence
            if (rate < -1.0 || rate > 10.0) {
                break;
            }
        }

        // Return best estimate even if not fully converged
        *result = rate;
        return FUNCTION_DEFINED;
    }

    return FUNCTION_UNDEFINED;
}
