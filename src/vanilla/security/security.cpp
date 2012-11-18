#include "vanilla/security/security.h"


using namespace std;
using namespace MG;


/* Base Security class */
MG_Security::MG_Security() : MG_XLObject(), myPrice(0.)
{}

void MG_Security::Swap(MG_Security& aRight)
{
	MG_XLObject::Swap(aRight);
	std::swap(myPrice, aRight.myPrice);
	myForwards.swap(aRight.myForwards);
	myFlows.swap(aRight.myFlows);
	myDfs.swap(aRight.myDfs);
}

MG_Security::~MG_Security()
{}

