#include "mgxll/cpp_iface/justatest.h"
#include "xlw/cache/xlcache.h"
#include "mgmodel/model.h"
#include "mgnova/exception.h"
#include "mgnova/utils/utils.h"
#include "mgmktdata/marketdata.h"
#include "mggenpricer/gensec/gensecurity.h"


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

MG_XLObjectPtr BSModel(const MG_Date& aAsOf, const double& aVol)
{
	return MG_XLObjectPtr(new MG_BSModel(aAsOf, aVol));
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
					,	const CellMatrix& aZeroRates)
{
	if (aZeroRates.Size() != aMaturities.Size())
		MG_THROW("Zero vector size and Maturities size are not consistent");

	vector<double>	vMaturities	= FromCellMatrixToVectorDouble	(aMaturities, 0);
	MJMatrix		vZeroRates	= FromCellMatrixToMJMatrix		(aZeroRates);

	return MG_XLObjectPtr(new MG_ZeroCurve(aAsOf, vMaturities, vZeroRates));
}

double ComputeZeroRate(MG_XLObjectPtr& aZeroCurve, const double& aMaturity)
{
	return dynamic_cast<MG_ZeroCurve*>(&*aZeroCurve)->ComputeValue(aMaturity);
}

MG_XLObjectPtr
VolatilityCurve_Create	(	const MG_Date	& aAsOf
						,	const CellMatrix& aMaturities
						,	const CellMatrix& aTenors
						,	const CellMatrix& aVolatilities)
{
	if (aVolatilities.Size() != aTenors.Size()*aMaturities.Size())
		MG_THROW("Volatilities matrix size and (Maturities,Tenors) size are not consistent");

	vector<double>	vMaturities	= FromCellMatrixToVectorDouble	(aMaturities, 0);
	vector<double>	vTenors		= FromCellMatrixToVectorDouble	(aTenors, 0);
	MJMatrix		vVols		= FromCellMatrixToMJMatrix		(aVolatilities);

	return MG_XLObjectPtr(new MG_IRVolatilityCurve(aAsOf, vMaturities, vTenors, vVols));
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
						,	const MG_XLObjectPtr& aZC)
{
	if (aExDivDates.Size() != aExDivDates.Size())
		MG_THROW("Ex dividends dates and payments days should be equal");

	vector<MG_Date>	vExDivDates		= FromCellMatrixToVectorDate	(aExDivDates, 0);
	vector<MG_Date>	vPaymentDates	= FromCellMatrixToVectorDate	(aPaymentDates, 0);
	vector<double>	vDividends		= FromCellMatrixToVectorDouble	(aDividends, 0);

	return MG_XLObjectPtr(new MG_DividendsTable(aAsOf, vExDivDates, vPaymentDates, vDividends, aZC));
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
