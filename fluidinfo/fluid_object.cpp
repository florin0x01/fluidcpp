#include "fluid_object.h"
//#include "callbacks.h"
//#include "callbacks.h"
//#include <boost/concept_check.hpp>

using namespace std;

std::vector<std::string> fluidinfo::Object::ids;
std::vector<FILE*> fluidinfo::Object::openFiles;

namespace fluidinfo {
struct MetaObj
{	
	fluidinfo::Object *obj;
	std::string tagrequest;
}; 
}

fluidinfo::Object::~Object()
{

}

void fluidinfo::Object::create()
{

  init();
  Json::Value root;
  if ( _about != "" ) {
    root["about"] = _about;  
    if ( root == Json::nullValue ) 
	{ 
		root["about"] = "";
    }
  }
  
  runCURL(POST, mainURL + "/objects", &root, FWcreate);  
}

std::vector<std::string> fluidinfo::Object::getIdsByQuery(const string& query)
{
	ids.clear();
	
	init();
	runCURL(GET, mainURL + "/objects?query=" + urlencode(query), NULL, FWgetIdsByQuery); 
	return this->ids;
}


bool fluidinfo::Object::hasTag(const string& tag)
{
  init();
  runCURL(HEAD, mainURL + "/objects/" + _id + "/" + tag, NULL, FWhasTag);
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
	//TODO do we need our username here?
	runCURL(GET, mainURL + "/objects/" + _id, NULL, FWgetTagPaths);	
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
  runCURL(GET, mainURL + "/objects/" + _id + "/" + tag, NULL, FWgetTagValue);
     if ( _tagMap.find(tag) != _tagMap.end() )
	  return _tagMap[tag];
  else
	return "";
}

void fluidinfo::Object::delTag(const std::string& tag, const std::string& tagPath)
{
	init();
	
	if ( tag == "" )
	  return;
	
	string url = mainURL;
	
	
	//TODO do we need our username here?
	if ( !tagPath.empty() )
		url = url + "/objects/" + _id + "/" + tagPath + "/" + tag;
	else
		url = url + "/objects/" + _id + "/" + tag;
	
	runCURL(DELETE, url);
	
	return;
}

void fluidinfo::Object::put(const std::string& tagPath, const std::string& tag, const std::string& filePath)
{
     if ( filePath.empty() )
	   return;
     
     if ( tag == "" )
	   return;
     
     string url = mainURL;
     
	 
	 //TOOD do we need our username here?
     if ( !tagPath.empty() )
		url = url + "/objects/" + _id + "/" + tagPath + "/" + tag;
     else
		url = url + "/objects/" + _id + "/" + tag;
     
     //TODO: use boost
     //get file extension
		
     size_t idx = filePath.find(".");
     string extension = "txt";
     
     if ( idx != std::string::npos ) {
	try {
		extension = filePath.substr(idx+1);
	}catch(...) {
		extension = "txt";
	}
     }
     
     //get file size
     //critical section
     unsigned long fileSize;
     FILE* f = fopen(filePath.c_str(), "rb");
     fseek(f, 0L, SEEK_SET);
     fseek(f, 0L, SEEK_END);
     
     fileSize = ftell(f);
     fseek(f, 0L, SEEK_SET);
     
     openFiles.push_back(f);
	
	 //TODO also standardize this by using runCURL
     init(true, "Content-Type: application/" + extension);
     
     curl_easy_setopt(handle, CURLOPT_UPLOAD, 1);
     curl_easy_setopt(handle, CURLOPT_URL, url.c_str());

     curl_easy_setopt(handle, CURLOPT_READFUNCTION, FWputBlob);
     curl_easy_setopt(handle, CURLOPT_READDATA, openFiles[openFiles.size()-1]);
     curl_easy_setopt(handle, CURLOPT_INFILESIZE, fileSize);
     
     curl_easy_perform(handle);
}

void fluidinfo::Object::putTag(const std::string& tag, const std::string& tagPath, const std::string& value)
{
	//set the Content Type to primitive value
	init(false, "Content-Type: application/vnd.fluiddb.value+json");
	
	if ( tag == "" )
	  return;
	
	string url = mainURL;
	
	if ( !tagPath.empty() )
		url = url + "/objects/" + _id + "/" + parentSession->AuthObj.username + "/" + tagPath + "/" + tag;
	else
		url = url + "/objects/" + _id + "/" + parentSession->AuthObj.username + "/" + tag;
	
	std::string valx = value;	
	Json::Value root;

	root = valx;
	
	runCURL(PUT, url, &root, FWputTag);
}

//////CALLBACKS////////////

size_t fluidinfo::Object::FWcreate (void* ptr, size_t size, size_t nmemb, void* p)
{
    fluidinfo::Object *x = (fluidinfo::Object*)p;
    size_t recsize = size * nmemb;
    if  ( recsize ) {
      char *buf = new char[recsize+1];
      memset(buf,0,recsize+1);
      memcpy(buf, ptr, recsize);
    #ifdef FLUID_DEBUG
	  std::cerr << "FWCreate(): " << buf << std::endl;
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
		
		Json::Value ids = root.get("ids", Json::nullValue);
		if ( ids == Json::nullValue )
			return recsize;
		
		if ( root["ids"].size() )
			Object::ids.clear();
	
		for (int i =0 ; i < root["ids"].size(); i++) 
		{
			Object::ids.push_back(root["ids"][i].asString());	
		}
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

size_t fluidinfo::Object::FWhasTag(void* ptr, size_t size, size_t nmemb, void* p)
{
   fluidinfo::Object *x = (fluidinfo::Object*)p;
   size_t recsize = size * nmemb;
   if ( recsize ) {
	char *buf = new char[recsize+1];
	memset(buf,0,recsize+1);
	memcpy(buf, ptr, recsize);
	std::cerr << "buf: " << buf << std::endl;
	delete[] buf;
   }
   return recsize;
}

size_t fluidinfo::Object::FWputBlob(void* ptr, size_t size, size_t nmemb, void* p)
{
   FILE* f = (FILE*)p;
  // ptr = (char*)malloc(512*sizeof(char)*1024); //512kB
   size_t read = 0;
   if(!feof(f))
	 read = fread(ptr, 512*sizeof(char)*1024, 1, f);
   else
	 fclose(f);
   return read;
}

size_t fluidinfo::Object::FWputTag(void* ptr, size_t size, size_t nmemb, void* p)
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

size_t fluidinfo::Object::FWgetTagValue(void* ptr, size_t size, size_t nmemb, void* p)
{
   std::cerr << "in FWGetTagValue() " << std::endl;
   fluidinfo::MetaObj *obj = (fluidinfo::MetaObj*)p;
   fluidinfo::Object *x = (fluidinfo::Object*)obj->obj;
   std::string tag_request = obj->tagrequest;
   
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


