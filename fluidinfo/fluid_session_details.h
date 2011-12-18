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

#ifndef _FLUID_SESSION_DETAILS_H_
#define _FLUID_SESSION_DETAILS_H_

#include "generic_stuff.h"
#include "fluid_session.h"
#include <stdexcept>
#include <vector>

namespace fluidinfo {

	
enum CURLRequestType
{
	GET,
	POST,
	PUT,
	HEAD,
	DELETE
};
	
class SessionDetails 
{

	
public:

    class Exception: public std::runtime_error
    {
    public:
        Exception(const std::string &x): std::runtime_error(x), x(x) { }
        const char* what() const throw () {
            return x.c_str();
        }
        ~Exception() throw() { }
    protected:
        std::string x;
    };

    SessionDetails() {
        _SSL = false;
        _init=false;
        connections++;
        _err = "";
        http_headers = NULL;
        parentSession = NULL;
   
	//std::cerr << "Initializing SessionDetails() " << std::endl;


        //SEGFAULT here?
        //permissionsMap();
        //policyMap();
        //categoriesMap();
    }

    virtual ~SessionDetails() {
   //     std::cerr << "~SesssionDetails " << std::endl;
        if ( _init )
            curl_easy_cleanup(handle);
        /*
        for ( int i = 0; i < handles.size(); i++) {
           curl_multi_remove_handle(parentSession->curl_multi_handle(), handles[i]);
           curl_easy_cleanup(handles[i]);
        }
        */

        /*
        	may segfault in certain conditions
            if (http_headers) {
                curl_slist_free_all(http_headers);
                http_headers = NULL;
            }*/

        connections--;
    }
    void setSSL(bool SSL) {
        _SSL = SSL;
    };

	//Error handling
    virtual void setError(std::string err) ;
	
    bool isError() 
	{
        return (_err.size());
    }
    
    bool hasErrors() { return !errorVector.empty(); }
	void clearErrors() { errorVector.clear(); clearLastError(); }
	void clearLastError() { _err = ""; }
	const std::string& lastError() const { return _err; }
	const std::vector<std::string>& getErrors() const { return errorVector; }

    virtual void setParentSession(Session *p) 
    {
        parentSession = p;
        setSSL(parentSession->getSSL());
    }
    

    void multiUpdateSession();

protected:
    uint32_t lastContentLength;	
	
    void init(bool multi=false, const std::string headers="Content-Type: application/json");
	void runCURL(CURLRequestType type, const std::string& url, 
				  const Json::Value* root=NULL, 
				  size_t (*callback)(void* ptr, size_t size, size_t nmemb, void* p)=NULL,
				  SessionDetails* writeObj = NULL
				);

    CURL* handle;

    //CURL *const Handle() { return handles[handles.size()-1]; }
    inline CURL* const Handle() {
        return handle;
    }

	std::vector<std::string> errorVector;

    bool _SSL;
    bool _init;
    std::string _err;

    fd_set read_set;
    fd_set write_set;
    fd_set exc_set;
    int maxfd;

    struct timeval select_timeout;


    std::string mainURL;

    Session* parentSession;

    static int connections;
    curl_slist* http_headers;

    static std::vector<std::string>& permissionsMap();
    static std::vector<std::string>& policyMap();
    static std::vector<std::string>& categoriesMap();

    friend class Session;
	friend class Tag;
    
private:
    //CURL *handle;
};

}

#endif
