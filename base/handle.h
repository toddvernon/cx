//-------------------------------------------------------------------------------------------------
//
//  handle.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxCounted, CxCountedPtr, CxHandle Class
//
//-------------------------------------------------------------------------------------------------

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
