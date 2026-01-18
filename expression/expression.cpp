//-------------------------------------------------------------------------------------------------
// CxExpression Class
//
// Parse a mathmatial expression into a token list then evaluate returning a double as the
// answer
//
//-------------------------------------------------------------------------------------------------

#define EXPRESSION_DEBUG

//-------------------------------------------------------------------------------------------------
// expression: system/includes					
//-------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "math.h"
#include <setjmp.h>


//-------------------------------------------------------------------------------------------------
// expression: local/includes					
//-------------------------------------------------------------------------------------------------
#include "scanDouble.h"
#include "token.h"
#include "expression.h"


//-------------------------------------------------------------------------------------------------
// expression: private/defines (expression type)		
//-------------------------------------------------------------------------------------------------
#define NORM_EXPRESSION	0
#define PARN_EXPRESSION	1
#define FUNC_EXPRESSION	2


//-------------------------------------------------------------------------------------------------
// Expression::Expression (constructor)	
//								
//	
//								
//-------------------------------------------------------------------------------------------------
CxExpression::CxExpression(CxString e_string) 
{
    //---------------------------------------------------------------------------------------------
    // Copy the expression
    //---------------------------------------------------------------------------------------------
    text = e_string;
    
    //---------------------------------------------------------------------------------------------
    // Mark expression as new
    //---------------------------------------------------------------------------------------------
    status = EVALUATION_NEW;
    
    //---------------------------------------------------------------------------------------------
    // Setup default database
    //---------------------------------------------------------------------------------------------
    
    var_intrinsic_db   = new CxExpressionIntrinsicVariableDatabase;
    func_intrinsic_db  = new CxExpressionIntrinsicFunctionDatabase;

    var_db  = new CxExpressionVariableDatabase;
    func_db = new CxExpressionFunctionDatabase;

    owns_var_db  = 1;
    owns_func_db = 1;
}


//-------------------------------------------------------------------------------------------------
// Expression::Expression (constructor)	
//								
//	
//								
//-------------------------------------------------------------------------------------------------
CxExpression::CxExpression(CxString e_string, 
	CxExpressionVariableDatabase *vdb_, 
	CxExpressionFunctionDatabase *fdb_)
{
    //---------------------------------------------------------------------------------------------
    // Copy the expression
    //---------------------------------------------------------------------------------------------
    text = e_string;
    
    //---------------------------------------------------------------------------------------------
    // Mark expression as new
    //---------------------------------------------------------------------------------------------
    status = EVALUATION_NEW;
    
    //---------------------------------------------------------------------------------------------
    // Setup database
    //---------------------------------------------------------------------------------------------

    var_intrinsic_db   = new CxExpressionIntrinsicVariableDatabase;
    func_intrinsic_db  = new CxExpressionIntrinsicFunctionDatabase;
    
	if (vdb_ == NULL ) {
		var_db = new CxExpressionVariableDatabase;
		owns_var_db = 1;
	} else {
		var_db = vdb_;
		owns_var_db = 0;
	}

	if (fdb_ == NULL ) {
		func_db = new CxExpressionFunctionDatabase;
		owns_func_db = 1;
	} else {
		func_db = fdb_;
		owns_func_db = 0;
	}
}


//-------------------------------------------------------------------------------------------------
// Expression::~Expression (destructor)
//
// Free allocated database objects.
//
//-------------------------------------------------------------------------------------------------
CxExpression::~CxExpression()
{
	//---------------------------------------------------------------------------------------------
	// Always delete intrinsic databases (always owned)
	//---------------------------------------------------------------------------------------------
	delete var_intrinsic_db;
	delete func_intrinsic_db;

	//---------------------------------------------------------------------------------------------
	// Only delete user databases if we created them
	//---------------------------------------------------------------------------------------------
	if (owns_var_db) {
		delete var_db;
	}
	if (owns_func_db) {
		delete func_db;
	}
}


//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// EXPRESSION_Parse:						
//								
//	Upon completion, function returns PARSE_SUCCESS or PARSE_ERROR. 
//  If PARSE_ERROR is returned then error_code contains the specific error.
//								
//
//-------------------------------------------------------------------------------------------------
CxExpression::parseStatus
CxExpression::Parse( void )
{
	parseStatus  pStatus = PARSE_ERROR;
		
	//---------------------------------------------------------------------------------------------
	// Parse the expression				
	//---------------------------------------------------------------------------------------------
	ParseToTokens( );

    
	//---------------------------------------------------------------------------------------------
	// Check the syntax					
	//---------------------------------------------------------------------------------------------
	pStatus = CheckSyntax();
	if (pStatus == PARSE_SUCCESS) {
		status = EVALUATION_PARSED;
	} else {
		status = EVALUATION_PARSE_ERROR;
	}

	//---------------------------------------------------------------------------------------------
	// return						
    //---------------------------------------------------------------------------------------------
	return(pStatus);
}


//-------------------------------------------------------------------------------------------------
// EXPRESSION_ParseToTokens :					
//								
//	Parses the string into an array of tokens.  This function always succeeds, however 
//	it can produce token lists with only an END token, or token lists with 'error tokens' inline.	 
// 
//  If an 'error token' is placed inline, then the next token is an END token. A token list with
//  error tokens should be evaluated as it will return an incorrect answer. 
//								
//		Possible normal tokens are:				
//		VARIABLE	  	: a variable name		
//		DOUBLE_NUMBER	: a number			
//		LEFT_PAREN		: the ( symbol		
//		RIGHT_PAREN		: the ) symbol		
//		PLUS_SIGN	  	: the + symbol		
//		MINUS_SIGN	  	: the - symbol		
//		MULT_SIGN		: the * symbol		
//		DIV_SIGN		: the / symbol		
//		COMMA		  	: the , symbol		
//		FUNCTION	  	: a function name		
//		END		  		: the end symbol		
//									
// Possible error tokens are :				
//		UNKNOWN_FUNCTION  : either no db or unknown 
//		UNKNOWN_VARIABLE  : either no db or unknown 
//		ILLEGAL_NUMBER	  : not a valid floating #	
//		UNBALANCED_PARENS : not as many ( as )		
//		UNKNOWN_SYMBOL	  : not any of the others	
//								
// Returns the error token number				
//								
//-------------------------------------------------------------------------------------------------
void
CxExpression::ParseToTokens( void )
{
    // temp string to hold symbols
	char	temp[500];
    char    *tptr = temp;

    int		done = FALSE;
	int		stat;

	int		left_paren_count = 0;
	int		right_paren_count = 0;
	
	double	pdouble;
	char	*cptr;

    // point to first character in source buffer
	cptr = text.data();


	//---------------------------------------------------------------------------------------------
	// eat leading white.						
	//---------------------------------------------------------------------------------------------
	while (CxExpressionToken::iswhite(*cptr)) {
		cptr++;
	}


	//---------------------------------------------------------------------------------------------
	// Nothing remaining ?						
	//---------------------------------------------------------------------------------------------
	if (CxExpressionToken::isnull(*cptr)) {
		token_list.append( CxExpressionToken(CxExpressionToken::END, "", 0.0));
		return;
	}


	//---------------------------------------------------------------------------------------------
	// Do till end of expression					
	//---------------------------------------------------------------------------------------------

	while (!done) {

		tptr = temp;

		//-----------------------------------------------------------------------------------------
		// eat leading white.					
		//-----------------------------------------------------------------------------------------
		while (CxExpressionToken::iswhite(*cptr)) {
			cptr++;
		}

		//-----------------------------------------------------------------------------------------
		// Check for variable name or function name.		
		//-----------------------------------------------------------------------------------------
		if (isalpha((int) *cptr) || *cptr=='"' || *cptr=='$') {
            

            debugPrintf("- ParseToTokens: looks like a function or variable name\n");

			//-------------------------------------------------------------------------------------
			// Get a variable name, or function (limit to 499 chars to prevent overflow)
			//-------------------------------------------------------------------------------------
			while (CxExpressionToken::isvarfunc(*cptr) && (tptr - temp) < 499) {
				*tptr = *cptr;
				cptr++;
				tptr++;
			}

			//-------------------------------------------------------------------------------------
			// Skip any remaining characters if name was too long
			//-------------------------------------------------------------------------------------
			while (CxExpressionToken::isvarfunc(*cptr)) {
				cptr++;
			}

			//-------------------------------------------------------------------------------------
			// Null terminate the name
			//-------------------------------------------------------------------------------------

			*tptr = 0;

			//-------------------------------------------------------------------------------------
			// strip off any spaces before the next token	
			//-------------------------------------------------------------------------------------

			while (CxExpressionToken::iswhite(*cptr)) {
                cptr++;
			}
	
			//-------------------------------------------------------------------------------------
			// Check to see if it is a function.		
			//-------------------------------------------------------------------------------------
			if (CxExpressionToken::isleftparen(*cptr)) {
		
				//---------------------------------------------------------------------------------
				// Which function			
				//---------------------------------------------------------------------------------

                debugPrintf("- ParseToTokens: its a function (%s)\n", temp);

				// see if the function is in the intrinsic library of functions
                if (func_intrinsic_db->FunctionDefined( temp ) == CxExpressionFunctionDatabase::FUNCTION_DEFINED) {

	                debugPrintf("- ParseToTokens: function (%s) found in intrinsic database\n", temp);

					// it is so put it in the token stream
                    token_list.append( CxExpressionToken(CxExpressionToken::FUNCTION, temp, 0.0 ));
			
				} else {

	                debugPrintf("- ParseToTokens: function (%s) not found in intrinsic database\n", temp);

					// it's not lets see if its in the user database
                	if (func_db->FunctionDefined( temp ) == CxExpressionFunctionDatabase::FUNCTION_DEFINED) {

		                debugPrintf("- ParseToTokens: function (%s) found in user database\n", temp);    
	                	token_list.append( CxExpressionToken(CxExpressionToken::FUNCTION, temp, 0.0 ));
    
   		         	} else {

						debugPrintf("- ParseToTokens: function (%s) not found in either database\n", temp);    
                    	token_list.append( CxExpressionToken(CxExpressionToken::UNKNOWN_FUNCTION, temp, 0.0));
                    	token_list.append( CxExpressionToken(CxExpressionToken::END, "", 0.0));
                	}
				}


			} else {

				//---------------------------------------------------------------------------------
				// Variable name			
				//---------------------------------------------------------------------------------

                debugPrintf("- ParseToTokens: its a variable (%s)\n", temp);


				if (var_intrinsic_db->VariableDefined( temp ) == CxExpressionVariableDatabase::VARIABLE_DEFINED) {

	                debugPrintf("- ParseToTokens: its a variable (%s) found in intrinsic database\n", temp);
                    token_list.append( CxExpressionToken(CxExpressionToken::VARIABLE, temp, 0.0 ));
				
				} else {

					debugPrintf("- ParseToTokens: variable (%s) not found in intrinsic database\n", temp);

	                if (var_db->VariableDefined( temp ) == CxExpressionVariableDatabase::VARIABLE_DEFINED) {

		                debugPrintf("- ParseToTokens: variable (%s) found in user database\n", temp);
    	                token_list.append( CxExpressionToken(CxExpressionToken::VARIABLE, temp, 0.0 ));

        	        } else {

		                debugPrintf("- ParseToTokens: variable (%s) not found in either database\n", temp);

	                    token_list.append( CxExpressionToken(CxExpressionToken::UNKNOWN_VARIABLE, temp, 0.0));
    	                token_list.append( CxExpressionToken(CxExpressionToken::END, "", 0.0));
					}
                }
			}
            
		} else	
		
		//-----------------------------------------------------------------------------------------
		// Check for number					
		//-----------------------------------------------------------------------------------------

		if (CxExpressionToken::is_in(*cptr, ".0123456789")) {


            debugPrintf("- ParseToTokens: its a number\n");


			cptr = scanDouble(cptr, &pdouble, &stat);
            
			if (stat == -1) {                


	            debugPrintf("- ParseToTokens: the number could not be parsed\n");

                token_list.append( CxExpressionToken(CxExpressionToken::ILLEGAL_NUMBER, "", 0.0));
                token_list.append( CxExpressionToken(CxExpressionToken::END, "", 0.0));
				return;
			}


            debugPrintf("- ParseToTokens: its a number(%lf)\n", pdouble);


            token_list.append( CxExpressionToken(CxExpressionToken::DOUBLE_NUMBER, "", pdouble));
		
		} else 

		//-----------------------------------------------------------------------------------------
		// Parens							
		//-----------------------------------------------------------------------------------------

		if (CxExpressionToken::isparen(*cptr)) {


            debugPrintf("- ParseToTokens: its a paren\n");


			if (CxExpressionToken::isleftparen(*cptr)) {
                
                debugPrintf("- ParseToTokens: its a left paren\n");

				left_paren_count++;

                token_list.append( CxExpressionToken(CxExpressionToken::LEFT_PAREN, "", 0.0));

			} else {
                
                debugPrintf("- ParseToTokens: its a right paren\n");

				right_paren_count++;
                
                token_list.append( CxExpressionToken(CxExpressionToken::RIGHT_PAREN, "", 0.0));
			}

			cptr++;
            
		} else 

		//-----------------------------------------------------------------------------------------
		// Math Signs							
		//-----------------------------------------------------------------------------------------
		
		if (CxExpressionToken::is_in(*cptr,"+-*/")) {
			
			switch(*cptr) {

				case '+' :
                    
                    debugPrintf("- ParseToTokens: its a + sign\n");

                    token_list.append( CxExpressionToken(CxExpressionToken::PLUS_SIGN, "", 0.0));
					break;

				case '-' :
                    
                    debugPrintf("- ParseToTokens: its a - sign\n");

                    token_list.append( CxExpressionToken(CxExpressionToken::MINUS_SIGN, "", 0.0));

					break;
	
				case '*' :
                    
                    debugPrintf("- ParseToTokens: its a * sign\n");

                    token_list.append( CxExpressionToken(CxExpressionToken::MULT_SIGN, "", 0.0));

					break;
				
				case '/' :
                    
                    debugPrintf("- ParseToTokens: its a / sign\n");

                    token_list.append( CxExpressionToken(CxExpressionToken::DIV_SIGN, "", 0.0));

					break;
			}

			cptr++;

		} else 

		//-----------------------------------------------------------------------------------------
		// comma							
		//-----------------------------------------------------------------------------------------

		if (*cptr==',') {
            
            debugPrintf("- ParseToTokens: its a , sign\n");

            token_list.append( CxExpressionToken(CxExpressionToken::COMMA, "", 0.0));

			cptr++;
		} else 

		//-----------------------------------------------------------------------------------------
		// NULL							
		//-----------------------------------------------------------------------------------------

		if (*cptr==0) {
            
            debugPrintf("- ParseToTokens: its the end of expression string \n");

			if (left_paren_count == right_paren_count) {

                debugPrintf("- ParseToTokens: parent count match \n");

                token_list.append( CxExpressionToken(CxExpressionToken::END, "", 0.0));

				done = 1;
                
                debugPrintf("- ParseToTokens: return from parse! \n");

                return;
                
			} else {

                debugPrintf("- ParseToTokens: parent count don't match \n");
                
                token_list.append( CxExpressionToken(CxExpressionToken::UNBALANCED_PARENS, "", 0.0));
                token_list.append( CxExpressionToken(CxExpressionToken::END, "", 0.0));

				return;
			}
            
		} else	

		//-----------------------------------------------------------------------------------------
		// Unknown Symbol						
		//-----------------------------------------------------------------------------------------

		{
            
            debugPrintf("- ParseToTokens: unknown symbol \n");

			temp[0] = *cptr;
			temp[1] = 0;
            
            token_list.append( CxExpressionToken(CxExpressionToken::UNKNOWN_SYMBOL, temp, 0.0));
            token_list.append( CxExpressionToken(CxExpressionToken::END, "", 0.0));
            
			return;
		}
	}
	
	return;
}


//-------------------------------------------------------------------------------------------------
// EXPRESSION_CheckSyntax :					
//								
// Set the token stack pointer to the top and start	walking down the token list looking for 
// obvious errors from the parse stage.  If this check succeeds then a more in depth check of
// correct syntax is made in CheckSyntax2 called at the end of this method.		
//								
//-------------------------------------------------------------------------------------------------
CxExpression::parseStatus
CxExpression::CheckSyntax( void )
{	
	int	args;
	int localError = 0;

	current_token = 0;

	//---------------------------------------------------------------------------------------------
	// Do syntax check for Existing Errors from parse step 
	//---------------------------------------------------------------------------------------------
	CxExpressionToken token	= CurrentToken( );
    
    debugPrintf("- CheckSyntax: got first token");
    debugTokenDump( token );
    debugPrintf("\n");
    
	//---------------------------------------------------------------------------------------------
	// looks for obvious error tokens in the stack
	//
	//---------------------------------------------------------------------------------------------

	while (token.ttype != CxExpressionToken::END) {

		switch(token.ttype)
		{
            case CxExpressionToken::UNKNOWN_VARIABLE:

				error_code  = UNKNOWN_VARIABLE;
				return( PARSE_ERROR );
			
            case CxExpressionToken::UNKNOWN_FUNCTION:
			  	error_code = UNKNOWN_FUNCTION;
			  	return( PARSE_ERROR );
			
            case CxExpressionToken::ILLEGAL_NUMBER:
			  	error_code = ILLEGAL_NUMBER;
			  	return( PARSE_ERROR );
			
            case CxExpressionToken::UNBALANCED_PARENS:
			  	error_code = UNBALANCED_PARENS;
			 	return( PARSE_ERROR );
			
			case CxExpressionToken::UNKNOWN_SYMBOL: 
			  	error_code = UNKNOWN_SYMBOL;
			  	return( PARSE_ERROR );
			
			default:
				break;
		}

		token = NextToken( );
        
        debugPrintf("- CheckSyntax: got next token");
        debugTokenDump( token );
        debugPrintf("\n");
	}

	//---------------------------------------------------------------------------------------------
	// Reset token pointer and check expression		
	//---------------------------------------------------------------------------------------------

	current_token = 0;

	return( CheckSyntax2( NORM_EXPRESSION, &args));
}


//-------------------------------------------------------------------------------------------------
// EXPRESSION_CheckSyntax2 :					
//								
// This a recursive function that can be invoked in any	one of three modes. 
// (normal) (parenthesis) (function)	
// 
// A basic set of <this token> vs <next token> logic is	applied, and in additon, specific mode 
// logic is applied where differences lie.					
//								
// If CheckSyntax failes it returns PARSE_ERROR and error_code contains the specific error
//					
//-------------------------------------------------------------------------------------------------

CxExpression::parseStatus
CxExpression::CheckSyntax2(int mode, int *args)
{
    CxExpressionToken token;
    CxExpressionToken next_token;
    CxExpressionToken func_token;
    
	int	comma_count;
	int	error_token;
	int	first;

	//---------------------------------------------------------------------------------------------
	// Initialize							
	//---------------------------------------------------------------------------------------------

	first		= 1;
	error_token = 0;
	comma_count = 0;

	//---------------------------------------------------------------------------------------------
	// Check um out						
	//---------------------------------------------------------------------------------------------

	while (1) {

	   	//-----------------------------------------------------------------------------------------
	   	// Get the current token, and the next token.		
	   	//-----------------------------------------------------------------------------------------
		token	  = CurrentToken( );
        
        debugPrintf("- CheckSyntax2: got current token --> ");
        debugTokenDump( token );;
        debugPrintf("\n");

		//-----------------------------------------------------------------------------------------
		// if token is end, just set next token to the end token as well as we always need a 
		// a next token
		//-----------------------------------------------------------------------------------------        
        if (token.ttype == CxExpressionToken::END) {
            next_token = token;
        } else {
            next_token = NextToken( );
        }
        
        debugPrintf("- CheckSyntax2: got next token --> ");
        debugTokenDump( next_token );
        debugPrintf("\n");
        

	    //------------------------------------------------------------------------------------------
	    // Switch on this token					
	    //------------------------------------------------------------------------------------------
		switch(token.ttype) {

     		case CxExpressionToken::PLUS_SIGN:
				
				switch( next_token.ttype ) {
                    case CxExpressionToken::PLUS_SIGN:
						break;
					
                    case CxExpressionToken::MINUS_SIGN:
						break;
					
                    case CxExpressionToken::MULT_SIGN:
						error_code = MULT_AFTER_PLUS;
						return( PARSE_ERROR );
					
                    case CxExpressionToken::DIV_SIGN:
						error_code = DIV_AFTER_PLUS;
						return( PARSE_ERROR );
					
                    case CxExpressionToken::LEFT_PAREN:
						break;
					
                    case CxExpressionToken::RIGHT_PAREN:
						error_code = RIGHT_PAREN_AFTER_PLUS;
						return( PARSE_ERROR );
					
                    case CxExpressionToken::DOUBLE_NUMBER:
						break;
					
                    case CxExpressionToken::VARIABLE:
						break;
					
                    case CxExpressionToken::FUNCTION:
						break;
			
                    case CxExpressionToken::COMMA:
						error_code = COMMA_AFTER_PLUS;
						return( PARSE_ERROR );
			
                    case CxExpressionToken::END:
						error_code = END_AFTER_PLUS;
						return( PARSE_ERROR );

					default:
						error_code = UNEXPECTED_TOKEN;
						return( PARSE_ERROR );
				
				}
				break;

			//-------------------------------------------------------------------------------------	
		
			case CxExpressionToken::MINUS_SIGN:
		  
				switch( next_token.ttype ) {
                    
					case CxExpressionToken::PLUS_SIGN:
						break;
					
                    case CxExpressionToken::MINUS_SIGN:
						break;
					
                    case CxExpressionToken::MULT_SIGN:
						error_code = MULT_AFTER_MINUS;
						return( PARSE_ERROR );
					
                    case CxExpressionToken::DIV_SIGN:
						error_code = DIV_AFTER_MINUS;
						return( PARSE_ERROR );
					
                    case CxExpressionToken::LEFT_PAREN:
						break;
					
                    case CxExpressionToken::RIGHT_PAREN:
						error_code = RIGHT_PAREN_AFTER_MINUS;
						return( PARSE_ERROR );
					
                    case CxExpressionToken::DOUBLE_NUMBER:
						break;
					
                    case CxExpressionToken::VARIABLE:
						break;
					
                    case CxExpressionToken::FUNCTION:
						break;
			
                    case CxExpressionToken::COMMA:
						error_code = COMMA_AFTER_MINUS;
						return( PARSE_ERROR );
			
                    case CxExpressionToken::END:
						error_code = END_AFTER_MINUS;
						return( PARSE_ERROR );

					default:
						error_code = UNEXPECTED_TOKEN;
						return( PARSE_ERROR );
				
				}
				break;

			//-------------------------------------------------------------------------------------	

			case CxExpressionToken::MULT_SIGN:

				if (first) { 
					error_code = INVALID_FIRST_SYMBOL;
					return( PARSE_ERROR ); 
				}
				
				switch( next_token.ttype ) {
					
                    case CxExpressionToken::PLUS_SIGN:
						break;
					
                    case CxExpressionToken::MINUS_SIGN:
						break;
					
                    case CxExpressionToken::MULT_SIGN:
						error_code = MULT_AFTER_MULT;
						return( PARSE_ERROR );
					
                    case CxExpressionToken::DIV_SIGN:
						error_code = DIV_AFTER_MULT;
						return( PARSE_ERROR );
					
                    case CxExpressionToken::LEFT_PAREN:
						break;
					
                    case CxExpressionToken::RIGHT_PAREN:
						error_code = RIGHT_PAREN_AFTER_MULT;
						return( PARSE_ERROR );
					
                    case CxExpressionToken::DOUBLE_NUMBER:
						break;
					
                    case CxExpressionToken::VARIABLE:
						break;
					
                    case CxExpressionToken::FUNCTION:
						break;
			
                    case CxExpressionToken::COMMA:
						error_code = COMMA_AFTER_MULT;
						return( PARSE_ERROR );
			
                    case CxExpressionToken::END:
						error_code = END_AFTER_MULT;
						return( PARSE_ERROR );

					default:
						error_code = UNEXPECTED_TOKEN;
						return( PARSE_ERROR );
				

				}
				break;

			//-------------------------------------------------------------------------------------	

			case CxExpressionToken::DIV_SIGN:
		  
				if (first) { 
					error_code = INVALID_FIRST_SYMBOL;
					return( PARSE_ERROR ); 
				}
				
				switch( next_token.ttype ) {
				
                    case CxExpressionToken::PLUS_SIGN:
						break;
					
					case CxExpressionToken::MINUS_SIGN: 
						break;
					
					case CxExpressionToken::MULT_SIGN: 
						error_code = MULT_AFTER_DIV;
						return( PARSE_ERROR );
					
					case CxExpressionToken::DIV_SIGN: 
						error_code = DIV_AFTER_DIV;
						return( PARSE_ERROR );
					
					case CxExpressionToken::LEFT_PAREN: 
						break;
					
					case CxExpressionToken::RIGHT_PAREN: 
						error_code = RIGHT_PAREN_AFTER_DIV;
						return( PARSE_ERROR );
					
					case CxExpressionToken::DOUBLE_NUMBER: 
						break;
					
					case CxExpressionToken::VARIABLE: 
						break;
					
					case CxExpressionToken::FUNCTION: 
						break;
			
					case CxExpressionToken::COMMA: 
						error_code = COMMA_AFTER_DIV;
						return( PARSE_ERROR );
			
					case CxExpressionToken::END: 
						error_code = END_AFTER_DIV;
						return( PARSE_ERROR );

					default:
						error_code = UNEXPECTED_TOKEN;
						return( PARSE_ERROR );

				}
				break;


			//-------------------------------------------------------------------------------------	


			case CxExpressionToken::COMMA :

				if (mode==NORM_EXPRESSION) { 
					error_code = INVALID_COMMA_PLACEMENT;
					return( PARSE_ERROR ); 
				}
			
				if (mode==PARN_EXPRESSION) { 
					error_code = INVALID_COMMA_PLACEMENT;
					return( PARSE_ERROR ); 
				}
			
				if (first) { 
					error_code = INVALID_FIRST_SYMBOL;
					return( PARSE_ERROR ); 
				}
			
				comma_count++;
			
				switch( next_token.ttype ) {

        	        case CxExpressionToken::PLUS_SIGN:
						break;
					
                	case CxExpressionToken::MINUS_SIGN:
						break;
					
	                case CxExpressionToken::MULT_SIGN:
						error_code = MULT_AFTER_COMMA;
						return( PARSE_ERROR );
				
            	    case CxExpressionToken::DIV_SIGN:
						error_code = DIV_AFTER_COMMA;
						return( PARSE_ERROR );
					
	                case CxExpressionToken::LEFT_PAREN:
						break;
					
        	        case CxExpressionToken::RIGHT_PAREN:
						error_code = RIGHT_PAREN_AFTER_COMMA;
						return( PARSE_ERROR );
					
	                case CxExpressionToken::DOUBLE_NUMBER:
						break;
					
                	case CxExpressionToken::VARIABLE:
						break;
					
	                case CxExpressionToken::FUNCTION:
						break;
			
    	            case CxExpressionToken::COMMA:
						error_code = COMMA_AFTER_COMMA;
						return( PARSE_ERROR );
			
        	        case CxExpressionToken::END:
						error_code = END_AFTER_COMMA;
						return( PARSE_ERROR );

					default:
						error_code = UNEXPECTED_TOKEN;
						return( PARSE_ERROR );
				
				}
				break;

			//-------------------------------------------------------------------------------------	

			case CxExpressionToken::LEFT_PAREN  :

				switch( next_token.ttype ) {
					
                	case CxExpressionToken::PLUS_SIGN:
						break;
					
	                case CxExpressionToken::MINUS_SIGN:
						break;
							
                	case CxExpressionToken::MULT_SIGN:
						error_code = MULT_AFTER_LEFT_PAREN;
						return( PARSE_ERROR );
					
	                case CxExpressionToken::DIV_SIGN:
						error_code = DIV_AFTER_LEFT_PAREN;
						return( PARSE_ERROR );
						
                	case CxExpressionToken::LEFT_PAREN:
						break;
					
	                case CxExpressionToken::RIGHT_PAREN:
						error_code = RIGHT_PAREN_AFTER_LEFT_PAREN;
						return( PARSE_ERROR );
					
                	case CxExpressionToken::DOUBLE_NUMBER:
						break;
					
            	    case CxExpressionToken::VARIABLE:
						break;
					
	                case CxExpressionToken::FUNCTION:
						break;
			
        	        case CxExpressionToken::COMMA:
						error_code = COMMA_AFTER_LEFT_PAREN;
						return( PARSE_ERROR );
			
	                case CxExpressionToken::END:
						error_code = END_AFTER_LEFT_PAREN;
						return( PARSE_ERROR );

					default:
						error_code = UNEXPECTED_TOKEN;
						return( PARSE_ERROR );
				}

			   
				//---------------------------------------------------------------------------------
				// check for NULL expression, excluding function body	
				//---------------------------------------------------------------------------------
				if ((next_token.ttype == CxExpressionToken::RIGHT_PAREN) && 
				    (mode!=FUNC_EXPRESSION)) {
					error_code = RIGHT_PAREN_AFTER_LEFT_PAREN;
					return( PARSE_ERROR );
				}
			
				//---------------------------------------------------------------------------------
				// if not a NULL function eval the inside		
				//---------------------------------------------------------------------------------
				if (next_token.ttype != CxExpressionToken::RIGHT_PAREN) {
					CheckSyntax2(PARN_EXPRESSION, args);
				} 

				//---------------------------------------------------------------------------------
				// if error bail					
				//---------------------------------------------------------------------------------
				if (error_code == PARSE_ERROR) {
					return( PARSE_ERROR );
				}
				break;


			//-------------------------------------------------------------------------------------	

   			case CxExpressionToken::RIGHT_PAREN :
		
				//---------------------------------------------------------------------------------
				// if function mode, then this is end recurse back 
				//---------------------------------------------------------------------------------
				if (first && (mode==FUNC_EXPRESSION)) {
					*args = 0;
					return( PARSE_SUCCESS );
				}

				if (first) { 
					error_code = INVALID_FIRST_SYMBOL;
					return( PARSE_ERROR ); 
				}

				switch( next_token.ttype ) {
		
    	            case CxExpressionToken::PLUS_SIGN:
						break;
					
	                case CxExpressionToken::MINUS_SIGN:
						break;
					
        	        case CxExpressionToken::MULT_SIGN:
						break;
					
	                case CxExpressionToken::DIV_SIGN:
						break;
					
         	       case CxExpressionToken::LEFT_PAREN:
						break;
					
                	case CxExpressionToken::RIGHT_PAREN:
						break;
					
	                case CxExpressionToken::DOUBLE_NUMBER:
						error_code = NUMBER_AFTER_RIGHT_PAREN;
						return( PARSE_ERROR );
						
                	case CxExpressionToken::VARIABLE:
						error_code = VARIABLE_AFTER_RIGHT_PAREN;
						return( PARSE_ERROR );
					
	                case CxExpressionToken::FUNCTION:
						error_code = FUNCTION_AFTER_RIGHT_PAREN;
						return( PARSE_ERROR );
			
	                case CxExpressionToken::COMMA:
						break;
			
        	        case CxExpressionToken::END:
						break;
						
					default:
						error_code = UNEXPECTED_TOKEN;
						return( PARSE_ERROR );
				
				}

				//---------------------------------------------------------------------------------
				// Return from recurse.				
				//---------------------------------------------------------------------------------
				if (mode==PARN_EXPRESSION) { 
					return( PARSE_SUCCESS ) ; 
				}
				if (mode==FUNC_EXPRESSION) { 
					*args = comma_count+1;
					return( PARSE_SUCCESS );
				}
				
				break;

			//-------------------------------------------------------------------------------------	


      		case CxExpressionToken::DOUBLE_NUMBER:

				switch( next_token.ttype ) {
                	case CxExpressionToken::PLUS_SIGN:
						break;
					
	                case CxExpressionToken::MINUS_SIGN:
						break;
					
            	    case CxExpressionToken::MULT_SIGN:
						break;
					
	                case CxExpressionToken::DIV_SIGN:
						break;
						
                	case CxExpressionToken::LEFT_PAREN:
						error_code = LEFT_PAREN_AFTER_NUMBER;
                    	return( PARSE_ERROR );
					
	                case CxExpressionToken::RIGHT_PAREN:
						break;
					
        	        case CxExpressionToken::DOUBLE_NUMBER:
						error_code = NUMBER_AFTER_NUMBER;
						return( PARSE_ERROR );
					
	                case CxExpressionToken::VARIABLE:
						error_code = VARIABLE_AFTER_NUMBER;
        	            return( PARSE_ERROR );
					
	                case CxExpressionToken::FUNCTION:
						error_code = FUNCTION_AFTER_NUMBER;
        	           	return( PARSE_ERROR );
						
                	case CxExpressionToken::COMMA:
						break;
					
                	case CxExpressionToken::END:
						break;
					
					default:
						error_code = UNEXPECTED_TOKEN;
						return( PARSE_ERROR );
				
				}
				break;


			//-------------------------------------------------------------------------------------	

			case CxExpressionToken::VARIABLE	   :

				switch( next_token.ttype ) {
                	case CxExpressionToken::PLUS_SIGN:
						break;
					
	                case CxExpressionToken::MINUS_SIGN:
						break;
					
            	    case CxExpressionToken::MULT_SIGN:
						break;
					
	                case CxExpressionToken::DIV_SIGN:
						break;
						
                	case CxExpressionToken::LEFT_PAREN:
						error_code = LEFT_PAREN_AFTER_VARIABLE;
                    	return( PARSE_ERROR );
					
	                case CxExpressionToken::RIGHT_PAREN:
						break;
						
                	case CxExpressionToken::DOUBLE_NUMBER:
						error_code = NUMBER_AFTER_VARIABLE;
                    	return( PARSE_ERROR );
					
	                case CxExpressionToken::VARIABLE:
						error_code = VARIABLE_AFTER_VARIABLE;
        	            return( PARSE_ERROR );
					
                	case CxExpressionToken::FUNCTION:
						error_code = FUNCTION_AFTER_VARIABLE;
                	    return( PARSE_ERROR );
					
	                case CxExpressionToken::COMMA:
						break;
					
        	        case CxExpressionToken::END:
						break;
					
					default:
						error_code = UNEXPECTED_TOKEN;
						return( PARSE_ERROR );
				
				}
				break;

			//-------------------------------------------------------------------------------------	

			case CxExpressionToken::FUNCTION:

				switch( next_token.ttype ) {
                	case CxExpressionToken::PLUS_SIGN:
						error_code = PLUS_AFTER_FUNCTION;
                    	return( PARSE_ERROR );
					
	                case CxExpressionToken::MINUS_SIGN:
						error_code = MINUS_AFTER_FUNCTION;
        	            return( PARSE_ERROR );
					
                	case CxExpressionToken::MULT_SIGN:
						error_code = MULT_AFTER_FUNCTION;
                    	return( PARSE_ERROR );
					
	                case CxExpressionToken::DIV_SIGN:
						error_code = DIV_AFTER_FUNCTION;
        	            return( PARSE_ERROR );
					
                	case CxExpressionToken::LEFT_PAREN:
                    	break;
						
 	               case CxExpressionToken::RIGHT_PAREN:
						error_code = RIGHT_PAREN_AFTER_FUNCTION;
        	            return( PARSE_ERROR );
					
                	case CxExpressionToken::DOUBLE_NUMBER:
						error_code = NUMBER_AFTER_FUNCTION;
                    	return( PARSE_ERROR );
					
	                case CxExpressionToken::VARIABLE:
						error_code = VARIABLE_AFTER_FUNCTION;
        	            return( PARSE_ERROR );
					
                	case CxExpressionToken::FUNCTION:
						error_code = FUNCTION_AFTER_FUNCTION;
                	    return( PARSE_ERROR );
					
	                case CxExpressionToken::COMMA:
						error_code = COMMA_AFTER_FUNCTION;
        	            return( PARSE_ERROR );
						
                	case CxExpressionToken::END:
						error_code = END_AFTER_FUNCTION;
                    	return( PARSE_ERROR );
					
					default:
						error_code = UNEXPECTED_TOKEN;
						return( PARSE_ERROR );
				}

				func_token = token;

				NextToken( );
	
				// recurse
				if (CheckSyntax2(FUNC_EXPRESSION, args) == PARSE_ERROR) {
					return( PARSE_ERROR );
				}
	
				func_token.value = (double) *args;

				break;

			//-------------------------------------------------------------------------------------	

			case CxExpressionToken::END :

				if (mode==PARN_EXPRESSION) { 
					error_code = UNBALANCED_PARENS; 
					return( PARSE_ERROR );
				}

				if (mode==FUNC_EXPRESSION) { 
					error_code = UNBALANCED_PARENS;
					return( PARSE_ERROR );
				}

				return( PARSE_SUCCESS );


			//-------------------------------------------------------------------------------------	

			default  :

				debugPrintf("FATAL ERROR : UNKNOWN TOKEN TYPE IN CHECK SYNTAX\n");
				exit(1);
	   	}

	   first=0;
	}
}


//-------------------------------------------------------------------------------------------------
// EXPRESSION_DumpTokens :						
//								
//		Dumps the token list to stdout.				
//		Used for debug.						
//								
//-------------------------------------------------------------------------------------------------
void CxExpression::DumpTokens( void )
{
    for (int c=0; c<token_list.entries(); c++) {

        CxExpressionToken token = token_list.at(c);
        token.Dump();
    }
}


//-------------------------------------------------------------------------------------------------
// EXPRESSION_UnParseExpression :				
//								
//		Using the token list, build an ascii representation of	
//		the expression.						
//								
//-------------------------------------------------------------------------------------------------
CxString
CxExpression::UnParseExpression(int num, char *s)
{
    CxString outputString;
       
    for (int c=0; c<token_list.entries(); c++) {

        CxExpressionToken token = token_list.at(c);
        outputString = outputString + token.asString();
    }
    
    return( outputString );
}


//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
// EXPRESSION_Evaluate :					
//								
//		Evalute the expressions token list.			
// This can generate run-time errors in the evaluation.	
//								
// Possible return values :				
//		OK			   : success
//		NULL_EXPRESSION : no expression
//								
//								
//-------------------------------------------------------------------------------------------------
CxExpression::expressionStatus
CxExpression::Evaluate(double *result)
{
	int	stat;

	//---------------------------------------------------------------------------------------------
	// If error from parse then bail			
	//---------------------------------------------------------------------------------------------

	if (status == EVALUATION_NEW) {
		return( EVALUATION_PARSE_ERROR );
	}

	if (status == EVALUATION_PARSE_ERROR) {
		return(EVALUATION_PARSE_ERROR );
	}

	if (status == EVALUATION_ERROR) {
		return(EVALUATION_ERROR);
	}

	if (status == EVALUATION_SUCCESS) {
		*result = this->result;
		return(EVALUATION_SUCCESS);
	}

	if (status == EVALUATION_PARSED) {
	
		//---------------------------------------------------------------------------------------------
		// Set up jump vector for fatal errors			
		//---------------------------------------------------------------------------------------------
		stat = setjmp(env_buf);
		if (stat) {

			//-----------------------------------------------------------------------------------------
			// EVALUATION ERROR				
			//-----------------------------------------------------------------------------------------
			error_token = current_token;
			error_code  = stat;
			status	    = EVALUATION_ERROR;
			return(status);
		}
		
		debugPrintf("-----[ EVALUATION ]------------------------------\n");

		//---------------------------------------------------------------------------------------------
		// Reset the token stack pointer, and get the first	
		// token.						
		//---------------------------------------------------------------------------------------------
		current_token = 0;
		error_token	  = 0;
    
		CxExpressionToken token = CurrentToken( );

		//---------------------------------------------------------------------------------------------
		// Check for NULL expression				
		//---------------------------------------------------------------------------------------------
		if (token.ttype == CxExpressionToken::END) {
			Error(NULL_EXPRESSION);
		}

		//---------------------------------------------------------------------------------------------
		// Start down the recursion tree
		//---------------------------------------------------------------------------------------------
		Level2(result);

		//---------------------------------------------------------------------------------------------
		// Mark the expression as evaluated and cache the result.
		//---------------------------------------------------------------------------------------------
		this->result = *result;
		status = EVALUATION_SUCCESS;

		//---------------------------------------------------------------------------------------------
		// Return						
		//---------------------------------------------------------------------------------------------
		return(EVALUATION_SUCCESS);
	}

	return(EVALUATION_ERROR);
}


//-------------------------------------------------------------------------------------------------
// EXPRESSION_Level1 :						
//								
//		Handle functions with unknown number of args		
//								
// Possible Error Codes :					
//		NONE						
//								
//-------------------------------------------------------------------------------------------------
void
CxExpression::Level1(double *result_array, int *args)
{
	int		c;
	int		op;

	//---------------------------------------------------------------------------------------------
	// Grab the current token (left paren)			
	//---------------------------------------------------------------------------------------------
	CxExpressionToken token = CurrentToken( );
	op = token.ttype;

	//---------------------------------------------------------------------------------------------
	// Grab the next token					
	//---------------------------------------------------------------------------------------------
	token = NextToken( );

	if (token.ttype == CxExpressionToken::RIGHT_PAREN) {
		op	  = token.ttype;
		token = NextToken( );
	}

	//---------------------------------------------------------------------------------------------
	// Grab the current token				
	//---------------------------------------------------------------------------------------------
	c = 0;
	while (op != CxExpressionToken::RIGHT_PAREN) {

		//-----------------------------------------------------------------------------------------
		// Check for too many arguments (max 20)
		//-----------------------------------------------------------------------------------------
		if (c >= 20) {
			Error(PRIM_E);
		}

		//-----------------------------------------------------------------------------------------
		// Start at the top.
		//-----------------------------------------------------------------------------------------
		Level2(&(result_array[c]));
		c++;

		//-----------------------------------------------------------------------------------------
		// Get the current operator			
		//-----------------------------------------------------------------------------------------
		token = CurrentToken( );;
		op	  = token.ttype;

		//-----------------------------------------------------------------------------------------
		// Get the next token				
		//-----------------------------------------------------------------------------------------
		token = NextToken( );
	}


	debugPrintf("ARGS=%d\n", c);

	for (int i=0; i<c; i++) {
		debugPrintf("result[%d]=%lf\n",i,result_array[i]);
	}

	*args = c;
}


//-------------------------------------------------------------------------------------------------
// EXPRESSION_Level2 :						
//								
//		Handle addition and subtraction expressions			
// left to right						
//								
// Possible Error Codes :					
//		NONE						
//								
//-------------------------------------------------------------------------------------------------
void
CxExpression::Level2(double *result)
{
	int			op;
	double		hold;

	//---------------------------------------------------------------------------------------------
	// Move down the tree					
	//---------------------------------------------------------------------------------------------

	Level3(result);

	//---------------------------------------------------------------------------------------------
	// Now handle simple expressions with + or -		
	//---------------------------------------------------------------------------------------------
		
	CxExpressionToken token = CurrentToken( );
	op	  = token.ttype;

	while (( op == CxExpressionToken::PLUS_SIGN) || ( op == CxExpressionToken::MINUS_SIGN)) {

		//-----------------------------------------------------------------------------------------
		// Grab the next token				
		//-----------------------------------------------------------------------------------------

		token = NextToken( );

		//-----------------------------------------------------------------------------------------
		// Move down the tree to get second token	
		//-----------------------------------------------------------------------------------------

		Level3( &hold);

		//-----------------------------------------------------------------------------------------
		// Perform the math.				
		//-----------------------------------------------------------------------------------------

		Arith(op, result, &hold);

		//-----------------------------------------------------------------------------------------
		// Get the current token, and look at operator 
		//-----------------------------------------------------------------------------------------
		
		token = CurrentToken( );
		op	  = token.ttype;
	}
	
}

	
//-------------------------------------------------------------------------------------------------
// EXPRESSION_Level3 :						
//								
//		Handle multiply and divide expressions left to right	
//								
// Possible Error Codes :					
//		NONE						
//								
//-------------------------------------------------------------------------------------------------
void
CxExpression::Level3(double *result)
{
	int			  op;
	double		  hold;

	//---------------------------------------------------------------------------------------------
	// Move down the tree					
	//---------------------------------------------------------------------------------------------

	Level4(result);


	//---------------------------------------------------------------------------------------------
	// Get the current token				
	//---------------------------------------------------------------------------------------------
 
    CxExpressionToken token = CurrentToken( );

	op	  = token.ttype;

	while (( op == CxExpressionToken::MULT_SIGN) || (op == CxExpressionToken::DIV_SIGN)) {
	
		//-----------------------------------------------------------------------------------------
		// Get the next token				
		//-----------------------------------------------------------------------------------------

		token = NextToken( );

		//-----------------------------------------------------------------------------------------
		// Move down the tree to get the second token	
		//-----------------------------------------------------------------------------------------

		Level4( &hold);

		//-----------------------------------------------------------------------------------------
		// Perform math.				
		//-----------------------------------------------------------------------------------------

		Arith(  op, result, &hold);

		//-----------------------------------------------------------------------------------------
		// Get the current token, and look at operator 
		//-----------------------------------------------------------------------------------------

		token = CurrentToken( );
		op	  = token.ttype;
	}
}


//-------------------------------------------------------------------------------------------------
// EXPRESSION_Level4 :						
//								
//		Handle exponent operations. (not used)		
//								
// Possible Error Codes :					
//		NONE						
//								
//-------------------------------------------------------------------------------------------------
void
CxExpression::Level4( double *result)
{
	double			hold;

	//---------------------------------------------------------------------------------------------
	// Move down the tree					
	//---------------------------------------------------------------------------------------------

	Level5( result);

	//---------------------------------------------------------------------------------------------
	// Get the current token				
	//---------------------------------------------------------------------------------------------

    CxExpressionToken token = CurrentToken( );
		
	if (token.ttype == CxExpressionToken::EXP_SIGN) {
		
		//-----------------------------------------------------------------------------------------
		// Get the next token				
		//-----------------------------------------------------------------------------------------

		token = NextToken( );

		//-----------------------------------------------------------------------------------------
		// Move down the tree to get exponent value	
		//-----------------------------------------------------------------------------------------

		Level4( &hold);

		//-----------------------------------------------------------------------------------------
		// Perform math.				
		//-----------------------------------------------------------------------------------------

		Arith(CxExpressionToken::EXP_SIGN, result, &hold);

		//-----------------------------------------------------------------------------------------
		// Get the current token			
		//-----------------------------------------------------------------------------------------
		
		token = CurrentToken( );
	}
}


//-------------------------------------------------------------------------------------------------
// EXPRESSION_Level5 :						
//								
//		Get leading sign and apply unary operations		
//								
// Possible Error Codes :					
//		NONE						
//								
//-------------------------------------------------------------------------------------------------
void
CxExpression::Level5( double *result)
{
	int				op;

	//---------------------------------------------------------------------------------------------
	// Get the current token				
	//---------------------------------------------------------------------------------------------

    CxExpressionToken token = CurrentToken( );

	//---------------------------------------------------------------------------------------------
	// If no sign, then assume positive.			
	//---------------------------------------------------------------------------------------------

	op = CxExpressionToken::PLUS_SIGN;

	//---------------------------------------------------------------------------------------------
	// Now get the sign it there is one.			
	//---------------------------------------------------------------------------------------------
	
	while ((token.ttype == CxExpressionToken::PLUS_SIGN) || (token.ttype == CxExpressionToken::MINUS_SIGN)) {
		// Toggle sign for each minus (plus signs don't change anything)
		if (token.ttype == CxExpressionToken::MINUS_SIGN) {
			op = (op == CxExpressionToken::PLUS_SIGN) ? CxExpressionToken::MINUS_SIGN : CxExpressionToken::PLUS_SIGN;
		}
		token = NextToken( );
	}

	//---------------------------------------------------------------------------------------------
	// Now get the operand.				
	//---------------------------------------------------------------------------------------------

	Level6(result);

	//---------------------------------------------------------------------------------------------
	// Now perform the unary operation.			
	//---------------------------------------------------------------------------------------------

	Unary(op, result);

}


//-------------------------------------------------------------------------------------------------
// EXPRESSION_Level6 :						
//								
// Decides whether or not a new expression is starting (left paren) or if we have a primitive 
// number, variable or function.  
//
// If a primitive then call	Primitive()					
//								
// Possible Error Codes :					
//		EXPERR_L6A					
//								
//-------------------------------------------------------------------------------------------------
void
CxExpression::Level6(double *result)
{
	//---------------------------------------------------------------------------------------------
	// Get the current token.				
	//---------------------------------------------------------------------------------------------

    CxExpressionToken token = CurrentToken( );
	
	//---------------------------------------------------------------------------------------------
	// Is this the beginning of a new expression ?		
	//---------------------------------------------------------------------------------------------

	if (token.ttype == CxExpressionToken::LEFT_PAREN) {

		//-----------------------------------------------------------------------------------------
		// Left paren, must be new expression. Get		
		// the next token				
		//-----------------------------------------------------------------------------------------
		
		token = NextToken( );

		//-----------------------------------------------------------------------------------------
		// Start at the top of the hierachy.		
		//-----------------------------------------------------------------------------------------

		Level2(result);

		//-----------------------------------------------------------------------------------------
		// Get the current token.			
		//-----------------------------------------------------------------------------------------

		token = CurrentToken( );
		
		if (token.ttype != CxExpressionToken::RIGHT_PAREN) {
			Error( L6A);
			exit(1);
		}

		//-----------------------------------------------------------------------------------------
		// Skip over right paren.			
		//-----------------------------------------------------------------------------------------

		token = NextToken( );

	} else {

		//-----------------------------------------------------------------------------------------
		// We got a primitive (number, variable, func) 
		//-----------------------------------------------------------------------------------------

		Primitive( result );
	}	
}


//-------------------------------------------------------------------------------------------------
// EXPRESSION_Primitive :					
//								
// At this point the token is assumed to be a primitive. A primitive can be a number, a 
// variable, or a function.  If it is a function then call Level1() that handles functions 
// with unknown number of args	
//								
// Possible Error Codes :					
//		EXPERR_PRIM_A					
//		EXPERR_PRIM_B					
//		EXPERR_PRIM_C					
//		EXPERR_PRIM_D					
//		EXPERR_PRIM_E					
//		EXPERR_PRIM_F					
//								
//-------------------------------------------------------------------------------------------------
CxExpression::expressionError
CxExpression::Primitive(double *result)
{
    CxExpressionToken func_token;
	double	arg_list[20];
	int		stat;
	int		key;
	int		args;
			
	//---------------------------------------------------------------------------------------------
	// Get current token, and handle appropriatly		
	//---------------------------------------------------------------------------------------------
    CxExpressionToken token = CurrentToken( );

	switch(token.ttype) {
	
		//-----------------------------------------------------------------------------------------
		// Handle number.				
		//-----------------------------------------------------------------------------------------
        case CxExpressionToken::DOUBLE_NUMBER :
			
			//-------------------------------------------------------------------------------------
			// grab the value from the token	
			//-------------------------------------------------------------------------------------
			*result = token.value;
		
			//-------------------------------------------------------------------------------------
			// get the next token			
			//-------------------------------------------------------------------------------------
			token	= NextToken( );

			return(OK);
	
		//-----------------------------------------------------------------------------------------
		// Handle variable				
		//-----------------------------------------------------------------------------------------
        case CxExpressionToken::VARIABLE	   :
			
			//-------------------------------------------------------------------------------------
			// if no variable evaluation user	
			// routine is available then		
			// return undefined.			
			//-------------------------------------------------------------------------------------
            
			stat = var_db->VariableEvaluate(token.text, result);
			if (stat == CxExpressionVariableDatabase::VARIABLE_UNDEFINED) {

				stat = var_intrinsic_db->VariableEvaluate(token.text, result);
				if (stat == CxExpressionVariableDatabase::VARIABLE_UNDEFINED) {
					Error(PRIM_A);
				}
			}
			// If var_db found the variable, result is already set
				
			//-------------------------------------------------------------------------------------
			// get the next token			
			//-------------------------------------------------------------------------------------
			token = NextToken( );

			return(OK);

		//-----------------------------------------------------------------------------------------
		// Handle function				
		//-----------------------------------------------------------------------------------------
        case CxExpressionToken::FUNCTION :
            
            
             func_token = CurrentToken();
            
			//-------------------------------------------------------------------------------------
			// grab the next token (left paren)	
			//-------------------------------------------------------------------------------------

			token = NextToken( );

			//-------------------------------------------------------------------------------------
			// call the EXPRESSION_Level1 to get	
			// the functions argument list		
			// users variable evaluation		
			//-------------------------------------------------------------------------------------

			Level1( arg_list, &args);

			//-------------------------------------------------------------------------------------
			// call the users function evaluation	
			// routine with the correct key to get 
			// the return value of the function	
			//-------------------------------------------------------------------------------------
            
            stat = func_db->FunctionEvaluate(func_token.text, args, arg_list, result);
            
			switch(stat) {
				
				//---------------------------------------------------------------------------------
				// user defined function was found, just go with that
				//---------------------------------------------------------------------------------
                case CxExpressionFunctionDatabase::FUNCTION_DEFINED :
					break;

				//---------------------------------------------------------------------------------
				// user defined function was found, so lets look for the intrinsic version
				//---------------------------------------------------------------------------------
                case CxExpressionFunctionDatabase::FUNCTION_UNDEFINED :
					
					// user version was not found, try intrinsic 
    		        stat = func_intrinsic_db->FunctionEvaluate(func_token.text, args, arg_list, result);
					switch(stat) {

						case CxExpressionFunctionDatabase::FUNCTION_DEFINED:
							// function was found, intrinsic function used
							break;
					
						case CxExpressionFunctionDatabase::FUNCTION_UNDEFINED:
							// neither version exists so thats an error
							Error( PRIM_B );
							break;

						case CxExpressionFunctionDatabase::FUNCTION_BAD_NUMBER_OF_ARGS:
							// function had bad number of arguments
							Error( PRIM_C );
							break;

						case CxExpressionFunctionDatabase::FUNCTION_RANGE_ERROR :
							// range error
							Error( PRIM_D );
							break;
					}	
					break;

				//---------------------------------------------------------------------------------
				// user database had Incorrect number of args call error routine		
				//
				//---------------------------------------------------------------------------------
				case CxExpressionFunctionDatabase::FUNCTION_BAD_NUMBER_OF_ARGS :
					Error(PRIM_E);
					break;

				//---------------------------------------------------------------------------------
				// user database had range error
				//
				//---------------------------------------------------------------------------------
                case CxExpressionFunctionDatabase::FUNCTION_RANGE_ERROR :
					Error(PRIM_F);
					break;

				//---------------------------------------------------------------------------------
				// User returned some other	
				// error.			
				//---------------------------------------------------------------------------------
				default :
					Error(PRIM_G);
					break;
			}

			return(OK);

		default:
			break;
	}

	//---------------------------------------------------------------------------------------------
	// Unknown token type					
	//---------------------------------------------------------------------------------------------
	
	Error(PRIM_H);
	
	return (INTERNAL_ERROR);
}


//-------------------------------------------------------------------------------------------------
// EXPRESSION_Error :						
//								
// An evaluation error occured and longjmp is used to terminate the evaluation
//								
// Possible Error Codes :					
//		NONE						
//								
//-------------------------------------------------------------------------------------------------
void CxExpression::Error(int code)
{
	longjmp(env_buf, code);
}


//-------------------------------------------------------------------------------------------------
// EXPRESSION_Arith :						
//								
// Perform common arithmatic between two double numbers.	
//								
// Possible Error Codes :					
//		EXPERR_ARITH_A					
//		EXPERR_ARITH_B					
//								
//-------------------------------------------------------------------------------------------------
CxExpression::expressionError
CxExpression::Arith(int op, double *number1, double *number2)
{
	switch(op) {
            
		//-----------------------------------------------------------------------------------------
		// Addition					
		//-----------------------------------------------------------------------------------------
        case CxExpressionToken::PLUS_SIGN	:
			*number1 = (*number1) + (*number2);
            return(OK);

		//-----------------------------------------------------------------------------------------
		// Subtraction					
		//-----------------------------------------------------------------------------------------
        case CxExpressionToken::MINUS_SIGN :
			*number1 = (*number1) - (*number2);
            return(OK);

		//-----------------------------------------------------------------------------------------
		// Multiplication				
		//-----------------------------------------------------------------------------------------
        case CxExpressionToken::MULT_SIGN	:
			*number1 = (*number1) * (*number2);
            return(OK);

		//-----------------------------------------------------------------------------------------
		// Division					
		//-----------------------------------------------------------------------------------------
        case CxExpressionToken::DIV_SIGN	:
			if (*number2==0.0) Error( ARITH_A);
			*number1 = (*number1) / (*number2);
            return(OK);

		//-----------------------------------------------------------------------------------------
		// Exponentiation				
		//-----------------------------------------------------------------------------------------
        case CxExpressionToken::EXP_SIGN	:
			*number1 = pow((double) *number1, (double) *number2);
            return(OK);

	}

	//---------------------------------------------------------------------------------------------
	// Unknown arithmetic operation.			
	//---------------------------------------------------------------------------------------------

	Error( ARITH_B);
	
	return (INTERNAL_ERROR); // guessed at return value - GMCC
}


//-------------------------------------------------------------------------------------------------
// EXPRESSION_Unary :						
//								
// Perform sign change on double number			
//								
// Possible Error Codes :					
//		EXPERR_UNARY_A					
//								
//-------------------------------------------------------------------------------------------------
CxExpression::expressionError
CxExpression::Unary(int op, double *number)
{
	//---------------------------------------------------------------------------------------------
	// Perform selected unary operation.			
	//---------------------------------------------------------------------------------------------
	switch(op) {

		//-----------------------------------------------------------------------------------------
		// Plus sign					
		//-----------------------------------------------------------------------------------------
        case CxExpressionToken::PLUS_SIGN	:
			return(OK);

		//-----------------------------------------------------------------------------------------
		// Minus sign					
		//-----------------------------------------------------------------------------------------
        case CxExpressionToken::MINUS_SIGN :
			*number = *number * -1.0;
			return(OK);
	}

	//---------------------------------------------------------------------------------------------
	// Unknown operation.					
	//---------------------------------------------------------------------------------------------

	Error(UNARY_A);

	return (INTERNAL_ERROR); // guessed at return value - GMCC
}


//-------------------------------------------------------------------------------------------------
// EXPRESSION_CurrentToken :					
//								
// Get the current token off the expressions stack.	
//								
//-------------------------------------------------------------------------------------------------
CxExpressionToken
CxExpression::CurrentToken( void )
{
    return(token_list.at(current_token));
}


//-------------------------------------------------------------------------------------------------
// EXPRESSION_NextToken :					
//								
// Increment the expressions token counter and return	
// the new token.						
//								
//-------------------------------------------------------------------------------------------------
CxExpressionToken
CxExpression::NextToken( void )
{
	current_token++;
    return( token_list.at( current_token ));
}

//-------------------------------------------------------------------------------------------------
// EXPRESSION_GetVariableList:					
//								
//		Get the total list of variables used in the expression	
//								
//-------------------------------------------------------------------------------------------------
CxSList<CxString>
CxExpression::GetVariableList( void )
{
	int c;
	CxSList<CxString> vlist;

	//---------------------------------------------------------------------------------------------
	// Look at each token					
	//---------------------------------------------------------------------------------------------
    
    for (int c=0; c<token_list.entries(); c++) {

        CxExpressionToken token = token_list.at(c);
        
        if ((token.ttype == CxExpressionToken::VARIABLE) || 
            (token.ttype == CxExpressionToken::UNKNOWN_VARIABLE)) {
            vlist.append( token.text );
        }
    }

	return(vlist);	
}


//-------------------------------------------------------------------------------------------------
// EXPRESSION_GetUnknownVariableList:				
//								
// Build the list of variables that were NOT resolved	
// during the parse stage.					
//								
//-------------------------------------------------------------------------------------------------
CxSList<CxString>
CxExpression::GetUnknownVariableList(void)
{
    int c;

    CxSList<CxString> vlist;
    
    for (int c=0; c<token_list.entries(); c++) {

        CxExpressionToken token = token_list.at(c);
        
        if (token.ttype == CxExpressionToken::UNKNOWN_VARIABLE) {
            vlist.append( token.text );
        }
    }
	return(vlist);	
}


//-------------------------------------------------------------------------------------------------
// EXPRESSION_GetKnownVariableList:				
//								
// Build a list of variables that were resolved during 
// the parse stage.					
//								
//-------------------------------------------------------------------------------------------------
CxSList<CxString>
CxExpression::GetKnownVariableList(void)
{
    int c;

    CxSList<CxString> vlist;
    
    for (int c=0; c<token_list.entries(); c++) {

        CxExpressionToken token = token_list.at(c);
        
        if (token.ttype == CxExpressionToken::VARIABLE)  {
            vlist.append( token.text );
        }
    }

	return(vlist);	
}


//-------------------------------------------------------------------------------------------------
// EXPRESSION_GetErrorString:					
//								
// Returns a string that describes the last error returned 
// from EXPRESSION_Parse, or EXPRESSION_Evaluate		
//								
//-------------------------------------------------------------------------------------------------
CxString CxExpression::GetErrorString( void )
{
	int c;
	
    CxString errorString;
    CxString token_text;
    
    if (error_token < token_list.entries()) {
        token_text = token_list.at( error_token).text;
    } else {
        token_text = CxString("");
    }
    
    
	//---------------------------------------------------------------------------------------------
	// Select error code					
	//---------------------------------------------------------------------------------------------
	switch(error_code) {

		//-----------------------------------------------------------------------------------------

		case OK :
            errorString.printf(" Expression OK ");
			break;

		case UNEXPECTED_TOKEN:
            errorString.printf( "ERR(%d, unexpected token) ", error_code);
			break;

		//-----------------------------------------------------------------------------------------

		case MULT_AFTER_PLUS:
            errorString.printf("ERR(%d, +* unexpected sequence)", error_code);
			break;

		case DIV_AFTER_PLUS:
			errorString.printf( "ERR(%d, +/ unexpected sequence)", error_code);
			break;

		case RIGHT_PAREN_AFTER_PLUS:
			errorString.printf( "ERR(%d, +) unexpected sequence)", error_code);
			break;

		case END_AFTER_PLUS:
			errorString.printf( "ERR(%d, bad terminal character +)", error_code);
			break;

		//-----------------------------------------------------------------------------------------

		case MULT_AFTER_MINUS:
			errorString.printf( "ERR(%d, -* unexpected sequence)", error_code);
			break;

		case DIV_AFTER_MINUS:
			errorString.printf( "ERR(%d, -/ unexpected sequence)", error_code);
			break;

		case RIGHT_PAREN_AFTER_MINUS:
			errorString.printf( "ERR(%d, -) unexpected sequence)", error_code);
			break;

		case END_AFTER_MINUS:
			errorString.printf( "ERR(%d, bad terminal character -)", error_code);
			break;
			
		//-----------------------------------------------------------------------------------------
		
		case MULT_AFTER_MULT:
			errorString.printf( "ERR(%d, ** unexpected sequence)", error_code);
			break;

		case DIV_AFTER_MULT:
			errorString.printf( "ERR(%d, */ unexpected sequence)", error_code);
			break;

		case RIGHT_PAREN_AFTER_MULT:
			errorString.printf( "ERR(%d, *) unexpected sequence)", error_code);
			break;

		case END_AFTER_MULT:
			errorString.printf( "ERR(%d, bad terminal character *)", error_code);
			break;

		//-----------------------------------------------------------------------------------------

		case MULT_AFTER_DIV:
			errorString.printf( "ERR(%d, // unexpected sequence)", error_code);
			break;

		case DIV_AFTER_DIV:
			errorString.printf( "ERR(%d, // unexpected sequence)", error_code);
			break;

		case RIGHT_PAREN_AFTER_DIV:
			errorString.printf( "ERR(%d, /) unexpected sequence)", error_code);
			break;

		case END_AFTER_DIV:
			errorString.printf( "ERR(%d, bad terminal character /)", error_code);
			break;

		//-----------------------------------------------------------------------------------------
		
		case MULT_AFTER_LEFT_PAREN:
			errorString.printf( "ERR(%d, (* unexpected sequence)", error_code);
			break;

		case DIV_AFTER_LEFT_PAREN:
			errorString.printf( "ERR(%d, (/ unexpected sequence)", error_code);
			break;

		case RIGHT_PAREN_AFTER_LEFT_PAREN:
			errorString.printf( "ERR(%d, () null expression)", error_code);
			break;

		case END_AFTER_LEFT_PAREN:
			errorString.printf( "ERR(%d, bad terminal character ()", error_code);
			break;

		//-----------------------------------------------------------------------------------------

		case NUMBER_AFTER_RIGHT_PAREN:
			errorString.printf(
				"ERR(%d, number unexpected after right paren)", 
				error_code);
			break;

		case  VARIABLE_AFTER_RIGHT_PAREN:
			errorString.printf(
				"ERR(%d, variable %s unexpected after right paren)", 
				error_code, token_text.data());
			break;

		case  FUNCTION_AFTER_RIGHT_PAREN:
			errorString.printf(
				"ERR(%d, function %s unexpected after right paren)", 
				error_code, token_text.data() );
			break;

		//-----------------------------------------------------------------------------------------

		case  LEFT_PAREN_AFTER_NUMBER:
			errorString.printf(
				"ERR(%d, ( unexpected after number)",
				error_code);
			break;

		case NUMBER_AFTER_NUMBER:
			errorString.printf(
				"ERR(%d, number unexpected after number)",
				error_code);
			break;

		case  VARIABLE_AFTER_NUMBER:
			errorString.printf(
				"ERR(%d, variable %s unexpected after number)",
				error_code, token_text.data() );
			break;

		case FUNCTION_AFTER_NUMBER:
			errorString.printf(
				"ERR(%d, function %s unexpected after number)",
				error_code, token_text.data() );
			break;

		//-----------------------------------------------------------------------------------------

		case LEFT_PAREN_AFTER_VARIABLE:
			errorString.printf(
				"ERR(%d, ( unexpected after variable %s)",
				error_code, token_text.data() );
			break;

		case  NUMBER_AFTER_VARIABLE:
			errorString.printf(
				"ERR(%d, number after variable %s unexpected)",
				error_code, token_text.data() );
			break;

		case VARIABLE_AFTER_VARIABLE:
			errorString.printf(
				"ERR(%d, variable %s unexpected)",
				error_code, token_text.data() );
			break;

		case FUNCTION_AFTER_VARIABLE:
			errorString.printf(
				"ERR(%d, function %s unexpected)",
				error_code, token_text.data() );
			break;

		//-----------------------------------------------------------------------------------------

		case  PLUS_AFTER_FUNCTION:
		case  MINUS_AFTER_FUNCTION:
		case  MULT_AFTER_FUNCTION:
		case  DIV_AFTER_FUNCTION :
		case  RIGHT_PAREN_AFTER_FUNCTION:
		case  NUMBER_AFTER_FUNCTION:
		case  VARIABLE_AFTER_FUNCTION:
		case  FUNCTION_AFTER_FUNCTION:
		case  END_AFTER_FUNCTION:
		case  COMMA_AFTER_FUNCTION:

			errorString.printf(
				"ERR(%d, internal error)",
				error_code);
			break;


		//-----------------------------------------------------------------------------------------

		case COMMA_AFTER_PLUS:
			errorString.printf(
				"ERR(%d, +, unexpected sequence)",
				error_code);
						break;

		case COMMA_AFTER_MINUS:
			errorString.printf(
				"ERR(%d, -, unexpected sequence)",
				error_code);
						break;

		case COMMA_AFTER_MULT:
			errorString.printf(
				"ERR(%d, *, unexpected sequence)",
				error_code);
						break;

		case COMMA_AFTER_DIV:
			errorString.printf(
				"ERR(%d, /, unexpected sequence)",
				error_code);
						break;

		case COMMA_AFTER_COMMA:
			errorString.printf(
				"ERR(%d, ,, unexpected sequence)",
				error_code);
						break;

		case COMMA_AFTER_LEFT_PAREN:
			errorString.printf(
				"ERR(%d, (, unexpected sequence)",
				error_code);
						break;

		//-----------------------------------------------------------------------------------------
		case RIGHT_PAREN_AFTER_COMMA:
			errorString.printf(
				"ERR(%d, ,) unexpected sequence)",
				error_code);
			break;

		case MULT_AFTER_COMMA:
			errorString.printf(
				"ERR(%d, ,* unexpected sequence)",
				error_code);
			break;

		case DIV_AFTER_COMMA:
			errorString.printf(
				"ERR(%d, ,/ unexpected sequence)",
                    error_code);
			break;

		case END_AFTER_COMMA:
			errorString.printf(
				"ERR(%d, bad terminal character ,)",
				error_code);
			break;

		//-----------------------------------------------------------------------------------------

		case UNKNOWN_FUNCTION:
			errorString.printf(
				"ERR(%d, unknown function %s)",
				error_code, token_text.data() );
						break;

		case ILLEGAL_NUMBER:
			errorString.printf(
				"ERR(%d, unable to parse number)",
				error_code);
						break;

		case UNBALANCED_PARENS:
			errorString.printf(
				"ERR(%d, unbalance parens)",
				error_code);
						break;

		case UNKNOWN_SYMBOL:
			errorString.printf(
				"ERR(%d, unknown symbol)",
				error_code);
						break;
			
		case INVALID_FIRST_SYMBOL :
			errorString.printf(
				"ERR(%d, unexpected initial symbol)",
				error_code);
						break;			

		case INVALID_COMMA_PLACEMENT :
			errorString.printf(
				"ERR(%d, invalid comma in non function)",
				error_code);
						break;

		case INCORRECT_NUMBER_OF_ARGS :
			errorString.printf(
				"ERR(%d, invalid number of args passed to function %s)",
				error_code, token_text.data() );
			break;

		case UNKNOWN_VARIABLE :
			errorString.printf(
				"ERR(%d, unknown variable %s)",
				 error_code, token_text.data() );
			break;

		//-----------------------------------------------------------------------------------------
		case L6A:
			errorString.printf(
				"ERR(%d, internal error EXPERR_L6A)",
				 error_code);
			break;

		case PRIM_A:
			errorString.printf(
				"ERR(%d, no variable database)",
				 error_code);
			break;

		case PRIM_B:
			errorString.printf(
				"ERR(%d, variable %s unknown to database)",
				 error_code, token_text.data() );
			break;

		case PRIM_C:
			errorString.printf(
				"ERR(%d, no function database)",
				 error_code);
			break;

		case PRIM_D:
			errorString.printf(
				"ERR(%d, incorrect number of function arguments passed to function)",
				 error_code);
			break;

		case PRIM_E:
			errorString.printf(
				"ERR(%d, internal error in function evaluation)",
				error_code);
			break;

		case PRIM_F:
			errorString.printf(
				"ERR(%d, internal error EXPRESSION_Primative)",
				error_code);
			break;

		case PRIM_G:
			errorString.printf(
				"ERR(%d, range error in function evaluation)",
				error_code);
			break;

		case ARITH_A:
			errorString.printf(
				"ERR(%d, attempt to divide by zero)",
				error_code);
			break;

		case ARITH_B:
			errorString.printf(
				"ERR(%d, internal error EXPRESSION_Arith)",
				error_code);
			break;

		case UNARY_A:
			errorString.printf(
				"ERR(%d, internal error EXPRESSION_Unary)",
				error_code);
			break;
		
		//-----------------------------------------------------------------------------------------

		default :
			errorString.printf(
								"ERR(%d, unknown internal error, please report)",
								error_code);
			break;
	
	}

	return( errorString );
}

void
CxExpression::debugTokenDump( CxExpressionToken token )
{
#ifdef EXPRESSION_DEBUG
	token.Dump();
#endif
}

void 
CxExpression::debugPrintf( const char * format, ... )
{
#ifdef EXPRESSION_DEBUG
	char buffer[1000];
  	va_list args;
  	va_start (args, format);
  	vsprintf (buffer, format, args);	
  	va_end (args);

	printf("%s", buffer);
#endif
}



