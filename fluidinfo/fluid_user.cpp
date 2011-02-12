#include "fluid_user.h"

using namespace std;

void fluidinfo::User::getSecurity(fluidinfo::security& sec , fluidinfo::categories categ)
{
  if ( autoCommit == true ) {
    commit();
    return;
    
  sec = securityMap[categ];
  stateVector.pushBack(STATE_GETSECURITY);
}

void fluidinfo::User::setSecurity(fluidinfo::security , fluidinfo::categories categ)
{
  if ( autoCommit == true ) {
      commit();
      return;
  }
  
  securityMap[categ] = security;
  stateVector.pushBack(STATE_SETSECURITY);
}

