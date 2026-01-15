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
