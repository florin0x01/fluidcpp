#include "fluid_session.h"
#include "fluid_session_details.h"

size_t fluidinfo::Session::HeaderFunction(void* ptr, size_t size, size_t nmemb, void* user)
{
    //std::cerr << "Header Function " << std::endl;
   //std::cerr << "---> " << (char*)ptr ;
   
   if ( strstr((char*)ptr, "Content-Length:") )
   {
	fluidinfo::SessionDetails *p = (fluidinfo::SessionDetails*)user;
	p->lastContentLength = atoi(strchr((char*)ptr, ':')+2);
	//std::cerr << "CL: " << p->lastContentLength << "\n";
   }
   
    if ( strstr((char*)ptr, "X-Fluiddb-Error-Class:") )
    {
		//should we throw directly?
		fluidinfo::SessionDetails *p = (fluidinfo::SessionDetails*)user;
		
		char *what = strchr((char*)ptr, ':')+2;
		if ( NULL == what )
			p->setError("Unknown error");
		
		if ( what[0] == 'T' ) what++;
		what[strlen(what)-1] = '\0';
		what[strlen(what)-1] = '\0';
		std::string whatStr = what;
		std::cerr << "ERRR: " << whatStr << "\n";
		if ( whatStr == "Unauthorized" )
			throw std::logic_error("Unauthorized error. Check username/password");
		p->setError(whatStr);
		
		if ( p->parentSession )
			if ( p->parentSession->cbMap_.find(whatStr) != p->parentSession->cbMap_.end() )
			{
				cbStruct s;
				s = p->parentSession->cbMap_[whatStr];
				s.cb(s.obj);
			}
    }
		
    //useless if exception gets thrown
    return size * nmemb;
}

void fluidinfo::Session::RegisterCallback(const std::string&err, cbFunction cb, void* obj )
{
	cbStruct s;
	s.cb = cb;
	s.obj = obj;
	cbMap_[err] = s;
}
