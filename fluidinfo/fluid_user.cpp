#include "fluid_user.h"
#include <stdarg.h>
#include <stdio.h>

using namespace std;

void fluidinfo::User::getSecurity(fluidinfo::security& sec , fluidinfo::categories categ, std::string xpath)
{

  categories categ_tmp;
  
  if ( (categ_tmp=reverse_xpathMap.find(xpath)) != reverse_xpathMap.end() ) {
      if ( categ_tmp == categ )
	sec = securityMap[categ_tmp];
  }
  
  else {
      dirty = true;
      state = STATE_GETSECURITY;
      doState(state, &sec, &categ, &xpath);
      
      //TODO
      //update security map in doState: sec = securityMap[categ_tmp];
  }
}

void fluidinfo::User::setSecurity(fluidinfo::security sec, fluidinfo::categories categ, std::string xpath)
{
 
      state = STATE_SETSECURITY;
      
      dirty = true;
      
      doState(state, sec, categ, xpath);
   
      return;

 
}

bool fluidinfo::User::doState(fluidinfo::User::USER_States state, ...)
{
   string url = mainURL;
   string category;
   string action;
   
   fluidinfo::security* sec = NULL;
   fluidinfo::categories* categ = NULL;
   std::string* xpath = NULL;
   
   va_list vl;
   va_start(vl, state);
   
    switch(state) {
      case STATE_ZERO:
	break;
	
      case STATE_NAME:
	  url = url + "/users/" + _name;
	break;

	   
      //CREATE,UPDATE,DELETE,LIST,CONTROL
	
	//NOTE: also think about /permissions ...
      case STATE_SETSECURITY:
	 // url = url + "/policies/" + _name + "/" + categoriesMap[whatVector.size()-1] + "/" + permissionsMap[securityMap[whatVector.size()-1];
	 sec = va_arg(vl, fluidinfo::security*);
	 categ = va_arg(vl, fluidinfo::categories*);
	 xpath = va_arg(vl, std::string*);
	 curl_easy_setopt(handle, CURLOPT_PUT, 1);
	 
	 url = url + "/policies/" + _name + "/" + categoriesMap[*categ] + "/" + permissionsMap[*sec];
	 
	 if ( *xpath != "" )
	   url = url + "/" + *xpath;
	  
	  //at the end
	  securityMap[*categ] = *sec;
	  xpathMap[*categ] = *xpath;
	  reverse_xpathMap[*xpath] = *categ;
	  break;
	  
      case STATE_GETSECURITY:
	 sec = va_arg(vl, fluidinfo::security*);
	 categ = va_arg(vl, fluidinfo::categories*);
	 xpath = va_arg(vl, std::string*);
	 
	 url = url + "/policies/" + _name + "/" + categoriesMap[*categ] + "/" + permissionsMap[*sec];
 
	 if ( *xpath != "" )
	   url = url + "/" + *xpath;
	 
	  curl_easy_setopt(handle, CURLOPT_HTTPGET, 1);
	  
	  //update security map
	 // *sec = securityMap[categ_tmp];
	  break;
    };
    
    curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
    
    dirty = false;
}


