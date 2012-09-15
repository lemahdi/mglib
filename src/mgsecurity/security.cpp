#include "mgsecurity/security.h"
#include "mgnova/utils/utils.h"
#include "mgmodel/model.h"


using namespace std;
using namespace MG;


/* Base Security class */
MG_Security::MG_Security() : MG_XLObject()
{}

void MG_Security::Swap(MG_Security& aRight)
{
	MG_XLObject::Swap(aRight);
}

MG_Security::~MG_Security()
{}


/* Base IR Security class */
MG_IRSecurity::MG_IRSecurity(	const MG_IRSecurity& aRight)
							:	MG_Security(aRight)
							,	mySchedule(aRight.mySchedule)
{}

void MG_IRSecurity::Swap(MG_IRSecurity& aRight)
{
	MG_Security::Swap(aRight);
	mySchedule.Swap(aRight.mySchedule);
}

MG_IRSecurity::MG_IRSecurity(	const MG_Schedule& aSched)
							:	MG_Security()
							,	mySchedule(aSched)
{}

MG_IRSecurity::~MG_IRSecurity()
{}

void MG_IRSecurity::GenerateSchedule(const MG_GenericDate& aStDt, const MG_GenericDate& aEdDt, const MG_IRIndex& aIRIndex)
{
	FREQUENCY_NAME vFreq = MG_utils::GetFrequencyFromIndex(aIRIndex.GetIndexName());
	mySchedule = MG_Schedule(aStDt, aEdDt, aIRIndex, vFreq);
}


/* Base IR Option class */
MG_IROption::MG_IROption(	const MG_IROption& aRight)
						:	MG_Security(aRight)
						,	myMaturity	(aRight.myMaturity)
						,	myUnderlying(aRight.myUnderlying->Clone())
{}

void MG_IROption::Swap(MG_IROption& aRight)
{
	MG_Security::Swap(aRight);
	myMaturity.Swap(aRight.myMaturity);
	myUnderlying.Swap(aRight.myUnderlying);
}

MG_IROption::MG_IROption(	const MG_GenericDate& aMaturity, const MG_IRSecurityPtr& aUnderlying)
						:	MG_Security()
						,	myMaturity	(aMaturity)
						,	myUnderlying(aUnderlying)
{}

MG_IROption::~MG_IROption()
{}

