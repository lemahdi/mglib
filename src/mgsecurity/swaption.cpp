#include "mgsecurity/swaption.h"
#include "mgsecurity/swapleg.h"
//#include "mgmodel/model.h"


using namespace std;
using namespace MG;


/* Forward Rate Agreement class */
MG_Swaption::MG_Swaption(	const MG_Swaption& aRight)
						:	MG_VanillaOption(aRight)
{}

void MG_Swaption::Swap(MG_Swaption& aRight)
{
	MG_VanillaOption::Swap(aRight);
}

MG_Swaption::MG_Swaption(	const MG_GenericDate& aMat
						,	const MG_Schedule	& aSched
						,	const double		& aStrike)
						:	MG_VanillaOption(aMat, MG_SecurityPtr(new MG_SwapLeg(aSched, K_RCV)), aStrike)
{
	myXLName = MG_CAPFL_XL_NAME;
}

MG_Swaption::~MG_Swaption()
{}

void MG_Swaption::PrePricing(const MG_Model& aMdl)
{
	myUnderlying->PrePricing(aMdl);
}

double MG_Swaption::Price(void) const
{
	return 0.;
}