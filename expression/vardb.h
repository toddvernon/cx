

#ifndef __vardb_h__
#define __vardb_h__

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

class CxExpressionVariableDatabase
{
  public:
    
    enum returnCode {
        VARIABLE_DEFINED,
        VARIABLE_UNDEFINED,
        VARIABLE_CIRCULAR_REFERENCE
    };
    
    // constructor
    CxExpressionVariableDatabase( void );
    
    // should return true in the subclass if the variable name is defined
    virtual returnCode VariableDefined( CxString name );

    // should return a status in the subclass with an error code, result will contain the
    // value of the variable.
    virtual returnCode VariableEvaluate( CxString name, double *result );
};


//-------------------------------------------------------------------------------------------------
// 
// 
// 
// 
//-------------------------------------------------------------------------------------------------

class CxExpressionIntrinsicVariableDatabase: public CxExpressionVariableDatabase
{
  public:

    // constructor
    CxExpressionIntrinsicVariableDatabase( void );
    
    // should return true in the subclass if the variable name is defined
    virtual returnCode VariableDefined( CxString name );

    
    // should return a status in the subclass with an error code, result will contain the
    // value of the variable.
    virtual returnCode VariableEvaluate( CxString name, double *result );

};

#endif
