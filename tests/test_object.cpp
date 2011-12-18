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

#include "fluid_object.h"
#include "fluid_ns.h"
#include "fluid_tag.h"
#include "fluid_session.h"
#include "fluid_session_details.h"

#include <iostream>

using namespace fluidinfo;
using namespace std;

Session session;

template <typename T>
void printVector(const std::vector<T> vec)
{
	for(int i=0; i < vec.size(); i++)
		std::cout << "\t" << vec[i] << "\n";
}

void initSession(const Session& Session, bool sandbox, bool ssl)
{
	authentication auth;
	
	auth.ReadFromFile("/tmp/pas.txt");	
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

void tagObject(const Object::Ptr& obj, const std::string& tag, const std::string& path, const std::string &value)
{
	obj->putTag(tag, path, value);	
}


int main(int argc, char** argv)
{
	initSession(session, false, false);
	
	Object::Ptr obj = createObject("CPPObj11", "This is a CPP API obj");
	if (!obj.get())
		throw std::logic_error("Cannot get object");
	
	std::cout << obj->getId() << "\n";
	std::cout << obj->getURI() << "\n";
	
	Namespace::Ptr ns = createNamespace("CPPNS1", "This is a CPP namespace");
	std::cout << ns->getId() << "\n";
	std::cout << ns->getUri() << "\n";

	//putTag and delTag automatically append username
	obj->putTag("CPPTag1", "", "CPPValue1");
	obj->putTag("CPPTag3", "", "Value3");
	obj->putTag("CPPTag2", "", "Value3");
	
	obj->delTag("CPPTag3");
	obj->delTag("CPPTag3", "cpp_path");
	
	if ( obj->hasErrors() )
	{
		for (int i=0; i < obj->getErrors().size(); i++)
			std::cout << "Error: " << obj->getErrors()[i] << "\n";
		obj->clearErrors();
	}
	
	std::string tag = "user/Tag1";
	
	std::cout << " => Object has " << tag << ":" << Object::hasTag(obj->getId(), tag , session) << "\n";
	
		
	std::cout << " => Object getIdsByQuery \n";
	
	std::vector<std::string> objs = Object::getIdsByQuery("has user/tag1", session);	
	printVector<std::string>(objs);

	std::vector<std::string> tagPaths;	
	
	Object resObject;
	resObject.setParentSession(&session);
	std::cout << "==> Tag paths for object " << objs[0] << "\n";
	resObject.setId(objs[0]);
	
	tagPaths = resObject.getTagPaths();
	
	//TODO test getById
	
	printVector<std::string>(tagPaths);
	std::cout << "Object about: " << resObject.getAbout() << "\n";
	
	
	std::string tagValue = resObject.getTagValue("user/tag1");
	
	std::cout << "Tag value: " << tagValue << std::endl;
	
	std::cout << "==> Obj getTagPaths() \n";
	
	tagPaths = obj->getTagPaths();
	printVector<std::string>(tagPaths);
	
	
	std::cout << "Object::getById() " << std::endl;
	Object::Ptr obj2 = Object::getById("1", session);
	std::cout << "About: " << obj2->getAbout() << std::endl;
	
	printVector<std::string>(obj2->getTagPaths());
	
	std::cout << "--------------\n";
	std::cout << obj2->getTagValue("nfpetrovici/tags/test");
	
	return 0;
}