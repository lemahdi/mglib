#include "mgxll/cpp_iface/justatest.h"
#include "xlw/cache/xlcache.h"
#include "mgmodel/model.h"
#include "mgnova/exception.h"
#include "mgnova/utils/utils.h"
#include "mgnova/argconvdef.h"
#include "mgnova/schedule.h"
#include "mgmktdata/marketdata.h"
#include "mggenpricer/gensec/gensecurity.h"
#include "mgnumerical/random.h"


using namespace std;
using namespace MG;
using namespace MG_utils;


MG_Date JustATest(const MG_Date& aDate)
{
	Coord vTop, vBottom;
	MG_XL_Cached::GetCaller(vTop, vBottom);

	string vSheetName;
	MG_XL_Cached::GetSheetNm(vSheetName);

	return aDate;
}

MG_XLObjectPtr Robot(const MG_Date& aAsOf, CellMatrix& aMktData)
{
	MG_MarketDataPtr vMktData(NULL);
	size_t vSize = aMktData.RowsInStructure();
	string vDesc;
	vector<MG_MarketDataPtr> vMDVect(vSize);
	for(size_t i=0; i<vSize; ++i)
		vMDVect[i] = MG_MarketDataPtr(aMktData(i, 0).MGObjectValue()->Clone());

	return MG_XLObjectPtr(new MG_Robot(aAsOf, vMDVect));
}

MG_XLObjectPtr BSModel(const MG_Date& aAsOf, MG_XLObjectPtr& aRobot)
{
	MG_BSModel* vMod = new MG_BSModel(aAsOf);
	MG_RobotPtr vRbt(aRobot);
	vMod->Register(vRbt);
	return MG_XLObjectPtr(vMod);
}


MG_XLObjectPtr Call(const double& Strike, const double& Maturity, const double& Forward)
{
	return MG_XLObjectPtr(new MG_Call(Strike, Maturity, Forward));
}

double Price(MG_XLObjectPtr& aSec, MG_XLObjectPtr& aMod)
{
	MG_Call* vSec = dynamic_cast<MG_Call*>(&*aSec);
	return dynamic_cast<MG_BSModel*>(&*aMod)->CallPrice(vSec->myFwd, vSec->myStrike, vSec->myMaturity);
}

MG_XLObjectPtr
ZeroCurve_Create	(	const MG_Date	& aAsOf
					,	const CellMatrix& aMaturities
					,	const CellMatrix& aZeroRates
					,	const string& aCcy
					,	const string& aUnderIndex
					,	const CellMatrix& aInterpolMeth)
{
	if (aZeroRates.Size() != aMaturities.Size())
		MG_THROW("Zero vector size and Maturities size are not consistent");

	MG_Vector	vMaturities	= FromCellMatrixToMGVectorDouble(aMaturities, 0);
	MG_Matrix	vZeroRates	= FromCellMatrixToMGMatrix		(aZeroRates);

	vector<int> vInterpolMeths = vector<int>(1, LIN_INTERPOL);
	if (!aInterpolMeth(0,0).IsEmpty())
		vInterpolMeths = FromCellMatrixToInterpolVector(aInterpolMeth);
	long vInterpolCode = MG_Interpolator::CreateInterpolTypes(vInterpolMeths);

	return MG_XLObjectPtr(new MG_ZeroCurve(aAsOf, vMaturities, vZeroRates, aCcy, aUnderIndex, vInterpolCode));
}

double ComputeZeroRate(MG_XLObjectPtr& aZeroCurve, const double& aMaturity)
{
	return dynamic_cast<MG_ZeroCurve*>(&*aZeroCurve)->ComputeValue(aMaturity);
}

MG_XLObjectPtr
VolatilityCurve_Create	(	const MG_Date	& aAsOf
						,	const CellMatrix& aMaturities
						,	const CellMatrix& aTenors
						,	const CellMatrix& aVolatilities
						,	const string& aCcy
						,	const string& aUnderIndex
						,	const CellMatrix& aInterpolMeths)
{
	if (aVolatilities.Size() != aTenors.Size()*aMaturities.Size())
		MG_THROW("Volatilities matrix size and (Maturities,Tenors) size are not consistent");

	MG_Vector vMaturities	= FromCellMatrixToMGVectorDouble(aMaturities, 0);
	MG_Vector vTenors		= FromCellMatrixToMGVectorDouble(aTenors, 0);
	MG_Matrix vVols			= FromCellMatrixToMGMatrix		(aVolatilities);

	vector<int> vInterpolMeths = vector<int>(2, LIN_INTERPOL);
	if (!aInterpolMeths(0,0).IsEmpty())
		vInterpolMeths = FromCellMatrixToInterpolVector(aInterpolMeths);
	long vInterpolCode = MG_Interpolator::CreateInterpolTypes(vInterpolMeths);

	return MG_XLObjectPtr(new MG_IRVolatilityCurve(aAsOf, vMaturities, vTenors, vVols, aCcy, aUnderIndex, vInterpolCode));
}

double ComputeVolatility(MG_XLObjectPtr& aVolCurve, const double& aTenor, const double& aMaturity)
{
	return dynamic_cast<MG_IRVolatilityCurve*>(&*aVolCurve)->ComputeValue(aTenor, aMaturity);
}

MG_XLObjectPtr
DividendsTable_Create	(	const MG_Date		& aAsOf
						,	const CellMatrix	& aExDivDates
						,	const CellMatrix	& aPaymentDates
						,	const CellMatrix	& aDividends
						,	const string		& aCcy
						,	const string		& aUnderIndex
						,	const MG_XLObjectPtr& aZC)
{
	if (aExDivDates.Size() != aExDivDates.Size())
		MG_THROW("Ex dividends dates and payments days should be equal");

	MG_Vector vExDivDates	= FromCellMatrixToMGVectorDate	(aExDivDates, 0);
	MG_Vector vPaymentDates	= FromCellMatrixToMGVectorDate	(aPaymentDates, 0);
	MG_Vector vDividends	= FromCellMatrixToMGVectorDouble(aDividends, 0);

	return MG_XLObjectPtr(new MG_DividendsTable(aAsOf, vExDivDates, vPaymentDates, vDividends, aCcy, aUnderIndex, aZC));
}

double ComputeDiscountedDivs(MG_XLObjectPtr& aDividends, const MG_Date& aT1, const MG_Date& aT2)
{
	assert(aT1 <= aT2);
	return dynamic_cast<MG_DividendsTable*>(&*aDividends)->ComputeValue(aT1.GetJulianDay(), aT2.GetJulianDay());
}

MG_XLObjectPtr GenSec_Create(const CellMatrix& aDealDesc)
{
	size_t vColsSize = aDealDesc.ColumnsInStructure();
	vector<bool> vIsDate(vColsSize);
	string vTmp, vDateStr("DATE");
	for(size_t i=0; i<vColsSize; ++i)
	{
		vTmp = aDealDesc(0,i).StringValue();
		vTmp = ToUpper(vTmp.substr(vTmp.size()-4));
		vIsDate[i] = vTmp==vDateStr ? true : false;
	}
	
	vector<string> vDealDesc = FromCellMatrixToVectorStr(aDealDesc, vIsDate);
	return MG_XLObjectPtr(new MG_GenSecurity(vDealDesc, vColsSize));
}

MG_XLObjectPtr RandGen_Create(const string& aType, const int& aDim)
{
	bool vIsRand	= RandGenConvertor.Exist(aType);
	bool vIsQRand	= QuasiRandGenConvertor.Exist(aType);
	MG_AbstractRandom* vRandGen(NULL);

	if (vIsRand)
	{
		RAND_TYPE vType = (RAND_TYPE)RandGenConvertor[aType];
		vRandGen = new MG_Random(vType);
	}

	if (vIsQRand)
	{
		QUASIRAND_TYPE vType = (QUASIRAND_TYPE)QuasiRandGenConvertor[aType];
		vRandGen = new MG_QuasiRandom(vType, aDim);
	}

	MG_XLObjectPtr vObj(vRandGen);
	return vObj;
}

MG_Date NextBusinessDay	(	MG_Date aDt
						,	const int& aDays
						,	const string& aCalendar)
{
	CALENDAR_NAME vCal = (CALENDAR_NAME)CalendarsNameConvertor[aCalendar];
	aDt.NextBusinessDay(aDays, vCal);

	return aDt;
}

MG_Date PreviousBusinessDay	(	MG_Date aDt
							,	const int& aDays
							,	const string& aCalendar)
{
	CALENDAR_NAME vCal = (CALENDAR_NAME)CalendarsNameConvertor[aCalendar];
	aDt.PreviousBusinessDay(aDays, vCal);

	return aDt;
}

double BetweenDates(	MG_Date aDate1
				,	MG_Date aDate2
				,	const string& aDayCount
				,	const bool& aIsFrac
				,	const string& aCalendar)
{
	CALENDAR_NAME vCal = CALENDAR_NAME_DEF;
	if (aCalendar != "")
		vCal = (CALENDAR_NAME)CalendarsNameConvertor[aCalendar];

	DAYCOUNT_NAME vDayCount = (DAYCOUNT_NAME)DayCountNameConvertor[aDayCount];

	return aDate2.BetweenDays(aDate1, vDayCount, aIsFrac, vCal);
}

MG_Date AddPeriod	(	MG_Date aDt
					,	const string	& aFreq
					,	const int		& aTimes
					,	const string	& aCalendar
					,	const string	& aAdjRule
					,	const bool		& aEndOfMonth)
{
	string vFreq;
	int vTimes;
	SplitFrequency(aFreq, vTimes, vFreq);
	if (vFreq == "Y") vFreq = "A";

	FREQUENCY_NAME vFreqNm = (FREQUENCY_NAME)FrequencyNameConvertor[vFreq];
	ADJRULE_NAME vAdjRule = (ADJRULE_NAME)AdjustmentRuleNameConvertor[aAdjRule];
	CALENDAR_NAME vCal = (CALENDAR_NAME)CalendarsNameConvertor[aCalendar];

	aDt.AddPeriod(vFreqNm, vTimes*aTimes, vCal, vAdjRule, aEndOfMonth);

	return aDt;
}


MG_XLObjectPtr Schedule_Create	(	const MG_Date	& aStartDate
								,	const MG_Date	& aEndDate
								,	const string	& aCurrency
								,	const string	& aFreq
								,	const string	& aIndexName
								,	const string	& aDayCount
								,	const string	& aIntRule
								,	const string	& aAdjRule
								,	const string	& aStubRule
								,	const string	& aResetTiming
								,	const string	& aPayTiming
								,	const string	& aResetCalendar
								,	const string	& aPayCalendar
								,	const int		& aResetGap
								,	const int		& aPayGap
								,	const bool		& aIsDecompound)
{
	MG_Currency vCcy(aCurrency);
	FREQUENCY_NAME vFreq = (FREQUENCY_NAME)FrequencyNameConvertor[aFreq];
	INDEX_NAME vIdxNm = (INDEX_NAME)IndexNameConvertor[aIndexName];
	DAYCOUNT_NAME vDayCount = (DAYCOUNT_NAME)DayCountNameConvertor[aDayCount];
	ADJ_NAME vIntAdj = (ADJ_NAME)AdjustmentNameConvertor[aIntRule];
	ADJRULE_NAME vAdjRule = (ADJRULE_NAME)AdjustmentRuleNameConvertor[aAdjRule];
	STUBRULE_NAME vStubRule = (STUBRULE_NAME)StubRuleNameConvertor[aStubRule];
	TIMING_NAME vRstTiming = (TIMING_NAME)TimingNameConvertor[aResetTiming];
	TIMING_NAME vPayTiming = (TIMING_NAME)TimingNameConvertor[aPayTiming];
	CALENDAR_NAME vRstCal = (CALENDAR_NAME)CalendarsNameConvertor[aResetCalendar];
	CALENDAR_NAME vPayCal = (CALENDAR_NAME)CalendarsNameConvertor[aPayCalendar];

	return MG_XLObjectPtr(new MG_Schedule	(	aStartDate, aEndDate
											,	vCcy, vFreq, vIdxNm
											,	vDayCount, vIntAdj, vAdjRule, vStubRule
											,	vRstTiming, vPayTiming
											,	vRstCal, vPayCal
											,	aResetGap, aPayGap, aIsDecompound));
}

CellMatrix Schedule_GetData(MG_XLObjectPtr& aSched, const string& aData)
{
	MG_Schedule& vSched = dynamic_cast<MG_Schedule&>(*aSched);

	MyArray vArray;
	if (aData == "ResetDate")
	{
		const vector<MG_Date>& vDts = vSched.GetResetDates();
		vArray.resize(vDts.size());
		for(size_t i=0; i<vArray.size(); ++i)
			vArray[i] = FromJulianDayToXLDate(vDts[i].GetJulianDay());
	}
	else if (aData == "StartDate")
	{
		const vector<MG_Date>& vDts = vSched.GetIntStartDates();
		vArray.resize(vDts.size());
		for(size_t i=0; i<vArray.size(); ++i)
			vArray[i] = FromJulianDayToXLDate(vDts[i].GetJulianDay());
	}
	else if (aData == "EndDate")
	{
		const vector<MG_Date>& vDts = vSched.GetIntEndDates();
		vArray.resize(vDts.size());
		for(size_t i=0; i<vArray.size(); ++i)
			vArray[i] = FromJulianDayToXLDate(vDts[i].GetJulianDay());
	}
	else if (aData == "PayDate")
	{
		const vector<MG_Date>& vDts = vSched.GetPayDates();
		vArray.resize(vDts.size());
		for(size_t i=0; i<vArray.size(); ++i)
			vArray[i] = FromJulianDayToXLDate(vDts[i].GetJulianDay());
	}
	else if (aData == "InterestDays")
	{
		const vector<unsigned int>& vDays = vSched.GetIntDays();
		vArray.resize(vDays.size());
		for(size_t i=0; i<vArray.size(); ++i)
			vArray[i] = vDays[i];
	}
	else if (aData == "InterestTerms")
		vArray = vSched.GetIntTerms();
	else if (aData == "FwdStartDate")
	{
		const vector<MG_Date>& vDts = vSched.GetFwdRateStartDates();
		vArray.resize(vDts.size());
		for(size_t i=0; i<vArray.size(); ++i)
			vArray[i] = FromJulianDayToXLDate(vDts[i].GetJulianDay());
	}
	else if (aData == "FwdEndDate")
	{
		const vector<MG_Date>& vDts = vSched.GetFwdRateEndDates();
		vArray.resize(vDts.size());
		for(size_t i=0; i<vArray.size(); ++i)
			vArray[i] = FromJulianDayToXLDate(vDts[i].GetJulianDay());
	}

	CellMatrix vInfos(vArray);
	return vInfos;
}


