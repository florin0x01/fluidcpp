#include "fluid_security.h"
#include <cstdio>
#include <fstream>

namespace fluidinfo
{
void authentication::ReadFromFile(const char* x)
{
	using namespace std;
	fstream f(x, ios::in);
	if (f.is_open()) 
	{
		f >> username >> password;
	}
	f.close();
}

}