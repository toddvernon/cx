//-------------------------------------------------------------------------------------------------
//
//  temp.cpp
//  cx
//
//  Copyright 2022-2025 Todd Vernon. All rights reserved.
//  Licensed under the Apache License, Version 2.0
//  See LICENSE file for details.
//
//  temp.cpp
//
//-------------------------------------------------------------------------------------------------

void CxJSONObject::walktree(const nx_json* json, CxJSONBase *cxjObject ) 
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
          			walktree( js, o );
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
          			walktree( js, a );
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
