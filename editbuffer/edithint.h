//
//  updatehints.h
//  EditBuffer
//
//  Created by Todd Vernon on 3/12/22.
//

#ifndef updatehints_h
#define updatehints_h


class CxEditHint {
    
  public:
    
    enum UPDATE_HINT {
        UPDATE_HINT_NONE,
        UPDATE_HINT_LINE,                  // update entire row line
        UPDATE_HINT_LINE_PAST_POINT,       // update row line past col
        UPDATE_HINT_SCREEN,                // update entire screen
        UPDATE_HINT_SCREEN_PAST_POINT,     // update screen past row, col point
    };
    
    enum CURSOR_HINT {
        CURSOR_HINT_NONE,                   // cursor did not move
        CURSOR_HINT_RIGHT,                  // cursor moved right one space
        CURSOR_HINT_LEFT,                   // cursor moved left one space
        CURSOR_HINT_UP,                     // cursor moved up one space
        CURSOR_HINT_DOWN,                   // cursor moved down one space
        CURSOR_HINT_JUMP,                   // cursor jumped
        CURSOR_HINT_WRAP_DOWN,
        CURSOR_HINT_WRAP_UP
    };
    
    unsigned long _startRow;                // update reference logical row
    unsigned long _startCol;                // update reference logical col

    CxEditHint::UPDATE_HINT _updateHint;  // the update hint
    CxEditHint::CURSOR_HINT _cursorHint;  // the cursor location hint
    
    
    CxEditHint( void );
    // constructor
    
    CxEditHint( CxEditHint::UPDATE_HINT updateHint_);
    // constructor
    
    CxEditHint( CxEditHint::CURSOR_HINT cursorHint_);
    // constructor
    
    CxEditHint( CxEditHint::UPDATE_HINT updateHint_, CxEditHint::CURSOR_HINT cursorHint_);
    // constructor
    
    CxEditHint( unsigned long startRow_, unsigned long startCol_,
               CxEditHint::UPDATE_HINT hint_, CxEditHint::CURSOR_HINT cursorHint_ );
    // constructor
    
    CxEditHint( const CxEditHint& hint_ );
    // copy constructor

    CxEditHint operator=( const CxEditHint& hint_);
    // assignment operator
    
    CxEditHint::UPDATE_HINT updateHint(void);
    // obtain the hint
    
    CxEditHint::CURSOR_HINT cursorHint(void);
    
    int startRow(void);
    // row
    
    int startCol(void);
    // col
    
    void printHint();
    
    
};



#endif /* updatehints_h */


 
