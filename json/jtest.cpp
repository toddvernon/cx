/*
 * Copyright (c) 2013 Yaroslav Stavnichiy <yarosla@gmail.com>
 *
 * This file is part of NXJSON.
 *
 * NXJSON is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * NXJSON is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with NXJSON. If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//#include <malloc.h>
#include <assert.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>


#include <cx/base/string.h>
#include <cx/base/file.h>

#include <cx/json/nxjson.h>
#include <cx/json/json_member.h>
#include <cx/json/json_string.h>
#include <cx/json/json_number.h>
#include <cx/json/json_boolean.h>
#include <cx/json/json_null.h>
#include <cx/json/json_object.h>
#include <cx/json/json_array.h>



#define ERROR(msg, p) fprintf(stderr, "ERROR: " msg " %s\n", (p));



static void dump(const nx_json* json, CxJSONBase *cxjObject ) 
{
	if (!json) {
    	return;
  	}

	switch (json->type) {

    	case NX_JSON_NULL:

			switch( cxjObject->type() ) {
	
				case CxJSONBase::ARRAY:
					{ 
  						CxJSONArray *parentObject = (CxJSONArray *) cxjObject;
						parentObject->append( new CxJSONNull( ) );
					}
					break;

 				case CxJSONBase::OBJECT:
                    {
						CxJSONObject *parentObject = (CxJSONObject *) cxjObject;
						parentObject->append( new CxJSONMember( json->key, new CxJSONNull( ) ));
					}
					break;
                default:
					std::cout << "ERROR" << std::endl;

			}
      		break;

    	case NX_JSON_OBJECT:

      		{
				CxJSONObject *o = new CxJSONObject();

        		nx_json* js=json->child;
        		for (js=json->child; js; js=js->next) 
				{
          			dump( js, o );
        		}

				switch( cxjObject->type() ) {
	
					case CxJSONBase::ARRAY:
						{ 
							CxJSONArray *parentObject = (CxJSONArray *) cxjObject;
							parentObject->append( o );
						}
						break;

	 				case CxJSONBase::OBJECT:
						{
							CxJSONObject *parentObject = (CxJSONObject *) cxjObject;
							parentObject->append( new CxJSONMember( json->key, o ));
						}
						break;

                	default:
						std::cout << "ERROR" << std::endl;
				
				}
				
      		}
      		break;

		case NX_JSON_ARRAY:

      		{
				CxJSONArray *a = new CxJSONArray();

        		nx_json* js=json->child;
        		for (js=json->child; js; js=js->next) 
				{
          			dump( js, a );
        		}

				switch( cxjObject->type() ) {
	
					case CxJSONBase::ARRAY: 
						{
							CxJSONArray *parentObject = (CxJSONArray *) cxjObject;
							parentObject->append( a );
						}
						break;

	 				case CxJSONBase::OBJECT:
						{
							CxJSONObject *parentObject = (CxJSONObject *) cxjObject;
							parentObject->append( new CxJSONMember( json->key, a ));
						}
						break;

                	default:
					std::cout << "ERROR" << std::endl;
								
				}
      		}
      		break;

		case NX_JSON_STRING:

			switch( cxjObject->type() ) {

				std::cout << "STRING=[" << json->text_value << "]" << std::endl;
	
				case CxJSONBase::ARRAY: 
					{
						CxJSONArray *parentObject = (CxJSONArray *) cxjObject;
						parentObject->append( new CxJSONString( json->text_value ) );
					}
					break;

 				case CxJSONBase::OBJECT:
					{
						CxJSONObject *parentObject = (CxJSONObject *) cxjObject;
						parentObject->append( new CxJSONMember( json->key, new CxJSONString( json->text_value)));
					}
					break;
                
				default:
					std::cout << "ERROR" << std::endl;
				
			}
			break;

    	case NX_JSON_INTEGER:

			switch( cxjObject->type() ) {
	
				case CxJSONBase::ARRAY: 
					{
						CxJSONArray *parentObject = (CxJSONArray *) cxjObject;
						parentObject->append( new CxJSONNumber( json->int_value ) );
					}
					break;

 				case CxJSONBase::OBJECT:
					{
						CxJSONObject *parentObject = (CxJSONObject *) cxjObject;
						parentObject->append( new CxJSONMember( json->key, new CxJSONNumber( json->int_value)));
					}
					break;
				
				default:
					std::cout << "ERROR" << std::endl;

			}
      		break;

    	case NX_JSON_DOUBLE:

			switch( cxjObject->type() ) {
	
				case CxJSONBase::ARRAY: 
					{
						CxJSONArray *parentObject = (CxJSONArray *) cxjObject;
						parentObject->append( new CxJSONNumber( json->dbl_value ) );
					}
					break;

 				case CxJSONBase::OBJECT:
					{
						CxJSONObject *parentObject = (CxJSONObject *) cxjObject;
						parentObject->append( new CxJSONMember( json->key, new CxJSONNumber( json->dbl_value)));
					}
					break;
				            
				default:
					std::cout << "ERROR" << std::endl;

			}
      		break;

    	case NX_JSON_BOOL:

			switch( cxjObject->type() ) {
	
				case CxJSONBase::ARRAY: 
					{
						CxJSONArray *parentObject = (CxJSONArray *) cxjObject;
						parentObject->append( new CxJSONBoolean( json->int_value ) );
					}
					break;

 				case CxJSONBase::OBJECT:
					{
						CxJSONObject *parentObject = (CxJSONObject *) cxjObject;
						parentObject->append( new CxJSONMember( json->key, new CxJSONBoolean( json->int_value)));
					}
					break;
				
                default:
					std::cout << "ERROR" << std::endl;

			}
      		break;

    	default:
      		break;
  	}
}



/**********
static void dump(const nx_json* json, char* out, char** end, int indent) {

  if (!json) {
    *end=out;
    return;
  }
  int i;
  for (i=0; i<indent; i++) *out++=' ';
  if (json->key) {
    strcpy(out, json->key);
    out+=strlen(json->key);
    *out++=':';
  }

  switch (json->type) {

    case NX_JSON_NULL:
      strcpy(out, "null");
      out+=4;
      break;

    case NX_JSON_OBJECT:
      *out++='{';
      *out++='\n';
      {
        nx_json* js=json->child;
        for (js=json->child; js; js=js->next) {
          dump(js, out, &out, indent+2);
        }
      }
      for (i=0; i<indent; i++) *out++=' ';
      *out++='}';
      break;

    case NX_JSON_ARRAY:
      *out++='[';
      *out++='\n';
      {
        nx_json* js=json->child;
        for (js=json->child; js; js=js->next) {
          dump(js, out, &out, indent+2);
        }
      }
      for (i=0; i<indent; i++) *out++=' ';
      *out++=']';
      break;

    case NX_JSON_STRING:
      *out++='"';
      strcpy(out, json->text_value);
      out+=strlen(json->text_value);
      *out++='"';
      break;

    case NX_JSON_INTEGER:
      out+=sprintf(out, "%lld", json->int_value);
      break;

    case NX_JSON_DOUBLE:
      out+=sprintf(out, "%le", json->dbl_value);
      break;

    case NX_JSON_BOOL:
      *out++=json->int_value?'T':'F';
      break;

    default:
      strcpy(out, "????");
      out+=4;
      break;
  }
  *out++='\n';
  *end=out;
}
**************/


int main() {




/**

CxJSONArray  *idArray   = new CxJSONArray();
idArray->append( new CxJSONNumber( 116 ) );
idArray->append( new CxJSONNumber( 943 ) );
idArray->append( new CxJSONNumber( 234 ) );

CxJSONObject *thumbNailObject = new CxJSONObject();
thumbNailObject->append( new CxJSONMember( "Url",    new CxJSONString("http://fred.com")));
thumbNailObject->append( new CxJSONMember( "Height", new CxJSONNumber( 125 )));
thumbNailObject->append( new CxJSONMember( "Width",  new CxJSONNumber( 100 )));

CxJSONObject *imageObject = new CxJSONObject();
imageObject->append(new CxJSONMember("Width",  new CxJSONNumber( 1000 )));
imageObject->append(new CxJSONMember("Height", new CxJSONNumber( 800 )));
imageObject->append(new CxJSONMember("Title",  new CxJSONString( "The Title" )));
imageObject->append(new CxJSONMember("Thumbnail", thumbNailObject));
imageObject->append(new CxJSONMember("Animated", new CxJSONBoolean( false )));
imageObject->append(new CxJSONMember("IDs", idArray));
imageObject->append(new CxJSONMember("Random", new CxJSONNull() ));

CxJSONObject *topObject = new CxJSONObject();
topObject->append( new CxJSONMember("Image", imageObject) );


std::cout << *topObject << std::endl;
**/


	CxJSONObject *topObject = new CxJSONObject();


    CxString buffer;
    CxString line;

    CxFile inputFile;

    if (!inputFile.open("input.txt","r")) {
	std::cout << "error opening file" << std::endl;
        exit(0);
    }

    line = inputFile.getUntil('\n');
    while (line.length()) {
	buffer += line;
        line = inputFile.getUntil('\n');
    }

    inputFile.close();

    char *txt = buffer.data();

    const nx_json* json=nx_json_parse_utf8(txt);
    if (!json) {
		printf("error: could not parse");
        return(0);
    }
      
	dump( json, topObject );
  
    nx_json_free(json);

	std::cout << *topObject << std::endl;


  return 0;
}
