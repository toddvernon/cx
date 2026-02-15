//-------------------------------------------------------------------------------------------------
//
//  freefunctor.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  freefunctor.h
//
//-------------------------------------------------------------------------------------------------

#ifndef FreeFunctor_h
#define FreeFunctor_h 1

#include <cx/functor/functor.h>


/***************************************************************************/

template <typename Fun>
class CxFreeFunctor0 : public CxFunctor
{

  public:

      CxFreeFunctor0 (const Fun &aFun);

      virtual void operator () ();


  private:

      Fun _aFun;
};

/***************************************************************************/

template <typename Fun, typename Parm1>
class CxFreeFunctor1 : public CxFunctor
{

  public:

      CxFreeFunctor1 (const Fun &aFun, Parm1 aParm1);

      virtual void operator () ();

  private:

      Fun   _aFun;
      Parm1 _aParm1;
};

/***************************************************************************/

template <typename Fun, typename Parm1, typename Parm2>
class CxFreeFunctor2 : public CxFunctor
{
  public:

      CxFreeFunctor2 (const Fun &aFun, Parm1 aParm1, Parm2 aParm2);

      virtual void operator () ();

  private:

      Fun   _aFun;
      Parm1 _aParm1;
      Parm2 _aParm2;
};

/***************************************************************************/

template <typename Fun, typename Parm1, typename Parm2, typename Parm3>
class CxFreeFunctor3 : public CxFunctor
{
  public:

      CxFreeFunctor3 (const Fun &aFun, Parm1 aParm1, Parm2 aParm2, Parm3 aParm3);

      virtual void operator () ();

  private:

      Fun   _aFun;
      Parm1 _aParm1;
      Parm2 _aParm2;
      Parm3 _aParm3;
};

/***************************************************************************/

template <typename Fun, typename Parm1, typename Parm2, typename Parm3, typename Parm4>
class CxFreeFunctor4 : public CxFunctor
{

  public:

      CxFreeFunctor4 (const Fun &aFun, Parm1 aParm1, Parm2 aParm2, Parm3 aParm3, Parm4 aParm4);

      virtual void operator () ();

  private:

      Fun   _aFun;
      Parm1 _aParm1;
      Parm2 _aParm2;
      Parm3 _aParm3;
      Parm4 _aParm4;
};

/***************************************************************************/

template <typename Fun, typename Parm1, typename Parm2, typename Parm3, typename Parm4, typename Parm5>
class CxFreeFunctor5 : public CxFunctor
{

  public:

      CxFreeFunctor5 (const Fun &aFun, Parm1 aParm1, Parm2 aParm2, Parm3 aParm3, Parm4 aParm4, Parm5 aParm5);

      virtual void operator () ();

  private:

      Fun   _aFun;
      Parm1 _aParm1;
      Parm2 _aParm2;
      Parm3 _aParm3;
      Parm4 _aParm4;
      Parm5 _aParm5;

};

/***************************************************************************/

template <typename Fun, typename Parm1, typename Parm2, typename Parm3, typename Parm4, typename Parm5, typename Parm6>
class CxFreeFunctor6 : public CxFunctor 
{
  public:

      CxFreeFunctor6 (const Fun &aFun, Parm1 aParm1, Parm2 aParm2, Parm3 aParm3, Parm4 aParm4, Parm5 aParm5, Parm6 aParm6);

      virtual void operator () ();

  private:

      Fun   _aFun;
      Parm1 _aParm1;
      Parm2 _aParm2;
      Parm3 _aParm3;
      Parm4 _aParm4;
      Parm5 _aParm5;
      Parm6 _aParm6;
};

/***************************************************************************/

// Parameterized Class CxFreeFunctor0
template <typename Fun>
inline CxFreeFunctor0<Fun>::CxFreeFunctor0 (const Fun &aFun)
  : _aFun( aFun )
{
}

/***************************************************************************/

// Parameterized Class CxFreeFunctor1 
template <typename Fun, typename Parm1>
inline CxFreeFunctor1<Fun,Parm1>::CxFreeFunctor1 (const Fun &aFun, Parm1 aParm1)
  : _aFun( aFun ),
    _aParm1( aParm1 )
{
}

/***************************************************************************/

// Parameterized Class CxFreeFunctor2 
template <typename Fun, typename Parm1, typename Parm2>
inline CxFreeFunctor2<Fun,Parm1,Parm2>::CxFreeFunctor2 (const Fun &aFun, Parm1 aParm1, Parm2 aParm2)
  : _aFun( aFun ),
    _aParm1( aParm1 ),
    _aParm2( aParm2 )
{
}

/***************************************************************************/

// Parameterized Class CxFreeFunctor3 
template <typename Fun, typename Parm1, typename Parm2, typename Parm3>
inline CxFreeFunctor3<Fun,Parm1,Parm2,Parm3>::CxFreeFunctor3 (const Fun &aFun, Parm1 aParm1, Parm2 aParm2, Parm3 aParm3)
  : _aFun( aFun ),
    _aParm1( aParm1 ),
    _aParm2( aParm2 ),
    _aParm3( aParm3 )
{
}

/***************************************************************************/

// Parameterized Class CxFreeFunctor4 
template <typename Fun, typename Parm1, typename Parm2, typename Parm3, typename Parm4>
inline CxFreeFunctor4<Fun,Parm1,Parm2,Parm3,Parm4>::CxFreeFunctor4 (const Fun &aFun, Parm1 aParm1, Parm2 aParm2, Parm3 aParm3, Parm4 aParm4)
  : _aFun( aFun ),
    _aParm1( aParm1 ),
    _aParm2( aParm2 ),
    _aParm3( aParm3 ),
    _aParm4( aParm4 )
{
}

/***************************************************************************/

// Parameterized Class CxFreeFunctor5 
template <typename Fun, typename Parm1, typename Parm2, typename Parm3, typename Parm4, typename Parm5>
inline CxFreeFunctor5<Fun,Parm1,Parm2,Parm3,Parm4,Parm5>::CxFreeFunctor5 (const Fun &aFun, Parm1 aParm1, Parm2 aParm2, Parm3 aParm3, Parm4 aParm4, Parm5 aParm5)
  : _aFun( aFun ),
    _aParm1( aParm1 ),
    _aParm2( aParm2 ),
    _aParm3( aParm3 ),
    _aParm4( aParm4 ),
    _aParm5( aParm5 )
{
}

/***************************************************************************/

// Parameterized Class CxFreeFunctor6 
template <typename Fun, typename Parm1, typename Parm2, typename Parm3, typename Parm4, typename Parm5, typename Parm6>
inline CxFreeFunctor6<Fun,Parm1,Parm2,Parm3,Parm4,Parm5,Parm6>::CxFreeFunctor6 (const Fun &aFun, Parm1 aParm1, Parm2 aParm2, Parm3 aParm3, Parm4 aParm4, Parm5 aParm5, Parm6 aParm6)
  : _aFun( aFun ),
    _aParm1( aParm1 ),
    _aParm2( aParm2 ),
    _aParm3( aParm3 ),
    _aParm4( aParm4 ),
    _aParm5( aParm5 ),
    _aParm6( aParm6 )
{
}

/***************************************************************************/

// Parameterized Class CxFreeFunctor0 
template <typename Fun>
void CxFreeFunctor0<Fun>::operator () ()
{
  _aFun();
}

/***************************************************************************/

// Parameterized Class CxFreeFunctor1 
template <typename Fun, typename Parm1>
void CxFreeFunctor1<Fun,Parm1>::operator () ()
{
  _aFun( _aParm1 );
}

/***************************************************************************/

// Parameterized Class CxFreeFunctor2 
template <typename Fun, typename Parm1, typename Parm2>
void CxFreeFunctor2<Fun,Parm1,Parm2>::operator () ()
{
  _aFun( _aParm1, _aParm2 );
}


/***************************************************************************/

// Parameterized Class CxFreeFunctor3 
template <typename Fun, typename Parm1, typename Parm2, typename Parm3>
void CxFreeFunctor3<Fun,Parm1,Parm2,Parm3>::operator () ()
{
  _aFun( _aParm1, _aParm2, _aParm3 );
}

/***************************************************************************/

// Parameterized Class CxFreeFunctor4 
template <typename Fun, typename Parm1, typename Parm2, typename Parm3, typename Parm4>
void CxFreeFunctor4<Fun,Parm1,Parm2,Parm3,Parm4>::operator () ()
{
  _aFun( _aParm1, _aParm2, _aParm3, _aParm4 );
}

/***************************************************************************/

// Parameterized Class CxFreeFunctor5 
template <typename Fun, typename Parm1, typename Parm2, typename Parm3, typename Parm4, typename Parm5>
void CxFreeFunctor5<Fun,Parm1,Parm2,Parm3,Parm4,Parm5>::operator () ()
{
  _aFun( _aParm1, _aParm2, _aParm3, _aParm4, _aParm5 );
}

/***************************************************************************/

// Parameterized Class CxFreeFunctor6 
template <typename Fun, typename Parm1, typename Parm2, typename Parm3, typename Parm4, typename Parm5, typename Parm6>
void CxFreeFunctor6<Fun,Parm1,Parm2,Parm3,Parm4,Parm5,Parm6>::operator () ()
{
  _aFun( _aParm1, _aParm2, _aParm3, _aParm4, _aParm5, _aParm6 );
}

/***************************************************************************/

#endif
