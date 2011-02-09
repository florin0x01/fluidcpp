#ifndef _FLUID_OBJECT_
#define _FLUID_OBJECT_
#include <generic_stuff.h>

class fluidinfo::Object
{
	public:
		Object() { }
		Object(std::string name):_name(name) { }
		~Object() { }

		void set(std::string about="") { _about = about; }
		void delete(std::string tag);

		bool commit(); //commits to Fluidinfo

	protected:
		std::string _name;
		std::string _about;
		std::string _id;
		std::string _uri;

		std::map<string,string> _tagMap; //tag name, tag value
		std::vector<string> _tagPaths;

		bool dirty; //if object is out of sync with Fluidinfo
};
#endif
