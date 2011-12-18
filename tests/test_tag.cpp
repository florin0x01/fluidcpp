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

#include "fluid_tag.h"
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
	
	Tag::Ptr result =Tag::Add(session, "CPPNS1/CPPYA", "TestTag", "TestTag desc");
	
	if ( result->hasErrors() )
	{
		printVector<std::string>("Got the following errors:", result->getErrors());	
	}
	
	Tag::UpdateDescription(session, "CPPNS1/CPPYA", "TestTag", "TestTag desc2");
	//Tag::Delete(session, "CPPNS1/CPPYA", "TestTag");
	
	Tag::Ptr t = Tag::Get(session, "CPPNS1/CPPYA", "TestTag");
	std::cout << t->Description() << std::endl;
	std::cout << t->Id() << std::endl;
	std::cout << t->Indexed() << std::endl;
	
	std::cout << "=======================\n";
	
	Namespace::Ptr ns(new Namespace());
	ns->setParentSession(&session);
	ns->setPath("CPPNS1/CPPYA");
	
	Tag::Ptr t2 = Tag::Get(*ns, "TestTag");
	std::cout << t2->Description() << std::endl;
	std::cout << t2->Id() << std::endl;
	std::cout << t2->Indexed() << std::endl;
	
	return 0;
	
}