#ifndef _CxRunnable_h_
#define _CxRunnable_h_



class CxRunnable
{    
  public:
     	
      CxRunnable( void ) { };
    // Constructor
        
    virtual ~CxRunnable( void ) { };
    // Destructor

    virtual void run( void ) = 0;
    // the run method

    virtual int isQuitRequest() { return 0; }
};


#endif
