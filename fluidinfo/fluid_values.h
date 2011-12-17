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