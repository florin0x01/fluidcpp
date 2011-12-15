#include "fluid_ns.h"
#include "fluid_tag.h"

using namespace std;

fluidinfo::Namespace::~Namespace()
{

}

void fluidinfo::Namespace::setError(string err)
{
    fluidinfo::SessionDetails::setError(err);
	if ( _err == "NamespaceAlreadyExists" ) {
		fresh = false;
		_nonexistent = false;
	}

	if ( _err == "JsonParseError" ) {

	}

	if ( _err == "NonexistentNamespace" ) {
		_nonexistent = true;
	}
	
}


void fluidinfo::Namespace::getSubNamespaceInfo(const std::string& subns, fluidinfo::Namespace& nsret, bool returnDescription, bool returnTags, bool returnNamespaces)
{
 
  init();
 
  string url = mainURL;
  
  std::string returndesc = ( (returnDescription==false) ? "False" : "True" );
  std::string returntags = ( (returnTags == false) ? "False" : "True" ); 
  std::string returnnamespaces = ( (returnNamespaces == false) ? "False" : "True" );
  
  url = url + "/namespaces/" + _nameChain + "/" + subns + "?returnDescription=" + returndesc + "&returnNamespaces=" + returnnamespaces + "&returnTags=" + returntags;
  
  runCURL(GET, url, NULL, FWgetSubNamespaceInfo, reinterpret_cast<fluidinfo::Namespace*>(&nsret));  
}

void fluidinfo::Namespace::addTag(const Tag& tag, bool indexed)
{
    init();
    
	Json::Value root;
	root["description"] = tag.Description();
    root["indexed"] = (indexed == false) ? "false" : "true";
    root["name"] = tag.Name();
    	
	// !!!
    runCURL(POST, mainURL + "/tags/" + _nameChain, &root, FWaddTag, const_cast<fluidinfo::Tag*>(&tag));
}

void fluidinfo::Namespace::updateDescription(const std::string& description)
{
   //set the Content Type to primitive value
   init(false, "Content-Type: application/vnd.fluiddb.value+json");
   Json::Value root;
  root["description"] = description;
  runCURL(PUT, mainURL + "/namespaces/" + _nameChain, &root, FWupdateDescription);
}

void fluidinfo::Namespace::Delete()
{
  init();
  runCURL(DELETE, mainURL + "/namespaces/" + _nameChain);
}

void fluidinfo::Namespace::create(const std::string& parentNs)
{
  init();
  string url = mainURL;
  //something fishy here regarding the relative path of the subnamespaces
  
  if (parentNs == "")
    url = url + "/namespaces/" + parentSession->AuthObj.username;
  
  else {
    url = url + "/namespaces/" + parentSession->AuthObj.username + "/" + parentNs;
    _nameChain = _nameChain + "/" + parentNs;
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
  if (done) 
    return 0;
   memcpy(ptr, p, strlen((const char*)p));
   //std::cerr << "Ptr: " << (char*)ptr << std::endl;
   
   /*
   std::cerr << "size: " << size << std::endl;
   std::cerr << "nmemb: " << nmemb << std::endl;
   std::cerr << "ptr: " << (char*)ptr << std::endl;
   std::cerr << "recsize: " << recsize << std::endl;
   */
   
   done = 1;
   return strlen((const char*)p);
}

size_t fluidinfo::Namespace::FWaddTag ( void* ptr, size_t size, size_t nmemb, void* p )
{
  
    fluidinfo::Tag *x = (fluidinfo::Tag*)p;
    size_t recsize = size * nmemb;
    if  ( recsize ) {
	char *buf = new char[recsize+1];
	memset(buf,0,recsize+1);
	memcpy(buf, ptr, recsize);
    #ifdef FLUID_DEBUG
	cerr << "FWaddTag(): " << buf << endl;
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

size_t fluidinfo::Namespace::FWgetSubNamespaceInfo(void* ptr, size_t size, size_t nmemb, void* p)
{

  fluidinfo::Namespace *ns = (fluidinfo::Namespace*)p;
  
    Json::Reader r;
    Json::Value root;
      
    r.parse((char*)ptr, root);
    
    Json::Value::Members members = root.getMemberNames();
    
     //std::cerr << root << std::endl;
    
    for(Json::Value::Members::iterator it = members.begin(); it != members.end(); ++it)
    {
     
	if ( *it == "tagNames" ) {
	     const Json::Value tagNames = root["tagNames"];
	     for( int index = 0; index < tagNames.size(); index++) 
	     {
		//tagNames[index].asString();
		ns->_tagNames.push_back(tagNames[index].asString());
	     }
	}
	
	else if ( *it == "namespaceNames" ) {
	      const Json::Value namespaceNames = root["namespaceNames"];
	      for( int index = 0; index < namespaceNames.size(); index++)
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
      
    //delete[] buf;
  
  return size * nmemb;
}


