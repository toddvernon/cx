
#ifndef __funcdb_h__
#define __funcdb_h__

//-------------------------------------------------------------------------------------------------
// expression: system/includes					
//-------------------------------------------------------------------------------------------------
#include <cx/base/string.h>


//-------------------------------------------------------------------------------------------------
// 
// 
// 
// 
//-------------------------------------------------------------------------------------------------

class CxExpressionFunctionDatabase
{
  public:
    
    enum returnCode {
        FUNCTION_DEFINED,
        FUNCTION_UNDEFINED,
        FUNCTION_BAD_NUMBER_OF_ARGS,
        FUNCTION_RANGE_ERROR
    };
        
    // constructor
    CxExpressionFunctionDatabase( void );
    
    // should return true in the subclass if the function name is defined
    virtual returnCode FunctionDefined( CxString name);
    
    // should return a status variable in the subclass with an error code, result will contain
    // the evaluated function
    virtual returnCode FunctionEvaluate( CxString name, int numberOfArgs, double *args, double *result);

};


//-------------------------------------------------------------------------------------------------
// 
// 
// 
// 
//-------------------------------------------------------------------------------------------------

class CxExpressionIntrinsicFunctionDatabase: public CxExpressionFunctionDatabase
{
  public:
            
    // constructor
    CxExpressionIntrinsicFunctionDatabase( void );
    
    // should return true in the subclass if the function name is defined
    virtual returnCode FunctionDefined( CxString name);
    
    // should return a status variable in the subclass with an error code, result will contain
    // the evaluated function
    virtual returnCode FunctionEvaluate( CxString name, int numberOfArgs, double *args, double *result);
	
	static double min(int args, double *arg_list);
	static double max(int args, double *arg_list);

};

#endif
