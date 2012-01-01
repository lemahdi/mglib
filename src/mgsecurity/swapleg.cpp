#include "mgsecurity/swapleg.h"


using namespace std;
using namespace MG;


/* IR Swap Leg class */
void MG_SwapLeg::Swap(MG_SwapLeg& aRight)
{
	MG_IRSecurity::Swap(aRight);
}

MG_SwapLeg::MG_SwapLeg	(	const MG_Schedule& aSched)
						:	MG_IRSecurity(aSched)
{}

MG_SwapLeg::~MG_SwapLeg()
{}

