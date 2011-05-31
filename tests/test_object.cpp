#include "fluid_object.h"
#include "fluid_ns.h"
#include "fluid_session.h"
#include "fluid_session_details.h"

#include <iostream>

using namespace fluidinfo;
using namespace std;

int main(int argc, char** argv)
{
	Session session;

	
	authentication auth;
	
	auth.password = "testfluidinfo";
	auth.username = "nfpetrovici";

	auth.password = "test";
	auth.username = "test";
	
	session.setAuthentication(auth);
	session.setName("testSession");
	//session.setSandbox(false);
	session.setSandbox(true);
	session.setSSL(false);
	
	if ( session.Start() == false ) {
		cout << "Could not start session :( " << endl;
		return -1;
	}
	
	
	Object myObj("TestObj");
	myObj.setParentSession(&session);
	string about="gigi";
	
	Namespace ns("CPPnsss99", "fluidinfo cpp namespace");
	ns.setParentSession(&session);
	
	ns.create();
	std::cout << "Namespace fresh: " << ns.isFresh() << std::endl;
	
	return 0;
	
	
//	cout << "About: " ;
//	cin>>about;
	myObj.isAbout(about);
	myObj.create();
	myObj.putTag("myFirstTag", "CPPnamespace", "myFirstTagValue");

	//myObj.hasTag("myFirstTag");
	
	cout << "Object id: " << myObj.getId() << endl;
	cout << "Object URI: " << myObj.getURI() << endl;
	
	return 0;
}