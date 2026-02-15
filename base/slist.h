//-------------------------------------------------------------------------------------------------
//
//  slist.h
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  CxSList Class
//
//-------------------------------------------------------------------------------------------------

#include <stdio.h>

#include <cx/base/exception.h>

#ifndef _CxSList_h_
#define _CxSList_h_

#define MYEXPORT 
//#include "cxbaseexport.h"

template <class T>
class MYEXPORT CxListNode
{
public:
    T data;
    CxListNode<T> *next;
    
   
    
    // must do operator <=
};

//class CxSListIterator<T>
template <class T>
class MYEXPORT CxSListIterator
{
public:
    CxSListIterator(CxListNode<T>* n) : m_currentNode(n) { }

    CxSListIterator(const CxSListIterator<T>& i)
        : m_currentNode(i.m_currentNode) { }

    CxSListIterator<T>& operator=(const CxSListIterator<T>& i);

    T& operator*( ) const
    { return m_currentNode->data; }

    CxSListIterator<T>& operator++();   // pre-fix increment

    CxSListIterator<T> operator++(int); // post-fix increment

    int operator==(const CxSListIterator<T>& i)
    {
        return (i.m_currentNode == m_currentNode);
    }

    int operator!=(const CxSListIterator<T>& i)
    {
        return (i.m_currentNode != m_currentNode);
    }

    CxListNode<T>* getCurrentNode() const 
        { return m_currentNode; }


private:
    CxListNode<T>* m_currentNode;
};



//-------------------------------------------------------------------------
// class CxSList
//
//-------------------------------------------------------------------------
template <class T>
class MYEXPORT CxSList
{
public:

	CxSList( void );
	// constructor

	CxSList( const CxSList<T>& slist_ );
	// copy constructor

	~CxSList( void );
	// destructor

	CxSList<T>& operator=( const CxSList<T>& slist_ );
	// assignment operator

    void insertAtHead( const T& item );
	// insert at head
    
    void insertAfter( int index, const T& newItem );

	void append( const T& item );
	// add item to the list

	void append( const CxSList<T>& slist_ );
	// add a list to the list

	void push( const T& item );
	// push an item onto the end of the list

	T pop( void );
	// remove the last item from the list

	T peek( void ) const;
	// get a copy of the last item on the list

	void replaceAt( int i, const T& item );
	// replace the item at index i

	void removeAt( int i );
	// remove the item at item i

	void clear( void );
	// clear the items from the list

	void clearAndDelete( void );
	// clear the items from the list and delete them

	size_t entries( void ) const;
	// return the number of items in the list
	
	T at( int i ) const;
	// return a copy of the item at index i
	
	T* objectAt( int i );

	T first( void );
	// return a copy of first item on the list

	T last( void );
	// return a copy of last item on the list

    void swap( int index1, int index2);
    // swap the to items in the list
    
    int partition( int low, int high);
    void quickSort( void );
    void quickSort( int low, int high);
    int compare( T left, T right);
    
    
    // The following methods provide compatibility with STL.
    
    CxSListIterator<T> begin() { return _head; }
    CxSListIterator<T> end() { return _tail->next; }
    void push_back( const T& item ) { append(item); }
    CxSListIterator<T> erase(CxSListIterator<T> pos);
    size_t size() const { return _entries; }
    int empty() const { return _entries == 0; }

protected:

	size_t  _entries;
	CxListNode<T>   *_head;
	CxListNode<T>   *_tail;
	CxListNode<T>   *_work;
	CxListNode<T>   _pointerToHead;

	void deepCopy( const CxSList<T>& slist_ );
	// copy the items in the list

	void setNull( void );
	// set the list to no items
};



//-------------------------------------------------------------------------
// CxSList<T>::
//
//-------------------------------------------------------------------------
template <class T>
CxSList<T>::CxSList( void  )
{
	setNull();
}

//-------------------------------------------------------------------------
// CxSList<T>::
//
//-------------------------------------------------------------------------
template <class T>
CxSList<T>::CxSList( const CxSList<T>& slist_ )
{
	setNull();
	deepCopy( slist_ );
}

//-------------------------------------------------------------------------
// CxSList<T>::operator=
//
//-------------------------------------------------------------------------
template< class T >
CxSList<T>& 
CxSList<T>::operator=( const CxSList<T>& slist_ )
{
	if ( &slist_ != this ) {
		clear();
		setNull();
		deepCopy( slist_ );
	}
	return( *this );
}

//-------------------------------------------------------------------------
// CxSList<T>::~CxSList<T>
//
//-------------------------------------------------------------------------
template <class T>
CxSList<T>::~CxSList( void )
{
	clear();
}


//-------------------------------------------------------------------------
// CxSList<T>::entries
//
//-------------------------------------------------------------------------
template <class T>
size_t
CxSList<T>::entries(void) const
{
	return(_entries);
}


//-------------------------------------------------------------------------
// CxSList<T>::setNull
//
//-------------------------------------------------------------------------
template <class T>
void
CxSList<T>::setNull( void )
{
	_head = NULL;
	_tail = NULL;
	_work = NULL;
	_entries = 0;
}

//-------------------------------------------------------------------------
// CxSList<T>::deepCopy
//
//-------------------------------------------------------------------------
template <class T>
void
CxSList<T>::deepCopy( const CxSList<T>& slist_ )
{
	if (slist_.entries() == 0) return;
	CxListNode<T> *n = slist_._head;

	do {
		append( n->data );
		n = n->next;
	} while (n != NULL );

	_work = _head;
}

//-------------------------------------------------------------------------
// CxSList<T>::replaceAt
//
//-------------------------------------------------------------------------
template <class T>
void
CxSList<T>::replaceAt( int i, const T& item )
{
	int count = 0;

	CxListNode<T> *n = _head;

	while (n != NULL) {

		if (i == count) {
			n->data = item;
			return;
		}

		n = n->next;
		count++;
	}

	throw CxException("CxSList::at(invalid index)");
}

//-------------------------------------------------------------------------
// CxSList<T>::erase
//
//-------------------------------------------------------------------------
template <class T>
CxSListIterator<T>
CxSList<T>::erase( CxSListIterator<T> i )
{
    CxListNode<T>* pNode = i.getCurrentNode();

    if (pNode == NULL) {
        return i;
    }

    CxListNode<T> *prev     = NULL;
    CxListNode<T> *n        = _head;
    CxListNode<T> *nextNode = NULL;

    while (n != NULL) {
        // If we have found the input node, fix up the
        // pointers and delete the node.

        nextNode = n->next;

        if (n == pNode) {
            if (pNode == _head) {
                _head = nextNode;
                _pointerToHead.next = _head;
                prev  = &_pointerToHead;
            }
            else {
                prev->next = nextNode;
            }

            // If the input node was the tail, make the
            // previous node the tail.

            if (pNode == _tail) {
                _tail = prev;
            }

            delete n;
            _entries--;

            break;
        }

        // Increment to the next node.
        prev = n;
        n = nextNode;
    }

    return prev;
}

//-------------------------------------------------------------------------
// CxSList<T>::removeAt
//
//-------------------------------------------------------------------------
template <class T>
void
CxSList<T>::removeAt( int i )
{
	int count = 0;

	CxListNode<T> *prev    = NULL;
	CxListNode<T> *n       = _head;

	while (n != NULL) {

		if (i == count) {

			// case where there is only one item
			if (( n == _head ) && ( n == _tail )) {
				_head = _tail = _work = NULL;
				delete n;
			} else

			// case where first item is deleted.
			if ( n == _head ) {
				_head = n->next;
				delete n;
			} else

			// case where last item is deleted.
			if ( n == _tail ) {
				prev->next = NULL;
				_tail = prev;
				delete n;
			} else

			// otherwise in the middle
			{
				prev->next = n->next;
				delete n;
			}

			_entries--;
			return;
		}
	
		prev = n;
		n    = n->next;
	
		count++;
	}

	throw CxException("CxSList::at(invalid index)");
}

//-------------------------------------------------------------------------
// CxSList<T>::push
//
//-------------------------------------------------------------------------
template <class T>
void
CxSList<T>::push( const T& item )
{
	append( item );
}


//-------------------------------------------------------------------------
// CxSList<T>::pop
//
//-------------------------------------------------------------------------
template <class T>
T
CxSList<T>::pop( void )
{
	int count = entries();

	if (count==0) {
		throw CxException("CxSList::at(invalid index)");
	}

	T data = at( count-1 );

	removeAt( count-1 );

	return( data );
};

//-------------------------------------------------------------------------
// CxSList<T>::peek
//
//-------------------------------------------------------------------------
template <class T>
T
CxSList<T>::peek( void ) const
{
	int count = entries();

	if (count==0) {
		throw CxException("CxSList::at(invalid index)");
	}

	T data = at( count-1 );

	return( data );
}

template <class T>
void
CxSList<T>::insertAtHead( const T& item )
{
	CxListNode<T> *n = new CxListNode<T>;

	if (n == NULL) {
		throw CxException("CxSList::insertAtHead(memory allocation error)");
	}

	n->data = item;
	n->next = NULL;

	CxListNode<T> *prevHead = _head;
	
	if (prevHead == NULL) {
		_head = n;
		_tail = n;
		n->next = NULL;
	} else {
		_head = n;
		n->next = prevHead;
	}

	_entries++;
    
}

template <class T>
void
CxSList<T>::insertAfter( int index, const T& newItem )
{
    CxListNode<T> *n       = _head;
    CxListNode<T> *newNode = new CxListNode<T>;
    
    newNode->data = newItem;
    newNode->next = NULL;
        
    int count = 0;
    
    // walk the list to find the index
    while (n != NULL) {
 
        // found the index
        if (index == count) {
        
            // make the new mode next pointer what the current nodes was
            newNode->next = n->next;
            
            // nake the current nodes next pointer then point at the new node
            n->next = newNode;
            
            // one more item
            _entries++;
            
            return;
            
        }
        count++;
        n = n->next;
    }
}
    
    
template <class T>
void
CxSList<T>::swap( int index1, int index2)
{
    if (index1 == index2) return;
    
     CxListNode<T> *n = _head;
     CxListNode<T> *node1 = NULL;
     CxListNode<T> *node2 = NULL;
    
    int count = 0;
    while (n != NULL) {

        if (index1 == count) {
            node1 = n;
        }
        if (index2==count) {
            node2 = n;
        }
        n = n->next;
        count++;
    }
 
    if ((node1 == NULL) || (node2 ==NULL)) {
        throw CxException("CxSList::at(invalid index)");
    }
    
    T tempData  = node1->data;
    node1->data = node2->data;
    node2->data = tempData;

}


template <class T>
int
CxSList<T>::partition( int low, int high)
{
    T pivot = at(high);
    int i = (low -1);
    
    for (int j=low; j<=high-1; j++) {
        
        T left = at(j);

        if (compare( left, pivot) <= 0) {
            i++;
            swap(j,i);
        }
    }
    
    swap(i+1, high);
    return(i+1);
}

template <class T>
int
CxSList<T>::compare( T leftItem, T rightItem  )
{
    int result = leftItem.compare( rightItem );
    return( result );
}

template <class T>
void
CxSList<T>::quickSort( void )
{
    int low  = 0;
    int high = entries() -1;

    quickSort( low, high);
}

template <class T>
void
CxSList<T>::quickSort( int low, int high)
{
    if (low<high) {
        
        int pi = partition(low, high);
        
        quickSort( low, pi-1);
        quickSort( pi+1, high);
    }
}


//-------------------------------------------------------------------------
// CxSList<T>::append
//
//-------------------------------------------------------------------------
template <class T>
void
CxSList<T>::append( const T& item )
{
	CxListNode<T> *n = new CxListNode<T>;

	if (n == NULL) {
		throw CxException("CxSList::append(memory allocation error)");
	}

	n->data = item;
	n->next = NULL;

    // if no items in the list
	if (_tail == NULL) {

		_head = n;
		_tail = n;
		_work = n;

    // else just slide in at the end of the list
	} else {

		_tail->next = n;
		_tail = n;
	}

	_entries++;
}

//-------------------------------------------------------------------------
// CxSList<T>::append
//
//-------------------------------------------------------------------------
template <class T>
void
CxSList<T>::append( const CxSList<T>& slist_ )
{
	const CxListNode<T> *n = slist_._head;

	while (n != NULL) {
		append( n->data );
		n = n->next;
	}
}

//-------------------------------------------------------------------------
// CxSList<T>::clear
//
//-------------------------------------------------------------------------
template <class T>
void
CxSList<T>::clear( void )
{
	_work = _head;

	while (_work != NULL ) {

		_head = _work->next;
		delete _work;
		_work = _head;
	}

	setNull();
}



//-------------------------------------------------------------------------
// CxSList<T>::clearAndDelete
//
//-------------------------------------------------------------------------
template <class T>
void
CxSList<T>::clearAndDelete( void )
{
	_work = _head;

	while (_work != NULL ) {

		_head = _work->next;
		
		delete _work->data;
		delete _work;

		_work = _head;
	}

	setNull();
}

//-------------------------------------------------------------------------
// CxSList<T>::at
//
//-------------------------------------------------------------------------
template <class T>
T
CxSList<T>::at( int i ) const
{
	int count = 0;

	const CxListNode<T> *n = _head;

	while (n != NULL) {

		if (i == count) {
			return( n->data );
		}

		n = n->next;
		count++;
	}

	throw CxException("CxSList::at(invalid index)");
}	


template <class T>
T*
CxSList<T>::objectAt( int i) 
{
	int count = 0;

	const CxListNode<T> *n = _head;

	while (n != NULL) {

		if (i == count) {
			return( &n->data );
		}

		n = n->next;
		count++;
	}

	throw CxException("CxSList::at(invalid index)");

}


//-------------------------------------------------------------------------
// CxSList<T>::first
//
//-------------------------------------------------------------------------
template <class T>
T
CxSList<T>::first( void )
{
	int count = entries();

	if (count==0) {
		throw CxException("CxSList::at(invalid index)");
	}

	T item = at( 0 );
	
	removeAt(0);

	return( item );
}

//-------------------------------------------------------------------------
// CxSList<T>::last
//
//-------------------------------------------------------------------------
template <class T>
T
CxSList<T>::last( void )
{
	int count = entries();

	if (count==0) {
		throw CxException("CxSList::at(invalid index)");
	}

	T item = at( count-1 );

	removeAt(count-1);

	return( item );
}


template <class T>
CxSListIterator<T>&
CxSListIterator<T>::operator=(const CxSListIterator<T>& i)
{
    if (&i != this) {
        m_currentNode = i.m_currentNode;
    }
    return *this;
}

template <class T>
CxSListIterator<T>&
CxSListIterator<T>::operator++()
{
    if (m_currentNode != NULL) {
        m_currentNode = m_currentNode->next;
    }
    return *this;
}

template <class T>
CxSListIterator<T> 
CxSListIterator<T>::operator++(int)
{
    CxSListIterator<T> temp(this);

    if (m_currentNode != NULL) {
        m_currentNode = m_currentNode->next;

    }
    return temp;
}

#endif
