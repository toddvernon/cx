/***************************************************************************
 *
 *  b64.cpp
 *
 *  CxB64Encoder, CxB64Decoder Classes
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
 **************************************************************************/


#include <cx/b64/b64.h>


//-------------------------------------------------------------------------
// CxB64Encoder::CxB64Encoder
//
//-------------------------------------------------------------------------
CxB64Encoder::CxB64Encoder( void )
{
	initialize();
	_inBuffer  = new CxRingBuffer( 65536 );
	_outBuffer = new CxRingBuffer( 87382 );  // ceil(65536/3)*4 + margin
}

//-------------------------------------------------------------------------
// CxB64Encoder::~CxB64Encoder
//
//-------------------------------------------------------------------------
CxB64Encoder::~CxB64Encoder( void )
{
	delete _inBuffer;
	delete _outBuffer;
}

//-------------------------------------------------------------------------
// CxB64Encoder::initialize
//
//-------------------------------------------------------------------------
void
CxB64Encoder::initialize( void )
{     
	int i;
    for (i = 0; i < 26; i++) {
        _dtable[i] = 'A' + i;
        _dtable[26 + i] = 'a' + i;
    }
    for (i = 0; i < 10; i++) {
        _dtable[52 + i] = '0' + i;
    }
    _dtable[62] = '+';   
    _dtable[63] = '/';
}

//-------------------------------------------------------------------------
// CxB64Encoder::process
//
//-------------------------------------------------------------------------
void
CxB64Encoder::process( void *data_, unsigned int len_, CxSList< CxString>& lineList )
{
    
	unsigned char *byteData = (unsigned char *) data_;
	unsigned long bytesWritten     = 0;
	unsigned long bytesLeft        = len_;
	unsigned long processBytesRead = 0;
	unsigned long num;

	//---------------------------------------------------------------------
	// while all the buffer has not been input
	//
	//---------------------------------------------------------------------
	while ( bytesLeft ) {

		//-----------------------------------------------------------------
		// Write data into the buffer
		//
		//-----------------------------------------------------------------
		num          = _inBuffer->write( &(byteData[bytesWritten]), bytesLeft );
		bytesWritten = bytesWritten + num;
		bytesLeft    = bytesLeft    - num;
		
		//-----------------------------------------------------------------
		// process all bytes in the input buffer
		//
		//-----------------------------------------------------------------
		process_input( 0 );

		//-----------------------------------------------------------------
		// process all bytes in the output buffer
		//
		//-----------------------------------------------------------------
		process_output( lineList, 0 );

	}
}

//-------------------------------------------------------------------------
// CxB64Encoder::process_input
//
//
//-------------------------------------------------------------------------
void
CxB64Encoder::process_input( int finalize_ )
{
	unsigned long processBytesRead;
	unsigned char o_data[4];
	unsigned char processData[3];

	//---------------------------------------------------------------------
	// process all bytes in the buffer 3 at a time
	//
	//---------------------------------------------------------------------
	processBytesRead = _inBuffer->read( processData, 3 );
	while ( processBytesRead == 3 ) {

		//-----------------------------------------------------------------
		// enocode the data
		//
		//-----------------------------------------------------------------
		encode_buffer( processData, o_data);

		//-----------------------------------------------------------------
		// store the output in another buffer
		//
		//-----------------------------------------------------------------
		if (_outBuffer->write( o_data, 4 )!=4) {
			std::cout << "ERROR" << std::endl;
		}

		//-----------------------------------------------------------------
		// more data
		//
		//-----------------------------------------------------------------
		processBytesRead = _inBuffer->read( processData, 3 );
	}

	//---------------------------------------------------------------------
	// left over data
	//
	//---------------------------------------------------------------------
	if ((processBytesRead<3) && (processBytesRead>0)) {

		//-----------------------------------------------------------------
		// is all the input done ?
		//
		//-----------------------------------------------------------------
		if (finalize_) {

			// Zero-fill unused bytes for correct RFC 4648 encoding
			if ( processBytesRead < 3 ) processData[2] = 0;
			if ( processBytesRead < 2 ) processData[1] = 0;

			encode_buffer( processData, o_data);

			if ( processBytesRead < 3) o_data[3]='=';
			if ( processBytesRead < 2) o_data[2]='=';

			if (_outBuffer->write( o_data, 4 ) != 4) {
				std::cout << "ERROR" << std::endl;
			}
	

		} else {

			if (_inBuffer->write( processData, processBytesRead )!=processBytesRead) {
				std::cout << "ERROR" << std::endl;
			}
		}
	}
}



//-------------------------------------------------------------------------
// CxB64Encoder::process_output
//
//
//-------------------------------------------------------------------------
void
CxB64Encoder::process_output( CxSList< CxString > &lineList, int finalize_ )
{
	unsigned long lineLength;
	char line_data[80];

	lineLength = _outBuffer->read( line_data, 72 );
	line_data[lineLength]=(char) 0;

	while ( lineLength == 72 ) {

		CxString line( line_data );
		lineList.append( line );

		lineLength = _outBuffer->read( line_data, 72 );
		line_data[lineLength] = (char) 0;
	}

	if ((lineLength<72) && (lineLength>0)) {
	
		if (finalize_) {

			CxString line( line_data );
			lineList.append( line );
				
		} else {

			if (_outBuffer->write( line_data, lineLength )!=lineLength) {
				std::cout << "ERROR" << std::endl;
			}
		}
	}
}


//-------------------------------------------------------------------------
// CxB64Encoder::encode_buffer
//
//
//-------------------------------------------------------------------------
void
CxB64Encoder::encode_buffer( unsigned char *in, unsigned char *out )
{
    out[0] = _dtable[ in[0] >> 2 ];
    out[1] = _dtable[ ((in[0] & 3)   << 4) | (in[1] >> 4) ];
    out[2] = _dtable[ ((in[1] & 0xF) << 2) | (in[2] >> 6) ];
    out[3] = _dtable[ in[2] & 0x3F ];
}




//-------------------------------------------------------------------------
// CxB64Encoder::finalize
//
//
//-------------------------------------------------------------------------
void
CxB64Encoder::finalize( CxSList< CxString >& lineList )
{
	process_input( 1 );
	process_output( lineList, 1 );
}



//-------------------------------------------------------------------------
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------




//-------------------------------------------------------------------------
// CxB64Decoder::CxB64Decoder
//
//-------------------------------------------------------------------------
CxB64Decoder::CxB64Decoder( void )
{
	initialize();
	_inBuffer  = new CxRingBuffer( 65536 );
}


//-------------------------------------------------------------------------
// CxB64Decoder::~CxB64Decoder
//
//-------------------------------------------------------------------------
CxB64Decoder::~CxB64Decoder( void )
{
	delete _inBuffer;
}


//-------------------------------------------------------------------------
// CxB64Decoder::initialize
//
//-------------------------------------------------------------------------
void
CxB64Decoder::initialize( void )
{   
    int i;
    
    for (i = 0; i < 255; i++) {
    	_dtable[i] = 0x80;  
    }
    for (i = 'A'; i <= 'Z'; i++) {
        _dtable[i] = 0 + (i - 'A');
    }
    for (i = 'a'; i <= 'z'; i++) {
        _dtable[i] = 26 + (i - 'a');
    }
    for (i = '0'; i <= '9'; i++) {
        _dtable[i] = 52 + (i - '0');
    }
    _dtable['+'] = 62;
    _dtable['/'] = 63;
    _dtable['='] = 0;
 }


//-------------------------------------------------------------------------
// CxB64Decoder::process
//
//-------------------------------------------------------------------------
CxBuffer
CxB64Decoder::process( CxSList< CxString >&  lines )
{
	unsigned int c;
	unsigned int totalLength = 0;

	//---------------------------------------------------------------------
	// get an idea the size of output buffer we need
	//
	//---------------------------------------------------------------------
	for (c=0; c<lines.entries(); c++) {
		totalLength += lines.at(c).length();
	}

	//---------------------------------------------------------------------
	// make a buffer to hold the output
	// 
	//---------------------------------------------------------------------
	_outputData = new unsigned char[ totalLength ];
	_outputPtr  = _outputData;
	_outputLen  = 0;

	//---------------------------------------------------------------------
	// now go through each line and process
	// 
	//---------------------------------------------------------------------
	for (c=0; c<lines.entries(); c++) {


		//-----------------------------------------------------------------
		// strip off invalid characters
		//
		//-----------------------------------------------------------------
		CxString line = CxString::netNormalize( lines.at(c) );

		unsigned char *byteData         = (unsigned char *) line.data();
		unsigned long  bytesWritten     = 0;
		unsigned long  bytesLeft        = line.length();
		unsigned long  processBytesRead = 0;
		unsigned long  num;


		//-----------------------------------------------------------------
		// while all the buffer has not been input
		//
		//-----------------------------------------------------------------
		while ( bytesLeft ) {


			//-------------------------------------------------------------
			// Write data into the buffer
			//
			//-------------------------------------------------------------
			num          = _inBuffer->write( &(byteData[bytesWritten]), bytesLeft );
			bytesWritten = bytesWritten + num;
			bytesLeft    = bytesLeft    - num;


			//-------------------------------------------------------------
			// process the input
			//
			//-------------------------------------------------------------
			process_input( 0 );

		} // process line
		
	} // process line list


	//---------------------------------------------------------------------
	// build a return buffer
	// 
	//---------------------------------------------------------------------
	CxBuffer retBuffer( _outputData, _outputLen );

	delete _outputData;

	return( retBuffer );

}


//-------------------------------------------------------------------------
// CxB64Decoder::process_input
//
//
//-------------------------------------------------------------------------
void
CxB64Decoder::process_input( int finalize_ )
{
	unsigned long processBytesRead;
	unsigned char o_data[3];
	unsigned char processData[4];

	//---------------------------------------------------------------------
	// process all bytes in the buffer 4 at a time
	//
	//---------------------------------------------------------------------
	processBytesRead = _inBuffer->read( processData, 4 );

	while ( processBytesRead == 4 ) {

		//-----------------------------------------------------------------
		// encode the data
		//
		//-----------------------------------------------------------------
		decode_buffer( processData, o_data);

		if ( processData[2] == '=' ) {
				
			*_outputPtr = o_data[0]; _outputPtr++; _outputLen++;

		} else
	
		if ( processData[3] == '=' ) {

			*_outputPtr = o_data[0]; _outputPtr++; _outputLen++;
			*_outputPtr = o_data[1]; _outputPtr++; _outputLen++;
			
		} else

		{ 
			*_outputPtr = o_data[0]; _outputPtr++; _outputLen++;
			*_outputPtr = o_data[1]; _outputPtr++; _outputLen++;
			*_outputPtr = o_data[2]; _outputPtr++; _outputLen++;
		}

		//-----------------------------------------------------------------
		// store the output in the output buffer
		//
		//-----------------------------------------------------------------
		//*_outputPtr = o_data[0]; _outputPtr++; _outputLen++;
		//*_outputPtr = o_data[1]; _outputPtr++; _outputLen++;
		//*_outputPtr = o_data[2]; _outputPtr++; _outputLen++;

		//-----------------------------------------------------------------
		// more data
		//
		//-----------------------------------------------------------------
		processBytesRead = _inBuffer->read( processData, 4 );

	}

	//---------------------------------------------------------------------
	// left over data
	//
	//---------------------------------------------------------------------
	if ((processBytesRead<4) && (processBytesRead>0)) {
	

		//-----------------------------------------------------------------
		// is all the input done ?
		//
		//-----------------------------------------------------------------	
		if (finalize_) {
		
			std::cout << "ERROR !!!!  Nnot enough data" << std::endl;


		} else {

			if (_inBuffer->write( processData, processBytesRead )!=processBytesRead) {
				std::cout << "ERROR:DECODER" << std::endl;
			}
		}
	}

}


//-------------------------------------------------------------------------
// CxB64Decoder::decode_buffer
//
//
//-------------------------------------------------------------------------
void
CxB64Decoder::decode_buffer( unsigned char *in, unsigned char *out )
{
	unsigned char b[4];

	b[0] = _dtable[ *in ]; in++;
	b[1] = _dtable[ *in ]; in++;
	b[2] = _dtable[ *in ]; in++;
	b[3] = _dtable[ *in ]; in++;

    out[0] = (b[0] << 2) | (b[1] >> 4);
    out[1] = (b[1] << 4) | (b[2] >> 2);
    out[2] = (b[2] << 6) |  b[3];
}



//-------------------------------------------------------------------------
// CxB64Decoder::finalize
//
//-------------------------------------------------------------------------
CxBuffer
CxB64Decoder::finalize( void )
{
	unsigned long totalLength = _inBuffer->size( );

	_outputData = new unsigned char[ totalLength ];
	_outputPtr  = _outputData;
	_outputLen  = 0;

	process_input( 1 );

	CxBuffer retBuffer( _outputData, _outputLen );

	delete _outputData;

	return( retBuffer );
}
