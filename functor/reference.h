//-------------------------------------------------------------------------------------------------
//
//  reference.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  reference.h
//
//-------------------------------------------------------------------------------------------------

#ifndef Reference_h
#define Reference_h 1

#include <cx/functor/simplereferencer.h>

/***************************************************************************/

template< class Type >
inline const CxSimpleReferencer< Type > CxReference( Type & Object )
{
  return CxSimpleReferencer< Type >( Object );
}



#endif
