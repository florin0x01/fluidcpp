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

#include "fluid_ns.h"
#include "fluid_session.h"
#include "fluid_session_details.h"

#include <iostream>

using namespace fluidinfo;
using namespace std;

Session session;

template <typename T>
void printVector(const std::string&header, const std::vector<T> vec)
{
	std::cout << header << ":" << std::endl;
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



int main(int argc, char** argv)
{
    initSession(session, false, false);
    Namespace::Ptr ns(new Namespace());
    ns->setParentSession(&session);
	
	
    ns->set("CPPYA", "CPPDesc");
    ns->create("CPPNS1");
	
	ns->set("CPPZZ", "CPPZZ");
	ns->create("CPPNS1");
	
	if ( ns->hasErrors() )
	{
		for(int i=0; i < ns->getErrors().size(); i++)
			std::cout << "Got errror: "  << ns->getErrors()[i] << std::endl;
		ns->clearErrors();
	}
	
	Tag tag;
	tag.setParentSession(&session);
	tag.Name() = "CPPTag1";
	tag.Description() = "CPPTag1 description";
	ns->addTag(tag);
	
	std::string uri, id;
	ns->addTag("CPPTag15", "CPPTag11 description", true, uri, id);
	std::cout << "id: " << id << std::endl;
	std::cout << "uri: " << uri << std::endl;
	ns->addTag("CPPTag25");
	std::cout << "id: " << id << std::endl;
	std::cout << "uri: " << uri << std::endl;
	
	

	Namespace::getSubNamespaceInfo("/namespaces/user_id/CPPNS1", *ns, session);
	printVector<std::string>("Namespaces", ns->getNamespaceNames());
	printVector<std::string>("Tags", ns->getTagNames());
	
	Namespace::getSubNamespaceInfo("/namespaces/user_id/tags", *ns, session);
	printVector<std::string>("Namespaces", ns->getNamespaceNames());
	printVector<std::string>("Tags", ns->getTagNames());
	
	Namespace::getSubNamespaceInfo("/namespaces/user_id/users", *ns, session);
	printVector<std::string>("Namespaces", ns->getNamespaceNames());
	printVector<std::string>("Tags", ns->getTagNames());
	
	
	
	ns->set("CPPYA", "CPPDesc");
    ns->create("CPPNS1");
	std::cout << "Path: " << ns->getPath() << std::endl;
	
	ns->updateDescription("NEw description");
	
	
	if ( !ns->Delete() )
	{
		std::cerr << "Could not delete object \n";
		printVector<std::string>("Errors", ns->getErrors());
	}
    return 0;
}