/*************************************************************************** 
 *
 *  functor.h
 *
 *
 *
 ***************************************************************************
 *
 * Copyright:      2001/03/01; Andreas Huber <andreas@huber.net>;
 *                 Zurich; Switzerland
 *
 * License:        This file is part of the downloadable accompanying
 *                 material for the article "Elegant Function Call Wrappers"
 *                 in the May 2001 issue of the C/C++ Users Journal
 *                 (www.cuj.com). The material may be used, copied,
 *                 distributed, modified and sold free of charge, provided
 *                 that this entire notice (copyright, license and
 *                 feedback) appears unaltered in all copies of the
 *                 material.  All material is provided "as is", without
 *                 express or implied warranty.
 *    
 ***************************************************************************/

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
