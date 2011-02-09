#ifndef _FLUID_USER_
#define _FLUID_USER_
#include <generic_stuff.h>

namespace fluidinfo{
	class User{
		public:
			User() { }
			User(std::string name):_name(name) { }
			~User(); //cannot delete user if autoCommit is on/off :)
			void set(std::string name) { _name = name; }
			void setAutoCommit(bool value) { autoCommit = value; }
			bool commit();
			void setSecurity(security &, categories categ);
			void getSecurity(security &, categories categ);
	
		protected:
			std::string _name;
			std::string _id;	
			bool dirty;
			bool autoCommit;
			security _securityObj;	
	};
}
#endif
