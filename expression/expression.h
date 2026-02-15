//-------------------------------------------------------------------------------------------------
//
//  expression.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  expression.h
//
//-------------------------------------------------------------------------------------------------

#ifndef __expression_h__
#define __expression_h__

//-------------------------------------------------------------------------------------------------
// expression: system/includes					
//-------------------------------------------------------------------------------------------------
#include <setjmp.h>

#include <cx/base/slist.h>
#include <cx/base/string.h>

#include <cx/expression/vardb.h>
#include <cx/expression/funcdb.h>
#include <cx/expression/token.h>


//-------------------------------------------------------------------------------------------------
// 
// 
// 
// 
//-------------------------------------------------------------------------------------------------

class CxExpression
{
  public:

	//---------------------------------------------------------------------------------------------
	// valid return codes from Parse method
	// 
	//---------------------------------------------------------------------------------------------

	enum parseStatus {
		PARSE_SUCCESS,
		PARSE_ERROR
	};

	enum expressionStatus {
		EVALUATION_NEW,
		EVALUATION_PARSED,
		EVALUATION_SUCCESS,

		EVALUATION_PARSE_ERROR,
		EVALUATION_ERROR
	};    

	//---------------------------------------------------------------------------------------------
	// valid return codes from Evaluate Method
	//
	//---------------------------------------------------------------------------------------------

    enum expressionError {
        
        OK                           = 0,
        
        //-----------------------------------------------------
        // (parse stage errors)
        //-----------------------------------------------------
     
        NULL_EXPRESSION              = 1,
		UNEXPECTED_TOKEN             = 2,

        MULT_AFTER_PLUS              = 100,
        DIV_AFTER_PLUS               = 101,
        RIGHT_PAREN_AFTER_PLUS       = 102,
        END_AFTER_PLUS               = 103,

        MULT_AFTER_MINUS             = 200,
        DIV_AFTER_MINUS              = 201,
        RIGHT_PAREN_AFTER_MINUS      = 202,
        END_AFTER_MINUS              = 203,

        MULT_AFTER_MULT              = 300,
        DIV_AFTER_MULT               = 301,
        RIGHT_PAREN_AFTER_MULT       = 302,
        END_AFTER_MULT               = 303,

        MULT_AFTER_DIV               = 400,
        DIV_AFTER_DIV                = 401,
        RIGHT_PAREN_AFTER_DIV        = 402,
        END_AFTER_DIV                = 403,

        MULT_AFTER_LEFT_PAREN        = 500,
        DIV_AFTER_LEFT_PAREN         = 501,
        RIGHT_PAREN_AFTER_LEFT_PAREN = 502,
        END_AFTER_LEFT_PAREN         = 503,

        NUMBER_AFTER_RIGHT_PAREN     = 600,
        VARIABLE_AFTER_RIGHT_PAREN   = 601,
        FUNCTION_AFTER_RIGHT_PAREN   = 602,

        LEFT_PAREN_AFTER_NUMBER      = 700,
        NUMBER_AFTER_NUMBER          = 701,
        VARIABLE_AFTER_NUMBER        = 702,
        FUNCTION_AFTER_NUMBER        = 703,
                                
        LEFT_PAREN_AFTER_VARIABLE    = 800,
        NUMBER_AFTER_VARIABLE        = 801,
        VARIABLE_AFTER_VARIABLE      = 802,
        FUNCTION_AFTER_VARIABLE      = 803,

        PLUS_AFTER_FUNCTION          = 900,
        MINUS_AFTER_FUNCTION         = 901,
        MULT_AFTER_FUNCTION          = 902,
        DIV_AFTER_FUNCTION           = 903,
        RIGHT_PAREN_AFTER_FUNCTION   = 904,
        NUMBER_AFTER_FUNCTION        = 905,
        VARIABLE_AFTER_FUNCTION      = 906,
        FUNCTION_AFTER_FUNCTION      = 907,
        END_AFTER_FUNCTION           = 908,

        COMMA_AFTER_PLUS             = 1010,
        COMMA_AFTER_MINUS            = 1011,
        COMMA_AFTER_MULT             = 1012,
        COMMA_AFTER_DIV              = 1013,
        COMMA_AFTER_COMMA            = 1014,
        COMMA_AFTER_LEFT_PAREN       = 1015,
        COMMA_AFTER_FUNCTION         = 1016,

        RIGHT_PAREN_AFTER_COMMA      = 1020,
        MULT_AFTER_COMMA             = 1021,
        DIV_AFTER_COMMA              = 1022,
        END_AFTER_COMMA              = 1023,

        UNKNOWN_FUNCTION             = 1000,
        
		ILLEGAL_NUMBER               = 1001,
        UNBALANCED_PARENS            = 1002,
        UNKNOWN_SYMBOL               = 1003,
        INVALID_COMMA_PLACEMENT      = 1004,
        INVALID_FIRST_SYMBOL         = 1005,
        INCORRECT_NUMBER_OF_ARGS     = 1006,
        UNKNOWN_VARIABLE             = 1007,
        
        INTERNAL_ERROR               = 1008,

        //-----------------------------------------------------
        // (evaluation stage errors)
        //-----------------------------------------------------
        L6A                          = 2000,

        PRIM_A                       = 3000,
        PRIM_B                       = 3001,
        PRIM_C                       = 3002,
        PRIM_D                       = 3003,
        PRIM_E                       = 3004,
        PRIM_F                       = 3005,
        PRIM_G                       = 3006,
		PRIM_H                       = 3007,
        ARITH_A                      = 4000,
        ARITH_B                      = 4001,

        UNARY_A                      = 5000
    };
    

    CxExpression( CxString estring );
    // constructor

    CxExpression( CxString estring, CxExpressionVariableDatabase *var_db, CxExpressionFunctionDatabase *func_db );
	// constructor

    ~CxExpression();
    // destructor

    parseStatus Parse(void);
    // parse the expression creating an internal token list for later evaluation
    
    expressionStatus Evaluate(double *result);
    // evaluate the parsed expression
    
    void DumpTokens();
    // dump all the tokens
    
    CxString UnParseExpression(int num, char *s);
    // unparse the token list back into an text expression
    
    CxString GetErrorString(void);
    // get the error code after parse or evaluation
    
    CxSList<CxString> GetVariableList(void);
    // the the list of variables used in the expression
    
    CxSList<CxString> GetUnknownVariableList(void);
    // get the list of variables that were not found in the database
    
    CxSList<CxString> GetKnownVariableList(void);
    // get the list of variables that were found in the database

    void setVariableDatabase(CxExpressionVariableDatabase *var_db);
    // set the variable database to use for evaluation
    // note: the expression does not take ownership of this database

  private:
    
    void ParseToTokens(void);
    // parse into a token list for later examination

    parseStatus CheckSyntax( void );
    parseStatus CheckSyntax2(int mode, int *args);

    void  Level1(double *result_array, int *args);
    void  Level2(double *result);
    void  Level3(double *result);
    void  Level4(double *result);
    void  Level5(double *result);
    void  Level6(double *result);
    expressionError   Primitive(double *result);
    void  Error(int code);
    expressionError   Arith(int op, double *number1, double *number2);
    expressionError   Unary(int op, double *number);
    
    CxExpressionToken CurrentToken( void );
    CxExpressionToken NextToken( void );

	void debugPrintf( const char * format, ... );    
	void debugTokenDump( CxExpressionToken token );

    
    CxString text;
    CxSList<CxExpressionToken> token_list;


    expressionStatus  status;
	// holds the current state of the overall class

    double result;

    int current_token;
    
    int error_token;

    int  error_code;
	// after check this contains the error

    jmp_buf env_buf;

    CxExpressionVariableDatabase *var_intrinsic_db;
    CxExpressionFunctionDatabase *func_intrinsic_db;

    CxExpressionVariableDatabase *var_db;
    CxExpressionFunctionDatabase *func_db;

    int owns_var_db;
    int owns_func_db;
};


#endif
