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

	/*
	auth.password = "test";
	auth.username = "test";
	*/
	
	session.setAuthentication(auth);
	session.setName("testSession");
	//session.setSandbox(false);
	session.setSandbox(false);
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
	
	std::cout << "Initializing namespace 2 " << std::endl;
	
	Namespace ns2;
	ns2.setParentSession(&session);
	
	ns.getSubNamespaceInfo("gigi", ns2);
	
	std::cout << "Namespace fresh: " << ns.isFresh() << std::endl;
	
	if ( ns2.exists() ) {
	      std::cout << "ns2 exists " << std::endl;
	      std::cout << "Ns2 Id: " << ns2.getId() << std::endl;
	    //  std::cout << "Ns2 Uri: " << ns2.getUri() << std::endl;
	      std::cout << "Ns2 description: " << ns2.getDescription() << std::endl;
	}
	
	else {
	     std::cout << "Non-existent namespace " << std::endl;
	     std::cout << "creating ns2 in subnamespace" << std::endl;
	     ns2.set("gigi", "gigi subnamespace in CPPnsss99");
	     ns2.create(ns.getName());
	}
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