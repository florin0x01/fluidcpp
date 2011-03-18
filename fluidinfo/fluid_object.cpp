#include "fluid_object.h"

using namespace std;

size_t XXX(void* ptr, size_t size, size_t nmemb, void* p)
{
cout << "I'm here " << endl;
  fluidinfo::Object *x = (fluidinfo::Object*)p;
  size_t recsize = size * nmemb;
  if  ( recsize ) {
    char *buf = new char[recsize+1];
    memset(buf,0,sizeof(x));
    memcpy(buf, ptr, recsize);
  //#ifdef FLUID_DEBUG
	cout << "FWCreate(): " << buf << endl;
  //#endif
    delete[] buf;
  }
  return recsize;	
}

void fluidinfo::Object::create()
{

  init();
  
  string url = mainURL;
  string doc = "";
  
  url = url + "/objects";
  
  cout << "Url is " << url << endl;
  
  CURLcode c;
  
  curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
  FILE *t = fdopen(1, "wt");
  if ( t == NULL )
	  cout << "Fail " << endl;
  c = curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, NULL);
  if ( c != CURLE_OK ) 
	  cout << "WRITEFUNCTION failed: " << c << endl;
  
  
  c = curl_easy_setopt(handle, CURLOPT_WRITEDATA, t);
  if  ( c != CURLE_OK )
	cout << "WRITEDATA failed: " << c << endl;
  
  
  curl_easy_setopt(handle, CURLOPT_POST, 1);
  
  dirty = true;

  Json::Value root;
  Json::FastWriter writer;
  
  if ( _about != "" ) {
    root["about"] = _about;  
    doc = writer.write(root);
    if ( root == Json::nullValue )
     doc = "";  
  }
 
 urlencode(doc);
  
  cout << "Will write " << endl << (char*)doc.c_str() << endl;
 
   //or set CURLOPT_POSTFIELDSIZE to 0?!!
   if ( doc != "" ) {
	curl_easy_setopt(handle, CURLOPT_POSTFIELDS, (char*)doc.c_str());
	curl_easy_setopt(handle, CURLOPT_POSTFIELDSIZE, doc.size()); 
   }
   else
	curl_easy_setopt(handle, CURLOPT_POSTFIELDSIZE, 0);
  
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

	cout << "I'm here " << endl;
  fluidinfo::Object *x = (fluidinfo::Object*)p;
  size_t recsize = size * nmemb;
  if  ( recsize ) {
    char *buf = new char[recsize+1];
    memset(buf,0,sizeof(x));
    memcpy(buf, ptr, recsize);
  //#ifdef FLUID_DEBUG
	cout << "FWCreate(): " << buf << endl;
  //#endif
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
}

size_t fluidinfo::Object::FWputTag(void* ptr, size_t size, size_t nmemb, void* p)
{
   fluidinfo::Object *x = (fluidinfo::Object*)p;
}

size_t fluidinfo::Object::FWgetByTag(void* ptr, size_t size, size_t nmemb, void* p)
{
   fluidinfo::Object *x = (fluidinfo::Object*)p;
}

size_t fluidinfo::Object::FWsetName(void* ptr, size_t size, size_t nmemb, void* p)
{
   fluidinfo::Object *x = (fluidinfo::Object*)p;
}

