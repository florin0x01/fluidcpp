#ifndef _FLUID_TAG_H
#define _FLUID_TAG_H

#include "generic_stuff.h"
#include "fluid_security.h"
#include "fluid_ns.h"
#include "fluid_session_details.h"

namespace fluidinfo
{

class Tag: public SessionDetails
{
public:
	Tag(): name_(""), description_(""), id_(""), nsPath_(""), indexed_(false) { }
	
	//GETTERS, SETTERS
	std::string& Name() { return name_; }
	const std::string& Name() const { return name_; }
	
	std::string& Description() { return description_; }
	const std::string& Description() const { return description_; }
	
	const std::string& NamespacePath() const { return nsPath_; }
	
	bool Indexed() { return indexed_ ; }
	
	static void Add(const std::string& name, const std::string& nsPath, bool indexed=true);
	static void Update(const std::string& name, const std::string& description, const std::string& nsPath);
	static void Update(const Tag& tag);
	
	Tag Get(const std::string& nsPath, bool returnDescription=true);
	Tag Get(const Namespace& ns, bool returnDescription=true);
private:
	Tag(const Tag& );
	std::string name_;
	std::string description_;
	std::string id_;
	std::string nsPath_;
	bool indexed_;
		
};

}

#endif