#include "mgsecurity/digital.h"
//#include "mgmodel/model.h"


using namespace std;
using namespace MG;


/* Forward Rate Agreement class */
MG_Digital::MG_Digital	(	const MG_Digital& aRight)
						:	MG_VanillaOption(aRight)
{}

void MG_Digital::Swap(MG_Digital& aRight)
{
	MG_VanillaOption::Swap(aRight);
}

MG_Digital::MG_Digital	(	const MG_GenericDate& aMat
						,	const double		& aStrike)
						:	MG_VanillaOption(aMat, MG_SecurityPtr(NULL), aStrike)
{
	myXLName = MG_CAPFL_XL_NAME;
}

MG_Digital::~MG_Digital()
{}

void MG_Digital::PrePricing(const MG_Model& aMdl)
{
	myUnderlying->PrePricing(aMdl);
}

double MG_Digital::Price(void) const
{
	return 0.;
}
