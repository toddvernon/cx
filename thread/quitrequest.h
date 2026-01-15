#ifndef _CxQuitRequest_h_
#define _CxQuitRequest_h_



class CxQuitRequest : public CxRunnable
{    
  public:
     	
    CxQuitRequest( void ) { }
    // Constructor
        
    ~CxQuitRequest( void ) { }
    // Destructor

    virtual int isQuitRequest() { return 1; }
    // This is a quit request

    virtual void run() { }
};


#endif
