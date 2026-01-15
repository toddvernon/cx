/***************************************************************************
 *
 *  CxCommand
 *
 *  CxCommand Class
 *
 ***************************************************************************
 *
 * (c) Copyright 2018 T.Vernon
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
#include <termios.h>
#include <unistd.h>

#include <iostream>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include <cx/base/string.h>
#include <cx/base/slist.h>
#include <cx/base/rule.h>
#include <cx/base/exception.h>
#include <cx/base/file.h>
#include <cx/base/star.h>
#include <cx/base/filename.h>
#include <cx/base/directory.h>
#include <cx/functor/functor.h>
#include <cx/functor/defercall.h>
#include <cx/functor/reference.h>

#include <cx/commandline/command.h>


//-------------------------------------------------------------------------
// CxCommand:: (constructor)
//
//-------------------------------------------------------------------------
CxCommand::CxCommand(void)
:_name(""), _f(NULL)
{
}


//-------------------------------------------------------------------------
// CxCommand:: (constructor)
//
//-------------------------------------------------------------------------
CxCommand::CxCommand( CxString name, CxFunctor *f)
{
    _name = name;
    _f    = f;
}


//-------------------------------------------------------------------------
// CxCommand:: (constructor)
//
//-------------------------------------------------------------------------
CxCommand::CxCommand( const CxCommand& c_ )
{
    if (&c_ != this) {
        _name = c_._name;
        _f    = c_._f;
    }
}


//-------------------------------------------------------------------------
// CxCommand:: (assignment operator)
//
//-------------------------------------------------------------------------
CxCommand
CxCommand::operator=( const CxCommand& c_)
{
    if (&c_ != this) {
        _name = c_._name;
        _f    = c_._f;
    }
    
    return( *this );
}


//-------------------------------------------------------------------------
// CxCommand::name
//
//-------------------------------------------------------------------------
CxString
CxCommand::name(void)
{
    return(_name );
}


//-------------------------------------------------------------------------
// CxCommand::call
//
//-------------------------------------------------------------------------
void
CxCommand::call(void)
{
    if (_f) (*_f)( );
}

