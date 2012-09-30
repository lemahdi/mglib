#include "mgsecurity/cap.h"
//#include "mgmodel/model.h"


using namespace std;
using namespace MG;


/* Forward Rate Agreement class */
MG_CapFloor::MG_CapFloor(	const MG_CapFloor& aRight)
						:	MG_VanillaOption(aRight)
{}

void MG_CapFloor::Swap(MG_CapFloor& aRight)
{
	MG_VanillaOption::Swap(aRight);
}

MG_CapFloor::MG_CapFloor(	const MG_GenericDate& aMat
						,	const MG_SwapLegPtr	& aUnd
						,	const double		& aStrike)
						:	MG_VanillaOption(aMat, aUnd, aStrike)
{
	myXLName = MG_CAPFL_XL_NAME;
}

MG_CapFloor::~MG_CapFloor()
{}

void MG_CapFloor::PrePricing(const MG_Model& aMdl)
{
	myUnderlying->PrePricing(aMdl);
}

double MG_CapFloor::Price(void) const
{
	return 0.;
}
