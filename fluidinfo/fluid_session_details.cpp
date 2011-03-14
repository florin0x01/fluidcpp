#include "fluid_session_details.h"

int fluidinfo::SessionDetails::connections = 0;
curl_slist* fluidinfo::SessionDetails::headers = NULL;

fluidinfo::SessionDetails::permissionsMap[CREATE] = "create";
fluidinfo::SessionDetails:permissionsMap[UPDATE] = "update";
fluidinfo::SessionDetails:permissionsMap[DELETE] = "delete";
fluidinfo::SessionDetails:permissionsMap[LIST] = "list";
fluidinfo::SessionDetails:permissionsMap[CONTROL] = "control";

fluidinfo::SessionDetails:policyMap[OPEN] = "open";
fluidinfo::SessionDetails:policyMap[CLOSED] = "closed";

fluidinfo::SessionDetails:categoriesMap[NS] = "namespaces";
fluidinfo::SessionDetails:categoriesMap[TAGS] = "tags";
fluidinfo::SessionDetails:categoriesMap[TAGVALUES] = "tag-values";
      