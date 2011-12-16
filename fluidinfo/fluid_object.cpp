#include "fluid_object.h"
#include <time.h>

using namespace std;

std::vector<FILE*> fluidinfo::Object::openFiles;

namespace fluidinfo {
//UGLY hack for getTagValue
struct MetaObj: public SessionDetails
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

std::vector<std::string> fluidinfo::Object::getIdsByQuery(const string& query, Session& session)
{	
	Object::Ptr obj(new Object());
	obj->setParentSession(&session);
	obj->init();	
	obj->runCURL(GET, obj->mainURL + "/objects?query=" + urlencode(query), 
			NULL, FWgetIdsByQuery, const_cast<Object*>(obj.get())); 
	return obj->ids;
}

bool fluidinfo::Object::hasTag(const std::string&id, const string& tag, Session& session)
{ 	
  Object::Ptr obj(new Object());
  obj->setParentSession(&session);
  obj->init();
  obj->hasTagResponse_ = true;
  session.RegisterCallback("NonexistentTag", FWhasTag, const_cast<Object*>(obj.get()));
  obj->runCURL(HEAD, obj->mainURL + "/objects/" + id + "/" + tag, NULL, NULL);
  return obj->hasTagResponse_;
}

//GET /objects/id - STATIC method
//@param oid of the object
fluidinfo::Object::Ptr fluidinfo::Object::getById(const std::string& oid, const Session& session)
{
	Object::Ptr obj(new Object());
	obj->setParentSession(const_cast<Session*>(&session));
	obj->init();
	obj->runCURL(GET, obj->mainURL + "/objects/" + oid + "?showAbout=True", NULL, 
		     FWgetTagPaths, const_cast<Object*>(obj.get()));
	return obj;
}

//GET /objects/id
vector< string > fluidinfo::Object::getTagPaths(bool cached)
{
	if ( cached )
	{
		std::cerr << "Returning tagPaths \n";
		return _tagPaths;
	}
	if ( _id == "" ) 
	{
	   std::cerr << "Id is empty \n";	 
	   return _tagPaths;
	}
	init();	
	//TODO do we need our username here?
	std::string url = mainURL + "/objects/" + _id + "?showAbout=True";
	std::cerr << "Url: " << url << "\n";

	runCURL(GET, url , NULL, FWgetTagPaths);	
	
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
  
  std::auto_ptr<MetaObj> metaObj(new MetaObj());
  metaObj->obj = this;
  metaObj->tagrequest = tag;
  
  runCURL(GET, mainURL + "/objects/" + _id + "/" + tag, NULL, FWgetTagValue, const_cast<MetaObj*>(metaObj.get()));
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
		url = url + "/objects/" + _id + "/" + parentSession->AuthObj.username + "/" + tagPath + "/" + tag;
	else
		url = url + "/objects/" + _id + "/" + parentSession->AuthObj.username + "/" + tag;
	
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
	try {   static int xx = 0;
   xx++;
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
	Object* obj = (fluidinfo::Object*)p;
	
	if ( obj->idx_bufferGetIdsByQuery_ < obj->lastContentLength )
	{
		if ( !obj->bufferGetIdsByQuery_)
		{
			obj->bufferGetIdsByQuery_ = new char[obj->lastContentLength+1];
			memset(obj->bufferGetIdsByQuery_, 0, obj->lastContentLength+1);
			obj->idx_bufferGetIdsByQuery_ = 0;	
		}
		
		memcpy(obj->bufferGetIdsByQuery_ + obj->idx_bufferGetIdsByQuery_, ptr, size * nmemb);
		obj->idx_bufferGetIdsByQuery_ += size * nmemb;
	
		if ( obj->idx_bufferGetIdsByQuery_ < obj->lastContentLength )
			return size * nmemb;
	}
	
	//we have the whole buffer here
	obj->idx_bufferGetIdsByQuery_ = 0;
	if ( recsize ) 
	{
		char *buf = obj->bufferGetIdsByQuery_;
		
		Json::Reader r;
		Json::Value root;
		r.parse(buf, root);
		
		//x->_tagPaths = root["tagPaths"];
		
		Json::Value ids = root.get("ids", Json::nullValue);
		if ( ids == Json::nullValue )
		{
			delete[] buf;
			return recsize;
		}
		if ( root["ids"].size() )
			obj->ids.clear();

		for (int i =0 ; i < root["ids"].size(); i++) 
		{
			obj->ids.push_back(root["ids"][i].asString());	
		}
			delete[] buf;
	}
	
	return recsize;
}

size_t fluidinfo::Object::FWgetTagPaths(void* ptr, size_t size, size_t nmemb, void* p)
{
   fluidinfo::Object *x = (fluidinfo::Object*)p;

//   std::cerr << "Last content length: " << x->lastContentLength << "\n";
//   std::cerr << "idx: " << x->idx_bufferGetTagPaths_ << "\n";
   
   if ( x->idx_bufferGetTagPaths_ < x->lastContentLength )
   {
	if ( !x->bufferGetTagPaths_ )
	{
	   x->bufferGetTagPaths_ = new char[x->lastContentLength+1];
	   memset(x->bufferGetTagPaths_, 0, x->lastContentLength+1);	
	   x->idx_bufferGetTagPaths_ = 0;
	}
   
	memcpy(x->bufferGetTagPaths_ + x->idx_bufferGetTagPaths_, ptr, size * nmemb);
	x->idx_bufferGetTagPaths_ += size * nmemb;
	
	if ( x->idx_bufferGetTagPaths_ < x->lastContentLength )
		return size * nmemb;
   }
  	
   //Now we have the whole buffer
   size_t recsize = size * nmemb;
   x->idx_bufferGetTagPaths_ = 0;
   
   if ( recsize ) 
   { 
	char *buf = x->bufferGetTagPaths_;	
	Json::Reader r;
	Json::Value root;
	r.parse(buf, root);
	
	Json::Value about = root.get("about", Json::nullValue);
	if ( about != Json::nullValue )
		x->_about = about.asString();
	else	
		x->_about = "";
	
	Json::Value tagPaths = root.get("tagPaths", Json::nullValue);
	if ( tagPaths == Json::nullValue )
	{
		delete[] buf;
		return recsize;
	}
	
	//std::cerr << "Tagpaths size: " << root["tagPaths"].size() << "\n";
	//std::cerr << "About: " << x->_about << "\n";
	
	for (int i =0 ; i < root["tagPaths"].size(); i++) 
	{
		x->_tagPaths.push_back(root["tagPaths"][i].asString());	
	}
	
	
	delete[] buf;
   }
   
 
   return recsize;
}

size_t fluidinfo::Object::FWhasTag(void* p)
{
   fluidinfo::Object *x = (fluidinfo::Object*)p;
   x->hasTagResponse_ = false;
   return 0;
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
   if ( ptr == NULL ) return 0;
   
   static bool completionPutTag_ = false;
   
   if ( completionPutTag_ == false ) 
   {
	   memcpy(ptr, p, strlen((const char*)p));
	   completionPutTag_ = true;
	   return strlen((const char*)p);	
   }
   else
   {
	completionPutTag_ = false;
	return 0;
   }
   
   return 0; 
}

size_t fluidinfo::Object::FWgetTagValue(void* ptr, size_t size, size_t nmemb, void* p)
{
   std::cerr << "in FWGetTagValue() " << std::endl;
   fluidinfo::MetaObj *obj = (fluidinfo::MetaObj*)p;
   fluidinfo::Object *x = (fluidinfo::Object*)obj->obj;
   
   if ( x->idx_bufferGetTagValue_ < x->lastContentLength )
   {
	if ( !x->bufferGetTagValue_ )
	{
	   x->bufferGetTagValue_ = new char[x->lastContentLength+1];
	   memset(x->bufferGetTagValue_, 0, x->lastContentLength+1);	
	   x->idx_bufferGetTagValue_ = 0;
	}
   
	memcpy(x->bufferGetTagValue_ + x->idx_bufferGetTagValue_, ptr, size * nmemb);
	x->idx_bufferGetTagValue_ += size * nmemb;
	
	if ( x->idx_bufferGetTagValue_ < x->lastContentLength )
		return size * nmemb;
   }
   
   std::string tag_request = obj->tagrequest;
   
   size_t recsize = size * nmemb;
   x->idx_bufferGetTagValue_ = 0;
   
   //Now we have the whole buffer
   if ( recsize ) 
   {
	char *buf = x->bufferGetTagValue_;
	x->_tagMap[tag_request] = buf;
	delete[] buf;
   }
   return recsize;
}


