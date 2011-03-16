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

   inline void init() {

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
        curl_easy_setopt(handle, CURLOPT_USERPWD, parentSession->AuthObj.password.c_str());
        curl_easy_setopt(handle, CURLOPT_HTTPHEADER, http_headers);

        //only add curl handle if we have a real "new" connection and make _init=true
        if ( _init == false ) {
            curl_multi_add_handle(parentSession->curl_multi_handle(), handle);
            _init = true;
        }
    }

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
    
    static std::vector<int> permissionsMap;
    static std::vector<int> categoriesMap;
    static std::vector<int> policyMap;
};

   int SessionDetails::connections;
   curl_slist* SessionDetails::http_headers;
   std::vector<int> SessionDetails::permissionsMap;
   std::vector<int> SessionDetails::categoriesMap;
   std::vector<int> SessionDetails::policyMap;
}

#endif
