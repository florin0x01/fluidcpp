#include "fluid_ns.h"

using namespace std;

fluidinfo::Namespace::~Namespace()
{

}


vector< Namespace* > fluidinfo::Namespace::getNamespaces(bool returnDescription, bool returnTags)
{
  _returnNamespaceDescription = returnDescription;
  _returnTagsDescription = returnTags;
  return _vns;
}

bool fluidinfo::Namespace::commit()
{

}

