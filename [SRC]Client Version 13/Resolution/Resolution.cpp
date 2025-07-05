#include "Resolution.h"

cResolution::cResolution()
{

}


cResolution::~cResolution()
{
}

short  cResolution::IsResolution()
{
	return sResolution;
}

/*bool cResolution::Is800x600(const string& name) {
	ifstream f(name.c_str());
	return f.good();
}*/
//Agregado Lalo
bool cResolution::Is1024x768(const string& name) {
	ifstream f(name.c_str());
	return f.good();
}