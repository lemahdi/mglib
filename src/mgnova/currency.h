/*
 * Copyright			: 2011 by MG
 * File					: currency.h
 * Version				: 0.1.23
 * Last changed			: 31 DEC 2011
 * Purpose				: MG_Currency is a struct for currency attributes
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "mgnova/typedef.h"
//#include "mgnova/object.h"
#include "mgnova/argconvdef.h"
//#include "mgnova/date.h"

//#include <vector>


MG_NAMESPACE_BEGIN


/* Solver Base Class */
struct MG_Currency
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_Currency)

	ASSIGN_OPERATOR(MG_Currency)
	//CLONE_METHOD(MG_Currency)
	SWAP_DECL(MG_Currency)

	MG_Currency	(const std::string& aCcyNm);
	MG_Currency	(const CURRENCY_NAME& aCcyNm);

	virtual ~MG_Currency(void);

	/* State */
	inline std::string GetCcyNm(void) const { return CurrenciesNameRevertor[myCcyNm]; }
private:
	void FillProperties(void);

	/* Engine */

public:
	CURRENCY_NAME	myCcyNm;
	int				mySpotDays;
	CALENDAR_NAME	myCalendar;
};


MG_NAMESPACE_END

