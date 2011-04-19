#include "fluid_session_details.h"
#include "fluid_security.h"
#include <vector>
#include <errno.h>

int fluidinfo::SessionDetails::connections = 0;
//curl_slist* fluidinfo::SessionDetails::http_headers = NULL;

std::vector< std::string >& fluidinfo::SessionDetails::permissionsMap()
{

    static volatile bool first = true;
    static std::vector<std::string> _permissionsMap;
  

    if ( first ) {
        _permissionsMap[CREATE] = "create";
        _permissionsMap[UPDATE] = "update";
        _permissionsMap[DELETE] = "delete";
        _permissionsMap[LIST] = "list";
        _permissionsMap[CONTROL] = "control";

	first = false;

    }
    
    return _permissionsMap;

}

std::vector<std::string>& fluidinfo::SessionDetails::policyMap() {
    static bool first = true;
    
    static std::vector<std::string> _policyMap;

    if ( first ) {
        _policyMap[OPEN] = "open";
        _policyMap[CLOSED] = "closed";
	first = false;
    }
    
    return _policyMap;
}

std::vector<std::string>& fluidinfo::SessionDetails::categoriesMap() {
    static bool first = true;
 
    static std::vector<std::string> _categoriesMap;

    if ( first ) {

        _categoriesMap[NS] = "namespaces";
        _categoriesMap[TAGS] = "tags";
        _categoriesMap[TAGVALUES] = "tag-values";
	
	first = false;
    }
    
    return _categoriesMap;
}

void fluidinfo::SessionDetails::init(bool multi, const std::string& headers)
{

    //User classes must call init() each time they do a request...
 
    /*** START CRITICAL SECTION ***/
    handle = curl_easy_init();
	
    //http_headers is not thread safe !
    if ( http_headers ) {
       curl_slist_free_all(http_headers);
    }
    http_headers = curl_slist_append(http_headers, headers.c_str());
    
    /*** END CRITICAL SECTION ***/
    

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

    //each time add a new different curl handle
   
    if ( multi == true ) {
		curl_multi_add_handle(parentSession->curl_multi_handle(), handle);
		std::cout << "Obtained handle " << handle << std::endl;
		handles.push_back(handle);

		printHandlesVector();

		handle = handles[handles.size()-1];

		std::cout << "Handle from vector: " << handle << std::endl;
		_init = true;
		std::cout << "Adding curl easy handle " << handle << std::endl;
    }
    
    //we can put an update() call here based on a flag?!
}

//TODO: critical section
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
	
        multi_perform_code = curl_multi_perform(parentSession->curl_multi_handle(), &running_handles);

        cres = curl_multi_fdset(parentSession->curl_multi_handle(), &read_set, &write_set, &exc_set,&maxfd);

		//if maxfd == -1 || running_handles == 0 No handles!
		
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
		
		//CRITICAL SECTION !
		
		std::cout << "CURL result : " << msg->data.result << std::endl;
		std::cout << strerror(errno) << std::endl;
		//std::cout << "Data: " << (msg->data.whatever?(char*)msg->data.whatever:"") << std::endl;
	      }
	     
      }while( msg_queue > 0 || msg != NULL );
}


