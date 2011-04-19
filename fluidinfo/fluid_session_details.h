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
	
	handles.reserve(1000);
	
	//SEGFAULT here?
	//permissionsMap();
	//policyMap();
	//categoriesMap();
    }

    virtual ~SessionDetails() {
	for ( int i = 0; i < handles.size(); i++) {
	   curl_multi_remove_handle(parentSession->curl_multi_handle(), handles[i]);
	   curl_easy_cleanup(handles[i]);
	}
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

    void update();

protected:

    void init(bool multi=true, const std::string& headers="Content-Type: application/json");
	   
    std::vector<CURL*> handles;
    
    CURL *const Handle() { return handles[handles.size()-1]; }

    inline void printHandlesVector() {
		for(int i=0; i < handles.size(); i++)
			std::cout << handles[i] << std::endl;
	}
    
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
	CURL *handle;
};

}

#endif
