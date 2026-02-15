//-------------------------------------------------------------------------------------------------
//
//  runnablefunctor.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  runnablefunctor.h
//
//-------------------------------------------------------------------------------------------------

#ifndef _CxRunnableFunctor_h_
#define _CxRunnableFunctor_h_

#include <cx/functor/functor.h>
#include "runnable.h"

class CxRunnableFunctor : public CxRunnable
{    
  public:
     	
    CxRunnableFunctor( CxFunctor* f ) : m_pFunctor( f ) { }
    // Constructor
        
    ~CxRunnableFunctor( void ) { delete m_pFunctor; }
    // Destructor

    virtual void run() { (*m_pFunctor)(); }

  private:
      CxFunctor* m_pFunctor;
};


#endif
