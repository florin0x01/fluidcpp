#ifndef _FLUID_NS_
#define _FLUID_NS_

#include <generic_stuff.h>
#include "fluid_security.h"

/*@@ TODO: operator[int], operator[string] to get namespace by index or string, put some methods in the base impl */
namespace fluidinfo{
	class Namespace
	{
		public:
			Namespace() { _name=""; }
			Namespace(std::string name,std::string description=""):_name(name),_description(description) { }
			~Namespace() { } //if autoCommit is true, call del() ?

			void set(std::string name, std::string description="") { _name = name; _description=description; }
			void setAutoCommit(bool value) { autoCommit = value; }

			std::string getName() { return _name; }
			std::string getDescription() { return _description; }
			std::vector<Namespace*> getNamespaces(bool returnDescription="", bool returnTags="");

			bool commit(); //commits to Fluidinfo				
			void getSecurity(security&, categories categ);
			void setSecurity(security&, categories categ);
			void del();

		protected:
			std::string _name;
			std::string _description;
			std::vector<Namespace*> _vns;
			std::vector<string> _tagNames;
		
			security _securityObj;
		
			bool autoCommit;
			bool dirty; //if object is out of sync with Fluidinfo
		
	}; 
}
#endif
