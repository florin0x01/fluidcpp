#include "fluid_values.h"
#include <cstring>

namespace fluidinfo
{

std::string Values::formatQuery(const std::string& query)
{
	if ( query.empty() ) return "";
	char *saveptr;
	char *x = (char*)query.c_str();
	char *f = strtok_r(x, "\"", &saveptr);
	std::cerr << "Got " << f << std::endl;
	std::string tokenList = std::string(urlencode(f));
	while (f != NULL)
	{
		f = strtok_r(NULL, "\"", &saveptr);
		if ( !f ) break;
		std::cerr << "Got " << f << std::endl;
		if ( strstr(f, "tag=") )
			tokenList = tokenList + std::string(f);
		else
			tokenList = tokenList + " and " + std::string(urlencode(f));
	}
	std::cerr << "Tokenlist: " << tokenList << std::endl;
	
	return tokenList;
}
	
Values::Ptr Values::Delete(const Session& ses,const std::string& query)
{
 Values::Ptr val(new Values());
 val->setParentSession(const_cast<Session*>(&ses));
 val->init();
 val->runCURL(DELETE, val->mainURL + "/values?query=" + formatQuery(query));
 return val;
}

Values::Ptr Values::Get(const Session& s, const std::string& query)
{
 Values::Ptr val(new Values());
 val->setParentSession(const_cast<Session*>(&s));
 val->init();
 val->runCURL(GET, val->mainURL + "/values?query=" + formatQuery(query), NULL, FWGet, const_cast<Values*>(val.get()));
 return val;
}

Values::Ptr Values::Put(const Session& s, const Json::Value& query)
{
 Values::Ptr val(new Values());
 val->setParentSession(const_cast<Session*>(&s));
 val->init();
 val->runCURL(PUT, val->mainURL + "/values", &query, 
			  FWput, const_cast<Values*>(val.get()));
 return val;
}

size_t Values::FWdelete(void* ptr, size_t size, size_t nmemb, void* p)
{

}

size_t Values::FWGet(void* ptr, size_t size, size_t nmemb, void* p)
{
fluidinfo::Values *x = (fluidinfo::Values*)p;

   if ( x->idx_bufferGetTagPaths_ < x->lastContentLength )
   {
	if ( !x->bufferGetTagPaths_ )
	{
	   x->bufferGetTagPaths_ = new char[x->lastContentLength+1];
	   memset(x->bufferGetTagPaths_, 0, x->lastContentLength+1);	
	   x->idx_bufferGetTagPaths_ = 0;
	}
   
	memcpy(x->bufferGetTagPaths_ + x->idx_bufferGetTagPaths_, ptr, size * nmemb);
	x->idx_bufferGetTagPaths_ += size * nmemb;
	
	if ( x->idx_bufferGetTagPaths_ < x->lastContentLength )
		return size * nmemb;
   }
  	
   //Now we have the whole buffer
   size_t recsize = size * nmemb;
   x->idx_bufferGetTagPaths_ = 0;
   
   char *buf = x->bufferGetTagPaths_;	
   if ( recsize ) 
   {
	   Json::Reader r;
	   Json::Value root;
	   r.parse(buf, root);
	   x->results = root;
	   delete[] buf;
	   x->bufferGetTagPaths_ = NULL; 
   }
   
   return recsize;
}

size_t Values::FWput(void* ptr, size_t size, size_t nmemb, void* p)
{
 if ( ptr == NULL ) return 0;
   
   static bool completionPutTag_ = false;
   
   if ( completionPutTag_ == false ) 
   {
	   memcpy(ptr, p, strlen((const char*)p));
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


}