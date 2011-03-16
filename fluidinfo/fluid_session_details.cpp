#include "fluid_session_details.h"

int fluidinfo::SessionDetails::connections = 0;
curl_slist* fluidinfo::SessionDetails::headers = NULL;

fluidinfo::SessionDetails::permissionsMap[CREATE] = "create";
fluidinfo::SessionDetails:permissionsMap[UPDATE] = "update";
fluidinfo::SessionDetails:permissionsMap[DELETE] = "delete";
fluidinfo::SessionDetails:permissionsMap[LIST] = "list";
fluidinfo::SessionDetails:permissionsMap[CONTROL] = "control";

fluidinfo::SessionDetails:policyMap[OPEN] = "open";
fluidinfo::SessionDetails:policyMap[CLOSED] = "closed";

fluidinfo::SessionDetails:categoriesMap[NS] = "namespaces";
fluidinfo::SessionDetails:categoriesMap[TAGS] = "tags";
fluidinfo::SessionDetails:categoriesMap[TAGVALUES] = "tag-values";


void fluidinfo::SessionDetails::update()
{

    FD_ZERO(&read_set);
    FD_ZERO(&write_set);
    FD_ZERO(&exc_set);

    /** Wait up to 5 seconds **/
    select_timeout.tv_sec = 5;
    select_timeout.tv_usec = 0;

    int timeout_ms = 0;
    int running_handles = 0;

    timespec req_time;
    req_time.tv_nsec = 100;
    req_time.tv_sec = 0;

    CURLMcode cres;
    CURLMcode multi_perform_code;

    do {

        multi_perform_code = curl_multi_perform(parentSession->curl_multi_handle(), &running_handles);

        cres = curl_multi_fdset(parentSession->curl_multi_handle(), &read_set, &write_set, &exc_set);


        if ( cres!= CURLM_OK )
        {
            //TODO throw exception ?
            return;
        }

        cres = curl_multi_timeout(parentSession->curl_multi_handle(),&timeout_ms);

        if ( cres!= CURLM_OK )
        {
            //TODO throw exception ?
            return;
        }

        if ( timeout_ms != 0)
            select_timeout.tv_sec = timeout_ms/1000;

        int retval = select(maxfd+1,&read_set,&write_set,&exc_set,&select_timeout);

        if ( retval == -1 )
        {
            //TODO throw exception ?
            return;
        }


    } while (multi_perform_code==CURLM_CALL_MULTI_PERFORM || running_handles > 0);


}


