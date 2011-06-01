#include "fluid_ns.h"
#include "fluid_session.h"
#include "fluid_session_details.h"

#include <iostream>

using namespace fluidinfo;
using namespace std;

bool initSession(Session& session, const authentication& auth)
{
  session.setAuthentication(auth);
  session.setName("testSession");
  //session.setSandbox(false);
  session.setSandbox(true);
  session.setSSL(false);
  
  return session.Start();
}

int main(int argc, char** argv)
{
    Session session;
    
    authentication auth;
    
    auth.password = "test";
    auth.username = "test";
    
    
    if ( !initSession(session, auth) ) {
	    cout << "Could not start session :( " << endl;
	    return -1;
    }
    
    std::string ns_name;
    cout << "Namespace name: ";
    cin >> ns_name;
    
    cout << "Creating namespace " << ns_name << endl;
    Namespace ns(ns_name);
    ns.setParentSession(&session);
    ns.create();
    std::cout << "Namespace exists: " << ns.exists() << endl;
    std::cout << "Namespace is fresh: " << ns.isFresh() << endl;
    std::cout << "Deleting namespace " << endl;
    ns.Delete();
    
    return 0;
}