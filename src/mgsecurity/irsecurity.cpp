#include "mgsecurity/irsecurity.h"
#include "mgnova/utils/utils.h"


using namespace std;
using namespace MG;


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

