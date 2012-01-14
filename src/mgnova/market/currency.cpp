#include "mgnova/market/currency.h"


using namespace std;
using namespace MG;


/*
 * Currency Struct
 */
MG_Currency::MG_Currency(	const MG_Currency& aRight)
						:	myCcyNm		(aRight.myCcyNm)
						,	mySpotDays	(aRight.mySpotDays)
						,	myCalendar	(aRight.myCalendar)
{}

void MG_Currency::Swap(MG_Currency& aRight)
{
	swap(myCcyNm, aRight.myCcyNm);
	swap(mySpotDays, aRight.mySpotDays);
	swap(myCalendar, aRight.myCalendar);
}

MG_Currency::MG_Currency(const string& aCcyNm)
{
	myCcyNm = (CURRENCY_NAME)CurrenciesNameConvertor[aCcyNm];
	myCalendar = (CALENDAR_NAME)CalendarsNameConvertor[CurrenciesNameRevertor[myCcyNm]];
	FillProperties();
}

MG_Currency::MG_Currency(const CURRENCY_NAME& aCcyNm) : myCcyNm(aCcyNm)
{
	FillProperties();
}

MG_Currency::~MG_Currency()
{}

void MG_Currency::FillProperties()
{
	switch (myCcyNm)
	{
	case K_USD:
	case K_EUR:
	case K_JPY:
	case K_GBP:
		mySpotDays = 2;
		break;
	};
}

