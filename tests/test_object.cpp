#include "fluid_object.h"
#include "fluid_ns.h"
#include "fluid_tag.h"
#include "fluid_session.h"
#include "fluid_session_details.h"

#include <iostream>

using namespace fluidinfo;
using namespace std;

Session session;

void initSession(const Session& Session, bool sandbox, bool ssl)
{
	authentication auth;
	
	auth.password = "anon";
	auth.username = "anon";	
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
	ns->set(name, description);
	ns->create();
	return ns;
}

Object::Ptr createObject(const std::string& name, const std::string& description)
{
	Object::Ptr obj;
	obj.reset(new Object());
	obj->setParentSession(&session);
	obj->setName(name);
	
	//TODO if About is empty then we receive an error from fluidinfo
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
	initSession(session, false, false);
	
	Object::Ptr obj = createObject("CPPObj11", "This is a CPP API obj");
	std::cout << obj->getId() << "\n";
	std::cout << obj->getURI() << "\n";
	
	
	Namespace::Ptr ns = createNamespace("CPPNS1", "This is a CPP namespace");
	std::cout << ns->getId() << "\n";
	std::cout << ns->getUri() << "\n";
	
	tagObject(obj, "CPPTag1", "", "CPPValue1");
	
	return 0;
}