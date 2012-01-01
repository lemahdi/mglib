#include "mgsecurity/security.h"


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

