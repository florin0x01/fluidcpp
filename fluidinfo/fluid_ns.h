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
			Namespace(string name,string description=""):_name(name),_description(description) { }
			~Namespace() { } //if autoCommit is true, call del() ?

			void set(string name, string description="") { _name = name; _description=description; }
			void setAutoCommit(bool value) { autoCommit = value; }

			string getName() { return _name; }
			string getDescription() { return _description; }
			vector<Namespace*> getNamespaces(bool returnDescription="", bool returnTags="");

			bool commit(); //commits to Fluidinfo				
			void getSecurity(security&, categories categ);
			void setSecurity(security&, categories categ);
			void del();

		protected:
			string _name;
			string _description;
			vector<Namespace*> _vns;
			vector<string> _tagNames;
		
			security _securityObj;
		
			bool autoCommit;
			bool dirty; //if object is out of sync with Fluidinfo
		
	}; 
}
#endif
