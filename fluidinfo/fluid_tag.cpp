/* 
Copyright 2011 Nicolae Florin Petrovici
 
This file is part of the Fluidinfo C++ library. 
For more information, regarding Fluidinfo, visit http://www.fluidinfo.com

Fluidinfo C++ library is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Fluidinfo C++ library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Fluidinfo C++ library.  If not, see <http://www.gnu.org/licenses/>.
*/  

#include "fluid_tag.h"
#include "fluid_ns.h"

using namespace std;
using namespace fluidinfo;

void Tag::setParentSession(Session* p)
{
  SessionDetails::setParentSession(p);
  nameChain_ = "tags/" + parentSession->AuthObj.username + "/";
}

Tag::Ptr Tag::Add(const Session& session, const std::string& nsPath, const std::string& name, 
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
  return tag;
}

Tag::Ptr Tag::UpdateDescription(const Session& session, const std::string& nsPath, const std::string& name, const std::string& description)
{
  Tag::ptr tag(new Tag());
  tag->setParentSession(const_cast<Session*>(&session));
  tag->init();

  Json::Value root;
  root["description"] = description;
  
  tag->runCURL(PUT, tag->mainURL + "/tags/" + tag->parentSession->AuthObj.username + "/" + nsPath + "/" + name, 
			   &root, FWUpdate, const_cast<Tag*>(tag.get()));
  return tag;
}

Tag::Ptr Tag::Delete(const Session& session, const std::string&nsPath, const std::string& name)
{
  Tag::ptr tag(new Tag());
  tag->setParentSession(const_cast<Session*>(&session));
  tag->init();
  tag->runCURL(DELETE, tag->mainURL + "/tags/" + tag->parentSession->AuthObj.username + "/" + nsPath + "/" + name, 
			   NULL, FWDelete, const_cast<Tag*>(tag.get()) );
  return tag;
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
  Tag::ptr tag(new Tag());
  tag->setParentSession(ns.parentSession);
  tag->init();
  std::string url = tag->mainURL + "/tags/" + ns.getPath() + "/" + name;
  url += "?returnDescription=True";
  tag->runCURL(GET, url, NULL, FWGet, const_cast<Tag*>(tag.get())); 
  return tag;
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




