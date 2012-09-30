#include "mgsecurity/option.h"
#include "mgmodel/model.h"


using namespace std;
using namespace MG;


/* Forward Rate Agreement class */
MG_VanillaOption::MG_VanillaOption	(	const MG_VanillaOption& aRight)
									:	MG_Security(aRight)
									,	myMatDt		(aRight.myMatDt)
									,	myUnderlying(aRight.myUnderlying->Clone())
									,	myStrike	(aRight.myStrike)
{}

void MG_VanillaOption::Swap(MG_VanillaOption& aRight)
{
	MG_Security::Swap(aRight);
	myMatDt.Swap(aRight.myMatDt);
	myUnderlying.Swap(aRight.myUnderlying);
	std::swap(myStrike, aRight.myStrike);
}

MG_VanillaOption::MG_VanillaOption	(	const MG_GenericDate& aMat
									,	const MG_SecurityPtr& aUnd
									,	const double		& aStrike)
									:	MG_Security()
									,	myMatDt		(aMat)
									,	myUnderlying(aUnd)
									,	myStrike	(aStrike)
{}

MG_VanillaOption::~MG_VanillaOption()
{}

void MG_VanillaOption::PrePricing(const MG_Model& aMdl)
{
	myMatDt.Rebuild(aMdl.GetAsOf());
}


