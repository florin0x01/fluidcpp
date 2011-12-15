#ifndef _FLUID_SECURITY_
#define _FLUID_SECURITY_
#include "generic_stuff.h"

namespace fluidinfo{
	
	
	enum permissions { PERMISSION_CREATE, PERMISSION_UPDATE, PERMISSION_DELETE, PERMISSION_LIST,PERMISSION_CONTROL } ;
		
	enum policy { POLICY_OPEN, POLICY_CLOSED };
	enum categories { CATEGORY_NS, CATEGORY_TAGS, CATEGORY_TAGVALUES };

	struct security {
		permissions perms;
		policy pol;
		std::set<std::string> exceptions;
	};
	
	struct authentication {
	      std::string username;
	      std::string password;
	};
	

}
#endif
