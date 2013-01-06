#include "vanilla/security/irsecurity.h"
#include "nova/utils/utils.h"


using namespace std;
using namespace MG;


/* Base IR Security class */
MG_IRSecurity::MG_IRSecurity(	const MG_IRSecurity& aRight)
							:	MG_Security(aRight)
							,	mySchedule	(aRight.mySchedule)
							,	myNX		(aRight.myNX)
{}

void MG_IRSecurity::Swap(MG_IRSecurity& aRight)
{
	MG_Security::Swap(aRight);
	mySchedule.Swap(aRight.mySchedule);
	std::swap(myNX, aRight.myNX);
}

MG_IRSecurity::MG_IRSecurity(	const NX_NAME& aNX
							,	const MG_TermStructure& aNotional)
							:	MG_Security(aNotional)
							,	myNX(aNX)
{}

MG_IRSecurity::MG_IRSecurity(	const MG_Schedule& aSched
							,	const NX_NAME& aNX
							,	const MG_TermStructure& aNotional)
							:	MG_Security(aNotional)
							,	mySchedule	(aSched)
							,	myNX		(aNX)
{}

MG_IRSecurity::~MG_IRSecurity()
{}

void MG_IRSecurity::GenerateSchedule(const MG_GenericDate& aStDt, const MG_GenericDate& aEdDt, const MG_IRIndex& aIRIndex)
{
	FREQUENCY_NAME vFreq = MG_utils::GetFrequencyFromIndex(aIRIndex.GetIndexName());
	mySchedule = MG_Schedule(aStDt, aEdDt, aIRIndex, vFreq);
}

void MG_IRSecurity::PrePricing(const MG_Model& aMdl)
{
}

