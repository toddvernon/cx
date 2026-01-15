//
//  stringutils.cpp
//  EditBuffer
//
//  Created by Todd Vernon on 3/22/22.
//

#include "stringutils.h"

/*static*/
unsigned long
CxStringUtils::calcTab(unsigned long pos, int tabSpaces)
{
    return( tabSpaces - (pos % tabSpaces));
}


/*static*/
CxString
CxStringUtils::fixTabs( CxString fromString, int tabSpaces)
{
    CxString removedTabs = CxStringUtils::fromTabExtensionFormat2( fromString );
    CxString newString   = CxStringUtils::toTabExtensionFormat2( removedTabs, tabSpaces );
    return( newString );
}


/*static*/
CxString
CxStringUtils::replaceTabExtensionsWithSpaces( CxString s)
{
    char *cPtr = s.data();
    
    // count the tab characters in the string
    while (*cPtr != (char) NULL) {
        
        if ((*cPtr == '\377') || (*cPtr == '\t')) {
            *cPtr = ' ';
        }
        
        cPtr++;
    }
    
    return( s );
}

/*static*/
CxString
CxStringUtils::toTabExtensionFormat2( CxString fromString, int tabSpaces)
{
	char *cPtr = fromString.data();
	unsigned long tabCount = 0;

	// count the tab characters in the string
	while (*cPtr != (char) NULL) {
		if (*cPtr == '\t') tabCount++;
		cPtr++;
	}

	// make a new buffer large enough to hold the string plus the space needed
	// for the tab extension characters to be added.

	char *newBuffer = new char[ fromString.length() + (tabCount * (tabSpaces-1)) + 1 ];

	// create a destination pointer to the new buffer
	char *newBufferPtr = newBuffer;

	// create a source pointer to the orginal string
	cPtr = fromString.data();

	// while the source pointer is not the end of the string
	while (*cPtr != (char) NULL) {
		
		// if the source pointer is a tab character
		if (*cPtr == '\t') {

			// get the number of extension characters to add to the destination
			// after the tab character
			
            unsigned long spaceCount = 
				CxStringUtils::calcTab(newBufferPtr - newBuffer, tabSpaces) - 1;
 
			// add tab character

			*newBufferPtr = '\t'; newBufferPtr++;
			
			// now add the extension characters required            
            for (unsigned long i=0; i<spaceCount; i++) {
				*newBufferPtr = '\377'; newBufferPtr++;
            }

        } else {

			// not a tab so just copy the character
			*newBufferPtr = *cPtr; newBufferPtr++;
        }

		// advance the source pointer
		cPtr++;
	}

	// put a null at the end of the string
	*newBufferPtr = (char) NULL;


	// copy the result into a cxstring
	CxString newString = newBuffer;

	// delete the destination buffer
	delete [] newBuffer;

	// return the new string
	return( newString );
}


/*static*/
CxString
CxStringUtils::toTabExtensionFormat( CxString fromString, int tabSpaces )
{
    CxString toString;
    unsigned long resultingColumn = 0;
	unsigned long fromStringLength = fromString.length();

    for (unsigned long c=0; c<fromStringLength; c++) {

        char ch = fromString.data()[c];

        if (ch == '\t'){
            
            unsigned long spaceCount = CxStringUtils::calcTab(resultingColumn, tabSpaces)-1;
            
            toString = toString + CxString("\t");
            resultingColumn = resultingColumn + 1;
            
            for (unsigned long i=0; i<spaceCount; i++) {
                toString = toString + CxString("\377");
                resultingColumn = resultingColumn + 1;
            }

        } else {

            toString = toString + CxString(fromString.data()[c]);
            resultingColumn = resultingColumn + 1;
        }
    }

    return( toString );
}


/*static*/
CxString
CxStringUtils::fromTabExtensionFormat( CxString fromString )
{
    CxString toString;
	unsigned long fromStringLength = fromString.length();

    for (unsigned long c=0; c<fromStringLength; c++) {

        char ch = fromString.data()[c];
        if (ch != '\377') {
            toString = toString + CxString(ch);
        }
    }

    return( toString );
}


/*static*/
CxString
CxStringUtils::fromTabExtensionFormat2( CxString fromString )
{
    char *newBuffer = new char[ fromString.length()+1 ];
    
    char *srcPtr  = fromString.data();
    char *destPtr = newBuffer;
    
    while (*srcPtr!= (char) NULL) {
        
        if (*srcPtr != '\377') {
            *destPtr = *srcPtr;  destPtr++;
        }
        
        srcPtr++;
    }
    
    *destPtr = (char) NULL;
    
    CxString returnString( newBuffer );
    
    delete [] newBuffer;

    return( returnString );
}


/*static*/
void
CxStringUtils::debugPrintString(CxString s)
{
    for (unsigned long c=0; c<s.length(); c++) {

        char ch = s.data()[c];

        if (ch == '\t'){

            printf("[tab]");

        } else if (ch == '\377') {

            printf("[exp-tab]");

        } else {
            printf("[%c]", s.data()[c]);
        }
    }
}
