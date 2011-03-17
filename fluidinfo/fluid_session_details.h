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
	std::cout << "Initializing SessionDetails() " << std::endl;
    }

    virtual ~SessionDetails() {
        curl_multi_remove_handle(parentSession->curl_multi_handle(), handle);
        curl_easy_cleanup(handle);
        if (http_headers) {
            curl_slist_free_all(http_headers);
            http_headers = NULL;
        }
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
    }

    void update();

protected:

	void init();
	
    CURL *handle;
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
    static curl_slist* http_headers;
    
    static std::vector<std::string> permissionsMap;
    static std::vector<std::string> categoriesMap;
    static std::vector<std::string> policyMap;
    
    static std::vector<std::string>& initpermissionsMap();
    static std::vector<std::string>& initpolicyMap();
    static std::vector<std::string>& initcategoriesMap();
};

    std::vector<std::string> fluidinfo::SessionDetails::permissionsMap;
    std::vector<std::string> fluidinfo::SessionDetails::categoriesMap;
    std::vector<std::string> fluidinfo::SessionDetails::policyMap;
}

#endif
