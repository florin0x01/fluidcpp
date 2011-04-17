#include "fluid_object.h"
#include <boost/concept_check.hpp>

using namespace std;

std::vector<std::string> fluidinfo::Object::ids;

void fluidinfo::Object::create()
{

  init();
  
  string url = mainURL;
  string doc = "";
  
  url = url + "/objects";
  
  cout << "Url is " << url << endl;
  
  CURLcode c;
  
  curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
 
  c = curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, FWcreate);
  if ( c != CURLE_OK ) 
	  cout << "WRITEFUNCTION failed: " << c << endl;
  
  
  c = curl_easy_setopt(handle, CURLOPT_WRITEDATA, this);
  if  ( c != CURLE_OK )
	cout << "WRITEDATA failed: " << c << endl;
  
  
  curl_easy_setopt(handle, CURLOPT_POST, 1);
  
  dirty = true;

  Json::Value root;
  
  Json::FastWriter writer;
  //Json::StyledWriter writer;
  
  if ( _about != "" ) {
    root["about"] = _about;  
    doc = writer.write(root);
    if ( root == Json::nullValue ) { 
     doc = "";  
     cout << "Null value " << endl;    
	    
    }
  }

  //char *c_data = (char*)malloc( (strlen(doc.c_str())+1) * sizeof(char));
  
 // auto_vec<char*> c_data(*(new char[strlen(doc.c_str()+1) * sizeof(char)]));
  
  //strcpy(c_data, doc.c_str());
 
  
  cout << "Will write " << doc.c_str() << "with size " << strlen(doc.c_str()) << endl;
 
   //or set CURLOPT_POSTFIELDSIZE to 0?!!
   if ( doc != "" ) {
	curl_easy_setopt(handle, CURLOPT_POSTFIELDSIZE, (long)strlen(doc.c_str())); 
	curl_easy_setopt(handle, CURLOPT_COPYPOSTFIELDS, doc.c_str());
   }
   else
	curl_easy_setopt(handle, CURLOPT_POSTFIELDSIZE, 0);
  
  
   
  //get the info back
  //_uri = ?
  //_id = ?
}

std::vector<std::string> fluidinfo::Object::getIdsByQuery(const string& query)
{
	ids.clear();
	
	init(); //easy handling only
	string url = mainURL;
	
	url = url + "/objects?query=" + urlencode(query);
	curl_easy_setopt(handle, CURLOPT_HTTPGET, 1);
	curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, FWgetIdsByQuery);
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, NULL);
	
	update();
	return this->ids;
}

bool fluidinfo::Object::hasTag(const string& tag)
{
  init();
  string url = mainURL;
  url = url + "/objects/" + _id + "/" + tag;
  
  curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, FWhasTag);
  curl_easy_setopt(handle, CURLOPT_WRITEDATA, this);
  curl_easy_setopt(handle, CURLOPT_NOBODY, 1); //for HEAD requests
  
  update();
  
  return true;
}

//GET /objects/id
vector< string > fluidinfo::Object::getTagPaths(bool cached)
{
	
	init();
	
	if ( cached )
		return _tagPaths;
	
	if ( _id == "" )
	   return _tagPaths;
	
	string url = mainURL;
	url = url + "/objects/" + _id;
	
	curl_easy_setopt(handle, CURLOPT_HTTPGET, 1);
	curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, FWgetTagPaths);
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, this);
	
	update();
	
	return _tagPaths;
}

string fluidinfo::Object::getTagValue(string tag)
{
  if ( _tagMap.find(tag) != _tagMap.end() )
	  return _tagMap[tag];
  
  init();
  
  if ( _about == "")
     return "";
  
  if ( tag == "" )
     return "";
  
  if ( _id == "" )
     return "";
  
  string url = mainURL;
  
  url = url + "/objects/" + _id + "/" + tag;
  
  curl_easy_setopt(handle, CURLOPT_HTTPGET, 1);
  curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
  curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, FWgetTagValue);
  curl_easy_setopt(handle, CURLOPT_WRITEDATA, this);
  
  update();
  
   if ( _tagMap.find(tag) != _tagMap.end() )
	  return _tagMap[tag];
  else
	return "";
	
}

void fluidinfo::Object::putTag(const std::string& tagPath, const std::string& tag)
{
	init();
	
	if ( tag == "" )
	  return;
	
	string url = mainURL;
	string doc = "";
	
	url = url + "/objects/" + _id + "/" + tagPath;
	
	curl_easy_setopt(handle, CURLOPT_PUT, 1);
	curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
	curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, FWputTag);
	curl_easy_setopt(handle, CURLOPT_WRITEDATA, this);
	
	//set CURLOPT_READDATA and CURLOPT_READFUNCTION
	if ( doc != "" ) {
	/*
	    curl_easy_setopt(handle, CURLOPT_POSTFIELDSIZE, (long)strlen(doc.c_str())); 
	    curl_easy_setopt(handle, CURLOPT_COPYPOSTFIELDS, doc.c_str())
	    */;
	}
	//else
	  //  curl_easy_setopt(handle, CURLOPT_POSTFIELDSIZE, 0);
  
	update();
	
	Json::Value root;
	Json::FastWriter writer;
	
	root = tag;
	
	doc = writer.write(root);
	
		
	
	return;
}

size_t fluidinfo::Object::FWgetIdsByQuery(void* ptr, size_t size, size_t nmemb, void* p)
{
	size_t recsize = size * nmemb;

	if ( recsize ) {
		  char *buf = new char[recsize+1];
		  memset(buf,0,recsize+1);
		  memcpy(buf, ptr, recsize);
		  Json::Reader r;
		Json::Value root;
		r.parse(buf, root);
		
		//x->_tagPaths = root["tagPaths"];
		
		Object::ids.reserve(root["ids"].size());
		
		for (int i =0 ; i < root["ids"].size(); i++) 
		{
			Object::ids[i] = root["ids"][i].asString();	
		}
		 delete[] buf;
	}
	
	return recsize;
}

size_t fluidinfo::Object::FWcreate(void* ptr, size_t size, size_t nmemb, void* p)
{


  fluidinfo::Object *x = (fluidinfo::Object*)p;
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

size_t fluidinfo::Object::FWgetTagPaths(void* ptr, size_t size, size_t nmemb, void* p)
{
   fluidinfo::Object *x = (fluidinfo::Object*)p;
   size_t recsize = size * nmemb;
   if ( recsize ) {
	char *buf = new char[recsize+1];
	memset(buf,0,recsize+1);
	memcpy(buf,ptr,recsize);
	
	Json::Reader r;
	Json::Value root;
	r.parse(buf, root);
	
	//x->_tagPaths = root["tagPaths"];
	
	x->_tagPaths.reserve(root["tagPaths"].size());
	
	for (int i =0 ; i < root["tagPaths"].size(); i++) 
	{
		x->_tagPaths[i] = root["tagPaths"][i].asString();	
	}
	
	delete[] buf;
   }
   return recsize;
}

size_t fluidinfo::Object::FWdelTag(void* ptr, size_t size, size_t nmemb, void* p)
{
   fluidinfo::Object *x = (fluidinfo::Object*)p;
}

size_t fluidinfo::Object::FWhasTag(void* ptr, size_t size, size_t nmemb, void* p)
{
   fluidinfo::Object *x = (fluidinfo::Object*)p;
   size_t recsize = size * nmemb;
   if ( recsize ) {
	char *buf = new char[recsize+1];
	memset(buf,0,recsize+1);
	memcpy(buf, ptr, recsize);
	delete[] buf;
   }
   return recsize;
}

size_t fluidinfo::Object::FWputTag(void* ptr, size_t size, size_t nmemb, void* p)
{
   fluidinfo::Object *x = (fluidinfo::Object*)p;
   size_t recsize = size * nmemb;
   if ( recsize ) {
	char *buf = new char[recsize+1];
	memset(buf, 0 , recsize+1);
	memcpy(buf, ptr, recsize);
	delete[] buf;
   }
   return recsize;
}

size_t fluidinfo::Object::FWgetTagValue(void* ptr, size_t size, size_t nmemb, void* p)
{
   fluidinfo::MetaObj *obj = (fluidinfo::MetaObj*)p;
   fluidinfo::Object *x = (fluidinfo::Object*)obj->obj;
   string tag_request = obj->tagrequest;
   
   size_t recsize = size * nmemb;
   if ( recsize ) {
	char *buf = new char[recsize+1];
	memset(buf,0,recsize+1);
	memcpy(buf, ptr, recsize);
	x->_tagMap[tag_request] = buf;
	delete[] buf;
   }
   return recsize;
}

size_t fluidinfo::Object::FWsetName(void* ptr, size_t size, size_t nmemb, void* p)
{
   fluidinfo::Object *x = (fluidinfo::Object*)p;
}

