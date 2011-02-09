#ifndef _FLUID_SECURITY_
#define _FLUID_SECURITY_
#include <generic_stuff.h>

namespace fluidinfo{
	enum permissions { CREATE,UPDATE,DELETE,LIST,CONTROL };
	enum policy { OPEN, CLOSED };
	enum categories { NS, TAGS, TAGVALUES };

	struct security {
		permissions perms;
		policy pol;
		std::set<string> exceptions;
	}
}
#endif
