//-------------------------------------------------------------------------------------------------
//
//  functor.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  functor.h
//
//-------------------------------------------------------------------------------------------------

#ifndef Functor_h
#define Functor_h 1

/***************************************************************************/

class CxFunctor 
{

  public:

      virtual ~CxFunctor()
      {}

      virtual void operator () () = 0;

};


#endif
