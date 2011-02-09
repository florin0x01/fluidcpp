#ifndef _FLUID_OBJECT_
#define _FLUID_OBJECT_
#include <generic_stuff.h>

class fluidinfo::Object
{
	public:
		Object() { }
		Object(string name):_name(name) { }
		~Object() { }

		void set(string about="") { _about = about; }
		void delete(string tag);

		bool commit(); //commits to Fluidinfo

	protected:
		string _name;
		string _about;
		string _id;
		string _uri;

		map<string,string> _tagMap; //tag name, tag value
		vector<string> _tagPaths;

		bool dirty; //if object is out of sync with Fluidinfo
};
#endif
