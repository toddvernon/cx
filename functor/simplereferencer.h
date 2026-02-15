//-------------------------------------------------------------------------------------------------
//
//  simplereferencer.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  simplereferencer.h
//
//-------------------------------------------------------------------------------------------------

#ifndef SimpleReferencer_h
#define SimpleReferencer_h 1


/***************************************************************************/

template <typename Type>
class CxSimpleReferencer 
{
  public:
 
      explicit CxSimpleReferencer (Type &Object);

      operator Type & () const;

  private:

      Type &_ObjectRef;
};

/***************************************************************************/

// Parameterized Class SimpleReferencer 
template <typename Type>
inline CxSimpleReferencer<Type>::CxSimpleReferencer (Type &Object)
  : _ObjectRef( Object )
{
}

/***************************************************************************/

template <typename Type>
inline CxSimpleReferencer<Type>::operator Type & () const
{
  return _ObjectRef;
}

/***************************************************************************/

#endif
