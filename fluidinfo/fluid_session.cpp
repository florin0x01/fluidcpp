#include "fluid_session.h"

bool fluidinfo::Session::start() {
	       curl_res = curl_global_init();
	       
	       if  ( curl_res ) 
		 return false;
	       
	       curl_box = curl_multi_init();
	       if (!curl_box)
		 return false;
	       
	       /* Options for each class - Namespace, User etc
	       if ( sandboxMode ) {
		  curl_easy_setopt(curl, CURLOPT_URL, "http://sandbox.fluidinfo.com");
		  return true;
	      }
	       
	       if ( SSL ) {
		  curl_easy_setopt(curl, CURLOPT_URL, "https://fluiddb.fluidinfo.com");
		  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); //do not verify cert
		  curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);  //do not verify host in cert
	      }
	       else
		 curl_easy_setopt(curl, CURLOPT_URL, "http://fluiddb.fluidinfo.com");
	       */
	       
	       /**
		* 	enum permissions { CREATE,UPDATE,DELETE,LIST,CONTROL };
	enum policy { OPEN, CLOSED };
	enum categories { NS, TAGS, TAGVALUES };*/
	       

	       return true;
  
}