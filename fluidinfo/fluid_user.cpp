#include "fluid_user.h"

using namespace std;

void fluidinfo::User::getSecurity(fluidinfo::security& sec , fluidinfo::categories categ, std::string xpath)
{
  if ( autoCommit == true ) {
    state = STATE_GETSECURITY;
    commit();
    return;
  }
    
  //sec = securityMap[categ];
  
  categories categ_tmp;
  
  if ( (categ_tmp=reverse_xpathMap.find(xpath)) != reverse_xpathMap.end() ) {
      if ( categ_tmp == categ )
	sec = securityMap[categ_tmp];
  }
  
  else {
      dirty = true;
      stateVector.pushBack(STATE_GETSECURITY);
  }
}

void fluidinfo::User::setSecurity(fluidinfo::security , fluidinfo::categories categ, std::string xpath)
{
  if ( autoCommit == true ) {
      state = STATE_SETSECURITY;
      commit();
      return;
  }
  
  securityMap[categ] = sec;
  xpathMap[categ] = xpath;
  reverse_xpathMap[xpath] = categ;
  dirty = true;
  stateVector.pushBack(STATE_SETSECURITY);
}

bool fluidinfo::User::doState(fluidinfo::User::USER_States state)
{
   string url = mainURL;
    switch(state) {
      case STATE_ZERO:
	break;
	
      case STATE_NAME:
	  url = url + "/users/" + 
	  curl_easy_setopt(handle,CURLOPT_URL,
	break;
    };
}


bool fluidinfo::User::commit()
{
  if ( autoCommit == false ) {
      //we have something in the state vector
      for (int i = stateVector.size()-1; i>=0; i--) 
	  doState(stateVector[i]);
  }
  
  else 
      //get the state and do the actions
      doState(state);
  
  return true;
}
