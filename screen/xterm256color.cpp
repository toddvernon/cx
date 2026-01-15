/***************************************************************************
 *
 *  CxXterm256Color
 *
 *  CxXterm256Color Class
 *
 ***************************************************************************
 *
 * (c) Copyright 2023 T.Vernon
 * ALL RIGHTS RESERVED
 *
 * The software and information contained herein are proprietary to, and
 * comprise valuable trade secrets of, T.Vernon , which
 * intends to preserve as trade secrets such software and information.
 * This software is furnished pursuant to a written license agreement and
 * may be used, copied, transmitted, and stored only in accordance with
 * the terms of such license and with the inclusion of the above copyright
 * notice.  This software and information or any other copies thereof may
 * not be provided or otherwise made available to any other person.
 *
 *
 **************************************************************************/

#include <stdio.h>

#include <cx/screen/color.h>


//-------------------------------------------------------------------------
// CxAnsiColor:: (constructor)
//
//-------------------------------------------------------------------------
CxXterm256Color::CxXterm256Color(void)
{
    _colorType = XTERM256_BASE;
    _value = -1;
    
    _colorMap.insert(CxString::toUpper("Black"),0);
    _colorMap.insert(CxString::toUpper("Maroon"),1);
    _colorMap.insert(CxString::toUpper("Green"),2);
    _colorMap.insert(CxString::toUpper("Olive"),3);
    _colorMap.insert(CxString::toUpper("Navy"),4);
    _colorMap.insert(CxString::toUpper("Purple"),5);
    _colorMap.insert(CxString::toUpper("Teal"),6);
    _colorMap.insert(CxString::toUpper("Silver"),7);
    _colorMap.insert(CxString::toUpper("Grey"),8);
    _colorMap.insert(CxString::toUpper("Red"),9);
    _colorMap.insert(CxString::toUpper("Lime"),10);
    _colorMap.insert(CxString::toUpper("Yellow"),11);
    _colorMap.insert(CxString::toUpper("Blue"),12);
    _colorMap.insert(CxString::toUpper("Fuchsia"),13);
    _colorMap.insert(CxString::toUpper("Aqua"),14);
    _colorMap.insert(CxString::toUpper("White"),15);
    _colorMap.insert(CxString::toUpper("Grey0"),16);
    _colorMap.insert(CxString::toUpper("NavyBlue"),17);
    _colorMap.insert(CxString::toUpper("DarkBlue"),18);
    _colorMap.insert(CxString::toUpper("Blue3"),19);
    _colorMap.insert(CxString::toUpper("Blue3"),20);
    _colorMap.insert(CxString::toUpper("Blue1"),21);
    _colorMap.insert(CxString::toUpper("DarkGreen"),22);
    _colorMap.insert(CxString::toUpper("DeepSkyBlue4"),23);
    _colorMap.insert(CxString::toUpper("DeepSkyBlue4"),24);
    _colorMap.insert(CxString::toUpper("DeepSkyBlue4"),25);
    _colorMap.insert(CxString::toUpper("DodgerBlue3"),26);
    _colorMap.insert(CxString::toUpper("DodgerBlue2"),27);
    _colorMap.insert(CxString::toUpper("Green4"),28);
    _colorMap.insert(CxString::toUpper("SpringGreen4"),29);
    _colorMap.insert(CxString::toUpper("Turquoise4"),30);
    _colorMap.insert(CxString::toUpper("DeepSkyBlue3"),31);
    _colorMap.insert(CxString::toUpper("DeepSkyBlue3"),32);
    _colorMap.insert(CxString::toUpper("DodgerBlue1"),33);
    _colorMap.insert(CxString::toUpper("Green3"),34);
    _colorMap.insert(CxString::toUpper("SpringGreen3"),35);
    _colorMap.insert(CxString::toUpper("DarkCyan"),36);
    _colorMap.insert(CxString::toUpper("LightSeaGreen"),37);
    _colorMap.insert(CxString::toUpper("DeepSkyBlue2"),38);
    _colorMap.insert(CxString::toUpper("DeepSkyBlue1"),39);
    _colorMap.insert(CxString::toUpper("Green3"),40);
    _colorMap.insert(CxString::toUpper("SpringGreen3"),41);
    _colorMap.insert(CxString::toUpper("SpringGreen2"),42);
    _colorMap.insert(CxString::toUpper("Cyan3"),43);
    _colorMap.insert(CxString::toUpper("DarkTurquoise"),44);
    _colorMap.insert(CxString::toUpper("Turquoise2"),45);
    _colorMap.insert(CxString::toUpper("Green1"),46);
    _colorMap.insert(CxString::toUpper("SpringGreen2"),47);
    _colorMap.insert(CxString::toUpper("SpringGreen1"),48);
    _colorMap.insert(CxString::toUpper("MediumSpringGreen"),49);
    _colorMap.insert(CxString::toUpper("Cyan2"),50);
    _colorMap.insert(CxString::toUpper("Cyan1"),51);
    _colorMap.insert(CxString::toUpper("DarkRed"),52);
    _colorMap.insert(CxString::toUpper("DeepPink4"),53);
    _colorMap.insert(CxString::toUpper("Purple4"),54);
    _colorMap.insert(CxString::toUpper("Purple4"),55);
    _colorMap.insert(CxString::toUpper("Purple3"),56);
    _colorMap.insert(CxString::toUpper("BlueViolet"),57);
    _colorMap.insert(CxString::toUpper("Orange4"),58);
    _colorMap.insert(CxString::toUpper("Grey37"),59);
    _colorMap.insert(CxString::toUpper("MediumPurple4"),60);
    _colorMap.insert(CxString::toUpper("SlateBlue3"),61);
    _colorMap.insert(CxString::toUpper("SlateBlue3"),62);
    _colorMap.insert(CxString::toUpper("RoyalBlue1"),63);
    _colorMap.insert(CxString::toUpper("Chartreuse4"),64);
    _colorMap.insert(CxString::toUpper("DarkSeaGreen4"),65);
    _colorMap.insert(CxString::toUpper("PaleTurquoise4"),66);
    _colorMap.insert(CxString::toUpper("SteelBlue"),67);
    _colorMap.insert(CxString::toUpper("SteelBlue3"),68);
    _colorMap.insert(CxString::toUpper("CornflowerBlue"),69);
    _colorMap.insert(CxString::toUpper("Chartreuse3"),70);
    _colorMap.insert(CxString::toUpper("DarkSeaGreen4"),71);
    _colorMap.insert(CxString::toUpper("CadetBlue"),72);
    _colorMap.insert(CxString::toUpper("CadetBlue"),73);
    _colorMap.insert(CxString::toUpper("SkyBlue3"),74);
    _colorMap.insert(CxString::toUpper("SteelBlue1"),75);
    _colorMap.insert(CxString::toUpper("Chartreuse3"),76);
    _colorMap.insert(CxString::toUpper("PaleGreen3"),77);
    _colorMap.insert(CxString::toUpper("SeaGreen3"),78);
    _colorMap.insert(CxString::toUpper("Aquamarine3"),79);
    _colorMap.insert(CxString::toUpper("MediumTurquoise"),80);
    _colorMap.insert(CxString::toUpper("SteelBlue1"),81);
    _colorMap.insert(CxString::toUpper("Chartreuse2"),82);
    _colorMap.insert(CxString::toUpper("SeaGreen2"),83);
    _colorMap.insert(CxString::toUpper("SeaGreen1"),84);
    _colorMap.insert(CxString::toUpper("SeaGreen1"),85);
    _colorMap.insert(CxString::toUpper("Aquamarine1"),86);
    _colorMap.insert(CxString::toUpper("DarkSlateGray2"),87);
    _colorMap.insert(CxString::toUpper("DarkRed"),88);
    _colorMap.insert(CxString::toUpper("DeepPink4"),89);
    _colorMap.insert(CxString::toUpper("DarkMagenta"),90);
    _colorMap.insert(CxString::toUpper("DarkMagenta"),91);
    _colorMap.insert(CxString::toUpper("DarkViolet"),92);
    _colorMap.insert(CxString::toUpper("Purple"),93);
    _colorMap.insert(CxString::toUpper("Orange4"),94);
    _colorMap.insert(CxString::toUpper("LightPink4"),95);
    _colorMap.insert(CxString::toUpper("Plum4"),96);
    _colorMap.insert(CxString::toUpper("MediumPurple3"),97);
    _colorMap.insert(CxString::toUpper("MediumPurple3"),98);
    _colorMap.insert(CxString::toUpper("SlateBlue1"),99);
    _colorMap.insert(CxString::toUpper("Yellow4"),100);
    _colorMap.insert(CxString::toUpper("Wheat4"),101);
    _colorMap.insert(CxString::toUpper("Grey53"),102);
    _colorMap.insert(CxString::toUpper("LightSlateGrey"),103);
    _colorMap.insert(CxString::toUpper("MediumPurple"),104);
    _colorMap.insert(CxString::toUpper("LightSlateBlue"),105);
    _colorMap.insert(CxString::toUpper("Yellow4"),106);
    _colorMap.insert(CxString::toUpper("DarkOliveGreen3"),107);
    _colorMap.insert(CxString::toUpper("DarkSeaGreen"),108);
    _colorMap.insert(CxString::toUpper("LightSkyBlue3"),109);
    _colorMap.insert(CxString::toUpper("LightSkyBlue3"),110);
    _colorMap.insert(CxString::toUpper("SkyBlue2"),111);
    _colorMap.insert(CxString::toUpper("Chartreuse2"),112);
    _colorMap.insert(CxString::toUpper("DarkOliveGreen3"),113);
    _colorMap.insert(CxString::toUpper("PaleGreen3"),114);
    _colorMap.insert(CxString::toUpper("DarkSeaGreen3"),115);
    _colorMap.insert(CxString::toUpper("DarkSlateGray3"),116);
    _colorMap.insert(CxString::toUpper("SkyBlue1"),117);
    _colorMap.insert(CxString::toUpper("Chartreuse1"),118);
    _colorMap.insert(CxString::toUpper("LightGreen"),119);
    _colorMap.insert(CxString::toUpper("LightGreen"),120);
    _colorMap.insert(CxString::toUpper("PaleGreen1"),121);
    _colorMap.insert(CxString::toUpper("Aquamarine1"),122);
    _colorMap.insert(CxString::toUpper("DarkSlateGray1"),123);
    _colorMap.insert(CxString::toUpper("Red3"),124);
    _colorMap.insert(CxString::toUpper("DeepPink4"),125);
    _colorMap.insert(CxString::toUpper("MediumVioletRed"),126);
    _colorMap.insert(CxString::toUpper("Magenta3"),127);
    _colorMap.insert(CxString::toUpper("DarkViolet"),128);
    _colorMap.insert(CxString::toUpper("Purple"),129);
    _colorMap.insert(CxString::toUpper("DarkOrange3"),130);
    _colorMap.insert(CxString::toUpper("IndianRed"),131);
    _colorMap.insert(CxString::toUpper("HotPink3"),132);
    _colorMap.insert(CxString::toUpper("MediumOrchid3"),133);
    _colorMap.insert(CxString::toUpper("MediumOrchid"),134);
    _colorMap.insert(CxString::toUpper("MediumPurple2"),135);
    _colorMap.insert(CxString::toUpper("DarkGoldenrod"),136);
    _colorMap.insert(CxString::toUpper("LightSalmon3"),137);
    _colorMap.insert(CxString::toUpper("RosyBrown"),138);
    _colorMap.insert(CxString::toUpper("Grey63"),139);
    _colorMap.insert(CxString::toUpper("MediumPurple2"),140);
    _colorMap.insert(CxString::toUpper("MediumPurple1"),141);
    _colorMap.insert(CxString::toUpper("Gold3"),142);
    _colorMap.insert(CxString::toUpper("DarkKhaki"),143);
    _colorMap.insert(CxString::toUpper("NavajoWhite3"),144);
    _colorMap.insert(CxString::toUpper("Grey69"),145);
    _colorMap.insert(CxString::toUpper("LightSteelBlue3"),146);
    _colorMap.insert(CxString::toUpper("LightSteelBlue"),147);
    _colorMap.insert(CxString::toUpper("Yellow3"),148);
    _colorMap.insert(CxString::toUpper("DarkOliveGreen3"),149);
    _colorMap.insert(CxString::toUpper("DarkSeaGreen3"),150);
    _colorMap.insert(CxString::toUpper("DarkSeaGreen2"),151);
    _colorMap.insert(CxString::toUpper("LightCyan3"),152);
    _colorMap.insert(CxString::toUpper("LightSkyBlue1"),153);
    _colorMap.insert(CxString::toUpper("GreenYellow"),154);
    _colorMap.insert(CxString::toUpper("DarkOliveGreen2"),155);
    _colorMap.insert(CxString::toUpper("PaleGreen1"),156);
    _colorMap.insert(CxString::toUpper("DarkSeaGreen2"),157);
    _colorMap.insert(CxString::toUpper("DarkSeaGreen1"),158);
    _colorMap.insert(CxString::toUpper("PaleTurquoise1"),159);
    _colorMap.insert(CxString::toUpper("Red3"),160);
    _colorMap.insert(CxString::toUpper("DeepPink3"),161);
    _colorMap.insert(CxString::toUpper("DeepPink3"),162);
    _colorMap.insert(CxString::toUpper("Magenta3"),163);
    _colorMap.insert(CxString::toUpper("Magenta3"),164);
    _colorMap.insert(CxString::toUpper("Magenta2"),165);
    _colorMap.insert(CxString::toUpper("DarkOrange3"),166);
    _colorMap.insert(CxString::toUpper("IndianRed"),167);
    _colorMap.insert(CxString::toUpper("HotPink3"),168);
    _colorMap.insert(CxString::toUpper("HotPink2"),169);
    _colorMap.insert(CxString::toUpper("Orchid"),170);
    _colorMap.insert(CxString::toUpper("MediumOrchid1"),171);
    _colorMap.insert(CxString::toUpper("Orange3"),172);
    _colorMap.insert(CxString::toUpper("LightSalmon3"),173);
    _colorMap.insert(CxString::toUpper("LightPink3"),174);
    _colorMap.insert(CxString::toUpper("Pink3"),175);
    _colorMap.insert(CxString::toUpper("Plum3"),176);
    _colorMap.insert(CxString::toUpper("Violet"),177);
    _colorMap.insert(CxString::toUpper("Gold3"),178);
    _colorMap.insert(CxString::toUpper("LightGoldenrod3"),179);
    _colorMap.insert(CxString::toUpper("Tan"),180);
    _colorMap.insert(CxString::toUpper("MistyRose3"),181);
    _colorMap.insert(CxString::toUpper("Thistle3"),182);
    _colorMap.insert(CxString::toUpper("Plum2"),183);
    _colorMap.insert(CxString::toUpper("Yellow3"),184);
    _colorMap.insert(CxString::toUpper("Khaki3"),185);
    _colorMap.insert(CxString::toUpper("LightGoldenrod2"),186);
    _colorMap.insert(CxString::toUpper("LightYellow3"),187);
    _colorMap.insert(CxString::toUpper("Grey84"),188);
    _colorMap.insert(CxString::toUpper("LightSteelBlue1"),189);
    _colorMap.insert(CxString::toUpper("Yellow2"),190);
    _colorMap.insert(CxString::toUpper("DarkOliveGreen1"),191);
    _colorMap.insert(CxString::toUpper("DarkOliveGreen1"),192);
    _colorMap.insert(CxString::toUpper("DarkSeaGreen1"),193);
    _colorMap.insert(CxString::toUpper("Honeydew2"),194);
    _colorMap.insert(CxString::toUpper("LightCyan1"),195);
    _colorMap.insert(CxString::toUpper("Red1"),196);
    _colorMap.insert(CxString::toUpper("DeepPink2"),197);
    _colorMap.insert(CxString::toUpper("DeepPink1"),198);
    _colorMap.insert(CxString::toUpper("DeepPink1"),199);
    _colorMap.insert(CxString::toUpper("Magenta2"),200);
    _colorMap.insert(CxString::toUpper("Magenta1"),201);
    _colorMap.insert(CxString::toUpper("OrangeRed1"),202);
    _colorMap.insert(CxString::toUpper("IndianRed1"),203);
    _colorMap.insert(CxString::toUpper("IndianRed1"),204);
    _colorMap.insert(CxString::toUpper("HotPink"),205);
    _colorMap.insert(CxString::toUpper("HotPink"),206);
    _colorMap.insert(CxString::toUpper("MediumOrchid1"),207);
    _colorMap.insert(CxString::toUpper("DarkOrange"),208);
    _colorMap.insert(CxString::toUpper("Salmon1"),209);
    _colorMap.insert(CxString::toUpper("LightCoral"),210);
    _colorMap.insert(CxString::toUpper("PaleVioletRed1"),211);
    _colorMap.insert(CxString::toUpper("Orchid2"),212);
    _colorMap.insert(CxString::toUpper("Orchid1"),213);
    _colorMap.insert(CxString::toUpper("Orange1"),214);
    _colorMap.insert(CxString::toUpper("SandyBrown"),215);
    _colorMap.insert(CxString::toUpper("LightSalmon1"),216);
    _colorMap.insert(CxString::toUpper("LightPink1"),217);
    _colorMap.insert(CxString::toUpper("Pink1"),218);
    _colorMap.insert(CxString::toUpper("Plum1"),219);
    _colorMap.insert(CxString::toUpper("Gold1"),220);
    _colorMap.insert(CxString::toUpper("LightGoldenrod2"),221);
    _colorMap.insert(CxString::toUpper("LightGoldenrod2"),222);
    _colorMap.insert(CxString::toUpper("NavajoWhite1"),223);
    _colorMap.insert(CxString::toUpper("MistyRose1"),224);
    _colorMap.insert(CxString::toUpper("Thistle1"),225);
    _colorMap.insert(CxString::toUpper("Yellow1"),226);
    _colorMap.insert(CxString::toUpper("LightGoldenrod1"),227);
    _colorMap.insert(CxString::toUpper("Khaki1"),228);
    _colorMap.insert(CxString::toUpper("Wheat1"),229);
    _colorMap.insert(CxString::toUpper("Cornsilk1"),230);
    _colorMap.insert(CxString::toUpper("Grey100"),231);
    _colorMap.insert(CxString::toUpper("Grey3"),232);
    _colorMap.insert(CxString::toUpper("Grey7"),233);
    _colorMap.insert(CxString::toUpper("Grey11"),234);
    _colorMap.insert(CxString::toUpper("Grey15"),235);
    _colorMap.insert(CxString::toUpper("Grey19"),236);
    _colorMap.insert(CxString::toUpper("Grey23"),237);
    _colorMap.insert(CxString::toUpper("Grey27"),238);
    _colorMap.insert(CxString::toUpper("Grey30"),239);
    _colorMap.insert(CxString::toUpper("Grey35"),240);
    _colorMap.insert(CxString::toUpper("Grey39"),241);
    _colorMap.insert(CxString::toUpper("Grey42"),242);
    _colorMap.insert(CxString::toUpper("Grey46"),243);
    _colorMap.insert(CxString::toUpper("Grey50"),244);
    _colorMap.insert(CxString::toUpper("Grey54"),245);
    _colorMap.insert(CxString::toUpper("Grey58"),246);
    _colorMap.insert(CxString::toUpper("Grey62"),247);
    _colorMap.insert(CxString::toUpper("Grey66"),248);
    _colorMap.insert(CxString::toUpper("Grey70"),249);
    _colorMap.insert(CxString::toUpper("Grey74"),250);
    _colorMap.insert(CxString::toUpper("Grey78"),251);
    _colorMap.insert(CxString::toUpper("Grey82"),252);
    _colorMap.insert(CxString::toUpper("Grey85"),253);
    _colorMap.insert(CxString::toUpper("Grey89"),254);
    _colorMap.insert(CxString::toUpper("Grey93"),255);
}

CxString
CxXterm256Color::terminalString(void)
{
    CxString s;
    return(s);
}

CxString
CxXterm256Color::resetTerminalString(void)
{
    CxString s;
    return(s);
    
}



//=================================================================================================


//-------------------------------------------------------------------------
// CxXterm256ForegroundColor:: (constructor)
//
//-------------------------------------------------------------------------
CxXterm256ForegroundColor::CxXterm256ForegroundColor(void)
{
    _colorType = XTERM256_FOREGROUND;
}


CxXterm256ForegroundColor::CxXterm256ForegroundColor( CxString colorName )
{
    _colorType = XTERM256_FOREGROUND;
    
    const int* v = _colorMap.find(CxString::toUpper(colorName));
    
    if (v == NULL) {
        _value = -1;
    } else {
        _value = *v;
    }
}

//CxXterm256ForegroundColor::CxXterm256ForegroundColor( int value )
//{
//    _colorType = XTERM256_FOREGROUND;
//
//    _value   = value;
//}


//-------------------------------------------------------------------------
// CxXterm256ForegroundColor:: (constructor)
//
//-------------------------------------------------------------------------
CxXterm256ForegroundColor::CxXterm256ForegroundColor( const CxXterm256ForegroundColor& c_ )
{
    if (&c_ != this) {
        _value   = c_._value;
    }
}


//-------------------------------------------------------------------------
// CxXterm256ForegroundColor:: (assignment operator)
//
//-------------------------------------------------------------------------
CxXterm256ForegroundColor
CxXterm256ForegroundColor::operator=( const CxXterm256ForegroundColor& c_)
{
    if (&c_ != this) {
        _value   = c_._value;
    }
    
    return( *this );
}

//-------------------------------------------------------------------------
// CxXterm256ForegroundColor::value
//
//-------------------------------------------------------------------------
int
CxXterm256ForegroundColor::value(void)
{
    return( _value );
}

//-------------------------------------------------------------------------
// CxXterm256ForegroundColor::valueAsInt
//
//-------------------------------------------------------------------------
int
CxXterm256ForegroundColor::valueAsInt(void)
{
    return( (int) _value );
}

//-------------------------------------------------------------------------
// CxXterm256ForegroundColor::terminalString
//
//-------------------------------------------------------------------------
CxString
CxXterm256ForegroundColor::terminalString(void)
{
    CxString s;
    if (_value == -1) return( s );

    char buffer[30];
    sprintf(buffer, "\033[38;5;%dm", (int) _value);
    return( buffer );
}

CxString
CxXterm256ForegroundColor::resetTerminalString(void)
{
    char buffer[30];
    sprintf(buffer, "\033[39m");
    return( buffer );
}


//=================================================================================================

//-------------------------------------------------------------------------
// CxXterm256BackgroundColor:: (constructor)
//
//-------------------------------------------------------------------------
CxXterm256BackgroundColor::CxXterm256BackgroundColor(void)
{
    _colorType = XTERM256_BACKGROUND;
}


CxXterm256BackgroundColor::CxXterm256BackgroundColor( CxString colorName )
{
    _colorType = XTERM256_BACKGROUND;
    
    const int* v =_colorMap.find(CxString::toUpper(colorName));
    
    if (v == NULL) {
        _value = -1;
    } else {
        _value = *v;
    }
    
    
}

//CxXterm256BackgroundColor::CxXterm256BackgroundColor( int value )
//{
//    _value   = value;
//}


//-------------------------------------------------------------------------
// CxXterm256BackgroundColor:: (constructor)
//
//-------------------------------------------------------------------------
CxXterm256BackgroundColor::CxXterm256BackgroundColor( const CxXterm256BackgroundColor& c_ )
{
    if (&c_ != this) {
        _value   = c_._value;
    }
}


//-------------------------------------------------------------------------
// CxXterm256BackgroundColor:: (assignment operator)
//
//-------------------------------------------------------------------------
CxXterm256BackgroundColor
CxXterm256BackgroundColor::operator=( const CxXterm256BackgroundColor& c_)
{
    if (&c_ != this) {
        _value   = c_._value;
    }
    
    return( *this );
}

//-------------------------------------------------------------------------
// CxXterm256BackgroundColor::value
//
//-------------------------------------------------------------------------
int
CxXterm256BackgroundColor::value(void)
{
    return( _value );
}

//-------------------------------------------------------------------------
// CxXterm256BackgroundColor::valueAsInt
//
//-------------------------------------------------------------------------
int
CxXterm256BackgroundColor::valueAsInt(void)
{
    return( (int) _value );
}

//-------------------------------------------------------------------------
// CxXterm256BackgroundColor::terminalString
//
//-------------------------------------------------------------------------
CxString
CxXterm256BackgroundColor::terminalString(void)
{
    char buffer[30];

    CxString s;
    if (_value == -1 ) {
        return( s );
    }
    
    sprintf(buffer, "\033[48;5;%dm", (int) _value);
    return( buffer );
}

CxString
CxXterm256BackgroundColor::resetTerminalString(void)
{
    char buffer[30];
    sprintf(buffer, "\033[39m");
    return( buffer );
}


// To send a 256 color foreground color one must print "\e[38;5;#m" where # is a number between 0 and 255. For background colors one must print "\e[48;5;#m"
// where # behaves in the same manner as foreground colors.
