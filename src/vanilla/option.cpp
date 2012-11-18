#include "vanilla/option.h"
#include "mgmodel/model.h"


using namespace std;
using namespace MG;


/* Forward Rate Agreement class */
MG_VanillaOption::MG_VanillaOption	(	const MG_VanillaOption& aRight)
									:	MG_Security(aRight)
									,	myMatDts	(aRight.myMatDts)
									,	myUnderlying((MG_Security*)aRight.myUnderlying->Clone())
									,	myStrike	(aRight.myStrike)
{}

void MG_VanillaOption::Swap(MG_VanillaOption& aRight)
{
	MG_Security::Swap(aRight);
	myMatDts.swap(aRight.myMatDts);
	std::swap(myUnderlying, aRight.myUnderlying);
	std::swap(myStrike, aRight.myStrike);
}

MG_VanillaOption::MG_VanillaOption	(	const vector<MG_Date>	& aMatDts
									,	const MG_Security		& aUnd
									,	const double			& aStrike)
									:	MG_Security()
									,	myMatDts	(aMatDts)
									,	myUnderlying((MG_Security*)aUnd.Clone())
									,	myStrike	(aStrike)
{}

MG_VanillaOption::~MG_VanillaOption()
{}
