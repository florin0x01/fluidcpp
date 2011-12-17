#ifndef _FLUID_OBJECT_
#define _FLUID_OBJECT_

#include "generic_stuff.h"
#include "fluid_security.h"
#include "fluid_session_details.h"
#include <boost/thread/mutex.hpp>


//TODO also implement /about functionality here

namespace fluidinfo{
	
class Object: public SessionDetails
{
public:
	typedef std::auto_ptr<Object> Ptr ;	
	
	explicit Object():_id(""),_uri(""),_about(""),
			bufferGetTagValue_(NULL), idx_bufferGetTagValue_(0),
			bufferGetIdsByQuery_(NULL), idx_bufferGetIdsByQuery_(0), 
			bufferGetTagPaths_(NULL), idx_bufferGetTagPaths_(0) { }
	
	explicit Object(const std::string& id):_id(id), _uri(""), _about(""),
			bufferGetTagValue_(NULL), idx_bufferGetTagValue_(0),
			bufferGetIdsByQuery_(NULL), idx_bufferGetIdsByQuery_(0), 
			bufferGetTagPaths_(NULL),idx_bufferGetTagPaths_(0) { }
	
	virtual ~Object();

	void create();

	void setId(const std::string& id) { _id = id; }
	void isAbout(std::string about="") { _about = about; }
	std::string getAbout() { return _about; }
	void setName(std::string name) { _name = name; }
	void delTag(const std::string& tag, const std::string& tagPath="");
	static bool hasTag(const std::string& id, const std::string& tag, Session& );
	
	void putTag(const std::string& tag, const std::string& tagPath, const std::string& value);
	void put(const std::string& tag, const std::string& tagPath, const std::string& filePath);
	
	void setError(std::string err);	
	
	std::string getTagValue(std::string tag);
	std::vector<std::string> getTagPaths(bool cached=false);
	static Ptr getById(const std::string& , const Session& );
	
	//Looks like fluidinfo does not have a function to get the URI based on the ID
	std::string getURI() { return _uri; }
	std::string getId() { return _id; }
	
		
	//this should be static somehow...
	static std::vector<std::string> getIdsByQuery(const std::string& query, Session& session);
	
	std::vector<std::string> ids;

	//should be protected
	std::vector<std::string> _tagPaths;
	//should be protected
	std::map<std::string,std::string> _tagMap; //tag name, tag value
	
protected:
	std::string _name;
	std::string _about;
	std::string _id;
	std::string _uri;
	

	static size_t FWdelTag(void* ptr, size_t size, size_t nmemb, void* p);
	
	static size_t FWhasTag(void* ptr);
	static size_t FWputTag(void *ptr, size_t size, size_t nmemb, void* p);
	static size_t FWputBlob(void *ptr, size_t size, size_t nmemb, void* p);
	static size_t FWcreate(void *ptr, size_t size, size_t nmemb, void* p);
	static size_t FWgetTagValue(void *ptr, size_t size, size_t nmemb, void* p);
	static size_t FWgetTagPaths(void *ptr, size_t size, size_t nmemb, void* p);
	static size_t FWgetIdsByQuery(void *ptr, size_t size, size_t nmemb, void* p);

	char *bufferGetTagValue_;
	uint32_t idx_bufferGetTagValue_;
	
	char* bufferGetIdsByQuery_;
	uint32_t idx_bufferGetIdsByQuery_;
	
	char* bufferGetTagPaths_;
	uint32_t idx_bufferGetTagPaths_;
	
	static std::vector<FILE*> openFiles;
	
	std::string delete_request;
	std::string doc;
	
	bool dirty; //if object is out of sync with Fluidinfo
private:
	bool hasTagResponse_;
};

}
#endif
