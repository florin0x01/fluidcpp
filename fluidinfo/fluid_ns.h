#ifndef _FLUID_NS_
#define _FLUID_NS_

#include "generic_stuff.h"
#include "fluid_security.h"
#include "fluid_session_details.h"


/*@@ TODO: operator[int], operator[string] to get namespace by index or string, put some methods in the base impl */
namespace fluidinfo{
	class Namespace: public SessionDetails
	{
		public:
			Namespace() { _name=""; }
			Namespace(std::string name,std::string description=""):_name(name),_description(description) { }
			virtual ~Namespace();

			void set(std::string name, std::string description="") { _name = name; _description=description; }
	
			std::string getName() { return _name; }
			std::string getDescription() { return _description; }
			std::vector<Namespace*> getNamespaces(bool returnDescription="", bool returnTags="");
			
			void getSecurity(security&, categories categ);
			void setSecurity(security&, categories categ);
			void del();
			void create();

		protected:
			std::string _name;
			std::string _description;
		
			std::string _id;
			std::string _uri;
			
			std::vector<Namespace*> _vns;
			std::vector<std::string> _tagNames;
		
			security _securityObj;
			
			
			//callbacks
			static size_t FWcreate(void *ptr, size_t size, size_t nmemb, void* p);

		
	}; 
	
	
}
#endif
