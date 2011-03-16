#ifndef _FLUID_SESSION_
#define _FLUID_SESSION_

#include "generic_stuff.h"
#include "fluid_security.h"
#include "fluid_session_details.h"

namespace fluidinfo{
  
  class Session {
      public :
	      /*
	    Session():CURLInitialized(false),_name(""),SSL(false),sandboxMode(false) { 
		
	    }*/
	    Session(std::string name="", authentication* auth=NULL):_name(name), AuthObj(*auth),SSL(false),sandboxMode(false) { 
	      CURLInitialized = false; 
	      
	    }
	    ~Session() { 
	      if ( CURLInitialized == true && curl_box) {
		  curl_global_cleanup();
		  curl_box = NULL;
	      }
	    }
	    void setAuthentication(const authentication& auth) { AuthObj = auth;  } 
	    void setName(std::string name="") { _name = name; }
	    void setSSL(bool _ssl=true) { SSL = _ssl; }
	    void setSandbox(bool sandbox=true) { sandboxMode = sandbox; }
	    CURLM* curl_multi_handle() const { return curl_box; }
	    
	    bool Start();
	    authentication AuthObj;
	    
      protected:

	    std::string _name;
	
	    

	    bool SSL;
	    bool CURLInitialized;
	    bool sandboxMode;
	    
	    CURLM *curl_box;
	    CURLcode curl_res;
	    
	//    friend class SessionDetails;
	    
  };
  
};

#endif