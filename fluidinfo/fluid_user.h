#ifndef _FLUID_USER_
#define _FLUID_USER_

#include "generic_stuff.h"
#include "fluid_security.h"
#include "fluid_session_details.h"

namespace fluidinfo{
	class User: public SessionDetails{
		public:
			User() { }
			User(std::string name):_name(name) { }
			~User();
			void set(std::string name) { 
			   
			      state = STATE_NAME;
			      doState(state);
			  
			}
		
			void setSecurity(security , categories categ, std::string xpath);
			
			void getSecurity(security &, categories categ, std::string xpath);
			std::string getName() { return _name; }
			std::string getId() { return _id; }
	
		protected:
			std::string _name;
			std::string _id;
			bool dirty;
			bool autoCommit;
			security _securityObj;	
			
			bool doState(USER_States state, ...);
			
			enum USER_States {
			    STATE_ZERO = 0,
			    STATE_NAME = 1,
			    STATE_SETSECURITY = 2,
			    STATE_GETSECURITY = 3
			};
			
			std::map<categories, security> securityMap;
			std::map<categories, std::string> xpathMap;
			std::map<std::string, categories> reverse_xpathMap;
			USER_States state;
	
	};
}
#endif
