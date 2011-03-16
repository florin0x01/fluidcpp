#include "fluid_object.h"

using namespace std;


void fluidinfo::Object::create()
{

  init();
  
  string url = mainURL;
  string doc = "";
  
  url = url + "/objects";
  
  curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
  curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, FWcreate);
  curl_easy_setopt(handle, CURLOPT_WRITEDATA, this);
  curl_easy_setopt(handle, CURLOPT_POST, 1);
  
  dirty = true;

  Json::Value root;
  Json::Writer writer;
  
  if ( _about != "" ) {
    root["about"] = _about;  
    doc = writer.write(root);
    if ( root == Json::nullValue )
     doc = "";  
  }
 
   //or set CURLOPT_POSTFIELDSIZE to 0?!!
   if ( doc != "" )
	curl_easy_setopt(handle, CURLOPT_POSTFIELDS, (char*)doc.c_str());
  
  //get the info back
  //_uri = ?
  //_id = ?
}

bool fluidinfo::Object::hasTag(string tag)
{
  init();
  curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, FWhasTag);
  curl_easy_setopt(handle, CURLOPT_WRITEDATA, this);
  return true;
}


void fluidinfo::Object::getByTag(string tag)
{
  init();
  
  if ( _about == "")
     return;
  
  string url = mainURL;
  
  url = url + "/about" + _about;
  
  curl_easy_setopt(handle, CURLOPT_HTTPGET, 1);
  curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
  curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, FWgetByTag);
  curl_easy_setopt(handle, CURLOPT_WRITEDATA, this);
}

size_t fluidinfo::Object::FWcreate(void* ptr, size_t size, size_t nmemb, void* p)
{
  fluidinfo::Object *x = p;
  size_t recsize = size * nmemb;
  if  ( recsize ) {
    char *buf = new char[recsize+1];
    memset(buf,0,sizeof(x));
    memcpy(buf, ptr, recsize);
  #ifdef FLUID_DEBUG
	cout << "FWCreate(): " << buf << endl;
  #endif
    delete[] buf;
  }
  return recsize;
  
}

size_t fluidinfo::Object::FWdelTag(void* ptr, size_t size, size_t nmemb, void* p)
{
  fluidinfo::Object *x = p;
}

size_t fluidinfo::Object::FWhasTag(void* ptr, size_t size, size_t nmemb, void* p)
{
  fluidinfo::Object *x = p;
}

size_t fluidinfo::Object::FWputTag(void* ptr, size_t size, size_t nmemb, void* p)
{
  fluidinfo::Object *x = p;
}

size_t fluidinfo::Object::FWgetByTag(void* ptr, size_t size, size_t nmemb, void* p)
{
  fluidinfo::Object *x = p;
}

size_t fluidinfo::Object::FWsetName(void* ptr, size_t size, size_t nmemb, void* p)
{
  fluidinfo::Object *x = p;
}

