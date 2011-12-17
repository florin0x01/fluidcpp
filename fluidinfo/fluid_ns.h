#ifndef _FLUID_NS_
#define _FLUID_NS_

#include "generic_stuff.h"
#include "fluid_security.h"
#include "fluid_session_details.h"
#include "fluid_tag.h"


/*@@ TODO: operator[int], operator[string] to get namespace by index or string, put some methods in the base impl */
namespace fluidinfo {
class Namespace: public SessionDetails
{
public:
	typedef std::auto_ptr<Namespace> Ptr ;

    Namespace():_name(""), _description(""), _id(""),
				bufferGetNsInfo_(NULL),idx_bufferGetNsInfo_(0)
    {
        _nameChain = "";
		_nsNotEmpty = false;
    }

    Namespace(std::string name,std::string description=""):_name(name),_description(description),_id(""), 
															_uri(""), fresh(true), _nonexistent(true),
															bufferGetNsInfo_(NULL),idx_bufferGetNsInfo_(0)
    {
        // _nameChain = parentSession->AuthObj.username + "/" + _name;
        _nameChain = "";
		_nsNotEmpty = false;
    }

    void setParentSession(Session *p) 
	{
        SessionDetails::setParentSession(p);
		_nameChain = parentSession->AuthObj.username;
		_name = _description = _id = _uri = "";
		_tagNames.clear();
		_namespaceNames.clear();
		bufferGetNsInfo_ = NULL;
		idx_bufferGetNsInfo_ = 0;
		_nsNotEmpty = false;
    }

    virtual ~Namespace();

	void addTag(const std::string& tag, const std::string& description="", bool indexed=true, std::string& uri=gTemp, std::string& id=gTemp);
    void addTag(const Tag& tag, bool indexed=true);

    void set(std::string name, std::string description="") {
		_name = name;
		_description=description;
		fresh=true;
		_nameChain = parentSession->AuthObj.username;
    }

	void setName(const std::string& name) { _name = name; }

    std::string getName() const {
        return _name;
    }
    std::string getDescription() const {
        return _description;
    }
    std::string getId() const {
        return _id;
    }
    std::string getUri() const {
        return _uri;
    }
    
    const std::vector<std::string>& getTagNames() const { return _tagNames; }
    const std::vector<std::string>& getNamespaceNames() const { return _namespaceNames; }

    static void getSubNamespaceInfo(const std::string& path, Namespace& ns, Session& session, bool returnDescription=true, bool returnTags=true, bool returnNamespaces=true);

    void getSecurity(security&, categories categ) { }
    void setSecurity(security&, categories categ) { }
    bool Delete();
    void updateDescription(const std::string &description);
    void create(const std::string& parentNs="");
    void setError(std::string err);
	
	void setPath(const std::string& path) { _nameChain = _nameChain + "/" + path; }
    const std::string& getPath() const { return _nameChain; } 
    
    bool isFresh() {
        return fresh;
    }

    bool exists() {
        return !_nonexistent;
    }

protected:
    std::string _name;
    std::string _nameChain;
    std::string _description;

    std::string _id;
    std::string _uri;
	
	static std::string gTemp;

    std::vector<std::string> _tagNames;
    std::vector<std::string> _namespaceNames;

    security _securityObj;

    bool fresh;
    bool _nonexistent;
	bool _nsNotEmpty;


    //callbacks
    static size_t FWcreate(void *ptr, size_t size, size_t nmemb, void* p);
    static size_t FWgetSubNamespaceInfo(void *ptr, size_t size, size_t nmemb, void* p);
    static size_t FWupdateDescription(void *ptr, size_t size, size_t nmemb, void* p);
	static size_t FWaddTag(void *ptr, size_t size, size_t nmemb, void* p);
	static size_t FWdelete(void *obj);

private:
	char *bufferGetNsInfo_;
	uint32_t idx_bufferGetNsInfo_;

};


}
#endif
