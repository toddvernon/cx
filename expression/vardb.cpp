//-------------------------------------------------------------------------------------------------
//
//  vardb.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  vardb.cpp
//
//-------------------------------------------------------------------------------------------------

#include <math.h>

#include <cx/expression/vardb.h>

//-------------------------------------------------------------------------------------------------
// expression: system/includes					
//-------------------------------------------------------------------------------------------------
#include <cx/base/slist.h>
#include <cx/base/string.h>


//-------------------------------------------------------------------------------------------------
// 
// 
// 
// 
//-------------------------------------------------------------------------------------------------

CxExpressionVariableDatabase::CxExpressionVariableDatabase( void )
{
}
   
    
CxExpressionVariableDatabase::returnCode 
CxExpressionVariableDatabase::VariableDefined( CxString name )
{
	return( VARIABLE_UNDEFINED );
}
    
CxExpressionVariableDatabase::returnCode 
CxExpressionVariableDatabase::VariableEvaluate( CxString name, double *result )
{
    return( VARIABLE_UNDEFINED );
}



//-------------------------------------------------------------------------------------------------
// 
// 
// 
// 
//-------------------------------------------------------------------------------------------------

CxExpressionIntrinsicVariableDatabase::CxExpressionIntrinsicVariableDatabase( void )
{
}
   
    
CxExpressionVariableDatabase::returnCode 
CxExpressionIntrinsicVariableDatabase::VariableDefined( CxString name )
{
	if (name == "M_E") {
		return( VARIABLE_DEFINED );
	}	

	if (name == "M_LOG2E") {
		return( VARIABLE_DEFINED );
	}
	
	if (name == "M_LOG10E") {
		return( VARIABLE_DEFINED );
	}	

	if (name == "M_LN2") {
		return( VARIABLE_DEFINED );
	}	
	
	if (name == "M_LN10") {
		return( VARIABLE_DEFINED );
	}	
	
	if (name == "M_PI") {
		return( VARIABLE_DEFINED );
	}	

	if (name == "M_PI_2") {
		return( VARIABLE_DEFINED );
	}	

	if (name == "M_PI_4") {
		return( VARIABLE_DEFINED );
	}	
	
	if (name == "M_1_PI") {
		return( VARIABLE_DEFINED );
	}	
	
	if (name == "M_2_PI") {
		return( VARIABLE_DEFINED );
	}	

	if (name == "M_2_SQRTPI") {
		return( VARIABLE_DEFINED );
	}
	
	if (name == "M_SQRT2") {
		return( VARIABLE_DEFINED );
	}	
	
	if (name == "M_SQRT1_2") {
		return( VARIABLE_DEFINED );
	}	

	
	return( VARIABLE_UNDEFINED );
}
    
CxExpressionVariableDatabase::returnCode 
CxExpressionIntrinsicVariableDatabase::VariableEvaluate( CxString name, double *result )
{
	if (name == "M_E") {
		*result = M_E;
		return( VARIABLE_DEFINED);
	}

	if (name == "M_LOG2E") {
		*result = M_LOG2E;
		return( VARIABLE_DEFINED );
	}
	
	if (name == "M_LOG10E") {
		*result = M_LOG10E;
		return( VARIABLE_DEFINED );
	}	

	if (name == "M_LN2") {
		*result = M_LN2;
		return( VARIABLE_DEFINED );
	}	
	
	if (name == "M_LN10") {
		*result = M_LN10;
		return( VARIABLE_DEFINED );
	}	
	
	if (name == "M_PI") {
		*result = M_PI;
		return( VARIABLE_DEFINED );
	}	

	if (name == "M_PI_2") {
		*result = M_PI_2;
		return( VARIABLE_DEFINED );
	}	

	if (name == "M_PI_4") {
		*result = M_PI_4;
		return( VARIABLE_DEFINED );
	}	
	
	if (name == "M_1_PI") {
		*result = M_1_PI;
		return( VARIABLE_DEFINED );
	}	
	
	if (name == "M_2_PI") {
		*result = M_2_PI;
		return( VARIABLE_DEFINED );
	}	

	if (name == "M_2_SQRTPI") {
		*result = M_2_SQRTPI;
		return( VARIABLE_DEFINED );
	}
	
	if (name == "M_SQRT2") {
		*result = M_SQRT2;
		return( VARIABLE_DEFINED );
	}	
	
	if (name == "M_SQRT1_2") {
		*result = M_SQRT1_2;
		return( VARIABLE_DEFINED );
	}	

    return( VARIABLE_UNDEFINED );
}
