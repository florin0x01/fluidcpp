#include "fluid_tag.h"
#include "fluid_ns.h"

using namespace std;
using namespace fluidinfo;

void Tag::setParentSession(Session* p)
{
  SessionDetails::setParentSession(p);
  nameChain_ = "tags/" + parentSession->AuthObj.username + "/";
}

void Tag::Add( const std::string& nsPath, const std::string& name, const std::string& description, bool indexed)
{
  Tag::ptr tag(new Tag());
  tag->init();

  Json::Value root;
  
  root["description"] = description;
  root["indexed"] = (indexed == true) ? "true" : "false";
  root["name"] = name;
 
  tag->runCURL(POST, tag->mainURL + "/tags/" + tag->parentSession->AuthObj.username + "/" + nsPath, 
			   &root, FWAdd, const_cast<Tag*>(tag.get())); 
}

Tag Tag::Get(const std::string& nsPath, const std::string& name, bool returnDescription)
{ 
  Tag::ptr tag(new Tag());
  tag->init();
  std::string url = tag->mainURL + "/" + nameChain_ + nsPath + "/" + name;
  if ( returnDescription == true )
	  url += "?returnDescription=True";
  else
	  url += "?returnDescription=False";
  tag->runCURL(GET, url, NULL, FWGet, const_cast<Tag*>(tag.get())); 
  return *tag;
}

Tag Tag::Get(const fluidinfo::Namespace& ns, const std::string& name, bool returnDescription)
{
	return Get(ns.getPath(), name, returnDescription);
}

void Tag::Update(const std::string& nsPath, const std::string& name, const std::string& description)
{
  Tag::ptr tag(new Tag());
  tag->init();

  Json::Value root;
  root["description"] = description;
  
  tag->runCURL(PUT, tag->mainURL + "/tags/" + tag->parentSession->AuthObj.username + "/" + nsPath + "/" + name, 
			   &root, FWUpdate, const_cast<Tag*>(tag.get()));
}

void Tag::Delete(const std::string&nsPath, const std::string& name)
{
  Tag::ptr tag(new Tag());
  tag->init();
  tag->runCURL(DELETE, tag->mainURL + "/tags/" + tag->parentSession->AuthObj.username + "/" + nsPath + "/" + name, 
			   NULL, FWDelete, const_cast<Tag*>(tag.get()) );
}

size_t Tag::FWAdd(void* ptr, size_t size, size_t nmemb, void* p)
{
	fluidinfo::Tag *x = (fluidinfo::Tag*)p;
    size_t recsize = size * nmemb;
    if  ( recsize ) {
      char *buf = new char[recsize+1];
      memset(buf,0,recsize+1);
      memcpy(buf, ptr, recsize);
    #ifdef FLUID_DEBUG
	  std::cerr << "FWAdd(): " << buf << std::endl;
    #endif

      Json::Reader r;
      Json::Value root;
      
      r.parse(buf, root);
      
      x->id_ =  root["id"].asString();
      x->uri_ = root["URI"].asString();
      
      delete[] buf;
    }
    
    return recsize;
}

size_t Tag::FWDelete(void* ptr, size_t size, size_t nmemb, void* p)
{
//nothing to put here
}

size_t Tag::FWGet(void* ptr, size_t size, size_t nmemb, void* p)
{
	fluidinfo::Tag *x = (fluidinfo::Tag*)p;
    size_t recsize = size * nmemb;
    if  ( recsize ) {
      char *buf = new char[recsize+1];
      memset(buf,0,recsize+1);
      memcpy(buf, ptr, recsize);
    #ifdef FLUID_DEBUG
	  std::cerr << "FWAdd(): " << buf << std::endl;
    #endif

      Json::Reader r;
      Json::Value root;
      
      r.parse(buf, root);
      
      x->indexed_ =  ("false" == root["indexed"].asString()) ? false : true;
      x->id_ = root["id"].asString();
	  Json::Value desc = root.get("description",Json::nullValue);
      if ( desc != Json::nullValue )
		x->description_ = desc.asString();
	  
      delete[] buf;
    }
    
    return recsize;
}

size_t Tag::FWUpdate(void* ptr, size_t size, size_t nmemb, void* p)
{
//nothing to put here
}




