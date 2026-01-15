

//-------------------------------------------------------------------------------------------------
// scanDouble: system/includes					
//-------------------------------------------------------------------------------------------------
#include <stdio.h>
#include <ctype.h>


//-------------------------------------------------------------------------------------------------
// scanDouble: private/defines					
//-------------------------------------------------------------------------------------------------
#define SYM_S 1	 // Sign Symbol '+-'					 
#define SYM_N 2	 // DIGIT SYMBOL '1234567890'			 
#define SYM_D 3	 // DECIMAL POINT SYMBOL '.'			 
#define SYM_E 4	 // EXPONENT SYMBOL 'e' or 'E'		 
#define SYM_O 5	 // OUT OF VALID SET OF CHARACTERS	 
#define SYM_M 6	 // MATH SYMBOL '//'			 
#define SYM_X 7	 // END OF LINE				 


//-------------------------------------------------------------------------------------------------
// scanDouble: private/macros					
//-------------------------------------------------------------------------------------------------
// CHANGE_STATE(x) scanDouble_debug_states(x); state = x; break */
#define CHANGE_STATE(x) state = x; break


int	 scanDouble_event(char c);
void scanDouble_debug_states(int c);


//-------------------------------------------------------------------------------------------------
// scanDouble:							
// Converts a character stream representing a floating 
// number into its double equiv. stat returns 0 if ok	
// -1 if an error occurs.					
//								 
//-------------------------------------------------------------------------------------------------
char *scanDouble(char *s,double *d,int *stat)
{
	int state;
	int theEvent;
	char *sptr = s;
	char theNumber[200];
	char *theNumberPtr = theNumber;

	state = 0;

	for (;;) {

		theEvent = scanDouble_event(*sptr);
		
		switch (state) {
				
			case 0 : // ---[ START ]----------------
				switch (theEvent) {
					case SYM_S :  CHANGE_STATE(1);
					case SYM_N :  CHANGE_STATE(2);	
					case SYM_D :  CHANGE_STATE(3);
					case SYM_E :  CHANGE_STATE(11);
					case SYM_O :  CHANGE_STATE(11);
					case SYM_M :  CHANGE_STATE(11);
					case SYM_X :  CHANGE_STATE(11);
				}
				break;
			case 1 : // ---[ SIGN FOUND ]-----------
				switch (theEvent) {
					case SYM_S :  CHANGE_STATE(11);
					case SYM_N :  CHANGE_STATE(2);	
					case SYM_D :  CHANGE_STATE(3);
					case SYM_E :  CHANGE_STATE(11);
					case SYM_O :  CHANGE_STATE(11);
					case SYM_M :  CHANGE_STATE(11);
					case SYM_X :  CHANGE_STATE(11);
				}
				break;
			case 2 : // ---[ NUMBER FOUND ]---------
				switch (theEvent) {
					case SYM_S :  CHANGE_STATE(9);
					case SYM_N :  CHANGE_STATE(2);	
					case SYM_D :  CHANGE_STATE(3);
					case SYM_E :  CHANGE_STATE(5);
					case SYM_O :  CHANGE_STATE(10);
					case SYM_M :  CHANGE_STATE(9);
					case SYM_X :  CHANGE_STATE(8);
				}
				break;
			case 3 : // ---[ DECIMAL FOUND ]--------
				switch (theEvent) {
					case SYM_S :  CHANGE_STATE(9);
					case SYM_N :  CHANGE_STATE(4);	
					case SYM_D :  CHANGE_STATE(10);
					case SYM_E :  CHANGE_STATE(5);
					case SYM_O :  CHANGE_STATE(10);
					case SYM_M :  CHANGE_STATE(9);
					case SYM_X :  CHANGE_STATE(8);
				}
				break;
			case 4 : // ---[ NUMBER AFTER DECIMAL FOUND ]-
				switch (theEvent) {
					case SYM_S :  CHANGE_STATE(9);
					case SYM_N :  CHANGE_STATE(4);	
					case SYM_D :  CHANGE_STATE(11);
					case SYM_E :  CHANGE_STATE(5);
					case SYM_O :  CHANGE_STATE(10);
					case SYM_M :  CHANGE_STATE(9);
					case SYM_X :  CHANGE_STATE(8);
				}
				break;
			case 5 : // ---[ EXPONENT FOUND ]-------
				switch (theEvent) {
					case SYM_S :  CHANGE_STATE(6);
					case SYM_N :  CHANGE_STATE(7);	
					case SYM_D :  CHANGE_STATE(11);
					case SYM_E :  CHANGE_STATE(11);
					case SYM_O :  CHANGE_STATE(11);
					case SYM_M :  CHANGE_STATE(11);
					case SYM_X :  CHANGE_STATE(11);
				}
				break;
			case 6 : // ---[ SIGN AFTER EXPONENT FOUND ]---
				switch (theEvent) {
					case SYM_S :  CHANGE_STATE(11);
					case SYM_N :  CHANGE_STATE(7);	
					case SYM_D :  CHANGE_STATE(11);
					case SYM_E :  CHANGE_STATE(11);
					case SYM_O :  CHANGE_STATE(11);
					case SYM_M :  CHANGE_STATE(11);
					case SYM_X :  CHANGE_STATE(11);
				}
				break;
			case 7 : // ---[ NUMBER AFTER EXPONENT FOUND ]--
				switch (theEvent) {
					case SYM_S :  CHANGE_STATE(9);
					case SYM_N :  CHANGE_STATE(7);	
					case SYM_D :  CHANGE_STATE(11);
					case SYM_E :  CHANGE_STATE(11);
					case SYM_O :  CHANGE_STATE(10);
					case SYM_M :  CHANGE_STATE(9);
					case SYM_X :  CHANGE_STATE(8);
				}
				break;
			case 8 : // ---[ VALID FLOAT FOUND WITH EOL TERMINATION ]--
				*theNumberPtr = '\000';
				sscanf(theNumber,"%lf",d);
				*stat = 0;
				return(sptr);
				 
			case 9 : // ---[ VALID FLOAT FOUND WITH MATHMATICAL TERMINATION ]--
				*theNumberPtr = '\000';
				sscanf(theNumber,"%lf",d);
				*stat = 0;
				return(sptr);
				 
			case 10: // ---[ VALID FLOAT FOUND WITH NON-MATHMATICAL TERMINATION ]--
				*theNumberPtr = '\000';
				sscanf(theNumber,"%lf",d);
				*stat = 0;
				return(sptr);
				 
			case 11: // ---[ INVALID FLOAT FOUND ]--
				*d	  = 0;
				*stat = -1;
				return(s);
				 
		}


		*theNumberPtr = *sptr;

		theNumberPtr++;
		if (state < 8) sptr++;	
	}
}


//-------------------------------------------------------------------------------------------------
// scanDouble_event:						
// Return a classification code for the current character	
//								
//-------------------------------------------------------------------------------------------------
int scanDouble_event(char c)
{
	if ((c=='+') || (c=='-')) return(SYM_S);

	if (isdigit((int) c)) return(SYM_N);

	if (c == '.') return(SYM_D);

	if ((c == 'e') || (c == 'E')) return(SYM_E);

	if ((c == '*') || (c == '/')) return(SYM_M);

	if (c == '\000') return(SYM_X);

	return(SYM_O);
}


//-------------------------------------------------------------------------------------------------
// scanDouble_debug_states:					
// Routine can be used to debug the state machine		
//								
//-------------------------------------------------------------------------------------------------
void scanDouble_debug_states(int c)
{
	printf("STATE = %d	// ",c);
	
	switch(c) {
		case 0	:  printf("Start Parsing\n");					break;
		case 1	:  printf("Sign Symbol Found\n");					break;
		case 2	:  printf("Number Found\n");						break;
		case 3	:  printf("Decimal Point Found\n");					break;
		case 4	:  printf("Number after Decimal Point Found\n");			break;
		case 5	:  printf("Exponent Symbol Found\n");				break;
		case 6	:  printf("Sign Symbol After Exponent Symbol Found\n");		break;
		case 7	:  printf("Number after Exponent Symbol Found\n");			break;
		
		case 8	:  printf("Valid Float found with EOL termination\n");			break;
		case 9	:  printf("Valid Float found with math termination\n");			break; 
		case 10 :  printf("Valid Float found with non-math termination\n");		break;
		case 11 :  printf("Invalid Float found, no characters read\n");			break;
	}
}
