#include "nova/glob/genericdate.h"
#include "nova/utils/utils.h"


using namespace std;
using namespace MG;


/*
 * Generic Date class
 */
MG_GenericDate::MG_GenericDate	(	const MG_GenericDate& aRight)
								:	MG_Object(aRight)
								,	myStart			(aRight.myStart)
								,	myTerm			(aRight.myTerm)
								,	myTimes			(aRight.myTimes)
								,	myFreq			(aRight.myFreq)
								,	myRefDate		(aRight.myRefDate?(MG_GenericDate*)aRight.myRefDate->Clone():NULL)
								,	myDate			(aRight.myDate)
								,	myIsGenDate		(aRight.myIsGenDate)
{}

void MG_GenericDate::Swap(MG_GenericDate& aRight)
{
	swap(const_cast<string&>(myStart), const_cast<string&>(aRight.myStart));
	swap(const_cast<string&>(myTerm), const_cast<string&>(aRight.myTerm));
	swap(myTimes, aRight.myTimes);
	swap(myFreq, aRight.myFreq);
	swap(myRefDate, aRight.myRefDate);
	myDate.Swap(aRight.myDate);
	swap(const_cast<bool&>(myIsGenDate), const_cast<bool&>(aRight.myIsGenDate));
}

MG_GenericDate::~MG_GenericDate()
{
	delete myRefDate;
}

MG_GenericDate::MG_GenericDate() : myTimes(0), myFreq(NB_FREQUENCIES), myRefDate(NULL), myIsGenDate(true)
{}

MG_GenericDate::MG_GenericDate	(	const string& aStart)
								:	myStart		(aStart)
								,	myRefDate	(NULL)
								,	myIsGenDate	(true)
{
	string vFreqS;
	MG_utils::SplitFrequency(aStart, myTimes, vFreqS);
	myFreq = (FREQUENCY_NAME)FrequencyNameConvertor[vFreqS];
}

MG_GenericDate::MG_GenericDate	(	const string& aRefStart
								,	const string& aStart)
								:	myStart		(aStart)
								,	myRefDate	(new MG_GenericDate(aRefStart))
								,	myIsGenDate	(true)
{
	string vFreqS;
	MG_utils::SplitFrequency(aStart, myTimes, vFreqS);
	myFreq = (FREQUENCY_NAME)FrequencyNameConvertor[vFreqS];
}

MG_GenericDate::MG_GenericDate	(	const MG_GenericDate& aRefStart
								,	const string& aStart)
								:	myStart		(aStart)
								,	myRefDate	(new MG_GenericDate(aRefStart))
								,	myIsGenDate	(true)
{
	string vFreqS;
	MG_utils::SplitFrequency(aStart, myTimes, vFreqS);
	myFreq = (FREQUENCY_NAME)FrequencyNameConvertor[vFreqS];
}

MG_GenericDate::MG_GenericDate	(	const MG_Date& aDt)
								:	myDate		(aDt)
								,	myTimes		(0)
								,	myFreq		(NB_FREQUENCIES)
								,	myRefDate	(NULL)
								,	myIsGenDate	(false)
{}

void MG_GenericDate::Rebuild(const MG_Date& aAsOf)
{
	if (!myIsGenDate)
		return;

	if (myRefDate)
	{
		myRefDate->Rebuild(aAsOf);
		myDate = myRefDate->GetDate();
	}
	else
		myDate = aAsOf;
	myDate.AddPeriod(myFreq, myTimes);
}
