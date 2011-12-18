/* 
Copyright 2011 Nicolae Florin Petrovici
 
This file is part of the Fluidinfo C++ library. 
For more information, regarding Fluidinfo, visit http://www.fluidinfo.com

Fluidinfo C++ library is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Fluidinfo C++ library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Fluidinfo C++ library.  If not, see <http://www.gnu.org/licenses/>.
*/  

#ifndef _FLUID_SESSION_
#define _FLUID_SESSION_

#include "generic_stuff.h"
#include "fluid_security.h"
#include <map>
//#include "fluid_session_details.h"

namespace fluidinfo{
typedef size_t (*cbFunction)(void* obj) ;    

class Session {
      public :
	      /*
	    Session():CURLInitialized(false),_name(""),SSL(false),sandboxMode(false) { 
		
	    }*/
	    explicit Session(std::string name="", authentication* auth=NULL):_name(name),SSL(false),sandboxMode(false) { 
	      CURLInitialized = false; 
	      _userAgent = "fluidcpp 0.5";
	      if ( auth != NULL )
		      AuthObj = *auth;
	    }
	    virtual ~Session() { 
	      if ( CURLInitialized == true && curl_box) {
		  curl_global_cleanup();
		  //memory leak
		  curl_box = NULL;
	      }
	    }
	    inline void setAuthentication(const authentication& auth) { AuthObj = auth;  } 
	    inline void setName(std::string name="") { _name = name; }
	    inline void setSSL(bool _ssl=true) { SSL = _ssl; }
	    inline bool getSSL() { return SSL; }
	    inline void setSandbox(bool sandbox=true) { sandboxMode = sandbox; }
	    inline CURLM* curl_multi_handle() const { return curl_box; }
	    inline bool Sandbox() { return sandboxMode; }
	    inline const char* UserAgent() { return _userAgent.c_str(); }
	    
	    static size_t HeaderFunction(void *ptr, size_t size, size_t nmemb, void* user);
	    
	    //should be protected
	    void RegisterCallback(const std::string& error, cbFunction, void* obj );
	    
	   inline bool Start() {
		 curl_res = curl_global_init(CURL_GLOBAL_ALL);
	       
	       if  ( curl_res ) 
		 return false;
	       
	       curl_box = curl_multi_init();
	       if (!curl_box)
		 return false; 
	       
	       return true;
	    }
	    authentication AuthObj;
private:
	   struct cbStruct 
	   {
		   cbFunction cb;
		   void *obj;
	   };
	   
      protected:
	    std::map<std::string, cbStruct > cbMap_;
	    
	    std::string _name;
	    std::string _userAgent;
	    

	    bool SSL;
	    bool CURLInitialized;
	    bool sandboxMode;
	    
	    CURLM *curl_box;
	    CURLcode curl_res;
	    
  };
  
};

#endif