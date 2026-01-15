/***************************************************************************
 *
 *  prop.cpp
 *
 *  CxPropEntry, CxPropertyList Class
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

#include <cx/json/json_factory.h>


//-------------------------------------------------------------------------
// CxJSONFactory::CxJSONFactory
//
//-------------------------------------------------------------------------
CxJSONFactory::CxJSONFactory(void)
{
}

//-------------------------------------------------------------------------
// CxJSONFactory::~CxJSONFactory
//
//-------------------------------------------------------------------------
CxJSONFactory::~CxJSONFactory(void)
{
}



//-------------------------------------------------------------------------
// CxString::operator<<
//
//-------------------------------------------------------------------------
CxJSONBase *
CxJSONFactory::parse( CxString txt)
{
    /*
    txt.stripLeading(" \t\n");
    txt.stripTrailing(" \t\n");
    
    // hack for parser
    if ((txt.firstChar('}') !=0) && (txt.lastChar('}')!=0)) {
        txt.insert("\"DUMMY\":",0);
    }
    
    printf("JSON=[%s]\n", txt.data());
*/
    
	//---------------------------------------------------------------------
	// use the nx_json C parser to build a tree we can convert
	// to C++ objects
	//
	//---------------------------------------------------------------------
    const nx_json* json=nx_json_parse_utf8( txt.data() );
    if (!json) {   
        return( NULL );
    }

	//---------------------------------------------------------------------
	// create and container object
	//
	//---------------------------------------------------------------------
	CxJSONObject *container = new CxJSONObject();

	//---------------------------------------------------------------------
	// create an object tree from the parser
	//
	//---------------------------------------------------------------------
    CxJSONFactory::walktree( json, container );
  
	//---------------------------------------------------------------------
	// free up the C parser
	//
	//---------------------------------------------------------------------
    nx_json_free(json);

	//---------------------------------------------------------------------
	// if things in container after walking parse tree is zero thats 
	// and error
	//
	//---------------------------------------------------------------------
	if (container->entries()==0) {
		return( NULL );
	}
                
	//---------------------------------------------------------------------
	// if things in container after walking parse tree is one thats 
	// an array of 1 thing, or an object.  return the type it is and 
	// delete the container object
	//
	//---------------------------------------------------------------------
 	if (container->entries()==1) {

		CxJSONMember *member= container->at(0);

		if (member) {

			CxJSONBase *innerItem = member->object();

			if (innerItem->type() == CxJSONBase::OBJECT) {

				CxJSONMember *m = (CxJSONMember *) container->removeAt(0);
				CxJSONObject *o = (CxJSONObject *) m->object();
				delete container;
				return( o );
			}

			if (innerItem->type() == CxJSONBase::ARRAY) {
	
				CxJSONMember *m = container->removeAt(0);
				CxJSONObject *a = (CxJSONObject *) m->object();
				delete container;
				return( a );
			}

			return( NULL );
		}
	}

	return( NULL );

}


//-------------------------------------------------------------------------
// 
//
//-------------------------------------------------------------------------
/* static */ 
void CxJSONFactory::walktree(const nx_json* json, CxJSONBase *cxjObject ) 
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
          			CxJSONFactory::walktree( js, o );
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
          			CxJSONFactory::walktree( js, a);
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

//				std::cout << "STRING=[" << json->text_value << "]" << std::endl;
	
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





