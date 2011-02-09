#include <generic_stuff.h>

namespace fluidinfo{
	class User{
		public:
			User() { }
			User(string name):_name(name) { }
			~User(); //cannot delete user if autoCommit is on/off :)
			void set(string name) { _name = name; }
			void setAutoCommit(bool value) { autoCommit = value; }
			bool commit();
			void setSecurity(security &, categories categ);
			void getSecurity(security &, categories categ);
	
		protected:
			string _name;
			string _id;	
			bool dirty;
			bool autoCommit;
			security _securityObj;	
	};
}
