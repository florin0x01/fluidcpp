#ifndef _GENERIC_STUFF
#define _GENERIC_STUFF

#include <cstdio>
#include <string>
#include <cstring>
#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <stdarg.h>

/** JSON stuff **/

#include <json/autolink.h>
#include <json/config.h>
#include <json/features.h>
#include <json/forwards.h>
#include <json/json.h>
#include <json/reader.h>
#include <json/value.h>
#include <json/writer.h>

/** CURL stuff **/

#include <curl/curl.h>

#define FLUID_HTTP_SSL "https://fluiddb.fluidinfo.com"
#define FLUID_HTTP "http://fluiddb.fluidinfo.com"

#define FLUID_DEBUG  1

#endif
