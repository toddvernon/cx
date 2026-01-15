/***************************************************************************
 *
 *  hashmap.h
 *
 *  CxHashmap Class
 *
 ***************************************************************************
 *
 * (c) Copyright 2000 T.Vernon
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
 **************************************************************************
 *
 *  BASED ON THE CODE CONTAINING THE FOLLOWING COPYRIGHT
 *
 **************************************************************************
 *
 *  This file is part of the xmlparse library.
 *
 *  Copyright (c) 2000 Th. Morus Walter (morus.walter@gmx.de)
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *	Redistributions of source code must retain the above copyright 
 *	notice, this list of conditions and the following disclaimer.
 *
 *	Redistributions in binary form must reproduce the above copyright 
 *	notice, this list of conditions and the following disclaimer in the 
 *	documentation and/or other materials provided with the distribution.
 *
 *	The names of its contributors may not be used to endorse or promote 
 *	products derived from this software without specific prior written
 *  permission. 
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 *  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 *  REGENTS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 *  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 *  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 *  OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 *  TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 *  THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 *  DAMAGE. 
 *
 **************************************************************************/


#ifndef __hashmap_
#define __hashmap_

#include <assert.h>



template<class key, class entry> class CxHashmapIterator;



//-------------------------------------------------------------------------
// CxHashmap::CxHashmap
//
//-------------------------------------------------------------------------
template<class key, class entry> class CxHashmap
{

    friend class CxHashmapIterator<key,entry>;

  private:

   	// static const int init_size = 32;

	//---------------------------------------------------------------------    
	// copy constructor
	//
	//---------------------------------------------------------------------    
    CxHashmap(const CxHashmap<key,entry> &table) {
		assert(0);
    };

	//---------------------------------------------------------------------    
	// operator equal
	//---------------------------------------------------------------------    
    CxHashmap<key,entry> &operator=(const CxHashmap<key,entry> &table) {
		assert(0);
    };


	//---------------------------------------------------------------------    
    // double the size of the hashmap
	//
	//---------------------------------------------------------------------    
    void resize(void) 
	{
		int new_size = hash_size + hash_size;

		const key **newkeys = new const key *[new_size];
		{ 
	    	const key **tmp = newkeys;
	    	for ( int i=0; i<new_size; i++ ) {
				*tmp++=(key *)0;
	    	} 
		}

		entry **newentries = new entry *[new_size];
		{ 
	    	entry **tmp = newentries;
	    	for ( int i=0; i<new_size; i++ ) {
				*tmp++=(entry *)0;
	    	}	 
		}
    
		for ( int i=0; i<hash_size; i++ ) {
	    	if ( keys[i] ) {
				unsigned int h = keys[i]->hashValue() & (new_size-1);
				for ( ; newkeys[h]; h = (h == 0) ? hash_size-1 : h-1 ) {
		    		if ( *keys[i] == *newkeys[h] )
					break;
				}
				newkeys[h]    = keys[i];
				newentries[h] = entries[i];
	    	}
		}

		delete [] keys;
		delete [] entries;
    
		keys      = newkeys;
		entries   = newentries;
		hash_size = new_size;
    };


    // actual size
    int hash_size;

    // used entries
    int hash_used;

    // the keys of the hashmap
    const key **keys;

    // the data entries
    entry **entries;


  public:

	//---------------------------------------------------------------------    
    // create empty hashmap of size `size'; note: size must be 2**n
	//
	//---------------------------------------------------------------------    
    CxHashmap(int ssize = 32) 
	{
	
		keys = new const key *[ssize];
		{
	    	const key **tmp = keys;
	    	for ( int i=0; i<ssize; i++ ) {
				*tmp++=(key *)0;
	    	}
		};

		entries = new entry *[ssize];
		{
	    	entry **tmp = entries;
	    	for ( int i=0; i<ssize; i++ ) {
				*tmp++=(entry *)0;
	    	}
		}

		hash_size = ssize;
		hash_used = 0;
    };


    //---------------------------------------------------------------------    
   	// destroy table, that is delete all keys and entries and the table itself
	//
    //---------------------------------------------------------------------    
    ~CxHashmap(void) 
	{

		for ( int i=0; i<hash_size; i++ ) {
	    	if ( keys[i] ) {
				delete keys[i];
				keys[i] = 0;
				delete entries[i];
				entries[i] = 0;
	    	}
		}

		delete [] keys;
		delete [] entries;
    };


    
    //---------------------------------------------------------------------    
	// access entry; note: the entry is created, if it does not exist 
	//
	//---------------------------------------------------------------------    
	entry &operator[](const key &thekey) 
	{

		if ( hash_size - hash_used < hash_size / 3 + 1 ) { // hash vergroessern
	    	resize();
		}

		unsigned int h = thekey.hashValue() & (hash_size-1);
		
		for ( ; keys[h]; h = (h == 0) ? hash_size-1 : h-1 ) {
	    	if ( thekey == *keys[h] ) break;
		}

		if ( !keys[h] ) {
	    	hash_used++;
	    	keys[h]=new key(thekey);
	    	entries[h]=new (entry);
		}

		return (entry&) *entries[h];
    };


	//---------------------------------------------------------------------    
    // find an entry; note: returns NULL if the entry does not exist
	//
	//---------------------------------------------------------------------    
    const entry *find(const key &thekey) const 
	{
		if ( !hash_used ) return 0;

		unsigned int h = thekey.hashValue() & (hash_size-1);
	
		for ( ;keys[h];h = (h == 0) ? hash_size-1 : h-1 ) {

	    	if ( thekey == *keys[h] ) return (entry*) entries[h];
		}

		return 0;
    };



	//---------------------------------------------------------------------        
	// insert item
	//
	//---------------------------------------------------------------------    
	bool insert(const key &thekey, const entry &theentry) 
	{
		bool ret=false;

		if ( hash_size - hash_used < hash_size / 3 + 1 ) { // hash vergroessern
	    	resize();
		}

		unsigned int h = thekey.hashValue() & (hash_size-1);
	
		for ( ; keys[h]; h = (h == 0) ? hash_size-1 : h-1 ) {
	    	if ( thekey == *keys[h] ) break;
		}

		if ( ! keys[h] ) {
	    	
			keys[h]=new key(thekey);
	    	hash_used++;

		} else {

	    	delete entries[h];
	    	ret=true;
		}

		entries[h]=new (entry)(theentry);
		return ret;
    };

	//---------------------------------------------------------------------    
	// return number of items
	//
	//---------------------------------------------------------------------    
    int getSize(void) const 
	{
		return hash_used;
    }
};



//-------------------------------------------------------------------------
// CxHashmapIterator::CxHashmapIterator
//
//-------------------------------------------------------------------------
template<class key, class entry> class CxHashmapIterator
{

  public:

	//---------------------------------------------------------------------    
	//---------------------------------------------------------------------    
	CxHashmapIterator( CxHashmap<key,entry> *_table) 
	{
		table = _table;
		index = -1;
    }

	//---------------------------------------------------------------------    
	//---------------------------------------------------------------------    
    bool next(void) 
	{
		index++;
		while ( index < table->hash_size && !table->keys[index] ) {
	    	index++;
		}

		return index < table->hash_size;
    };

	//---------------------------------------------------------------------    
	//---------------------------------------------------------------------    
    const key *getKey(void) 
	{
		if ( index == -1 ) return NULL;
		return table->keys[index];
    }

	//---------------------------------------------------------------------    
	//---------------------------------------------------------------------    
    entry *getEntry(void) 
	{
		if ( index == -1 ) return NULL;
		return table->entries[index];
    }


  private:

	//---------------------------------------------------------------------    
	//---------------------------------------------------------------------    
    CxHashmapIterator(void);


    CxHashmap<key,entry> *table;

    int index;
};


#endif
