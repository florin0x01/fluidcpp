#include "fluid_tag.h"
#include "fluid_ns.h"

using namespace std;
using namespace fluidinfo;

void Tag::setParentSession(Session* p)
{
  SessionDetails::setParentSession(p);
  nameChain_ = "tags/" + parentSession->AuthObj.username + "/";
}

void Tag::Add(const Session& session, const std::string& nsPath, const std::string& name, 
			  const std::string& description, bool indexed)
{
  Tag::ptr tag(new Tag());
  tag->setParentSession(const_cast<Session*>(&session));
  tag->init();

  Json::Value root;
  
  root["description"] = description;
  root["indexed"] = indexed;
  root["name"] = name;
 
  tag->runCURL(POST, tag->mainURL + "/tags/" + tag->parentSession->AuthObj.username + "/" + nsPath, 
			   &root, FWAdd, const_cast<Tag*>(tag.get())); 
}

void Tag::UpdateDescription(const Session& session, const std::string& nsPath, const std::string& name, const std::string& description)
{
  Tag::ptr tag(new Tag());
  tag->setParentSession(const_cast<Session*>(&session));
  tag->init();

  Json::Value root;
  root["description"] = description;
  
  tag->runCURL(PUT, tag->mainURL + "/tags/" + tag->parentSession->AuthObj.username + "/" + nsPath + "/" + name, 
			   &root, FWUpdate, const_cast<Tag*>(tag.get()));
}

void Tag::Delete(const Session& session, const std::string&nsPath, const std::string& name)
{
  Tag::ptr tag(new Tag());
  tag->setParentSession(const_cast<Session*>(&session));
  tag->init();
  tag->runCURL(DELETE, tag->mainURL + "/tags/" + tag->parentSession->AuthObj.username + "/" + nsPath + "/" + name, 
			   NULL, FWDelete, const_cast<Tag*>(tag.get()) );
}

Tag::Ptr Tag::Get(const Session& session, const std::string& nsPath, const std::string& name)
{ 
  Tag::ptr tag(new Tag());
  tag->setParentSession(const_cast<Session*>(&session));
  tag->init();
  std::string url = tag->mainURL + "/" + tag->nameChain_ + nsPath + "/" + name;
  url += "?returnDescription=True";
  
  tag->runCURL(GET, url, NULL, FWGet, const_cast<Tag*>(tag.get())); 
  return tag;
}

Tag::Ptr Tag::Get(const fluidinfo::Namespace& ns, const std::string& name)
{
	return Get(*ns.parentSession, ns.getPath(), name);
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
   if ( ptr == NULL ) return 0;
   
   static bool completionUpdate_ = false;
   
   if ( completionUpdate_ == false ) 
   {
	   memcpy(ptr, p, strlen((const char*)p));
	   completionUpdate_ = true;
	   return strlen((const char*)p);	
   }
   else
   {
	completionUpdate_ = false;
	return 0;
   }
   
   return 0; 
}




