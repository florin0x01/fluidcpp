#include "fluid_ns.h"

using namespace std;

fluidinfo::Namespace::~Namespace()
{

}


void fluidinfo::Namespace::getSubNamespaceInfo(const std::string& subns, fluidinfo::Namespace& nsret, bool returnDescription, bool returnTags, bool returnNamespaces)
{
 
  init();
 
  string url = mainURL;
  
  std::string returndesc = ( (returnDescription==false) ? "False" : "True" );
  std::string returntags = ( (returnTags == false) ? "False" : "True" ); 
  std::string returnnamespaces = ( (returnNamespaces == false) ? "False" : "True" );
  
  url = url + "/namespaces/" + _nameChain + "/" + subns + "?returnDescription=" + returndesc + "&returnNamespaces=" + returnnamespaces + "&returnTags=" + returntags;
  
  std::cout << "Url is : " << url << std::endl;
  
  curl_easy_setopt(handle, CURLOPT_HTTPGET, 1);
  curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
  curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, FWgetSubNamespaceInfo);
  curl_easy_setopt(handle, CURLOPT_WRITEDATA, &nsret);
	
  curl_easy_perform(handle);
  
}

void fluidinfo::Namespace::create(const std::string& parentNs)
{
    init();
  
  string url = mainURL;
  string doc = "";
  
  //something fishy here regarding the relative path of the subnamespaces
  
  if (parentNs == "")
    url = url + "/namespaces/" + parentSession->AuthObj.username;
  
  else {
    url = url + "/namespaces/" + parentSession->AuthObj.username + "/" + parentNs;
    _nameChain = _nameChain + "/" + parentNs;
  }
  cout << "Url is " << url << endl;
  
  CURLcode c;
  
  curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
 
  c = curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, FWcreate );
  if ( c != CURLE_OK ) 
	  cout << "WRITEFUNCTION failed: " << c << endl;
  
  
  c = curl_easy_setopt(handle, CURLOPT_WRITEDATA, this);
  if  ( c != CURLE_OK )
	cout << "WRITEDATA failed: " << c << endl;
  
  
  curl_easy_setopt(handle, CURLOPT_POST, 1);
  
  Json::Value root;
  
  //Json::FastWriter writer;
  
  Json::StyledWriter writer;
  
  root["description"] = _description;
  root["name"] = _name;
  
  doc = writer.write(root);
  
  if ( root == Json::nullValue ) {
      doc = "";
      cout << "Null value " << endl;
  }	

   
  cout << "Will write " << doc.c_str() << "with size " << strlen(doc.c_str()) << endl;
  cout << "Curl handle"  << handle << endl;
 
   //or set CURLOPT_POSTFIELDSIZE to 0?!!
   if ( doc != "" ) {
	curl_easy_setopt(handle, CURLOPT_POSTFIELDSIZE, (long)strlen(doc.c_str())); 
	curl_easy_setopt(handle, CURLOPT_COPYPOSTFIELDS, doc.c_str());
   }
   else
	curl_easy_setopt(handle, CURLOPT_POSTFIELDSIZE, 0);
  
  curl_easy_perform(handle);
   
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
	cout << "FWCreate(): " << buf << endl;
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

size_t fluidinfo::Namespace::FWgetSubNamespaceInfo(void* ptr, size_t size, size_t nmemb, void* p)
{

  fluidinfo::Namespace *ns = (fluidinfo::Namespace*)p;
  
    Json::Reader r;
    Json::Value root;
      
    r.parse((char*)ptr, root);
    
    Json::Value::Members members = root.getMemberNames();
    
     //std::cout << root << std::endl;
    
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


