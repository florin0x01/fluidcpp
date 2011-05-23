#include "fluid_ns.h"

using namespace std;

fluidinfo::Namespace::~Namespace()
{

}


vector< Namespace* > fluidinfo::Namespace::getNamespaces(bool returnDescription, bool returnTags)
{
  _returnNamespaceDescription = returnDescription;
  _returnTagsDescription = returnTags;
  return _vns;
}

void fluidinfo::Namespace::create()
{

}

size_t fluidinfo::Namespace::FWcreate(void* ptr, size_t size, size_t nmemb, void* p)
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

