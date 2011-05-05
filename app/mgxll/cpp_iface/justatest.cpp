#include "mgxll/cpp_iface/justatest.h"
#include "xlw/cache/xlcache.h"
#include "mgmodel/model.h"
#include "mgnova/exception.h"
#include "mgnova/utils/utils.h"
#include "mgmktdata/marketdata.h"


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
