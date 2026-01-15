//
//  stringutils.hpp
//  EditBuffer
//
//  Created by Todd Vernon on 3/22/22.
//

#include <stdio.h>
#include <cx/base/string.h>


#ifndef stringutils_h
#define stringutils_h


class CxStringUtils {
    
  public:
    
    static unsigned long calcTab(unsigned long pos, int tabSpaces);
    // calculates the number of spaces needed to represent the tab correctly
    
    static CxString fixTabs( CxString fromString, int tabSpaces);
    // reformats a string so inject spaces after tabs to make sure they render correctly
    
    static CxString fromTabExtensionFormat( CxString fromString);
    // removes tab extension characters from string
    
    static CxString fromTabExtensionFormat2(CxString fromString);
    // optimized version of the original to reduced number of objects created
    
    static CxString toTabExtensionFormat( CxString fromString, int tabSpaces);
    // converts strings to and from tab extension format
        
    static CxString toTabExtensionFormat2( CxString fromString, int tabSpaces);
	// optimized version of the orginal to reduce the number of object creates
	// and copies
    
    static CxString replaceTabExtensionsWithSpaces( CxString s);


    static void debugPrintString(CxString s);
    // helper method to look at strings with tab extensions
};

#endif /* stringutils_h */
