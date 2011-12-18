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

#ifndef FLUID_QUERY_H_
#define FLUID_QUERY_H_

#include "generic_stuff.h"
#include "fluid_security.h"
#include "fluid_session_details.h"
#include <map>

namespace fluidinfo
{
class Values: public SessionDetails
{
public:
	typedef std::auto_ptr<Values> Ptr;
	explicit Values():bufferGetTagPaths_(NULL), idx_bufferGetTagPaths_(0) { }
	static Ptr Get(const Session&, const std::string& query);
	static Ptr Put(const Session&, const Json::Value& query);
	static Ptr Delete(const Session&, const std::string& query);
	const Json::Value& getResults() const { return results; }
	
protected:
	
	char* bufferGetTagPaths_;
	uint32_t idx_bufferGetTagPaths_;
	
	//callbacks
	static size_t FWGet(void *ptr, size_t size, size_t nmemb, void* p);
	static size_t FWput(void *ptr, size_t size, size_t nmemb, void* p);
	static size_t FWdelete(void *ptr, size_t size, size_t nmemb, void* p);
	static std::string formatQuery(const std::string &query);
	
	Json::Value results;
};
}

#endif