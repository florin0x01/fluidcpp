#include "fluid_session_details.h"
#include "fluid_security.h"
#include <vector>
#include <errno.h>

int fluidinfo::SessionDetails::connections = 0;
//curl_slist* fluidinfo::SessionDetails::http_headers = NULL;

namespace 
{
void checkAndThrow(CURLcode c)
{
	if ( c != CURLE_OK )
		throw std::logic_error("Error in curl_easy_setopt");
}
}

std::vector< std::string >& fluidinfo::SessionDetails::permissionsMap()
{

    static volatile bool first = true;
    static std::vector<std::string> _permissionsMap;


    if ( first ) {
        _permissionsMap[PERMISSION_CREATE] = "create";
        _permissionsMap[PERMISSION_UPDATE] = "update";
        _permissionsMap[PERMISSION_DELETE] = "delete";
        _permissionsMap[PERMISSION_LIST] = "list";
        _permissionsMap[PERMISSION_CONTROL] = "control";

        first = false;

    }

    return _permissionsMap;

}

std::vector<std::string>& fluidinfo::SessionDetails::policyMap() 
{
    static bool first = true;

    static std::vector<std::string> _policyMap;

    if ( first ) {
        _policyMap[POLICY_OPEN] = "open";
        _policyMap[POLICY_CLOSED] = "closed";
        first = false;
    }

    return _policyMap;
}

std::vector<std::string>& fluidinfo::SessionDetails::categoriesMap() 
{
    static bool first = true;

    static std::vector<std::string> _categoriesMap;

    if ( first ) {

        _categoriesMap[CATEGORY_NS] = "namespaces";
        _categoriesMap[CATEGORY_TAGS] = "tags";
        _categoriesMap[CATEGORY_TAGVALUES] = "tag-values";

        first = false;
    }

    return _categoriesMap;
}

void fluidinfo::SessionDetails::runCURL(fluidinfo::CURLRequestType type, const std::string& url, 
										const Json::Value* root, 
										size_t (*callback)(void* ptr, size_t size, size_t nmemb, void* p),
										SessionDetails* writeDataObj
   									)
{
	if ( !writeDataObj )
		writeDataObj = this;
	
	CURLcode c;
	
	Json::FastWriter writer;
	std::string doc = "";
	if ( NULL != root ) 
	{
		doc = writer.write(*root);
	//	std::cerr << "Root not null\n";
	}
	/*
	std::cerr << "Method " << type << " Doc is " << doc.c_str() << "\n";
	
	std::cerr << "Url is " << url << "\n";
	*/
	
	checkAndThrow (curl_easy_setopt(handle, CURLOPT_URL, url.c_str()));
	http_headers = curl_slist_append(http_headers, "Expect: ");
	checkAndThrow (curl_easy_setopt(handle, CURLOPT_HTTPHEADER, http_headers));
	
	switch(type)
	{
		case HEAD:
			 curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, callback);
			 curl_easy_setopt(handle, CURLOPT_WRITEDATA, writeDataObj);
			 curl_easy_setopt(handle, CURLOPT_NOBODY, 1); //for HEAD requests
		break;
		
		case GET:
			 checkAndThrow (curl_easy_setopt(handle, CURLOPT_HTTPGET, 1));
			 checkAndThrow (curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, callback));
			 checkAndThrow (curl_easy_setopt(handle, CURLOPT_WRITEDATA, writeDataObj));
		break;
		
		case POST:
			 if ( doc != "" ) 
			 {
				checkAndThrow (curl_easy_setopt(handle, CURLOPT_POSTFIELDSIZE, (long)strlen(doc.c_str()))); 
				checkAndThrow (curl_easy_setopt(handle, CURLOPT_COPYPOSTFIELDS, doc.c_str()));
			 }
			 else 
				 checkAndThrow (curl_easy_setopt(handle, CURLOPT_POSTFIELDSIZE, 0));
			 checkAndThrow (curl_easy_setopt(handle, CURLOPT_POST, 1L));
			 checkAndThrow (curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, callback));
			 checkAndThrow (curl_easy_setopt(handle, CURLOPT_WRITEDATA, writeDataObj));
		break;
		
		case PUT:
			checkAndThrow (curl_easy_setopt(handle, CURLOPT_UPLOAD, 1L));
			checkAndThrow (curl_easy_setopt(handle, CURLOPT_READFUNCTION, callback));
			checkAndThrow (curl_easy_setopt(handle, CURLOPT_READDATA, doc.c_str()));
			checkAndThrow (curl_easy_setopt(handle, CURLOPT_INFILESIZE, doc.size()));
  
		break;
		
		case DELETE:
			 checkAndThrow (curl_easy_setopt(handle, CURLOPT_URL, url.c_str()));
			 checkAndThrow (curl_easy_setopt(handle, CURLOPT_NOBODY, 1L));
			 checkAndThrow (curl_easy_setopt(handle, CURLOPT_CUSTOMREQUEST, "DELETE"));
		break;
		
		default:
			throw std::logic_error("No available method for this request");
		break;
		
	}
	
	curl_easy_perform(handle);
	
}

void fluidinfo::SessionDetails::setError(std::string err)
{
	//std::cerr << "setError called " << std::endl;	
	_err = err;
	errorVector.push_back(_err);
}


void fluidinfo::SessionDetails::init(bool multi, const std::string headers)
{
    //User classes must call init() each time they do a request...

	if ( !parentSession )
	{
		setError("NULL session");
		return;
	}		
	
    /*** START CRITICAL SECTION ***/
    if ( !_init ) {
   //     std::cerr << "Reinit() " << std::endl;
        handle = curl_easy_init();
    }

    else curl_easy_reset(handle);

    curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, fluidinfo::Session::HeaderFunction);
    curl_easy_setopt(handle, CURLOPT_WRITEHEADER, this);

   // std::cerr << "Calling init on handle " << handle << std::endl;

    //http_headers is not thread safe !
    if ( http_headers ) {
        curl_slist_free_all(http_headers);
        http_headers = NULL;
    }
    http_headers = curl_slist_append(http_headers, headers.c_str());

    /*** END CRITICAL SECTION ***/

    curl_easy_setopt(handle, CURLOPT_USERAGENT, parentSession->UserAgent());

    if ( _SSL == true ) {
        curl_easy_setopt(handle, CURLOPT_URL, FLUID_HTTP_SSL);
        curl_easy_setopt(handle, CURLOPT_SSL_VERIFYPEER, 0L); //skip peer verification
        curl_easy_setopt(handle, CURLOPT_SSL_VERIFYHOST, 0L); //do not verify host

        mainURL = FLUID_HTTP_SSL;
    }
    else {
        if ( parentSession->Sandbox() == true ) {
            std::cerr << "Sandbox mode." << std::endl;
            curl_easy_setopt(handle, CURLOPT_URL, FLUID_SANDBOX_HTTP);
            mainURL = FLUID_SANDBOX_HTTP;
        }
        else {
            curl_easy_setopt(handle, CURLOPT_URL, FLUID_HTTP);
            mainURL = FLUID_HTTP;
        }
    }

    //curl_easy_setopt(handle, CURLOPT_VERBOSE, 1);

    curl_easy_setopt(handle, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
    std::string ugly_user_pwd = parentSession->AuthObj.username + ":" + parentSession->AuthObj.password;
    curl_easy_setopt(handle, CURLOPT_USERPWD, ugly_user_pwd.c_str());
    curl_easy_setopt(handle, CURLOPT_HTTPHEADER, http_headers);

    if ( multi == true ) {
        if ( !_init ) {
            if ( curl_multi_add_handle(parentSession->curl_multi_handle(), handle) != CURLM_OK )
                std::cerr << "Could not add handle " << std::endl;

            std::cerr << "Obtained handle " << handle << std::endl;

            std::cerr << "Handle from vector: " << handle << std::endl;

            std::cerr << "Adding curl easy handle " << handle << std::endl;
        }

        //handle = handles[handles.size()-1];

    }

    if ( !_init )
        _init = true;

    //we can put an update() call here based on a flag?!
}

//TODO: critical section
void fluidinfo::SessionDetails::multiUpdateSession()
{
    /** Wait up to 1 seconds **/

    FD_ZERO(&read_set);
    FD_ZERO(&write_set);
    FD_ZERO(&exc_set);


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

        do {
            multi_perform_code = curl_multi_perform(parentSession->curl_multi_handle(), &running_handles);
        } while (multi_perform_code == CURLM_CALL_MULTI_PERFORM);

        std::cerr << "Multi perform code: " << multi_perform_code << std::endl;
        std::cerr << "Running handles: " << running_handles << std::endl;
        //std::cerr << "update(): running handles: " << running_handles << std::endl;

        maxfd = 0;

        cres = curl_multi_fdset(parentSession->curl_multi_handle(), &read_set, &write_set, &exc_set,&maxfd);

        std::cerr << "Cres: " << cres << std::endl;

        //if maxfd == -1 || running_handles == 0 No handles!

        std::cerr << "Maxfd: " << maxfd << std::endl;

        if ( maxfd == -1 )
        {
            std::cerr << "Maxfd is -1 " << std::endl;
            return;
        }

        if ( cres!= CURLM_OK )
        {
            //TODO throw exception ?
            std::cerr << "multi_fdset failed " << std::endl;
            return;
        }

        cres = curl_multi_timeout(parentSession->curl_multi_handle(),&timeout_ms);

        if ( cres!= CURLM_OK )
        {
            //TODO throw exception ?
            std::cerr << "curl multi_timeout failed " << std::endl;
            return;
        }

        // std::cerr << "timeout_ms returned: " << timeout_ms << std::endl;

        if ( timeout_ms != 0 || timeout_ms != -1)
            select_timeout.tv_sec = timeout_ms/1000;

        int retval = select(maxfd+1,&read_set,&write_set,&exc_set,&select_timeout);

        if ( retval == -1 )
        {
            //TODO throw exception ?
            std::cerr << "select() failed " << std::endl;
            return;
        }

    } while (multi_perform_code==CURLM_CALL_MULTI_PERFORM || running_handles > 0);

    CURLMsg *msg = NULL;
    int msg_queue = 0;

    do {
        msg = curl_multi_info_read(parentSession->curl_multi_handle(), &msg_queue);
        std::cerr << "Messages in queue left: " << msg_queue << std::endl;
        if ( msg ) {
            std::cerr << "Received msg: " << std::endl;
            std::cerr << "Message type: " << msg->msg << std::endl;
            std::cerr << "CURL easy handle: " << msg->easy_handle << std::endl;

            //CRITICAL SECTION !

            std::cerr << "CURL result : " << msg->data.result << std::endl;
            std::cerr << strerror(errno) << std::endl;
            //std::cerr << "Data: " << (msg->data.whatever?(char*)msg->data.whatever:"") << std::endl;
        }

    } while ( msg_queue > 0 || msg != NULL );
}


