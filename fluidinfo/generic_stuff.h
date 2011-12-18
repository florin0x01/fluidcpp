/* 
Copyright 2011 Nicolae Florin Petrovici
 
This file is part of the Fluidinfo C++ library. 
For more information, regarding Fluidinfo, visit http://www.fluidinfo.com

Fluidinfo C++ library is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Fluidinfo C++ library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with Fluidinfo C++ library.  If not, see <http://www.gnu.org/licenses/>.
*/  

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

#include <sys/types.h>
#include <sys/time.h>

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
#include <stdlib.h>
#include <stdio.h>
#include <auto_ptr.h>

#define FLUID_HTTP_SSL "https://fluiddb.fluidinfo.com"
#define FLUID_HTTP "http://fluiddb.fluidinfo.com"
#define FLUID_SANDBOX_HTTP "http://sandbox.fluidinfo.com"

#define FLUID_DEBUG  1

template<class T_>
struct auto_vec{
  T_* t_;
  auto_vec( T_* t ): t_( t ) {}
  ~auto_vec() { delete[] t_; }
  T_* get() const { return t_; }
  T_* operator->() const { return get(); }
  T_& operator*() const { return *get(); }
};

//based on javascript encodeURIComponent()
std::string char2hex( char dec );
std::string urlencode(const std::string &c);

#endif
