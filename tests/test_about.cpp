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

#include "fluid_about.h"
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
	About::Ptr ab = About::createObject(session, "random_string44");
	std::cout << ab->getId() << "\n";
	std::cout << ab->getUri() << "\n";
	ab = About::createOrUpdateTag(session, "random_string44", "tag_about", "test_val");
	if ( ab->hasErrors() ) printVector<std::string>("ERRORS", ab->getErrors());
	About::deleteTag(session,"random_string44", "tag_about");
	
	ab = About::getInfo(session, "id1", "tags/Copyright");
	
	std::cout << "id: " << ab->getId() << std::endl;
	if ( ab->hasErrors() ) printVector<std::string>("ERRORS", ab->getErrors());
	else printVector<std::string>("",ab->getTagPaths());
	std::cout << ab->getGenericValue() << std::endl;
	
	ab = About::getInfo(session, "id1");
	
	std::cout << "id: " << ab->getId() << std::endl;
	if ( ab->hasErrors() ) printVector<std::string>("ERRORS", ab->getErrors());
	else printVector<std::string>("",ab->getTagPaths());
	std::cout << ab->getGenericValue() << std::endl;
	
	return 0;	
}