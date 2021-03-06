#include "vanilla/security/option.h"
#include "vanilla/model/model.h"


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
	myUnderlying.Swap(aRight.myUnderlying);
	std::swap(myStrike, aRight.myStrike);
}

MG_VanillaOption::MG_VanillaOption	(	const vector<MG_Date>	& aMatDts
									,	const MG_SecurityPtr	& aUnd
									,	const double			& aStrike)
									:	MG_Security()
									,	myMatDts	(aMatDts)
									,	myUnderlying((MG_Security*)aUnd->Clone())
									,	myStrike	(aStrike)
{}

MG_VanillaOption::~MG_VanillaOption()
{}
