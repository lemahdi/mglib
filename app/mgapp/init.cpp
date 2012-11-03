#include "../app/mgapp/init.h"
#include "mgmktdata/marketdata.h"


using namespace std;


MG_NAMESPACE_BEGIN


MG_ZeroCurve* BuildZC(const MG_Date& aAsOf)
{
	vector<double> vMat(10);
	vector<double> vDfs(10);

	vMat[0] = 0.5; vMat[1] = 1; vMat[2] = 2; vMat[3] = 3; vMat[4] = 4;
	vMat[5] = 5; vMat[6] = 6; vMat[7] = 7; vMat[8] = 8; vMat[9] = 10;

	vDfs[0] = 0.99; vDfs[1] = 0.96; vDfs[2] = 0.9; vDfs[3] = 0.85; vDfs[4] = 8;
	vDfs[5] = 0.7; vDfs[6] = 0.6; vDfs[7] = 0.5; vDfs[8] = 0.4; vDfs[9] = 0.3;

	return new MG_ZeroCurve(aAsOf, vMat, vDfs, "EUR", "EURIB", interpoltypeLinear);
}


MG_NAMESPACE_END