/***************************************************************************
 *
 *  cond.cpp
 *
 *  CxCondition Class
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
 * adapted from:
 *   http://www.ucs.br.so/ch19.htm
 *
 **************************************************************************/


#include <cx/base/string.h>
#include <cx/thread/cond.h>



//-------------------------------------------------------------------------
// CxCondition::CxCondition
//
//-------------------------------------------------------------------------
CxCondition::CxCondition( void )
{

#ifdef _LINUX_
    pthread_cond_init( &_condition, 0 );
#endif

#ifdef WIN32

#endif

}


//-------------------------------------------------------------------------
// CxCondition::~CxCondition
//
//-------------------------------------------------------------------------
CxCondition::~CxCondition( void )  
{
#ifdef _LINUX_ 
    pthread_cond_destroy( &_condition );
#endif

#ifdef WIN32
#endif

}


//-------------------------------------------------------------------------
// CxCondition::acquire
//
//-------------------------------------------------------------------------
void
CxCondition::wait( CxMutex *mutex_ )
{

#ifdef _LINUX_
	pthread_cond_wait( &_condition, mutex_->pthread_mutex() );
#endif

#ifdef WIN32

#endif

}


//-------------------------------------------------------------------------
// CxCondition::timedWait
//
// int pthread_cond_timedwait(
//      pthread_cond_t *cond,
//      pthread_mutex_t *mutex,
//      const struct timespec *abstime);
//
//-------------------------------------------------------------------------
// pthread_cond_timedwait  atomically unlocks mutex and waits on cond, as 
// pthread_cond_wait does, but it also bounds the duration of the wait. 
// If cond has not been signaled within  the  amount of  time  specified  
// by abstime,  the mutex  mutex is re-acquired and pthread_cond_timedwait
// returns the error ETIMEDOUT.  The abstime parameter specifies an 
// absolute time, with the  same origin as time(2) and gettimeofday(2): 
// an abstime of 0 corresponds to 00:00:00 GMT, January 1, 1970.
//
// POSIX.4 structure for a time value.  This is like a `struct timeval' but
// has nanoseconds instead of microseconds.  */
//
// struct timespec
//  {
//    long int tv_sec;        /* Seconds.  */
//    long int tv_nsec;       /* Nanoseconds.  */
//  };
//
//-------------------------------------------------------------------------
CxCondition::waitResult
CxCondition::timedWait( CxMutex *mutex_, time_t sec_ )
{

#ifdef _LINUX_
	time_t t = time(NULL);

	struct timespec ts;
	ts.tv_sec  = t + sec_;
	ts.tv_nsec = 0;

	if (pthread_cond_timedwait( &_condition, mutex_->pthread_mutex(), &ts )==ETIMEDOUT) {
		return( CxCondition::CONDITION_TIMEOUT );
	}
#endif

#ifdef WIN32

#endif

	return( CxCondition::CONDITION_SIGNAL );

}


//-------------------------------------------------------------------------
// CxCondition::release
//
//-------------------------------------------------------------------------
void
CxCondition::signal( void )
{

#ifdef _LINUX_
    pthread_cond_signal( &_condition );
#endif

#ifdef WIN32

#endif

}
