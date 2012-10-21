#include "mgsecurity/cap.h"
#include "mgsecurity/swapleg.h"
//#include "mgmodel/model.h"


using namespace std;
using namespace MG;


/* Forward Rate Agreement class */
MG_CapFloor::MG_CapFloor(	const MG_CapFloor& aRight)
						:	MG_VanillaOption(aRight)
						,	myCallPut(aRight.myCallPut)
{}

void MG_CapFloor::Swap(MG_CapFloor& aRight)
{
	MG_VanillaOption::Swap(aRight);
	std::swap(myCallPut, aRight.myCallPut);
}

MG_CapFloor::MG_CapFloor(	const MG_GenericDate& aMat
						,	const MG_Schedule	& aSched
						,	const CALLPUT_NAME	& aCallPut
						,	const double		& aStrike)
						:	MG_VanillaOption(aMat, MG_SecurityPtr(new MG_SwapLeg(aSched, K_RCV)), aStrike)
						,	myCallPut(aCallPut)
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
