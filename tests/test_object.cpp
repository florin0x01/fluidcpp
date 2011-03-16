#include "fluidinfo/fluid_object.h"
#include "fluidinfo/fluid_session.h"
#include "fluidinfo/fluid_session_details.h"

#include <iostream>

using namespace fluidinfo;
using namespace std;

int main(int argc, char** argv)
{
	Session session;

	authentication auth;
	auth.password = "testfluidinfo";
	auth.username = "nfpetrovici";
	session.setAuthentication(auth);
	session.setName("testSession");
	session.setSandbox(false);
	session.setSSL(true);
	
	if ( session.Start() == false ) {
		cout << "Could not start session :( " << endl;
		return -1;
	}
	
	
	Object myObj("Nini");
	myObj.setParentSession(&session);
	myObj.create();
	myObj.hasTag("gigi");
	myObj.update();
	
	cout << "Object id: " << myObj.getId() << endl;
	cout << "Object URI: " << myObj.getURI() << endl;
	
	return 0;
}