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
	      void ReadFromFile(const char* x);
	};
	

}
#endif
