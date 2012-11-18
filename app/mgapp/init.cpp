#include "../app/mgapp/init.h"
#include "mktdata/marketdata.h"


using namespace std;


MG_NAMESPACE_BEGIN


MG_ZeroCurve* BuildZC(const MG_Date& aAsOf)
{
	vector<double> vMat(10);
	vector<double> vDfs(10);

	vMat[0] = 0.5; vMat[1] = 1; vMat[2] = 2; vMat[3] = 3; vMat[4] = 4;
	vMat[5] = 5; vMat[6] = 6; vMat[7] = 7; vMat[8] = 8; vMat[9] = 10;

	for(size_t i=0; i<10; ++i)
		vDfs[i] = 2.;

	return new MG_ZeroCurve(aAsOf, vMat, vDfs, "EUR", "EURIB", interpoltypeLinear);
}


MG_NAMESPACE_END