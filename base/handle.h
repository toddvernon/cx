/*************************************************************************** 
 *
 *  handle.h
 *
 *  CxCounted, CxCountedPtr, CxHandle Class
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


#include <stdio.h>

//#include <cx/base/cntbody.h>


#ifndef _CxHandle_h_
#define _CxHandle_h_


//-------------------------------------------------------------------------
// class CxCounted
//
//------------------------------------------------------------------------
class CxCounted
{
  public:
	virtual ~CxCounted( )
	{ }

	unsigned Add( )
	{ 
		unsigned int retval;
		retval = ++_iCount;
		return( retval ); 
	}
	unsigned Remove( )
	{ 
		unsigned int retval;
		retval = --_iCount;
		return( retval ); 
	}

	void Reset()
	{ _iCount = 0;  }


  protected:
	CxCounted( ) : _iCount(0)
	{ }  

  private:
	unsigned  _iCount;
};



//-------------------------------------------------------------------------
// class CxCountedPtr
//
//------------------------------------------------------------------------
class CxCountedPtr
: public CxCounted
{
  public:

	CxCountedPtr( ): _pData(NULL) 
	{} 	// Null initialization

	CxCountedPtr( void* p ): _pData(p)
	{}

	void* _pData;
};



//-------------------------------------------------------------------------
// class CxHandle<T>
//
//------------------------------------------------------------------------
template< class T >
class CxHandle
{
  public:
	CxHandle( )
	{ Bind (new CxCountedPtr(NULL));} 	// Null initialization

	CxHandle( T* pObject )
	{ Bind (new CxCountedPtr (pObject)); }

	CxHandle (const CxHandle<T>& copy)
	{ Bind (copy._pCount); }

	virtual ~CxHandle( )
	{ Unbind( ); }

	CxHandle<T>& operator=( const CxHandle<T>& copy )
	{ if (_pCount != copy._pCount) { Unbind(); Bind (copy._pCount); } return *this;}

	T& operator*( ) const
	{ NullCheck(); return *_pObject; }

	operator T*( ) const
	{ return _pObject; }

	T* operator->( ) const
	{ NullCheck(); return _pObject; }

	T* operator&( ) const
	{ NullCheck(); return _pObject; }

	int IsNull() const
	{ return (_pObject==NULL); }

   private:

	void Bind( CxCountedPtr* c )
	{ 
		_pCount = c;
		_pCount->Add();
		_pObject = (T*)_pCount->_pData;
	}

	void Unbind( )
	{
		if (_pCount->Remove() == 0 )
		{
			delete _pObject;
			delete _pCount;
		}
	}

	void NullCheck() const
	{ if (IsNull()) throw("CxHandle::NullCheck():Invalid Pointer Reference"); }

  private:
	T*            _pObject;
	CxCountedPtr* _pCount;
};


#endif
