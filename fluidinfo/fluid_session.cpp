/* 
Copyright 2011 Nicolae Florin Petrovici
 
This file is part of the Fluidinfo C++ library. 
For more information, regarding Fluidinfo, visit http://www.fluidinfo.com

Fluidinfo C++ library is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Fluidinfo C++ library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Fluidinfo C++ library.  If not, see <http://www.gnu.org/licenses/>.
*/  

#include "fluid_session.h"
#include "fluid_session_details.h"

size_t fluidinfo::Session::HeaderFunction(void* ptr, size_t size, size_t nmemb, void* user)
{
   std::cerr << "---> " << (char*)ptr ;
   
   //if( strstr((char*)ptr, "Unauthorized") )
	//	throw std::logic_error("Unauthorized");
   
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
	//	if ( whatStr == "Unauthorized" )
		//	throw std::logic_error("Unauthorized error. Check username/password");
		p->setError(whatStr);
		
		if ( p->parentSession )
			if ( p->parentSession->cbMap_.find(whatStr) != p->parentSession->cbMap_.end() )
			{
				cbStruct s;
				s = p->parentSession->cbMap_[whatStr];
				s.cb(s.obj);
			}
    }
	
    return size * nmemb;
}

void fluidinfo::Session::RegisterCallback(const std::string&err, cbFunction cb, void* obj )
{
	cbStruct s;
	s.cb = cb;
	s.obj = obj;
	cbMap_[err] = s;
}
