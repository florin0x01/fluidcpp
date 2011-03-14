#ifndef _FLUID_SESSION_DETAILS_H_
#define _FLUID_SESSION_DETAILS_H_

#include "generic_stuff.h"

namespace fluidinfo{

class Session;

    class SessionDetails {
	  public:
	      SessionDetails() { _SSL = false; _sandbox=false; _init=false; connections++; }
	      
	      virtual ~SessionDetails() { 
		curl_multi_remove_handle(parentSession->curl_box, handle);
		curl_easy_cleanup(handle);
		if (http_headers) {
		  curl_slist_free_all(http_headers);
		  http_headers = NULL;
		}
		connections--;
	      }
	      void setSSL(bool SSL) { _SSL = SSL; };
	      void setSandBox(bool sandboxMode=false) { _sandbox = sandboxMode; }
	      void setParentSession(Session *p) { parentSession = p; }
	      
	      
	  protected:
	    
	      void init() {
		
		//User classes must call init() each time they do a request...
		
		/*
		  if ( _init ) 
		      return;
		*/
		
		  if ( _init == false )
		    handle = curl_easy_init();
		  
		  if ( http_headers && _init == false) {
		    curl_slist_free_all(http_headers);
		    http_headers = NULL;
		    http_headers = curl_slist_append(http_headers, "application/json");
		  }
		  
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
		  
		  curl_easy_setopt(handle, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
		  curl_easy_setopt(handle, CURLOPT_USERPWD, parentSession->AuthObj.password);
		  curl_easy_setopt(handle, CURLOPT_HTTPHEADER, http_headers);
		  
		  //only add curl handle if we have a real "new" connection and make _init=true
		  if ( _init == false ) {
		    curl_multi_add_handle(parentSession->curl_box, handle);
		    _init = true;
		  }
	      }
	    
	      CURL *handle;
	      static int connections;
	      static curl_slist *http_headers;
	      bool _SSL;
	      bool _sandbox;
	      bool _init;
	      
	     static std::vector<int> permissionsMap;
	     static std::vector<int> categoriesMap;
	     static std::vector<int> policyMap;
	    
	     std::string mainURL;
	      
	     Session* parentSession;
    };
  
}

#endif