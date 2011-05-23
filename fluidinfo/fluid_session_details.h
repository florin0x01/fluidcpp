#ifndef _FLUID_SESSION_DETAILS_H_
#define _FLUID_SESSION_DETAILS_H_

#include "generic_stuff.h"
#include "fluid_session.h"

namespace fluidinfo {

class SessionDetails {
public:
    SessionDetails() {
        _SSL = false;
        _sandbox=false;
        _init=false;
        connections++;
	http_headers = NULL;
	std::cout << "Initializing SessionDetails() " << std::endl;

	
	//SEGFAULT here?
	//permissionsMap();
	//policyMap();
	//categoriesMap();
    }

    virtual ~SessionDetails() {
	std::cout << "~SesssionDetails " << std::endl;
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
    void setSandBox(bool sandboxMode=false) {
        _sandbox = sandboxMode;
    }
    void setParentSession(Session *p) {
        parentSession = p;
		setSSL(parentSession->getSSL());
    }

    void multiUpdateSession();

protected:

    void init(bool multi=false, const std::string headers="Content-Type: application/json");
	   
    CURL* handle;
    
    //CURL *const Handle() { return handles[handles.size()-1]; }
    inline CURL* const Handle() { return handle; }
    
 
    
    bool _SSL;
    bool _sandbox;
    bool _init;

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
    
private:
	//CURL *handle;
};

}

#endif
