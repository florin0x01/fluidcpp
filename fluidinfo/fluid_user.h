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
			~User(); //cannot delete user if autoCommit is on/off :)
			void set(std::string name) { _name = name; }
			void setAutoCommit(bool value) { autoCommit = value; }
			bool commit();
			void setSecurity(security , categories categ);
			void getSecurity(security &, categories categ);
	
		protected:
			std::string _name;
			std::string _id;
			bool dirty;
			bool autoCommit;
			security _securityObj;	
			
			enum USER_States {
			    STATE_ZERO = 0,
			    STATE_SETSECURITY = 1,
			    STATE_GETSECURITY = 2
			};
			
			vector<USER_States> stateVector;
			map<int, security> securityMap;
	};
}
#endif
