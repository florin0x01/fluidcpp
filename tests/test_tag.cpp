#include "fluid_tag.h"
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
	
	Tag::Add(session, "CPPNS1/CPPYA", "TestTag", "TestTag desc");
	Tag::UpdateDescription(session, "CPPNS1/CPPYA", "TestTag", "TestTag desc2");
	//Tag::Delete(session, "CPPNS1/CPPYA", "TestTag");
	
	Tag::Ptr t = Tag::Get(session, "CPPNS1/CPPYA", "TestTag");
	std::cout << t->Description() << std::endl;
	std::cout << t->Id() << std::endl;
	std::cout << t->Indexed() << std::endl;
	
	return 0;
	
}