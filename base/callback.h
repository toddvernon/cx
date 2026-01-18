/***************************************************************************
 *
 *  callback.h 
 *
 *  
 *
 ***************************************************************************
 *
 * (c) Copyright 1999,2000,2001 T.Vernon
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

#include <cx/base/exception.h>
#include <cx/base/cntbody.h>


#ifndef _CxCallback_h_
#define _CxCallback_h_



//-------------------------------------------------------------------------
// class CxBaseCallbackBody::CxBaseCallbackBody
//
//-------------------------------------------------------------------------
template <class P1>
class CxBaseCallbackBody : public CxCountedBody
{
  public:
    virtual void operator()(P1) const = 0;
};



//-------------------------------------------------------------------------
// class CxNilCallbackBody::CxNilCallbackBody
//
//-------------------------------------------------------------------------
template <class P1>
class CxNilCallbackBody : public CxBaseCallbackBody<P1>
{
  public:
    void operator()(P1 parm_) const
    {
        throw CxCallbackException("CxNilCallbackBody::operator(): nil callback invoked");
    }
};



//-------------------------------------------------------------------------
// class CxFunctionCallbackBody::CxFunctionCallbackBody
//
//-------------------------------------------------------------------------
template <class P1, class Function>
class CxFunctionCallbackBody : public CxBaseCallbackBody<P1>
{
  public:
    CxFunctionCallbackBody(Function& function_):
        _function(function_) 
    {
    }

    void operator()(P1 parm_) const
    {
        _function(parm_);
    }

  private:
    Function _function;
};



//-------------------------------------------------------------------------
// class CxMemberCallbackBody::CxMemberCallbackBody
//
//-------------------------------------------------------------------------
template <class P1, class Client, class Member>
class CxMemberCallbackBody : public CxBaseCallbackBody<P1>
{
  public:

    CxMemberCallbackBody(Client& client_, Member member_):
        _client(client_), _member(member_)
    {
    }

    void operator()(P1 parm_) const
    {
        ((&_client)->*_member)(parm_);
    }

  private:
    Client& _client;
    Member  _member;
};



//-------------------------------------------------------------------------
// class CxCallback::CxCallback
//
//-------------------------------------------------------------------------
template <class P1>
class CxCallback
{
  public:
    CxCallback() : body_( new CxNilCallbackBody<P1> )
    {
        this->incBodyCount();
    }

    CxCallback( CxBaseCallbackBody<P1>* body ) : body_(body)
    {
        this->incBodyCount();
    }

    CxCallback( const CxCallback<P1>& callback ) : body_( callback.body_ )
    {
        this->incBodyCount();
    }

    ~CxCallback() 
    {
        this->decBodyCount(); 
        body_ = 0;
    }

    CxCallback<P1>& operator=( const CxCallback<P1>& callback )
    {
        if (body_ != callback.body_) {
            this->decBodyCount();
            body_ = callback.body_;
            this->incBodyCount();
        }
        return *this;
    }

    void operator()(P1 p1) 
    {
        (*body_)(p1);
    }

  private:

    CxBaseCallbackBody<P1>* body_;

    void incBodyCount() 
    {
        body_->incCount();
    }

    void decBodyCount() 
    {
        body_->decCount(); 
        if(body_->count() == 0) delete body_;
    }
};


//-------------------------------------------------------------------------
// make_callback
//
//-------------------------------------------------------------------------
template <class P1, class Client, class Member>
CxCallback<P1>
make_callback( CxCallback<P1> * , Client& client_, Member member_)
{
    return CxCallback<P1>( new CxMemberCallbackBody<P1,Client,Member>(client_,member_) );
}


//-------------------------------------------------------------------------
// make_callback
//
//-------------------------------------------------------------------------
template <class P1, class Function>
CxCallback<P1>
make_callback( CxCallback<P1> *, Function function_ )
{
    return CxCallback<P1>( new CxFunctionCallbackBody<P1,Function>(function_) );
}


#endif
