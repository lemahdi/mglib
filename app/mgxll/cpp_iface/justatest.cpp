#include "mgxll/cpp_iface/justatest.h"
#include "xlw/mgw/xlcache.h"
#include "mgmodel/model.h"


using namespace std;
using namespace MG;


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
	return MG_XLObjectPtr(new MG_Call(100, 120, 0.5));
}

double Price(MG_XLObjectPtr& aSec, MG_XLObjectPtr& aMod)
{
	MG_Call* vSec = dynamic_cast<MG_Call*>(&*aSec);
	return dynamic_cast<MG_BSModel*>(&*aMod)->CallPrice(vSec->myFwd, vSec->myStrike, vSec->myMaturity);
}
