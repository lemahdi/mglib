#include "vanilla/security/security.h"


using namespace std;
using namespace MG;


/* Base Security class */
MG_Security::MG_Security(const MG_TermStructure& aNotional)
	: MG_XLObject()
	, myNotional(aNotional)
{}

void MG_Security::Swap(MG_Security& aRight)
{
	MG_XLObject::Swap(aRight);
	myForwards.swap(aRight.myForwards);
	myFlows.swap(aRight.myFlows);
	myDfs.swap(aRight.myDfs);
	myNotionals.swap(aRight.myNotionals);
	myNotional.Swap(aRight.myNotional);
}

MG_Security::~MG_Security()
{}

