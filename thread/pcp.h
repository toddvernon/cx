//-------------------------------------------------------------------------------------------------
//
//  pcp.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxPriorityQueue Class
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>
#include <iostream.h>

#include <cx/base/string.h>
#include <cx/base/slist.h>
#include <cx/base/exception.h>
#include <cx/thread/mutex.h>
#include <cx/thread/thread.h>
#include <cx/thread/cond.h>


#ifndef _CxPriorityQueue_
#define _CxPriorityQueue_


//-------------------------------------------------------------------------
// CxPriorityQueue::
//
//-------------------------------------------------------------------------

template <class T> 
class CxPriorityQueue
{
  public:

	CxPriorityQueue( size_t qSize );
	// constructor, pass in the max value for
	// for queue length

	~CxPriorityQueue( void );
	// destructor	

	void enQueue( T item, time_t waitSec_=0 );
	// add an item to the queue in the currect location given
	// it priority.	
	// throws CxConditionTimeoutException if time expires

	T deQueue( double minPriority_, double maxPriority_, time_t waitSec_=0 );
	// remove an item or items from the sample queue that occurred between min
	// and max sample time.
	// throws CxConditionTimeoutException if time expires

  private:

	size_t       _qSize;
	CxMutex      _lock;
	CxCondition  _notFull;
	CxCondition  _notEmpty;
	CxSList< T > _list;
};


//-------------------------------------------------------------------------
//
//
//-------------------------------------------------------------------------
template <class T>
CxPriorityQueue<T>::CxPriorityQueue( size_t qSize_ )
:_qSize( qSize_ )
{
}


//-------------------------------------------------------------------------
// CxPriorityQueue::CxPriorityQueue
//
//-------------------------------------------------------------------------
template <class T>
CxPriorityQueue<T>::~CxPriorityQueue( void )
{
}


//-------------------------------------------------------------------------
// CxPriorityQueue::enQueue
//
//-------------------------------------------------------------------------
template <class T>
void 
CxPriorityQueue<T>::enQueue( T s_, time_t sec_ )
{
	// acquire the high level lock
	_lock.acquire();

	// got the lock, now wait in a loop until such time
	// that we can insert the item in the queue
	while (_list.entries() == _qSize) {

		// if our wait time is infinate then wait for the not full signal to
		// come from the deQueue method
		if (sec_==0) {
			_notFull.wait( &_lock );
		} else {
		
			// if we are not waiting forever then wait until we can't wait
			// anymore
			if (_notFull.timedWait( &_lock, sec_)==CxCondition::CONDITION_TIMEOUT) {
				_lock.release();
				throw CxConditionTimeoutException("Timeout waiting on condition");
			}
		}
	}

	// ok insert the message in the message queue according to the correct priority
	int c;

	CxMessage *lastMessage = NULL;
	CxMessage *thisMessage = NULL;

	int nMessages = _list.entries();

	for (c=0; c<nMessages; c++) {

		lastMessage = thisMessage;
		thisMessage = _list.at(c);

		// if this is the first item in the list
		if (c==0) {
			if ( newMessage->sampleTime() < thisMessage->sampleTime() ) {
				_list.insertBefore( c, newMessage );
				break;
			}
		}

		// if this is not the first item
		if ((c>0) && (c<nMessage-1)) {
			// if the message is between this and last
			if ((newMessage->sampleTime() >= lastMessage->sampleTime()) &&
		    	(newMessage->sampleTime() <= thisMessage->sampleTime())) 
				_list.insertBefore( c, newMessage );
			}
		}

		// if this is the last item in the list
		if (c==nMessage-1) {
			
			if (newMessage->sampleTime() >= thisMessage->sampleTime()) {
				_list.insertAfter( c, newMessage );
				break;
			}
		}
	}

	// ok we are done, release the lock.
	_lock.release();

	// we just put one on the queue so signal as not empty
	_notEmpty.signal();
}


//-------------------------------------------------------------------------
// CxPriorityQueue::deQueue
//
//-------------------------------------------------------------------------
template <class T>
T
CxPriorityQueue<T>::deQueue( double minPriority_, double maxPriority_, time_t sec_ )
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
