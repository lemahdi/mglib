#include "mgnova/irindex.h"


using namespace std;
using namespace MG;


/* IR Descriptor class */
MG_IRIndex::MG_IRIndex	(	const MG_IRIndex& aRight)
						:	MG_XLObject(aRight)
						,	myCcy(aRight.myCcy), myIndexName(aRight.myIndexName)
						,	myRstCal(aRight.myRstCal), myPayCal(aRight.myPayCal)
						,	myDayCount(aRight.myDayCount)
						,	myAdjRule(aRight.myAdjRule)
						,	myRstTiming(aRight.myRstTiming), myPayTiming(aRight.myPayTiming)
						,	myRstGap(aRight.myRstGap), myPayGap(aRight.myPayGap)
{}

void MG_IRIndex::Swap(MG_IRIndex& aRight)
{
	MG_XLObject::Swap(aRight);

	swap(myIndexName, aRight.myIndexName);
	myCcy.Swap(aRight.myCcy);
	swap(myDayCount, aRight.myDayCount);
	swap(myAdjRule, aRight.myAdjRule);
	swap(myRstTiming, aRight.myRstTiming);
	swap(myPayTiming, aRight.myPayTiming);
	swap(myRstCal, aRight.myRstCal);
	swap(myPayCal, aRight.myPayCal);
	swap(myRstGap, aRight.myRstGap);
	swap(myPayGap, aRight.myPayGap);
}

MG_IRIndex::MG_IRIndex	(	const INDEX_NAME	& aIdxNm
						,	const MG_Currency	& aCcy
						,	const DAYCOUNT_NAME	& aDayCount
						,	const ADJRULE_NAME	& aAdjRule
						,	const TIMING_NAME	& aRstTiming
						,	const TIMING_NAME	& aPayTiming
						,	const CALENDAR_NAME	& aRstCal
						,	const CALENDAR_NAME	& aPayCal
						,	const int			& aRstGap
						,	const int			& aPayGap)
						:	myIndexName(aIdxNm), myCcy(aCcy)
						,	myDayCount(aDayCount)
						,	myAdjRule(aAdjRule)
						,	myRstTiming(aRstTiming), myPayTiming(aPayTiming)
						,	myRstCal(aRstCal), myPayCal(aPayCal)
						,	myRstGap(aRstGap), myPayGap(aPayGap)
{
	myXLName = MG_IRINDEX_XL_NAME;
}

MG_IRIndex::~MG_IRIndex()
{}

