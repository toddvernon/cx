//-------------------------------------------------------------------------------------------------
//
//  funcdb.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  funcdb.cpp
//
//-------------------------------------------------------------------------------------------------

#include <math.h>
#include <cx/base/string.h>
#include <cx/expression/funcdb.h>


//-------------------------------------------------------------------------------------------------
// 
// 
// 
// 
//-------------------------------------------------------------------------------------------------
    
CxExpressionFunctionDatabase::CxExpressionFunctionDatabase( void )
{
}
    

CxExpressionFunctionDatabase::returnCode 
CxExpressionFunctionDatabase::FunctionDefined( CxString name )
{
	return( FUNCTION_UNDEFINED );
}
    

CxExpressionFunctionDatabase::returnCode 
CxExpressionFunctionDatabase::FunctionEvaluate( CxString name, int numberOfArgs, double *args, double *result)
{
	return( FUNCTION_UNDEFINED );
}


//-------------------------------------------------------------------------------------------------
// 
// 
// 
// 
//-------------------------------------------------------------------------------------------------
    
CxExpressionIntrinsicFunctionDatabase::CxExpressionIntrinsicFunctionDatabase( void )
{
}
    

CxExpressionFunctionDatabase::returnCode 
CxExpressionIntrinsicFunctionDatabase::FunctionDefined( CxString name )
{
    if (name == "SIN")    return FUNCTION_DEFINED;
    if (name == "COS")    return FUNCTION_DEFINED;
    if (name == "TAN")    return FUNCTION_DEFINED;
    if (name == "ACOS")   return FUNCTION_DEFINED;
    if (name == "ASIN")   return FUNCTION_DEFINED;
    if (name == "ATAN")   return FUNCTION_DEFINED;
    if (name == "SINH")   return FUNCTION_DEFINED;
    if (name == "COSH")   return FUNCTION_DEFINED;
    if (name == "TANH")   return FUNCTION_DEFINED;
    if (name == "ASINH")  return FUNCTION_DEFINED;
    if (name == "ACOSH")  return FUNCTION_DEFINED;
    if (name == "ATANH")  return FUNCTION_DEFINED;
    if (name == "LOG")    return FUNCTION_DEFINED;
    if (name == "EXP")    return FUNCTION_DEFINED;
    if (name == "LOG10")  return FUNCTION_DEFINED;
    if (name == "ALOG10") return FUNCTION_DEFINED;
    if (name == "ABS")    return FUNCTION_DEFINED;
    if (name == "CEIL")   return FUNCTION_DEFINED;
    if (name == "FLOOR")  return FUNCTION_DEFINED;
    if (name == "SQRT")   return FUNCTION_DEFINED;
    if (name == "ATAN2")  return FUNCTION_DEFINED;
    if (name == "POW")    return FUNCTION_DEFINED;
    if (name == "MIN")    return FUNCTION_DEFINED;
    if (name == "MAX")    return FUNCTION_DEFINED;
    if (name == "R2D")    return FUNCTION_DEFINED;
    if (name == "D2R")    return FUNCTION_DEFINED;
	return( FUNCTION_UNDEFINED );
}
    

CxExpressionFunctionDatabase::returnCode 
CxExpressionIntrinsicFunctionDatabase::FunctionEvaluate( CxString name, int args, double *arg_list, double *result)
{
    if (name == "SIN") {
        if (args != 1) return(FUNCTION_BAD_NUMBER_OF_ARGS);
        *result = sin(arg_list[0]);
        return FUNCTION_DEFINED;
    }

    if (name == "COS")    {
        if (args != 1) return(FUNCTION_BAD_NUMBER_OF_ARGS);
        *result = cos(arg_list[0]);
        return FUNCTION_DEFINED;
    }

    if (name == "TAN") {
        if (args != 1) return(FUNCTION_BAD_NUMBER_OF_ARGS);
        *result = tan(arg_list[0]);
        return FUNCTION_DEFINED;
    }

    if (name == "ACOS") {
        if (args != 1) return(FUNCTION_BAD_NUMBER_OF_ARGS);
        *result = acos(arg_list[0]);
        return FUNCTION_DEFINED;
    }

    if (name == "ASIN") {
        if (args != 1) return(FUNCTION_BAD_NUMBER_OF_ARGS);
        *result = asin(arg_list[0]);
        return FUNCTION_DEFINED;
    }

    if (name == "ATAN") {
        if (args != 1) return(FUNCTION_BAD_NUMBER_OF_ARGS);
        *result = atan(arg_list[0]);
        return FUNCTION_DEFINED;
    }

    if (name == "SINH") {
        if (args != 1) return(FUNCTION_BAD_NUMBER_OF_ARGS);
        *result = sinh(arg_list[0]);
        return FUNCTION_DEFINED;
    }

    if (name == "COSH") {
        if (args != 1) return(FUNCTION_BAD_NUMBER_OF_ARGS);
        *result = cosh(arg_list[0]);
        return FUNCTION_DEFINED;
    }

    if (name == "TANH") {
        if (args != 1) return(FUNCTION_BAD_NUMBER_OF_ARGS);
        *result = tanh(arg_list[0]);
        return FUNCTION_DEFINED;
    }

    if (name == "ASINH") {
        if (args != 1) return(FUNCTION_BAD_NUMBER_OF_ARGS);
        *result = asinh(arg_list[0]);
        return FUNCTION_DEFINED;
    }

    if (name == "ACOSH") {
        if (args != 1) return(FUNCTION_BAD_NUMBER_OF_ARGS);
        *result = acosh(arg_list[0]);
        return FUNCTION_DEFINED;
    }

    if (name == "ATANH") {
        if (args != 1) return(FUNCTION_BAD_NUMBER_OF_ARGS);
        *result = atanh(arg_list[0]);
        return FUNCTION_DEFINED;
    }

    if (name == "LOG") {
        if (args != 1) return(FUNCTION_BAD_NUMBER_OF_ARGS);
        if (arg_list[0]<=0.0) return(FUNCTION_RANGE_ERROR);
        *result = log(arg_list[0]);
        return FUNCTION_DEFINED;
    }

    if (name == "EXP") {
        if (args != 1) return(FUNCTION_BAD_NUMBER_OF_ARGS);
        *result = exp(arg_list[0]);
        return FUNCTION_DEFINED;
    }

    if (name == "LOG10") {
        if (args != 1) return(FUNCTION_BAD_NUMBER_OF_ARGS);
        if (arg_list[0]<=0.0) return(FUNCTION_RANGE_ERROR);
        *result = log10(arg_list[0]);
        return FUNCTION_DEFINED;
    }

    if (name == "ALOG10") {
        if (args != 1) return(FUNCTION_BAD_NUMBER_OF_ARGS);
        *result = pow((double) 10.0, arg_list[0]);
        return FUNCTION_DEFINED;
    }

    if (name == "ABS") {
        if (args != 1) return(FUNCTION_BAD_NUMBER_OF_ARGS);
        *result = fabs(arg_list[0]);
        return FUNCTION_DEFINED;
    }

    if (name == "CEIL") {
        if (args != 1) return(FUNCTION_BAD_NUMBER_OF_ARGS);
        *result = ceil(arg_list[0]);
        return FUNCTION_DEFINED;
    }

    if (name == "FLOOR") {
        if (args != 1) return(FUNCTION_BAD_NUMBER_OF_ARGS);
        *result = floor(arg_list[0]);
        return FUNCTION_DEFINED;
    }

    if (name == "SQRT") {
        if (args != 1) return(FUNCTION_BAD_NUMBER_OF_ARGS);
        if (arg_list[0] < 0.0) return(FUNCTION_RANGE_ERROR);
        *result = sqrt(arg_list[0]);
        return FUNCTION_DEFINED;
    }

    if (name == "ATAN2") { //2
        if (args != 2) return(FUNCTION_BAD_NUMBER_OF_ARGS);
        *result = atan2(arg_list[0], arg_list[1]);
        return FUNCTION_DEFINED;
    }

    if (name == "POW") { //2
        if (args != 2) return(FUNCTION_BAD_NUMBER_OF_ARGS);
        *result = pow(arg_list[0], arg_list[1]);
        return FUNCTION_DEFINED;
    }

    if (name == "MIN") { // any
        *result = CxExpressionIntrinsicFunctionDatabase::min(args, arg_list);
        return FUNCTION_DEFINED;
    }

    if (name == "MAX") { //any
        *result = CxExpressionIntrinsicFunctionDatabase::max(args, arg_list);
        return FUNCTION_DEFINED;
    }

    if (name == "R2D") {
        if (args != 1) return(FUNCTION_BAD_NUMBER_OF_ARGS);
        *result = arg_list[0] * 57.29577951;
        return FUNCTION_DEFINED;
    }

    if (name == "D2R") {
        if (args != 1) return(FUNCTION_BAD_NUMBER_OF_ARGS);
        *result = arg_list[0] / 57.29577951;
        return FUNCTION_DEFINED;
    }
	return( FUNCTION_UNDEFINED );
}

/*static*/
double 
CxExpressionIntrinsicFunctionDatabase::max(int args, double *arg_list)
{
	double max;
    int    c;

    max = arg_list[0];

    for (c=0; c<args; c++) {
        if (arg_list[c] > max) max = arg_list[c];
    }

    return(max);
}


/*static*/
double
CxExpressionIntrinsicFunctionDatabase::min(int args, double *arg_list)
{
double min;
    int    c;

    min = arg_list[0];

    for (c=0; c<args; c++) {
        if (arg_list[c] < min) min = arg_list[c];
    }

    return(min);
}
