#include "fluid_object.h"
#include "fluid_ns.h"
#include "fluid_session.h"
#include "fluid_session_details.h"

#include <iostream>

using namespace fluidinfo;
using namespace std;

Session session;

void initSession(const Session& Session, bool sandbox, bool ssl)
{
	authentication auth;
	
	auth.password = "testfluidinfo";
	auth.username = "nfpetrovici";	
	session.setAuthentication(auth);
	session.setName("testSession");
	session.setSandbox(sandbox);
	session.setSSL(ssl);
	
	if ( session.Start() == false )
		throw std::logic_error("Could not start session");
	
}

Namespace::Ptr createNamespace(const std::string& name, const std::string& description)
{
	Namespace::Ptr ns;
	ns.reset(new Namespace());
	ns->setParentSession(&session);
	ns->set("CXXNS", "C++ namespace");
	ns->create();
	return ns;
}

Object::Ptr createObject(const std::string& name, const std::string& description)
{
	Object::Ptr obj;
	obj.reset(new Object());
	obj->setName(name);
	obj->isAbout(description);
	obj->create();
	return obj;
}

void tagObject(Object::Ptr obj, const std::string& tag, const std::string& path, const std::string &value)
{
	obj->putTag(tag, path, value);	
}


int main(int argc, char** argv)
{
	
	Object myObj("TestObj");
	myObj.setParentSession(&session);
	string about="gigi";
	
	//get the handle for the "Acvariu" namespace
	
	//check getSubNamespaceInfo if namespace does not exist
	Namespace ns;
	ns.setParentSession(&session);
	ns.getSubNamespaceInfo("Acvariu", ns);
	
	cout << ns.getId() << endl;
	cout << ns.getDescription() << endl;
	
	
	cout << ns.isFresh() << endl;
	cout << ns.isError() << endl;
	
	cout << "About: " ;
	cin>>about;

	myObj.isAbout(about);
	myObj.create();
	myObj.putTag("myFirstTag", "test/Acvariu", "myFirstTagValue");

	//myObj.hasTag("myFirstTag");
	
	cout << "Object id: " << myObj.getId() << endl;
	cout << "Object URI: " << myObj.getURI() << endl;
	
	return 0;
}