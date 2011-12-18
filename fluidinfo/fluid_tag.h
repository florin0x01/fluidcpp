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

#ifndef _FLUID_TAG_H
#define _FLUID_TAG_H

#include "generic_stuff.h"
#include "fluid_security.h"
#include "fluid_session_details.h"

namespace fluidinfo
{

class Namespace;

class Tag: public SessionDetails
{
public:
	typedef std::auto_ptr<Tag> Ptr;
	typedef Ptr ptr;
	
	Tag(const Tag& ) { }
	Tag(): name_(""), description_(""), id_(""), nsPath_(""), nameChain_(""), indexed_(false) { }
	
	void setParentSession(Session *p);
  
	//GETTERS, SETTERS

	std::string& Id() { return id_; }
	std::string& Uri(){ return uri_; }
	
	const std::string& Id() const { return id_; }
	const std::string& Uri() const { return uri_; }
	
	std::string& Name() { return name_; }
	const std::string& Name() const { return name_; }
	
	std::string& Description() { return description_; }
	const std::string& Description() const { return description_; }
	
	const std::string& NamespacePath() const { return nsPath_; }
	
	bool Indexed() { return indexed_ ; }
	
	static Ptr Add(const Session& session, const std::string& nsPath, const std::string& name, 
					const std::string& description="", bool indexed=true);
	static Ptr UpdateDescription(const Session& session, const std::string& nsPath, const std::string& name, 
					   const std::string& description);
//	static void Update(const Tag& tag);
	static Ptr Delete(const Session& session, const std::string& nsPath, const std::string& name);
	
	
	static Ptr Get(const Session& session, const std::string& nsPath, const std::string& name);
	static Ptr Get(const Namespace& ns, const std::string& name);
	
	
private:
	std::string name_;
	std::string description_;
	std::string id_;
	std::string uri_;
	std::string nsPath_;
	std::string nameChain_;
	bool indexed_;
	
protected:
	//callbacks
    static size_t FWAdd		(void *ptr, size_t size, size_t nmemb, void* p);
    static size_t FWUpdate	(void *ptr, size_t size, size_t nmemb, void* p);
	static size_t FWGet		(void *ptr, size_t size, size_t nmemb, void* p);
	static size_t FWDelete	(void *ptr, size_t size, size_t nmemb, void* p);	
	
	friend class Namespace;
		
};

}

#endif