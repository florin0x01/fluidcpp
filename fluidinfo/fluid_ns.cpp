#include "fluid_ns.h"

using namespace std;

fluidinfo::Namespace::~Namespace()
{

}


void fluidinfo::Namespace::getSubNamespaceInfo(const std::string& subns, bool returnDescription, bool returnTags)
{
 // _returnNamespaceDescription = returnDescription;
 // _returnTagsDescription = returnTags;
 
  //clear vector;
 
 for(int i = 0; i < _vns.size(); i++)
 {
    delete _vns[i];
 }
 
  _vns.clear();
 
  init();
 
  string url = mainURL;
  
  std::string returndesc = ( (returnDescription==false) ? "False" : "True" );
  std::string returntags = ( (returnTags == false) ? "False" : "True" ); 
  
  url = url + "/namespaces/" + _name + "/" + subns + "?returnDescription=" + returndesc + "&returnTags=" + returntags;
  
  curl_easy_setopt(handle, CURLOPT_HTTPGET, 1);
  curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
  curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, FWgetSubNamespaceInfo);
  curl_easy_setopt(handle, CURLOPT_WRITEDATA, this);
	
  curl_easy_perform(handle);
  
  
  
}

void fluidinfo::Namespace::create()
{
    init();
  
  string url = mainURL;
  string doc = "";
  
  url = url + "/namespaces/" + parentSession->AuthObj.username;
  
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
    
    for(Json::Value::Members::iterator it = members.begin(); it != members.end(); ++it)
    {
	if ( it->c_str() == "tagNames" ) {
	  
	}
	
	else if ( it->c_str() == "namespaceNames" ) {
	  
	}
	
	else if ( it->c_str() == "id" ) {
	  
	}
	
	else if ( it->c_str() == "description" ) {
	  
	}
	
	else {
	    //??
	}
    }
    
//    x->_id =  root["id"].asString();
 //   x->_uri = root["URI"].asString();
      
    //delete[] buf;
  
  return size * nmemb;
}


