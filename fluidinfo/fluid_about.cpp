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

#include "fluid_about.h"
#include <iostream>

using namespace fluidinfo;
using namespace std;

void About::setError(string err)
{
    SessionDetails::setError(err);
    if ( err == "NonexistentTag" )
        hasTag_ = false;
}

About::Ptr About::createObject(const Session& ses, const std::string& aboutx)
{
    About::Ptr about(new About());
    about->setParentSession(const_cast<Session*>(&ses));
    about->init();
    about->runCURL(POST, about->mainURL + "/about/" + urlencode(aboutx), NULL, FWcreate);
    return about;
}

About::Ptr About::createOrUpdateTag(const Session& ses, const std::string& aboutx, const std::string& tag, const std::string& val)
{
    About::Ptr about(new About());
    about->setParentSession(const_cast<Session*>(&ses));
    about->init(false, "Content-Type: application/vnd.fluiddb.value+json");
    Json::Value root = val;
    about->runCURL(PUT, about->mainURL + "/about/" + urlencode(aboutx) + "/" + ses.AuthObj.username + "/" + tag, &root, FWput);
    return about;
}

About::Ptr About::deleteTag(const Session& ses, const std::string& aboutx, const std::string& tag)
{
    About::Ptr about(new About());
    about->setParentSession(const_cast<Session*>(&ses));
    about->init();
    about->runCURL(DELETE, about->mainURL + "/about/" + urlencode(aboutx) + "/" + ses.AuthObj.username + "/" + tag);
    return about;
}

About::Ptr About::getInfo(const Session& ses, const std::string& aboutx, std::string tag)
{
    About::Ptr about(new About());
    about->setParentSession(const_cast<Session*>(&ses));
    about->init();
	if ( tag.size() )
		about->runCURL(GET, about->mainURL + "/about/" + urlencode(aboutx) + "/" + ses.AuthObj.username +"/" + tag, NULL, FWgetInfo);
    else
 		about->runCURL(GET, about->mainURL + "/about/" + urlencode(aboutx), NULL, FWgetInfo);
	return about;
}

bool About::hasTag(const Session& ses, const std::string& aboutx, const std::string& tag)
{
    About::Ptr about(new About());
    about->setParentSession(const_cast<Session*>(&ses));
    about->init();
    about->hasTag_ = true;
    about->runCURL(HEAD, about->mainURL + "/about/" + urlencode(aboutx) + "/" + ses.AuthObj.username +"/" + tag);
    return about->hasTag_;
}

size_t About::FWput(void* ptr, size_t size, size_t nmemb, void* p)
{
    if ( ptr == NULL ) return 0;

    static bool completionPutTag_ = false;

    if ( completionPutTag_ == false )
    {
        std::string tmp = (const char*)p;
        memcpy(ptr, p, tmp.size());
        completionPutTag_ = true;
        return strlen((const char*)p);
    }
    else
    {
        completionPutTag_ = false;
        return 0;
    }

    return 0;
}

size_t About::FWcreate(void* ptr, size_t size, size_t nmemb, void* p)
{
    fluidinfo::About *x = (fluidinfo::About*)p;
    size_t recsize = size * nmemb;
    if  ( recsize ) {
        char *buf = new char[recsize+1];
        memset(buf,0,recsize+1);
        memcpy(buf, ptr, recsize);
#ifdef FLUID_DEBUG
        //  std::cerr << "FWCreate(): " << buf << std::endl;
#endif

        Json::Reader r;
        Json::Value root;

        r.parse(buf, root);

        x->id_ =  root["id"].asString();
        x->uri_ = root["URI"].asString();

        delete[] buf;
    }

    return recsize;
}

size_t About::FWgetInfo(void* ptr, size_t size, size_t nmemb, void* p)
{
    fluidinfo::About *x = (fluidinfo::About*)p;
    if ( x->idx_bufferGetTagValue_ < x->lastContentLength )
    {
        if ( !x->bufferGetTagValue_ )
        {
            x->bufferGetTagValue_ = new char[x->lastContentLength+1];
            memset(x->bufferGetTagValue_, 0, x->lastContentLength+1);
            x->idx_bufferGetTagValue_ = 0;
        }

        memcpy(x->bufferGetTagValue_ + x->idx_bufferGetTagValue_, ptr, size * nmemb);
        x->idx_bufferGetTagValue_ += size * nmemb;

        if ( x->idx_bufferGetTagValue_ < x->lastContentLength )
            return size * nmemb;
    }
    
    char *buf = x->bufferGetTagValue_;
	x->idx_bufferGetTagValue_ = 0;
	
    if (!(size * nmemb)) { x->bufferGetTagValue_ = NULL;  return 0; };
	
	Json::Reader r;
	Json::Value root;
	r.parse(buf, root);
	
	if ( !root.size() )
	{
		try {
				x->genericValue_ = root.asString();
				x->bufferGetTagValue_ = NULL;
				return size * nmemb;	
		}catch(...) { return size * nmemb; }
	}
	
	Json::Value id = root.get("id", Json::nullValue);
	Json::Value tagPaths = root.get("tagPaths", Json::nullValue);
	if ( id != Json::nullValue )
		x->id_ = id.asString();
	if ( tagPaths != Json::nullValue )
	{
		x->tagPaths_.clear();
		for (int i =0 ; i < root["tagPaths"].size(); i++) 
		{
			x->tagPaths_.push_back(root["tagPaths"][i].asString());	
		}
	}
	

	x->bufferGetTagValue_ = NULL;
	delete[] buf;
	return size * nmemb;
}


