//-------------------------------------------------------------------------------------------------
//
//  memberfunctor.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  memberfunctor.h
//
//-------------------------------------------------------------------------------------------------

#ifndef MemberFunctor_h
#define MemberFunctor_h 1


#include <cx/functor/functor.h>

/***************************************************************************/

template <typename CalleePtr, typename MemFunPtr>
class CxMemberFunctor0 : public CxFunctor
{
  public:

      CxMemberFunctor0 (const CalleePtr &pCallee, const MemFunPtr &pMemFun);

      virtual void operator () ();

  private:

      CalleePtr _pCallee;
      MemFunPtr _pMemFun;
};

/***************************************************************************/

template <typename CalleePtr, typename MemFunPtr, typename Parm1>
class CxMemberFunctor1 : public CxFunctor
{
  public:
 
      CxMemberFunctor1 (const CalleePtr &pCallee, const MemFunPtr &pMemFun, Parm1 aParm1);

      virtual void operator () ();

  private:

      CalleePtr _pCallee;
      MemFunPtr _pMemFun;
      Parm1     _aParm1;
};

/***************************************************************************/

template <typename CalleePtr, typename MemFunPtr, typename Parm1, typename Parm2>
class CxMemberFunctor2 : public CxFunctor
{

  public:

      CxMemberFunctor2 (const CalleePtr &pCallee, const MemFunPtr &pMemFun, Parm1 aParm1, Parm2 aParm2);

      virtual void operator () ();

  private:

      CalleePtr _pCallee;
      MemFunPtr _pMemFun;
      Parm1     _aParm1;
      Parm2     _aParm2;
};

/***************************************************************************/

template <typename CalleePtr, typename MemFunPtr, typename Parm1, typename Parm2, typename Parm3>
class CxMemberFunctor3 : public CxFunctor
{
  public:

      CxMemberFunctor3 (const CalleePtr &pCallee, const MemFunPtr &pMemFun, Parm1 aParm1, Parm2 aParm2, Parm3 aParm3);

      virtual void operator () ();

  private:

      CalleePtr _pCallee;
      MemFunPtr _pMemFun;
      Parm1     _aParm1;
      Parm2     _aParm2;
      Parm3     _aParm3;
};

/***************************************************************************/

template <typename CalleePtr, typename MemFunPtr, typename Parm1, typename Parm2, typename Parm3, typename Parm4>
class CxMemberFunctor4 : public CxFunctor
{
  public:

      CxMemberFunctor4 (const CalleePtr &pCallee, const MemFunPtr &pMemFun, Parm1 aParm1, Parm2 aParm2, Parm3 aParm3, Parm4 aParm4);

      virtual void operator () ();

  private:

      CalleePtr _pCallee;
      MemFunPtr _pMemFun;
      Parm1     _aParm1;
      Parm2     _aParm2;
      Parm3     _aParm3;
      Parm4     _aParm4;
};

/***************************************************************************/

template <typename CalleePtr, typename MemFunPtr, typename Parm1, typename Parm2, typename Parm3, typename Parm4, typename Parm5>
class CxMemberFunctor5 : public CxFunctor
{
  public:

      CxMemberFunctor5 (const CalleePtr &pCallee, const MemFunPtr &pMemFun, Parm1 aParm1, Parm2 aParm2, Parm3 aParm3, Parm4 aParm4, Parm5 aParm5);

      virtual void operator () ();

  private:

      CalleePtr _pCallee;
      MemFunPtr _pMemFun;
      Parm1     _aParm1;
      Parm2     _aParm2;
      Parm3     _aParm3;
      Parm4     _aParm4;
      Parm5     _aParm5;
};

/***************************************************************************/

template <typename CalleePtr, typename MemFunPtr, typename Parm1, typename Parm2, typename Parm3, typename Parm4, typename Parm5, typename Parm6>
class CxMemberFunctor6 : public CxFunctor
{
  public:

      CxMemberFunctor6 (const CalleePtr &pCallee, const MemFunPtr &pMemFun, Parm1 aParm1, Parm2 aParm2, Parm3 aParm3, Parm4 aParm4, Parm5 aParm5, Parm6 aParm6);

      virtual void operator () ();

  private:

      CalleePtr _pCallee;
      MemFunPtr _pMemFun;
      Parm1     _aParm1;
      Parm2     _aParm2;
      Parm3     _aParm3;
      Parm4     _aParm4;
      Parm5     _aParm5;
      Parm6     _aParm6;
};

/***************************************************************************/

// Parameterized Class CxMemberFunctor0 
template <typename CalleePtr, typename MemFunPtr>
inline CxMemberFunctor0<CalleePtr,MemFunPtr>::CxMemberFunctor0 (const CalleePtr &pCallee, const MemFunPtr &pMemFun)
  : _pCallee( pCallee ),
    _pMemFun( pMemFun )
{
}

/***************************************************************************/

// Parameterized Class CxMemberFunctor1 
template <typename CalleePtr, typename MemFunPtr, typename Parm1>
inline CxMemberFunctor1<CalleePtr,MemFunPtr,Parm1>::CxMemberFunctor1 (const CalleePtr &pCallee, const MemFunPtr &pMemFun, Parm1 aParm1)
  : _pCallee( pCallee ),
    _pMemFun( pMemFun ),
    _aParm1( aParm1 )
{
}

/***************************************************************************/

// Parameterized Class CxMemberFunctor2 
template <typename CalleePtr, typename MemFunPtr, typename Parm1, typename Parm2>
inline CxMemberFunctor2<CalleePtr,MemFunPtr,Parm1,Parm2>::CxMemberFunctor2 (const CalleePtr &pCallee, const MemFunPtr &pMemFun, Parm1 aParm1, Parm2 aParm2)
  : _pCallee( pCallee ),
    _pMemFun( pMemFun ),
    _aParm1( aParm1 ),
    _aParm2( aParm2 )
{
}

/***************************************************************************/

// Parameterized Class CxMemberFunctor3 
template <typename CalleePtr, typename MemFunPtr, typename Parm1, typename Parm2, typename Parm3>
inline CxMemberFunctor3<CalleePtr,MemFunPtr,Parm1,Parm2,Parm3>::CxMemberFunctor3 (const CalleePtr &pCallee, const MemFunPtr &pMemFun, Parm1 aParm1, Parm2 aParm2, Parm3 aParm3)
  : _pCallee( pCallee ),
    _pMemFun( pMemFun ),
    _aParm1( aParm1 ),
    _aParm2( aParm2 ),
    _aParm3( aParm3 )
{
}

/***************************************************************************/

// Parameterized Class CxMemberFunctor4 
template <typename CalleePtr, typename MemFunPtr, typename Parm1, typename Parm2, typename Parm3, typename Parm4>
inline CxMemberFunctor4<CalleePtr,MemFunPtr,Parm1,Parm2,Parm3,Parm4>::CxMemberFunctor4 (const CalleePtr &pCallee, const MemFunPtr &pMemFun, Parm1 aParm1, Parm2 aParm2, Parm3 aParm3, Parm4 aParm4)
  : _pCallee( pCallee ),
    _pMemFun( pMemFun ),
    _aParm1( aParm1 ),
    _aParm2( aParm2 ),
    _aParm3( aParm3 ),
    _aParm4( aParm4 )
{
}

/***************************************************************************/

// Parameterized Class CxMemberFunctor5 
template <typename CalleePtr, typename MemFunPtr, typename Parm1, typename Parm2, typename Parm3, typename Parm4, typename Parm5>
inline CxMemberFunctor5<CalleePtr,MemFunPtr,Parm1,Parm2,Parm3,Parm4,Parm5>::CxMemberFunctor5 (const CalleePtr &pCallee, const MemFunPtr &pMemFun, Parm1 aParm1, Parm2 aParm2, Parm3 aParm3, Parm4 aParm4, Parm5 aParm5)
  : _pCallee( pCallee ),
    _pMemFun( pMemFun ),
    _aParm1( aParm1 ),
    _aParm2( aParm2 ),
    _aParm3( aParm3 ),
    _aParm4( aParm4 ),
    _aParm5( aParm5 )
{
}

/***************************************************************************/

// Parameterized Class CxMemberFunctor6 
template <typename CalleePtr, typename MemFunPtr, typename Parm1, typename Parm2, typename Parm3, typename Parm4, typename Parm5, typename Parm6>
inline CxMemberFunctor6<CalleePtr,MemFunPtr,Parm1,Parm2,Parm3,Parm4,Parm5,Parm6>::CxMemberFunctor6 (const CalleePtr &pCallee, const MemFunPtr &pMemFun, Parm1 aParm1, Parm2 aParm2, Parm3 aParm3, Parm4 aParm4, Parm5 aParm5, Parm6 aParm6)
  : _pCallee( pCallee ),
    _pMemFun( pMemFun ),
    _aParm1( aParm1 ),
    _aParm2( aParm2 ),
    _aParm3( aParm3 ),
    _aParm4( aParm4 ),
    _aParm5( aParm5 ),
    _aParm6( aParm6 )
{
}

/***************************************************************************/

// Parameterized Class CxMemberFunctor0 
template <typename CalleePtr, typename MemFunPtr>
void CxMemberFunctor0<CalleePtr,MemFunPtr>::operator () ()
{
  ( ( *_pCallee ).*_pMemFun )();
}

/***************************************************************************/

// Parameterized Class CxMemberFunctor1 
template <typename CalleePtr, typename MemFunPtr, typename Parm1>
void CxMemberFunctor1<CalleePtr,MemFunPtr,Parm1>::operator () ()
{
  ( ( *_pCallee ).*_pMemFun )( _aParm1 );
}

/***************************************************************************/

// Parameterized Class CxMemberFunctor2 
template <typename CalleePtr, typename MemFunPtr, typename Parm1, typename Parm2>
void CxMemberFunctor2<CalleePtr,MemFunPtr,Parm1,Parm2>::operator () ()
{
  ( ( *_pCallee ).*_pMemFun )( _aParm1, _aParm2 );
}

/***************************************************************************/

// Parameterized Class CxMemberFunctor3 
template <typename CalleePtr, typename MemFunPtr, typename Parm1, typename Parm2, typename Parm3>
void CxMemberFunctor3<CalleePtr,MemFunPtr,Parm1,Parm2,Parm3>::operator () ()
{
  ( ( *_pCallee ).*_pMemFun )( _aParm1, _aParm2, _aParm3 );
}

/***************************************************************************/

// Parameterized Class CxMemberFunctor4 
template <typename CalleePtr, typename MemFunPtr, typename Parm1, typename Parm2, typename Parm3, typename Parm4>
void CxMemberFunctor4<CalleePtr,MemFunPtr,Parm1,Parm2,Parm3,Parm4>::operator () ()
{
  ( ( *_pCallee ).*_pMemFun )( _aParm1, _aParm2, _aParm3, _aParm4 );
}

/***************************************************************************/

// Parameterized Class CxMemberFunctor5 
template <typename CalleePtr, typename MemFunPtr, typename Parm1, typename Parm2, typename Parm3, typename Parm4, typename Parm5>
void CxMemberFunctor5<CalleePtr,MemFunPtr,Parm1,Parm2,Parm3,Parm4,Parm5>::operator () ()
{
  ( ( *_pCallee ).*_pMemFun )( _aParm1, _aParm2, _aParm3, _aParm4, _aParm5 );
}

/***************************************************************************/

// Parameterized Class CxMemberFunctor6 
template <typename CalleePtr, typename MemFunPtr, typename Parm1, typename Parm2, typename Parm3, typename Parm4, typename Parm5, typename Parm6>
void CxMemberFunctor6<CalleePtr,MemFunPtr,Parm1,Parm2,Parm3,Parm4,Parm5,Parm6>::operator () ()
{
  ( ( *_pCallee ).*_pMemFun )( _aParm1, _aParm2, _aParm3, _aParm4, _aParm5, _aParm6 );
}

/***************************************************************************/

#endif
