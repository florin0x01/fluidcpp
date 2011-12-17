#ifndef FLUID_ABOUT_H
#define FLUID_ABOUT_H

#include "generic_stuff.h"
#include "fluid_security.h"
#include "fluid_session_details.h"
#include <vector>
#include <string>

namespace fluidinfo
{
class About: public SessionDetails
{
public:
	explicit About():id_(""), uri_(""), hasTag_(false), bufferGetTagValue_(NULL), idx_bufferGetTagValue_(0),genericValue_("") { }
	typedef std::auto_ptr<About> Ptr;
	static Ptr createObject(const Session&, const std::string& about);
	static Ptr getInfo(const Session&, const std::string& about, std::string tag="");
	static bool hasTag(const Session&, const std::string& about, const std::string& tag);
	static Ptr createOrUpdateTag(const Session&, const std::string& about, const std::string& tag, const std::string& val);
	static Ptr deleteTag(const Session&, const std::string& about, const std::string& tag);
	void setError(std::string err);
	
	const std::string& getId() const { return id_; }
	const std::string& getUri() const { return uri_; }
	const std::vector<std::string>& getTagPaths() const { return tagPaths_; }
	const std::string& getGenericValue() const { return genericValue_; }
	
protected:
	std::string id_;
	std::string uri_;
	std::vector<std::string> tagPaths_;
	bool hasTag_;
	char *bufferGetTagValue_;
	uint32_t idx_bufferGetTagValue_;
	std::string genericValue_;
	
	//callbacks
	static size_t FWcreate(void *ptr, size_t size, size_t nmemb, void* p);
	static size_t FWput(void *ptr, size_t size, size_t nmemb, void* p);
	static size_t FWgetInfo(void *ptr, size_t size, size_t nmemb, void* p);
};
}

#endif