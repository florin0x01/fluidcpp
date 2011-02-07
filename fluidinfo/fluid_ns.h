#include <generic_stuff.h>

/*@@ TODO: operator[int], operator[string] to get namespace by index or string, put some methods in the base impl */
class fluidinfo::Namespace
{
	public:
		Namespace() { _name=""; }
		Namespace(string name,string description=""):_name(name),_description(description) { }
		~Namespace() { }

		void add(string name);
		void set(string name, string description="") { _name = name; _description=description; }

		string getName() { return _name; }
		string getDescription() { return _description; }
		vector<Namespace*> getNamespaces(bool returnDescription="", bool returnTags="");

		void commit(); //commits to Fluidinfo				

	protected:
		string _name;
		string _description;
		vector<Namespace*> _vns;
		vector<string> tagNames;

		bool dirty; //if object is out of sync with Fluidinfo
}; 
