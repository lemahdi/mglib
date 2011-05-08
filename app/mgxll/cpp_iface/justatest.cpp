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
	size_t vMatSize;
	bool vIsMatRow(false), vIsTenorRow(true);

	if (aMaturities.ColumnsInStructure()!=1 && aMaturities.RowsInStructure()!=1)
		MG_THROW("Maturities should be a one row or column structure");
	vMatSize = aMaturities.ColumnsInStructure() * aMaturities.RowsInStructure();
	vIsMatRow = aMaturities.ColumnsInStructure() == 1;

	if (aZeroRates.ColumnsInStructure()!=1 || aZeroRates.RowsInStructure()!=vMatSize)
		MG_THROW("Zero vector size and Maturities size are not consistent");

	vector<double> vMaturities = FromCellMatrixToVectorDouble(aMaturities, 0, vIsMatRow);
	MJMatrix vZeroRates = FromCellMatrixToMJMatrix(aZeroRates);

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
	size_t vMatSize, vTenorSize;
	bool vIsMatRow(false), vIsTenorRow(true);

	if (aMaturities.ColumnsInStructure()!=1 && aMaturities.RowsInStructure()!=1)
		MG_THROW("Maturities should be a one row or column structure");
	vMatSize = aMaturities.ColumnsInStructure() * aMaturities.RowsInStructure();
	vIsMatRow = aMaturities.ColumnsInStructure() == 1;

	vTenorSize = aTenors.ColumnsInStructure() * aTenors.RowsInStructure();
	if (aTenors.ColumnsInStructure()!=1 && aTenors.RowsInStructure()!=1)
		MG_THROW("Tenors should be a one row or column structure");
	vIsTenorRow = aTenors.ColumnsInStructure() == 1;

	if (aVolatilities.ColumnsInStructure()!=vTenorSize || aVolatilities.RowsInStructure()!=vMatSize)
		MG_THROW("Volatilities matrix size and (Maturities,Tenors) size are not consistent");

	vector<double> vMaturities = FromCellMatrixToVectorDouble(aMaturities, 0, vIsMatRow);
	vector<double> vTenors = FromCellMatrixToVectorDouble(aTenors, 0, vIsTenorRow);

	MJMatrix vVols = FromCellMatrixToMJMatrix(aVolatilities);

	return MG_XLObjectPtr(new MG_IRVolatilityCurve(aAsOf, vMaturities, vTenors, vVols));
}

double ComputeVolatility(MG_XLObjectPtr& aVolCurve, const double& aTenor, const double& aMaturity)
{
	return dynamic_cast<MG_IRVolatilityCurve*>(&*aVolCurve)->ComputeValue(aTenor, aMaturity);
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
