#ifndef _FLUID_SESSION_
#define _FLUID_SESSION_

#include "generic_stuff.h"
#include "fluid_security.h"

namespace fluidinfo{

  class Session {
      public :
	    Session():CURLInitialized(false),_name(""),SSL(false),sandboxMode(false) { 
		
	    }
	    Session(std::string name="", authentication auth=NULL):_name(name), AuthObj(auth),SSL(false),sandboxMode(false) { 
	      CURLInitialized = false; 
	      
	    }
	    ~Session() { 
	      if ( CURLInitialized == true && curl) {
		  curl_global_cleanup();
		  curl_easy_cleanup(curl);
		  curl = NULL;
	      }
	    }
	    setAuthentication(const authentication& auth) { AuthObj = auth;  } 
	    setName(std::string name="") { _name = name; }
	    setSSL(bool _ssl=true) { SSL = _ssl; }
	    setSandbox(bool sandbox=true) { sandboxMode = sandbox; }
	    
	    bool start();
	    
	    
      protected:

	    std::string _name;
	    authentication AuthObj;
	    
	    bool SSL;
	    bool CURLInitialized;
	    bool sandboxMode;
	    
	    CURL *curl;
	    CURLcode curl_res;
	    
  };
  
};

#endif