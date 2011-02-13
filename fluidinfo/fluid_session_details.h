#ifndef _FLUID_SESSION_DETAILS_H_
#define _FLUID_SESSION_DETAILS_H_

#include "generic_stuff.h"

namespace fluidinfo{

class Session;

    class SessionDetails {
	  public:
	      SessionDetails() { _SSL = false; _sandbox=false; _init=false; }
	      virtual ~SessionDetails() { 
		curl_multi_remove_handle(parentSession->curl_box, handle);
		curl_easy_cleanup(handle);
	      }
	      void setSSL(bool SSL) { _SSL = SSL; };
	      void setSandBox(bool sandboxMode=false) { _sandbox = sandboxMode; }
	      void setParentSession(Session *p) { parentSession = p; }
	      
	      void init() {
		  if ( _init ) 
		      return;
		
		  handle = curl_easy_init();

		  if ( _SSL == true ) {
		    curl_easy_setopt(handle, CURLOPT_URL, FLUID_HTTP_SSL);
		    curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0L); //skip peer verification
		    curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0L); //do not verify host
		    
		    mainURL = FLUID_HTTP_SSL;
		  }
		  else {
		    curl_easy_setopt(handle, CURLOPT_URL, FLUID_HTTP);
		    mainURL = FLUID_HTTP;
		  }
		  
		  curl_multi_add_handle(parentSession->curl_box, handle);
		  
		  _init = true;
	      }
	      
	  protected:
	      CURL *handle;
	      bool _SSL;
	      bool _sandbox;
	      bool _init;
	      
	      std::string mainURL;
	      
	      Session* parentSession;
    };
  
}

#endif