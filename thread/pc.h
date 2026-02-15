//-------------------------------------------------------------------------------------------------
//
//  pc.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxPCQueue Class
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <iostream>

#include <cx/base/string.h>
#include <cx/base/slist.h>
#include <cx/base/exception.h>
#include <cx/thread/mutex.h>
#include <cx/thread/thread.h>
#include <cx/thread/cond.h>


#ifndef _CxPCQueue_
#define _CxPCQueue_


//-------------------------------------------------------------------------
// CxPCQueue::
//
//-------------------------------------------------------------------------

template <class T> 
class CxPCQueue
{
  public:

	CxPCQueue( size_t qSize );
	// constructor, pass in the max value for
	// for queue length

	~CxPCQueue( void );
	// destructor	

	void enQueue( T item, time_t sec_=0 );
	// add an item to the fifo, if the fifo is full this
	// method will block if 0 is specified for seconds.			
	// throws CxConditionTimeoutException if time expires

	T deQueue( time_t sec_=0 );
	// remove an item from the fifo, if the fifo is empty
	// this item will block if 0 is specified for seconds,
	// throws CxConditionTimeoutException if time expires

  private:

	size_t       _qSize;
	CxMutex      _lock;
	CxCondition  _notFull;
	CxCondition  _notEmpty;
	CxSList< T > _list;
};


//-------------------------------------------------------------------------
// CxPCQueue::CxPCQueue
//
//-------------------------------------------------------------------------
template <class T>
CxPCQueue<T>::CxPCQueue( size_t qSize_ )
:_qSize( qSize_ )
{
}


//-------------------------------------------------------------------------
// CxPCQueue::CxPCQueue
//
//-------------------------------------------------------------------------
template <class T>
CxPCQueue<T>::~CxPCQueue( void )
{
}


//-------------------------------------------------------------------------
// CxPCQueue::enQueue
//
//-------------------------------------------------------------------------
template <class T>
void 
CxPCQueue<T>::enQueue( T s_, time_t sec_ )
{
	_lock.acquire();

	while (_list.entries() == _qSize) {

		if (sec_==0) {
			_notFull.wait( &_lock );
		} else {
			if (_notFull.timedWait( &_lock, sec_)==CxCondition::CONDITION_TIMEOUT) {
				_lock.release();
				throw CxConditionTimeoutException("Timeout waiting on condition");
			}
		}
	}

	_list.append( s_ );

	_lock.release();

	_notEmpty.signal();
}


//-------------------------------------------------------------------------
// CxPCQueue::deQueue
//
//-------------------------------------------------------------------------
template <class T>
T
CxPCQueue<T>::deQueue( time_t sec_ )
{
	_lock.acquire();

	while ( !_list.entries() ) {

		if (sec_==0) {
			_notEmpty.wait( &_lock );
		} else {
			if (_notEmpty.timedWait( &_lock, sec_ )==CxCondition::CONDITION_TIMEOUT) {
				_lock.release();
				throw CxConditionTimeoutException("Timeout waiting on condition");
			}
		}
	}

	T s = _list.first();

	_lock.release();

	_notFull.signal();

	return( s );
}


#endif
