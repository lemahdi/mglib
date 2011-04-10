#include "exception.h"

#include <sstream>

using namespace std;
using namespace MG;


/*
 * Constructors
 */
MG_Exception::MG_Exception()
{}

MG_Exception::MG_Exception(const std::string& aFile, const unsigned int& aLine, const string& aMsg) : exception()
{
	stringstream vSS;
	vSS << "File: " << aFile << ", Line: " << aLine << ", " << aMsg << endl;
	myMsg = vSS.str();
}
