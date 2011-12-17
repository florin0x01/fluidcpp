#include "fluid_ns.h"
#include "fluid_tag.h"

using namespace std;

std::string fluidinfo::Namespace::gTemp = "";

fluidinfo::Namespace::~Namespace()
{

}

//We could also have done the Delete() functionality via the setError...
void fluidinfo::Namespace::setError(string err)
{
    fluidinfo::SessionDetails::setError(err);
    if ( _err == "NamespaceAlreadyExists" ) {
        fresh = false;
        _nonexistent = false;
		std::cerr << "ALLL \n";
    }

    if ( _err == "JsonParseError" ) {

    }

    if ( _err == "NonexistentNamespace" ) {
        _nonexistent = true;
    }

}


void fluidinfo::Namespace::getSubNamespaceInfo(const std::string& path, Namespace& ns, Session& session, bool returnDescription, bool returnTags, bool returnNamespaces)
{

	ns.setParentSession(&session);
    ns.init();

    string url = ns.mainURL;

    std::string returndesc = ( (returnDescription==false) ? "False" : "True" );
    std::string returntags = ( (returnTags == false) ? "False" : "True" );
    std::string returnnamespaces = ( (returnNamespaces == false) ? "False" : "True" );

    url = url + path + "?returnDescription=" + returndesc + "&returnNamespaces=" + returnnamespaces + "&returnTags=" + returntags;

    ns.runCURL(GET, url, NULL, FWgetSubNamespaceInfo, const_cast<Namespace*>(&ns));
}




void fluidinfo::Namespace::addTag(const std::string& tagv, const std::string& description, bool indexed,
                                  std::string& uri, std::string& id)
{

    Tag::ptr tag(new Tag());

    Json::Value root;
    root["description"] = description;
    root["indexed"] = indexed;
    root["name"] = tagv;

    // !!!
    runCURL(POST, mainURL + "/tags/" + _nameChain, &root, FWaddTag, const_cast<fluidinfo::Tag*>(tag.get()));
    uri = tag->Uri();
    id = tag->Id();
}


void fluidinfo::Namespace::addTag(const Tag& tag, bool indexed)
{
    init();

    Json::Value root;
    root["description"] = tag.Description();
    root["indexed"] = indexed;
    root["name"] = tag.Name();

    // !!!
    runCURL(POST, mainURL + "/tags/" + _nameChain, &root, FWaddTag, const_cast<fluidinfo::Tag*>(&tag));
}

void fluidinfo::Namespace::updateDescription(const std::string& description)
{
    init();
    Json::Value root;
    root["description"] = description;
    runCURL(PUT, mainURL + "/namespaces/" + _nameChain, &root, FWupdateDescription);
}

bool fluidinfo::Namespace::Delete()
{
    init();
	parentSession->RegisterCallback("NamespaceNotEmpty", FWdelete, const_cast<Namespace*>(this));
    runCURL(DELETE, mainURL + "/namespaces/" + _nameChain);
	if ( _nsNotEmpty == false )
		_nameChain = parentSession->AuthObj.username;
	return !_nsNotEmpty;
}

void fluidinfo::Namespace::create(const std::string& parentNs)
{
    //NEED to supply a name via the set function
    if ( _name == "" )
	{
		setError("Name not supplied in Namespace::create() method");
        return;
	}
    init();
    string url = mainURL;
    //something fishy here regarding the relative path of the subnamespaces

    std::cerr << "Parentns: " << parentNs << std::endl;

    if (parentNs == "")
    {
        url = url + "/namespaces/" +  parentSession->AuthObj.username;
        _nameChain = _nameChain + "/" + _name;
    }
    else {
        url = url + "/namespaces/" + parentSession->AuthObj.username + "/" + parentNs;
        // _nameChain = _nameChain + "/" + parentNs;
        std::cerr << "Namechain1: " << _nameChain << "\n";
        _nameChain = _nameChain + "/" + parentNs + "/" + _name  ;
        std::cerr << "Namechain2: " << _nameChain << "\n";
    }
    cerr << "Url is " << url << endl;
    Json::Value root;
    root["description"] = _description;
    root["name"] = _name;

    runCURL(POST, url, &root, FWcreate);
}

//callbacks

size_t fluidinfo::Namespace::FWcreate(void* ptr, size_t size, size_t nmemb, void* p)
{

    fluidinfo::Namespace *x = (fluidinfo::Namespace*)p;
    size_t recsize = size * nmemb;
    if  ( recsize ) {
        char *buf = new char[recsize+1];
        memset(buf,0,recsize+1);
        memcpy(buf, ptr, recsize);
#ifdef FLUID_DEBUG
        cerr << "FWCreate(): " << buf << endl;
#endif

        Json::Reader r;
        Json::Value root;

        r.parse(buf, root);

        x->_id =  root["id"].asString();
        x->_uri = root["URI"].asString();

        delete[] buf;
        x->_nonexistent = false;
    }

    return recsize;

}

size_t fluidinfo::Namespace::FWupdateDescription(void* ptr, size_t size, size_t nmemb, void* p)
{
    //must do this some other way
    static int done = 0;
    if (!done)
    {
        memcpy(ptr, p, strlen((const char*)p));
        done = 1;
        return strlen((const char*)p);
    }
    else
    {
        done = 0;
        return 0;
    }
}

size_t fluidinfo::Namespace::FWaddTag ( void* ptr, size_t size, size_t nmemb, void* p )
{

    fluidinfo::Tag *x = (fluidinfo::Tag*)p;
    size_t recsize = size * nmemb;
    if  ( recsize )
    {
        char *buf = new char[recsize+1];
        memset(buf,0,recsize+1);
        memcpy(buf, ptr, recsize);
#ifdef FLUID_DEBUG
        //	cerr << "FWaddTag(): " << buf << endl;
#endif

        Json::Reader r;
        Json::Value root;

        r.parse(buf, root);

        x->Id() =  root["id"].asString();
        x->Uri() = root["URI"].asString();

        delete[] buf;
    }


    return recsize;
}

size_t fluidinfo::Namespace::FWdelete(void* p)
{
   fluidinfo::Namespace *x = (fluidinfo::Namespace*)p;
   x->_nsNotEmpty = true;
   return 0;
}

size_t fluidinfo::Namespace::FWgetSubNamespaceInfo(void* ptr, size_t size, size_t nmemb, void* p)
{

    fluidinfo::Namespace *ns = (fluidinfo::Namespace*)p;
	std::cerr << "Content length: " << ns->lastContentLength << std::endl;

	if ( ns->idx_bufferGetNsInfo_ < ns->lastContentLength )
	{
		if ( !ns->bufferGetNsInfo_)
		{
			ns->bufferGetNsInfo_ = new char[ns->lastContentLength+1];
			memset(ns->bufferGetNsInfo_, 0, ns->lastContentLength+1);
			ns->idx_bufferGetNsInfo_ = 0;	
		}
		memcpy(ns->bufferGetNsInfo_ + ns->idx_bufferGetNsInfo_, ptr, size * nmemb);
		ns->idx_bufferGetNsInfo_ += size * nmemb;
	
		if ( ns->idx_bufferGetNsInfo_ < ns->lastContentLength )
			return size * nmemb;
	}
	
		std::cerr << ns->idx_bufferGetNsInfo_ << std::endl;
	
	//Now we have the whole buffer
	ns->idx_bufferGetNsInfo_ = 0;
	if ( !(size * nmemb) ) return 0;
	
	char* buf = ns->bufferGetNsInfo_;

    Json::Reader r;
    Json::Value root;

    r.parse((char*)buf, root);

    Json::Value::Members members = root.getMemberNames();

    for (Json::Value::Members::iterator it = members.begin(); it != members.end(); ++it)
    {

        if ( *it == "tagNames" ) {
			ns->_tagNames.clear();
            const Json::Value tagNames = root["tagNames"];
            for ( int index = 0; index < tagNames.size(); index++)
            {
                //tagNames[index].asString();
                ns->_tagNames.push_back(tagNames[index].asString());
            }
        }

        else if ( *it == "namespaceNames" ) {
			ns->_namespaceNames.clear();
            const Json::Value namespaceNames = root["namespaceNames"];
            for ( int index = 0; index < namespaceNames.size(); index++)
            {
                ns->_namespaceNames.push_back(namespaceNames[index].asString());
            }
        }

        else if ( *it == "id" ) {
            ns->_id = root["id"].asString();
            ns->_nonexistent = false;
        }

        else if ( *it == "description" ) {
            ns->_description = root["description"].asString();
        }

        else {
        }
    }

//    x->_id =  root["id"].asString();
//   x->_uri = root["URI"].asString();

    delete[] buf;

    return size * nmemb;
}


