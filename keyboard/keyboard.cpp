//-------------------------------------------------------------------------------------------------
//
//  keyboard.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxKeyboard Class
//
//-------------------------------------------------------------------------------------------------

#include <cx/keyboard/keyboard.h>

/*
 
 Ctrl    ASCII    Dec    Hex    Meaning
 ^@      NUL      000    00    Null character
 ^A      SOH      001    01    Start of Header
 ^B      STX      002    02    Start of Text
 ^C      ETX      003    03    End of Text
 ^D      EOT      004    04    End of Transmission
 ^E      ENQ      005    05    Enquiry
 ^F      ACK      006    06    Acknowledge
 ^G      BEL      007    07    Bell
 ^H      BS       008    08    Backspace
 ^I      HT       009    09    Horizontal tab
 ^J      LF       010    0A    Line feed
 ^K      VT       011    0B    Vertical tab
 ^L      FF       012    0C    Form feed
 ^M      CR       013    0D    Carriage return
 ^N      SO       014    0E    Shift out
 ^O      SI       015    0F    Shift in
 ^P      DLE      016    10    Data link escape
 ^Q      DCL      017    11    Xon (transmit on)
 ^R      DC2      018    12    Device control 2
 ^S      DC3      019    13    Xoff (transmit off)
 ^T      DC4      020    14    Device control 4
 ^U      NAK      021    15    Negative acknowledge
 ^V      SYN      022    16    Synchronous idle
 ^W      ETB      023    17    End of transmission
 ^X      CAN      024    18    Cancel
 ^Y      EM       025    19    End of medium
 ^Z      SUB      026    1A    Substitute
 ^[      ESC      027    1B    Escape
 ^\      FS       028    1C    File separator
 ^]      GS       029    1D    Group separator
 ^^      RS       030    1E    Record separator
 ^_      US       031    1F    Unit separator
 SP     032        20          Space
 */

// Static member definition for idle callbacks
CxSList< CxFunctor * > CxKeyboard::idleCallbackQueue;


//-------------------------------------------------------------------------
// CxKeyboard::addIdleCallback
//
// Add a callback to be called during keyboard idle (~100ms intervals)
//-------------------------------------------------------------------------
void
CxKeyboard::addIdleCallback( CxFunctor *callback )
{
    idleCallbackQueue.append( callback );
}


//-------------------------------------------------------------------------
// CxKeyboard::CxKeyboard
//
//-------------------------------------------------------------------------

CxKeyboard::CxKeyboard( void )
{
    // control characters
    keyHash.insert("0",                     "CONTROL:@");
    keyHash.insert("1",                     "CONTROL:A");
    keyHash.insert("2",                     "CONTROL:B");
    keyHash.insert("3",                     "CONTROL:C");
    keyHash.insert("4",                     "CONTROL:D");
    keyHash.insert("5",                     "CONTROL:E");
    keyHash.insert("6",                     "CONTROL:F");
    keyHash.insert("7",                     "CONTROL:G");
    keyHash.insert("8",                     "CONTROL:H");

    keyHash.insert("9",                     "TAB:<HT>");
    keyHash.insert("10",                    "NEWLINE:<NL>");
    
    //keyHash.insert("10",                    "CONTROL:J");
    keyHash.insert("11",                    "CONTROL:K");
    keyHash.insert("12",                    "CONTROL:L");
    
    keyHash.insert("13",                    "RETURN:<CR>");
    
    keyHash.insert("14",                    "CONTROL:N");
    keyHash.insert("15",                    "CONTROL:O");
    keyHash.insert("16",                    "CONTROL:P");
    keyHash.insert("17",                    "CONTROL:Q");
    keyHash.insert("18",                    "CONTROL:R");
    keyHash.insert("19",                    "CONTROL:S");
    keyHash.insert("20",                    "CONTROL:T");
    keyHash.insert("21",                    "CONTROL:U");
    keyHash.insert("22",                    "CONTROL:V");
    keyHash.insert("23",                    "CONTROL:W");
    keyHash.insert("24",                    "CONTROL:X");
    keyHash.insert("25",                    "CONTROL:Y");
    keyHash.insert("26",                    "CONTROL:Z");
        
    keyHash.insert("28",                    "CONTROL:<FS>");
    keyHash.insert("29",                    "CONTROL:<GS>");
    keyHash.insert("30",                    "CONTROL:<RS>");
    keyHash.insert("31",                    "CONTROL:<US>");
    
    // symbols
    keyHash.insert("32",                    "SYMBOL: ");
    keyHash.insert("33",                    "SYMBOL:!");
    keyHash.insert("34",                    "SYMBOL:\"");
    keyHash.insert("35",                    "SYMBOL:#");
    keyHash.insert("36",                    "SYMBOL:$");
    keyHash.insert("37",                    "SYMBOL:%");
    keyHash.insert("38",                    "SYMBOL:&");
    keyHash.insert("39",                    "SYMBOL:'");
    keyHash.insert("40",                    "SYMBOL:(");
    keyHash.insert("41",                    "SYMBOL:)");
    keyHash.insert("42",                    "SYMBOL:*");
    keyHash.insert("43",                    "SYMBOL:+");
    keyHash.insert("44",                    "SYMBOL:,");
    keyHash.insert("45",                    "SYMBOL:-");
    keyHash.insert("46",                    "SYMBOL:.");
    keyHash.insert("47",                    "SYMBOL:/");
    
    // numbers
    keyHash.insert("48",                    "NUMBER:0");
    keyHash.insert("49",                    "NUMBER:1");
    keyHash.insert("50",                    "NUMBER:2");
    keyHash.insert("51",                    "NUMBER:3");
    keyHash.insert("52",                    "NUMBER:4");
    keyHash.insert("53",                    "NUMBER:5");
    keyHash.insert("54",                    "NUMBER:6");
    keyHash.insert("55",                    "NUMBER:7");
    keyHash.insert("56",                    "NUMBER:8");
    keyHash.insert("57",                    "NUMBER:9");
    
    // more symbols
    keyHash.insert("58",                    "SYMBOL::");
    keyHash.insert("59",                    "SYMBOL:;");
    keyHash.insert("60",                    "SYMBOL:<");
    keyHash.insert("61",                    "SYMBOL:=");
    keyHash.insert("62",                    "SYMBOL:>");
    keyHash.insert("63",                    "SYMBOL:?");
    keyHash.insert("64",                    "SYMBOL:@");
    
    // uppercase alpha
    keyHash.insert("65",                    "UPPERCASE-ALPHA:A");
    keyHash.insert("66",                    "UPPERCASE-ALPHA:B");
    keyHash.insert("67",                    "UPPERCASE-ALPHA:C");
    keyHash.insert("68",                    "UPPERCASE-ALPHA:D");
    keyHash.insert("69",                    "UPPERCASE-ALPHA:E");
    keyHash.insert("70",                    "UPPERCASE-ALPHA:F");
    keyHash.insert("71",                    "UPPERCASE-ALPHA:G");
    keyHash.insert("72",                    "UPPERCASE-ALPHA:H");
    keyHash.insert("73",                    "UPPERCASE-ALPHA:I");
    keyHash.insert("74",                    "UPPERCASE-ALPHA:J");
    keyHash.insert("75",                    "UPPERCASE-ALPHA:K");
    keyHash.insert("76",                    "UPPERCASE-ALPHA:L");
    keyHash.insert("77",                    "UPPERCASE-ALPHA:M");
    keyHash.insert("78",                    "UPPERCASE-ALPHA:N");
    keyHash.insert("79",                    "UPPERCASE-ALPHA:O");
    keyHash.insert("80",                    "UPPERCASE-ALPHA:P");
    keyHash.insert("81",                    "UPPERCASE-ALPHA:Q");
    keyHash.insert("82",                    "UPPERCASE-ALPHA:R");
    keyHash.insert("83",                    "UPPERCASE-ALPHA:S");
    keyHash.insert("84",                    "UPPERCASE-ALPHA:T");
    keyHash.insert("85",                    "UPPERCASE-ALPHA:U");
    keyHash.insert("86",                    "UPPERCASE-ALPHA:V");
    keyHash.insert("87",                    "UPPERCASE-ALPHA:W");
    keyHash.insert("88",                    "UPPERCASE-ALPHA:X");
    keyHash.insert("89",                    "UPPERCASE-ALPHA:Y");
    keyHash.insert("90",                    "UPPERCASE-ALPHA:Z");
    
    // more symbols
    keyHash.insert("91",                    "SYMBOL:[");
    keyHash.insert("92",                    "SYMBOL:\\");
    keyHash.insert("93",                    "SYMBOL:]");
    keyHash.insert("94",                    "SYMBOL:^");
    keyHash.insert("95",                    "SYMBOL:_");
    keyHash.insert("96",                    "SYMBOL:`");
    
    // lowercase alpha
    keyHash.insert("97",                     "LOWERCASE-ALPHA:a");
    keyHash.insert("98",                     "LOWERCASE-ALPHA:b");
    keyHash.insert("99",                     "LOWERCASE-ALPHA:c");
    keyHash.insert("100",                    "LOWERCASE-ALPHA:d");
    keyHash.insert("101",                    "LOWERCASE-ALPHA:e");
    keyHash.insert("102",                    "LOWERCASE-ALPHA:f");
    keyHash.insert("103",                    "LOWERCASE-ALPHA:g");
    keyHash.insert("104",                    "LOWERCASE-ALPHA:h");
    keyHash.insert("105",                    "LOWERCASE-ALPHA:i");
    keyHash.insert("106",                    "LOWERCASE-ALPHA:j");
    keyHash.insert("107",                    "LOWERCASE-ALPHA:k");
    keyHash.insert("108",                    "LOWERCASE-ALPHA:l");
    keyHash.insert("109",                    "LOWERCASE-ALPHA:m");
    keyHash.insert("110",                    "LOWERCASE-ALPHA:n");
    keyHash.insert("111",                    "LOWERCASE-ALPHA:o");
    keyHash.insert("112",                    "LOWERCASE-ALPHA:p");
    keyHash.insert("113",                    "LOWERCASE-ALPHA:q");
    keyHash.insert("114",                    "LOWERCASE-ALPHA:r");
    keyHash.insert("115",                    "LOWERCASE-ALPHA:s");
    keyHash.insert("116",                    "LOWERCASE-ALPHA:t");
    keyHash.insert("117",                    "LOWERCASE-ALPHA:u");
    keyHash.insert("118",                    "LOWERCASE-ALPHA:v");
    keyHash.insert("119",                    "LOWERCASE-ALPHA:w");
    keyHash.insert("120",                    "LOWERCASE-ALPHA:x");
    keyHash.insert("121",                    "LOWERCASE-ALPHA:y");
    keyHash.insert("122",                    "LOWERCASE-ALPHA:z");
    
    keyHash.insert("123",                    "SYMBOL:{");
    keyHash.insert("124",                    "SYMBOL:|");
    keyHash.insert("125",                    "SYMBOL:}");
    keyHash.insert("126",                    "SYMBOL:~");
    
    keyHash.insert("127",                   "BACKSPACE:<backspace>");
    
    keyHash.insert("194:186",               "OPTION:<option-0>");
    keyHash.insert("194:161",               "OPTION:<option-1>");
    keyHash.insert("226:132:162",           "OPTION:<option-2>");
    keyHash.insert("194:163",               "OPTION:<option-3>");
    keyHash.insert("194:162",               "OPTION:<option-4>");
    keyHash.insert("226:136:158",           "OPTION:<option-5>");
    keyHash.insert("194:167",               "OPTION:<option-6>");
    keyHash.insert("194:182",               "OPTION:<option-7>");
    keyHash.insert("226:128:162",           "OPTION:<option-8>");
    keyHash.insert("194:170",               "OPTION:<option-9>");
    
    keyHash.insert("195:165",               "OPTION:<option-a>");
    keyHash.insert("226:136:171",           "OPTION:<option-b>");
    keyHash.insert("195:167",               "OPTION:<option-c>");
    keyHash.insert("226:136:130",           "OPTION:<option-d>");
    //e
    keyHash.insert("198:146",               "OPTION:<option-f>");
    keyHash.insert("194:169",               "OPTION:<option-g>");
    keyHash.insert("203:153",               "OPTION:<option-h>");
    keyHash.insert("226:136:134",           "OPTION:<option-i>");
    // j
    keyHash.insert("203:154",               "OPTION:<option-k>");
    keyHash.insert("194:172",               "OPTION:<option-l>");
    keyHash.insert("194:181",               "OPTION:<option-m>");
    // n
    keyHash.insert("195:184",               "OPTION:<option-o>");
    keyHash.insert("207:128",               "OPTION:<option-p>");
    keyHash.insert("197:147",               "OPTION:<option-q>");
    keyHash.insert("194:174",               "OPTION:<option-r>");
    keyHash.insert("195:159",               "OPTION:<option-s>");
    keyHash.insert("226:128:160",           "OPTION:<option-t>");
    //u
    keyHash.insert("226:136:154",           "OPTION:<option-v>");
    keyHash.insert("226:136:145",           "OPTION:<option-w>");
    keyHash.insert("226:137:136",           "OPTION:<option-x>");
    keyHash.insert("194:165",               "OPTION:<option-y>");
    keyHash.insert("206:169",               "OPTION:<option-z>");
    
    keyHash.insert("194:171",               "OPTION:<option-back-slash>");
    keyHash.insert("195:166",               "OPTION:<option-single-quote>");
    keyHash.insert("195:183",               "OPTION:<option-forward-slash>");
    keyHash.insert("226:128:166",           "OPTION:<option-semi-colon>");
    keyHash.insert("226:137:160",           "OPTION:<option-equals>");
    keyHash.insert("226:137:164",           "OPTION:<option-comma>");
    keyHash.insert("226:137:165",           "OPTION:<option-period>");
    keyHash.insert("226:128:147",           "OPTION:<option-minus>");
    keyHash.insert("226:128:152",           "OPTION:<option-right-bracket>");
    keyHash.insert("226:128:156",           "OPTION:<option-left-bracket>");
        
    keyHash.insert("27:27:91:53:126",       "OPTION:<option-page-up>");
    keyHash.insert("27:27:91:54:126",       "OPTION:<option-page-down>");
    keyHash.insert("27:27:91:65",           "OPTION:<option-arrow-up>");
    keyHash.insert("27:27:91:66",           "OPTION:<option-arrow-down>");
    keyHash.insert("27:27:91:67",           "OPTION:<option-arrow-right>");
    keyHash.insert("27:27:91:68",           "OPTION:<option-arrow-left>");
    
    keyHash.insert("27:91:49:59:57:70",     "OPTION:<option-end>");
    keyHash.insert("27:91:49:59:57:72",     "OPTION:<option-home>");
    keyHash.insert("27:91:51:126",          "OPTION:<option-delete>");
    keyHash.insert("27:27:91:53:126",       "OPTION:<option-page-up>");
    keyHash.insert("27:27:91:54:126",       "OPTION:<option-page-down>");
    keyHash.insert("27:91:49:59:57:70",     "OPTION:<option-end>");
    keyHash.insert("27:91:49:59:57:72",     "OPTION:<option-home>");
    
    keyHash.insert("195:133",               "SHIFT-OPTION:<shift-option-a>");
    keyHash.insert("196:177",               "SHIFT-OPTION:<shift-option-b>");
    keyHash.insert("195:135",               "SHIFT-OPTION:<shift-option-c>");
    keyHash.insert("195:142",               "SHIFT-OPTION:<shift-option-d>");
    keyHash.insert("194:180",               "SHIFT-OPTION:<shift-option-e>");
    keyHash.insert("195:143",               "SHIFT-OPTION:<shift-option-f>");
    keyHash.insert("203:157",               "SHIFT-OPTION:<shift-option-g>");
    keyHash.insert("195:147",               "SHIFT-OPTION:<shift-option-h>");
    keyHash.insert("203:134",               "SHIFT-OPTION:<shift-option-i>");
    keyHash.insert("195:148",               "SHIFT-OPTION:<shift-option-j>");
    keyHash.insert("239:163:191",           "SHIFT-OPTION:<shift-option-k>");
    keyHash.insert("195:156",               "SHIFT-OPTION:<shift-option-l>");
    keyHash.insert("195:130",               "SHIFT-OPTION:<shift-option-m>");
    keyHash.insert("203:156",               "SHIFT-OPTION:<shift-option-n>");
    keyHash.insert("195:152",               "SHIFT-OPTION:<shift-option-o>");
    keyHash.insert("226:136:143",           "SHIFT-OPTION:<shift-option-p>");
    keyHash.insert("197:146",               "SHIFT-OPTION:<shift-option-q>");
    keyHash.insert("226:128:176",           "SHIFT-OPTION:<shift-option-r>");
    keyHash.insert("195:141",               "SHIFT-OPTION:<shift-option-s>");
    keyHash.insert("203:135",               "SHIFT-OPTION:<shift-option-t>");
    keyHash.insert("194:168",               "SHIFT-OPTION:<shift-option-u>");
    keyHash.insert("226:151:138",           "SHIFT-OPTION:<shift-option-v>");
    keyHash.insert("226:128:158",           "SHIFT-OPTION:<shift-option-w>");
    keyHash.insert("203:155",               "SHIFT-OPTION:<shift-option-x>");
    keyHash.insert("195:129",               "SHIFT-OPTION:<shift-option-y>");
    keyHash.insert("194:184",               "SHIFT-OPTION:<shift-option-z>");
    
    keyHash.insert("194:121",               "SHIFT-OPTION:<shift-option-question>");
    keyHash.insert("194:175",               "SHIFT-OPTION:<shift-option-less-than>");
    keyHash.insert("194:176",               "SHIFT-OPTION:<shift-option-star>");
    keyHash.insert("194:177",               "SHIFT-OPTION:<shift-option-plus>");
    keyHash.insert("194:183",               "SHIFT-OPTION:<shift-option-left-paren>");
    keyHash.insert("194:187",               "SHIFT-OPTION:<shift-option-pipe>");
    keyHash.insert("195:134",               "SHIFT-OPTION:<shift-option-double-quote>");
    keyHash.insert("195:154",               "SHIFT-OPTION:<shift-option-colon>");
    keyHash.insert("203:152",               "SHIFT-OPTION:<shift-option-greater-than>");
    keyHash.insert("226:128:161",           "SHIFT-OPTION:<shift-option-ampersand>");
    keyHash.insert("226:128:185",           "SHIFT-OPTION:<shift-option-pound>");
    keyHash.insert("226:128:186",           "SHIFT-OPTION:<shift-option-dollar>");
    keyHash.insert("226:129:132",           "SHIFT-OPTION:<shift-option-exclamation>");
    keyHash.insert("226:130:172",           "SHIFT-OPTION:<shift-option-at>");
    keyHash.insert("239:172:129",           "SHIFT-OPTION:<shift-option-percent>");
    keyHash.insert("239:172:130",           "SHIFT-OPTION:<shift-option-carot>");
    keyHash.insert("226:128:148",           "SHIFT-OPTION:<shift-option-underscore>");
    keyHash.insert("226:128:153",           "SHIFT-OPTION:<shift-option-right-brace>");
    keyHash.insert("226:128:154",           "SHIFT-OPTION:<shift-option-right-paren>");
    keyHash.insert("226:128:157",           "SHIFT-OPTION:<shift-option-left-brace>");
    
    keyHash.insert("27",                    "COMMAND:<init>");
                   
    keyHash.insert("27:91:53:126",          "CURSOR:<page-up>");
    keyHash.insert("27:91:54:126",          "CURSOR:<page-down>");
    keyHash.insert("27:91:65",              "CURSOR:<arrow-up>");
    keyHash.insert("27:91:66",              "CURSOR:<arrow-down>");
    keyHash.insert("27:91:67",              "CURSOR:<arrow-right>");
    keyHash.insert("27:91:68",              "CURSOR:<arrow-left>");
    keyHash.insert("27:91:70",              "CURSOR:<end>");
    keyHash.insert("27:91:72",              "CURSOR:<home>");
    
    keyHash.insert("27:91:49:59:50:68",     "SHIFT-CURSOR:<shift-arrow-right>");
    
    keyHash.insert("27:91:49:53:126",       "FUNCTION:<F5>");
    keyHash.insert("27:91:49:55:126",       "FUNCTION:<F6>");
    keyHash.insert("27:91:49:59:50:80",     "FUNCTION:<F13>");
    keyHash.insert("27:91:49:59:50:83",     "FUNCTION:<F16>");
    keyHash.insert("27:91:49:53:59:50:126", "FUNCTION:<F17>");
    keyHash.insert("27:91:49:55:59:50:126", "FUNCTION:<F18>");
    keyHash.insert("27:91:49:56:59:50:126", "FUNCTION:<F19>");
    
    setupTerminalFeatures();
}



//-------------------------------------------------------------------------
// CxCommandLine:: (destructor)
//
//-------------------------------------------------------------------------

CxKeyboard::~CxKeyboard( void )
{
    teardownTerminalFeatures();
}


void
CxKeyboard::setupTerminalFeatures(void)
{
    
    // tcgetattr gets the parameters of the current terminal
    // STDIN_FILENO will tell tcgetattr that it should write the settings
    // of stdin to oldt
    tcgetattr( STDIN_FILENO, &_oldt);
    

    //now the settings will be copied    
    _newt = _oldt;

  	// The c_iflag field describes the basic terminal input control:
    //---------------------------------------------------------------- 
    // BRKINT     - Signal interrupt on break.
    // ICRNL      - Map CR to NL on input.
    // IGNBRK     - Ignore break condition.
    // IGNCR      - Ignore CR
    // IGNPAR     - Ignore characters with parity errors.
    // INLCR      - Map NL to CR on input.
    // INPCK      - Enable input parity check.
    // ISTRIP     - Strip character
    // IUCLC      - Map upper-case to lower-case on input (LEGACY).
    // IXANY      - Enable any character to restart output.
    // IXOFF      - Enable start/stop input control.
    // IXON       - Enable start/stop output control.
    // PARMRK     - Mark parity errors.
	//
    _newt.c_iflag |= IGNBRK;

	// turns off the cntrl=-s ctrl-c flow control
    _newt.c_iflag &= ~(IXON | IXOFF);


	// ICANON	  - Enable canonical mode 
	// ECHO       - Echo input characters
	// ECHOK      - If ICANON set, kill characters erases current line
	// ECKOE      - if ICANON set, erase char erases preceeding input character
	// ECHONL     - if ICANON set, echo NL character even if ECHO not set
	// ISIG       - When any of the characters INTR, QUIT, SUSP, or DSUSP are 
	//              received, generate the corresponding signal.
	// IEXTEN	  - Enable implementation-defined input processing. This flag, 
	//              as well as ICANON must be enabled for the special characters 
	//              EOL2, LNEXT, REPRINT, WERASE to be interpreted, and for the 
	//              IUCLC flag to be effective.
	//
    _newt.c_lflag &= ~(ICANON | ECHO | ECHOK | ECHOE | ECHONL | ISIG | IEXTEN);


	// Set blocking read mode - select() handles timeout for portability.
	// VTIME units vary across Unix systems (tenths of seconds on POSIX,
	// but may be seconds on old SunOS). Using select() with explicit
	// microsecond timeout is consistent across all platforms.
	//
    _newt.c_cc[VMIN]  = 1; // wait for at least 1 character
    _newt.c_cc[VTIME] = 0; // no timeout - select() handles this

    tcsetattr(fileno(stdin), TCSANOW, &_newt);     
}


//-------------------------------------------------------------------------
// CxKeyboard::teardownTerminalFeatures
//
//-------------------------------------------------------------------------
void
CxKeyboard::teardownTerminalFeatures(void)
{
    //restore the old settings
    tcsetattr( STDIN_FILENO, TCSANOW, &_oldt);
}

//-------------------------------------------------------------------------------------------------
// CxKeyboard::getAction
//
//-------------------------------------------------------------------------------------------------
/*static*/
char
CxKeyboard::readKey(CxKeyboard::BLOCKING mode = WAIT)
{
    int nread;
    char c;
    fd_set fds;
    struct timeval tv;

    if (mode == CxKeyboard::WAIT) {

        while (1) {

            // Use select() with 100ms timeout - portable across all Unix systems
            FD_ZERO(&fds);
            FD_SET(STDIN_FILENO, &fds);
            tv.tv_sec = 0;
            tv.tv_usec = 100000;  // 100ms timeout

            int ready = select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);

            if (ready == -1) {
                if (errno != EINTR) {
                    printf("error in call to select in WAIT mode\n");
                    printf("select() failed: %d, %s\n", errno, strerror(errno));
                    exit(0);
                }
                // EINTR - interrupted by signal, just retry
                continue;
            }

            if (ready == 0) {
                // Timeout - no key pressed, call idle callbacks
                for (int i = 0; i < idleCallbackQueue.entries(); i++) {
                    CxFunctor *f = idleCallbackQueue.at(i);
                    (*f)();
                }
                continue;
            }

            // Data available - read it
            nread = read(STDIN_FILENO, &c, 1);

            if (nread == -1) {
                if (errno != EINTR) {
                    printf("error in call to read in WAIT mode\n");
                    printf("read() failed: %d, %s\n", errno, strerror(errno));
                    exit(0);
                }
                continue;
            }

            if (nread > 0) {
                return c;
            }
        }
    }

    if (mode == CxKeyboard::NO_WAIT) {

        // Use select() with 100ms timeout for escape sequence detection
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        tv.tv_sec = 0;
        tv.tv_usec = 100000;  // 100ms timeout

        int ready = select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);

        if (ready <= 0) {
            // Timeout or error - no data available
            return((char) 255);
        }

        nread = read(STDIN_FILENO, &c, 1);

        if (nread == -1) {
            printf("error in call to read in NO_WAIT mode\n");
            exit(0);
        }

        if (nread == 0) {
            return((char) 255);
        }

        return c;
    }

    return (char) 255;
}


//-------------------------------------------------------------------------------------------------
// CxKeyboard::getAction
//
//-------------------------------------------------------------------------------------------------
/*static*/
CxKeyAction
CxKeyboard::getAction( void )
{
    CxString keyString;
//    int c = getchar();
    
    int c = readKey();
    
    //---------------------------------------------------------------------------------------------
    // Handle normal keys
    //
    //---------------------------------------------------------------------------------------------
    if ((c>=31) && (c<=127)) {
        keyString = handleKey(c);
        keyString = keyHash[ keyString ];
    }
    
    //---------------------------------------------------------------------------------------------
    // handle control key sequences
    //
    //---------------------------------------------------------------------------------------------
    if (c<31) {
        keyString = handleControlKeySequence(c);
        keyString = keyHash[ keyString ];
    }
    
    //---------------------------------------------------------------------------------------------
    // handle option key sequences
    //
    //---------------------------------------------------------------------------------------------
    if ((c==194) || (c==195) || (c==197) || (c==198) ||
        (c==203) || (c==206) || (c==207) || (c==226) ||
        (c==239)) {
        keyString = handleOptionSequence( c );
        keyString = keyHash[ keyString ];
    }
    
    //---------------------------------------------------------------------------------------------
    // handle escape sequences
    //
    //---------------------------------------------------------------------------------------------
    if (c==27) {
        keyString = handleEscapeSequence(c);
        keyString = keyHash[ keyString ];
    }
    
    CxKeyAction keyAction( keyString );
    
    return(keyAction);
}


//-------------------------------------------------------------------------------------------------
// CxKeyboard::handleOptionSequence3
//
//-------------------------------------------------------------------------------------------------
/*static*/
CxString
CxKeyboard::handleOptionSequence3(int c)
{
    int c1 = c;
    
//    int c2 = getchar();
//    int c3 = getchar();
    
    int c2 = readKey();
    int c3 = readKey();
    
    char buffer[100];
    sprintf(buffer, "%d:%d:%d", c1, c2, c3);
    return( buffer );
}


//-------------------------------------------------------------------------------------------------
// CxKeyboard::getAction
//
//-------------------------------------------------------------------------------------------------
/*static*/
CxString
CxKeyboard::handleOptionSequence2(int c)
{
    int c1 = c;
    //int c2 = getchar();
        
    int c2 = readKey();
    
    char buffer[100];
    sprintf(buffer, "%d:%d", c1, c2);
    return( buffer );
}


//-------------------------------------------------------------------------------------------------
// CxKeyboard::getAction
//
//-------------------------------------------------------------------------------------------------
/*static*/
CxString
CxKeyboard::handleOptionSequence( int c )
{
    int c1 = c;
    
    if (c1==226) {
        return( handleOptionSequence3( c1 ) );
    }
    
    if (c1==239) {
        return( handleOptionSequence3( c1 ) );
    }
    
    if (c1==194) {
        return( handleOptionSequence2( c1 ) );
    }
    
    if (c1==195) {
        return( handleOptionSequence2( c1 ) );
    }
    
    if (c1==197) {
        return( handleOptionSequence2( c1 ) );
    }
    
    if (c1==198) {
        return( handleOptionSequence2( c1 ) );
    }
    
    if (c1==203) {
        return( handleOptionSequence2( c1 ) );
    }
    
    if (c1==206) {
        return( handleOptionSequence2( c1 ) );
    }
    
    if (c1==207) {
        return( handleOptionSequence2( c1 ) );
    }
    
    char buffer[100];
    sprintf(buffer, "<unknown:c1:%d>", c1);
    return(buffer);
}


//-------------------------------------------------------------------------------------------------
// CxKeyboard::handleEscapeSequence
//
//-------------------------------------------------------------------------------------------------
/*static*/
CxString
CxKeyboard::handleEscapeSequence(int c)
{
    //---------------------------------------------------------------------------------------------
    // assign character 1
    //---------------------------------------------------------------------------------------------

    int c1 = c;

    //---------------------------------------------------------------------------------------------
    // get one more character, looking for esc, esc
    //---------------------------------------------------------------------------------------------
        
    //int c2 = getchar();
    int c2 = readKey(CxKeyboard::NO_WAIT);

    
    //---------------------------------------------------------------------------------------------
    // if read timed out, then the user hit the escape key
    //---------------------------------------------------------------------------------------------
    if (c2==(char) 255) {
        return("27");
    }
        
        
    //---------------------------------------------------------------------------------------------
    // read character 2 and 3
    //---------------------------------------------------------------------------------------------

    //int c3 = getchar();
    int c3 = readKey();
    
    //---------------------------------------------------------------------------------------------
    // arrow up
    //---------------------------------------------------------------------------------------------
    if ((c1==27) && (c2==91) && (c3==65)) {
        return( "27:91:65" );
    }
    
    //---------------------------------------------------------------------------------------------
    // arrow down
    //---------------------------------------------------------------------------------------------
    if ((c1==27) && (c2==91) && (c3==66)) {
        return( "27:91:66" );
    }
    
    //---------------------------------------------------------------------------------------------
    // arrow right
    //---------------------------------------------------------------------------------------------
    if ((c1==27) && (c2==91) && (c3==67)) {
        return( "27:91:67" );
    }
    
    //---------------------------------------------------------------------------------------------
    // arrow left
    //---------------------------------------------------------------------------------------------
    if ((c1==27) && (c2==91) && (c3==68)) {
        return( "27:91:68" );
    }
    
    //---------------------------------------------------------------------------------------------
    // end
    //---------------------------------------------------------------------------------------------
    if ((c1==27) && (c2==91) && (c3==70)) {
        return( "27:91:70" );
    }
    
    //---------------------------------------------------------------------------------------------
    // home
    //---------------------------------------------------------------------------------------------
    if ((c1==27) && (c2==91) && (c3==72)) {
        return( "27:91:72" );
    }
    
    //---------------------------------------------------------------------------------------------
    // read character 4
    //---------------------------------------------------------------------------------------------
    //int c4=getchar();
    int c4 = readKey();
    
    //---------------------------------------------------------------------------------------------
    // option-delete
    //---------------------------------------------------------------------------------------------
    if ((c1==27) && (c2==91) && (c3==51)) {
        
        if (c4==126) {
            return( "27:91:51:126" );
        }
    }
    
    //---------------------------------------------------------------------------------------------
    // page-up
    //---------------------------------------------------------------------------------------------
    if ((c1==27) && (c2==91) && (c3==53) && (c4==126)) {
        return( "27:91:53:126" );
    }
    
    //---------------------------------------------------------------------------------------------
    // page-down
    //---------------------------------------------------------------------------------------------
    if ((c1==27) && (c2==91) && (c3==54) && (c4==126)) {
        return( "27:91:54:126" );
    }
    
    //---------------------------------------------------------------------------------------------
    // option arrow up
    //---------------------------------------------------------------------------------------------
    if ((c1==27) && (c2==27) && (c3==91) && (c4==65)) {
        return( "27:27:91:65" );
    }
    
    //---------------------------------------------------------------------------------------------
    // option arrow down
    //---------------------------------------------------------------------------------------------
    if ((c1==27) && (c2==27) && (c3==91) && (c4==66)) {
        return( "27:27:91:66" );
    }
    
    //---------------------------------------------------------------------------------------------
    // option arrow left
    //---------------------------------------------------------------------------------------------
    if ((c1==27) && (c2==27) && (c3==91) && (c4==68)) {
        return( "27:27:91:68" );
    }
    
    //---------------------------------------------------------------------------------------------
    // option arrow right
    //---------------------------------------------------------------------------------------------
    if ((c1==27) && (c2==27) && (c3==91) && (c4==67)) {
        return( "27:27:91:67" );
    }
    
    //---------------------------------------------------------------------------------------------
    // read character 5
    //---------------------------------------------------------------------------------------------

    //int c5 = getchar();
    int c5 = readKey();
    
    //---------------------------------------------------------------------------------------------
    // option-page-xx
    //---------------------------------------------------------------------------------------------
    if ((c1==27) && (c2==27) && (c3==91)) {
        
        //-----------------------------------------------------------------------------------------
        // option page up
        //-----------------------------------------------------------------------------------------
        if ((c4==53) && (c5==126)) {
            return( "27:27:91:53:126" );
        }
        
        //-----------------------------------------------------------------------------------------
        // option page down
        //-----------------------------------------------------------------------------------------
        if ((c4==54) && (c5==126)) {
            return( "27:27:91:54:126" );
        }
        
    }
    
    //---------------------------------------------------------------------------------------------
    // function keys
    //---------------------------------------------------------------------------------------------
    if ((c1==27) && (c2==91) && (c3==49)) {
        
        //-----------------------------------------------------------------------------------------
        // F5
        //-----------------------------------------------------------------------------------------
        if ((c4==53) && (c5==126)) {
            return( "27:91:49:53:126" );
        }
        
        //-----------------------------------------------------------------------------------------
        // F6
        //-----------------------------------------------------------------------------------------
        if ((c4==55) && (c5==126)) {
            return( "27:91:49:55:126" );
        }

        //-----------------------------------------------------------------------------------------
        // read character 6
        //-----------------------------------------------------------------------------------------
        //int c6 = getchar();
        int c6 = readKey();
        
        //-----------------------------------------------------------------------------------------
        // F13
        //-----------------------------------------------------------------------------------------
        if ((c4==59) && (c5==50) && (c6==80)) {
            return( "27:91:49:59:50:80" );
        }
        
        //-----------------------------------------------------------------------------------------
        // F16
        //-----------------------------------------------------------------------------------------
        if ((c4==59) && (c5==50) && (c6==83)) {
            return( "27:91:49:59:50:83" );
        }
        
        //-----------------------------------------------------------------------------------------
        // option-end
        //-----------------------------------------------------------------------------------------
        if ((c4==59) && (c5==57) && (c6==70)) {
            return( "27:91:49:59:57:70" );
        }
        
        //-----------------------------------------------------------------------------------------
        // option-home
        //-----------------------------------------------------------------------------------------
        if ((c4==59) && (c5==57) && (c6==72)) {
            return( "27:91:49:59:57:72" );
        }
        
        if ((c4==59) && (c5==50) && (c6==68)) {
            return( "27:91:49:59:50:68" );
        }
        
        //-----------------------------------------------------------------------------------------
        // read character 7
        //-----------------------------------------------------------------------------------------
        //int c7 = getchar();
        int c7 = readKey();
        
        //-----------------------------------------------------------------------------------------
        // F17
        //-----------------------------------------------------------------------------------------
        if ((c4==53) && (c5==59) && (c6==50) && (c7==126)) {
            return("27:91:49:53:59:50:126");
        }
        
        //-----------------------------------------------------------------------------------------
        // F18
        //-----------------------------------------------------------------------------------------
        if ((c4==55) && (c5==59) && (c6==50) && (c7==126)) {
            return("27:91:49:55:59:50:126");
        }
        
        //-----------------------------------------------------------------------------------------
        // F19
        //-----------------------------------------------------------------------------------------
        if ((c4==56) && (c5==59) && (c6==50) && (c7==126)) {
            return("27:91:49:56:59:50:126");
        }
    }
    
    return( "<unknown>" );
}


//-------------------------------------------------------------------------------------------------
// CxKeyboard::controlKeySequence
//
//-------------------------------------------------------------------------------------------------
/*static*/
CxString
CxKeyboard::handleControlKeySequence( int c )
{
    char buffer[5];
    sprintf(buffer, "%d", c);
    return(buffer);
}


//-------------------------------------------------------------------------------------------------
// CxKeyboard::handleKey
//
//-------------------------------------------------------------------------------------------------
/*static*/
CxString
CxKeyboard::handleKey(int c )
{
    char buffer[5];
    sprintf(buffer, "%d", c);
    return(buffer);
}
