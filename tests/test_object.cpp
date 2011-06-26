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

	
	//auth.password = "test";
	//auth.username = "test";
	
	
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