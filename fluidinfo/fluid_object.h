#ifndef _FLUID_OBJECT_
#define _FLUID_OBJECT_

#include "generic_stuff.h"
#include "fluid_security.h"
#include "fluid_session_details.h"

//TODO also implement /about functionality here

namespace fluidinfo{
	
  class Object: public SessionDetails
{
	public:
		Object() {  }
		explicit Object(std::string name):_name(name) { }
		~Object() { }

		void create();

		void isAbout(std::string about="") { _about = about; }
		void setName(std::string name) { _name = name; }
		void delTag(const std::string& tag, const std::string& tagPath="");
		bool hasTag(const std::string& tag);
		
		void putTag(const std::string& tag, const std::string& tagPath, const std::string& value);
		void put(const std::string& tag, const std::string& tagPath, const std::string& filePath);
		
		
		std::string getTagValue(std::string tag);
		std::vector<std::string> getTagPaths(bool cached=false);
		std::string getURI() { return _uri; }
		std::string getId() { return _id; }
		
		//this should be static somehow...
		std::vector<std::string> getIdsByQuery(const std::string& query);
		
		static std::vector<std::string> ids;

	protected:
		std::string _name;
		std::string _about;
		std::string _id;
		std::string _uri;
		
	//	static size_t FWsetName(void *ptr, size_t size, size_t nmemb,void* p);
	//	static size_t FWdelTag(void* ptr, size_t size, size_t nmemb, void* p);
		static size_t FWhasTag(void* ptr, size_t size, size_t nmemb, void* p);
		static size_t FWputTag(void *ptr, size_t size, size_t nmemb, void* p);
		static size_t FWputBlob(void *ptr, size_t size, size_t nmemb, void* p);
		static size_t FWcreate(void *ptr, size_t size, size_t nmemb, void* p);
		static size_t FWgetTagValue(void *ptr, size_t size, size_t nmemb, void* p);
		static size_t FWgetTagPaths(void *ptr, size_t size, size_t nmemb, void* p);
		static size_t FWgetIdsByQuery(void *ptr, size_t size, size_t nmemb, void* p);
		
		std::map<std::string,std::string> _tagMap; //tag name, tag value
		std::vector<std::string> _tagPaths;
		
		static std::vector<FILE*> openFiles;
		
		std::string delete_request;
		
		bool dirty; //if object is out of sync with Fluidinfo
};

  struct MetaObj {
	Object *obj;
	std::string tagrequest;
  };
}
#endif
