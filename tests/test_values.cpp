#include "fluid_values.h"
#include "fluid_ns.h"
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

int main(int argc, char** argv)
{
    initSession(session, false, false);
	//Notice that the actual query is between "" !!
	std::cout << Values::Get(session, /*Start query*/"\"userid/tag/book>0.3 and userid/tag/book2 < 0.9\""/*End query*/
										"&tag=userid/tag/book&tag=userid/tag/book2"
							)->getResults();
	Values::Delete(session, "\"userid/tag/book > 0.3\"&tag=userid/tag/book");						
	return 0;
}