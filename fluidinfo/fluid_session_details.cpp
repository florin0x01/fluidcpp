#include "fluid_session_details.h"
#include "fluid_security.h"
#include <vector>
#include <errno.h>

int fluidinfo::SessionDetails::connections = 0;
curl_slist* fluidinfo::SessionDetails::http_headers = NULL;

std::vector< std::string >& fluidinfo::SessionDetails::initpermissionsMap()
{

    static bool first = true;

    if ( first ) {
        fluidinfo::SessionDetails::permissionsMap[CREATE] = "create";
        fluidinfo::SessionDetails::permissionsMap[UPDATE] = "update";
        fluidinfo::SessionDetails::permissionsMap[DELETE] = "delete";
        fluidinfo::SessionDetails::permissionsMap[LIST] = "list";
        fluidinfo::SessionDetails::permissionsMap[CONTROL] = "control";

	first = false;

    }

}

std::vector<std::string>& fluidinfo::SessionDetails::initpolicyMap() {
    static bool first = true;

    if ( first ) {
        fluidinfo::SessionDetails::policyMap[OPEN] = "open";
        fluidinfo::SessionDetails::policyMap[CLOSED] = "closed";
	first = false;
    }
}

std::vector<std::string>& fluidinfo::SessionDetails::initcategoriesMap() {
    static bool first = true;

    if ( first ) {

        fluidinfo::SessionDetails::categoriesMap[NS] = "namespaces";
        fluidinfo::SessionDetails::categoriesMap[TAGS] = "tags";
        fluidinfo::SessionDetails::categoriesMap[TAGVALUES] = "tag-values";
	
	first = false;
    }
}

void fluidinfo::SessionDetails::init()
{

    //User classes must call init() each time they do a request...

    /*
      if ( _init )
          return;
    */


    if ( _init == false ) {

        handle = curl_easy_init();
        curl_slist_free_all(http_headers);
        http_headers = NULL;
        http_headers = curl_slist_append(http_headers, "Content-Type: application/json");
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

    curl_easy_setopt(handle, CURLOPT_VERBOSE, 1);
    curl_easy_setopt(handle, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    std::string ugly_user_pwd = parentSession->AuthObj.username + ":" + parentSession->AuthObj.password;
    curl_easy_setopt(handle, CURLOPT_USERPWD, ugly_user_pwd.c_str());
    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, http_headers);

    //only add curl handle if we have a real "new" connection and make _init=true
    if ( _init == false ) {
        curl_multi_add_handle(parentSession->curl_multi_handle(), handle);
        _init = true;
	std::cout << "Adding curl easy handle " << handle << std::endl;
    }
}

void fluidinfo::SessionDetails::update()
{

    FD_ZERO(&read_set);
    FD_ZERO(&write_set);
    FD_ZERO(&exc_set);

    /** Wait up to 1 seconds **/
    select_timeout.tv_sec = 1;
    select_timeout.tv_usec = 0;

    long int timeout_ms = 0;
    int running_handles = 0;

    timespec req_time;
    req_time.tv_nsec = 100;
    req_time.tv_sec = 0;

    CURLMcode cres;
    CURLMcode multi_perform_code;

    do {
//	std::cout << "Running update () " << std::endl;
	
        multi_perform_code = curl_multi_perform(parentSession->curl_multi_handle(), &running_handles);

        cres = curl_multi_fdset(parentSession->curl_multi_handle(), &read_set, &write_set, &exc_set,&maxfd);

	//std::cout << "maxfd: " << maxfd << std::endl;

        if ( cres!= CURLM_OK )
        {
            //TODO throw exception ?
            std::cout << "multi_fdset failed " << std::endl;
            return;
        }

        cres = curl_multi_timeout(parentSession->curl_multi_handle(),&timeout_ms);

        if ( cres!= CURLM_OK )
        {
            //TODO throw exception ?
            std::cout << "curl multi_timeout failed " << std::endl;
            return;
        }
        
       // std::cout << "timeout_ms returned: " << timeout_ms << std::endl;

        if ( timeout_ms != 0 || timeout_ms != -1)
            select_timeout.tv_sec = timeout_ms/1000;

        int retval = select(maxfd+1,&read_set,&write_set,&exc_set,&select_timeout);

        if ( retval == -1 )
        {
            //TODO throw exception ?
            return;
        }

	//std::cout << "Running handles: " << running_handles << std::endl;

    } while (multi_perform_code==CURLM_CALL_MULTI_PERFORM || running_handles > 0);

      CURLMsg *msg = NULL;
      int msg_queue = 0;
      do {
	      msg = curl_multi_info_read(parentSession->curl_multi_handle(), &msg_queue);
		 std::cout << "Messages in queue left: " << msg_queue << std::endl;
	      if ( msg ) {
		std::cout << "Received msg: " << std::endl;
		std::cout << "Message type: " << msg->msg << std::endl;
		std::cout << "CURL easy handle: " << msg->easy_handle << std::endl;
		std::cout << "CURL result : " << msg->data.result << std::endl;
		std::cout << strerror(errno) << std::endl;
		//std::cout << "Data: " << (msg->data.whatever?(char*)msg->data.whatever:"") << std::endl;
	      }
	     
      }while( msg_queue > 0 || msg != NULL );
}


