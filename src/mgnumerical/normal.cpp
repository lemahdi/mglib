#include "mgnumerical/normal.h"

#include <math.h>
#include <assert.h>


using namespace std;
using namespace MG;


template<>
MG_CdfNormalPtr MG_SCdfNormal::myInstance = MG_CdfNormalPtr(NULL);


void MG_CdfNormal::Init(void)
{
	myGaussCst.myA.resize(5);
	myGaussCst.myA[0] = 1.161110663653770e-002;
	myGaussCst.myA[1] = 3.951404679838207e-001;
	myGaussCst.myA[2] = 2.846603853776254e+001;
	myGaussCst.myA[3] = 1.887426188426510e+002;
	myGaussCst.myA[4] = 3.209377589138469e+003;

	myGaussCst.myB.resize(5);
	myGaussCst.myB[0] = 1.767766952966369e-001;
	myGaussCst.myB[1] = 8.344316438579620e+000;
	myGaussCst.myB[2] = 1.725514762600375e+002;
	myGaussCst.myB[3] = 1.813893686502485e+003;
	myGaussCst.myB[4] = 8.044716608901563e+003;

	myGaussCst.myC.resize(9);
	myGaussCst.myC[0] = 2.15311535474403846e-8;
	myGaussCst.myC[1] = 5.64188496988670089e-1;
	myGaussCst.myC[2] = 8.88314979438837594e00;
	myGaussCst.myC[3] = 6.61191906371416295e01;
	myGaussCst.myC[4] = 2.98635138197400131e02;
	myGaussCst.myC[5] = 8.81952221241769090e02;
	myGaussCst.myC[6] = 1.71204761263407058e03;
	myGaussCst.myC[7] = 2.05107837782607147e03;
	myGaussCst.myC[8] = 1.23033935479799725E03;

	myGaussCst.myD.resize(9);
	myGaussCst.myD[0] = 1.00000000000000000e00;
	myGaussCst.myD[1] = 1.57449261107098347e01;
	myGaussCst.myD[2] = 1.17693950891312499e02;
	myGaussCst.myD[3] = 5.37181101862009858e02;
	myGaussCst.myD[4] = 1.62138957456669019e03;
	myGaussCst.myD[5] = 3.29079923573345963e03;
	myGaussCst.myD[6] = 4.36261909014324716e03;
	myGaussCst.myD[7] = 3.43936767414372164e03;
	myGaussCst.myD[8] = 1.23033935480374942e03;

	myGaussCst.myP.resize(6);
	myGaussCst.myP[0] = 1.63153871373020978e-2;
	myGaussCst.myP[1] = 3.05326634961232344e-1;
	myGaussCst.myP[2] = 3.60344899949804439e-1;
	myGaussCst.myP[3] = 1.25781726111229246e-1;
	myGaussCst.myP[4] = 1.60837851487422766e-2;
	myGaussCst.myP[5] = 6.58749161529837803e-4;

	myGaussCst.myQ.resize(6);
	myGaussCst.myQ[0] = 1.00000000000000000e00;
	myGaussCst.myQ[1] = 2.56852019228982242e00;
	myGaussCst.myQ[2] = 1.87295284992346047e00;
	myGaussCst.myQ[3] = 5.27905102951428412e-1;
	myGaussCst.myQ[4] = 6.05183413124413191e-2;
	myGaussCst.myQ[5] = 2.33520497626869185e-3;


	myGaussCst.myInvA.resize(6);
	myGaussCst.myInvA[0] = -3.969683028665376e+01;
	myGaussCst.myInvA[1] = 2.209460984245205e+02;
	myGaussCst.myInvA[2] = -2.759285104469687e+02;
	myGaussCst.myInvA[3] = 1.383577518672690e+02;
	myGaussCst.myInvA[4] = -3.066479806614716e+01;
	myGaussCst.myInvA[5] = 2.506628277459239e+00;

	myGaussCst.myInvB.resize(5);
	myGaussCst.myInvB[0] = -5.447609879822406e+01;
	myGaussCst.myInvB[1] = 1.615858368580409e+02;
	myGaussCst.myInvB[2] = -1.556989798598866e+02;
	myGaussCst.myInvB[3] = 6.680131188771972e+01;
	myGaussCst.myInvB[4] = -1.328068155288572e+01;

	myGaussCst.myInvC.resize(6);
	myGaussCst.myInvC[0] = -7.784894002430293e-03;
	myGaussCst.myInvC[1] = -3.223964580411365e-01;
	myGaussCst.myInvC[2] = -2.400758277161838e+00;
	myGaussCst.myInvC[3] = -2.549732539343734e+00;
	myGaussCst.myInvC[4] = 4.374664141464968e+00;
	myGaussCst.myInvC[5] = 2.938163982698783e+00;

	myGaussCst.myInvD.resize(4);
	myGaussCst.myInvD[0] = 7.784695709041462e-03;
	myGaussCst.myInvD[1] = 3.224671290700398e-01;
	myGaussCst.myInvD[2] = 2.445134137142996e+00;
	myGaussCst.myInvD[3] = 3.754408661907416e+00;


	/*myGaussCst.myV.resize(15);
	myGaussCst.myV[0] = 1.25331437315500;
	myGaussCst.myV[1] = 0.6556795424187985;
	myGaussCst.myV[2] = 0.4213692292880545;
	myGaussCst.myV[3] = 0.3045902987101033;
	myGaussCst.myV[4] = 0.2366523829135607;
	myGaussCst.myV[5] = 0.1928081047153158;
	myGaussCst.myV[6] = 0.1623776608968675;
	myGaussCst.myV[7] = 0.1401041834530502;
	myGaussCst.myV[8] = 0.1231319632579329;
	myGaussCst.myV[9] = 0.1097872825783083;
	myGaussCst.myV[10] = 0.09902859647173193;
	myGaussCst.myV[11] = 0.09017567550106468;
	myGaussCst.myV[12] = 0.08276628650136917;
	myGaussCst.myV[13] = 0.0764757610162485;
	myGaussCst.myV[14] = 0.07106958053885211;

	myGaussCst.myInvATmp.resize(4);
	myGaussCst.myInvATmp[0] = 2.50662823884;
	myGaussCst.myInvATmp[1] = -18.61500062529;
	myGaussCst.myInvATmp[2] = 41.39119773534;
	myGaussCst.myInvATmp[3] = -25.44106049637;

	myGaussCst.myInvBTmp.resize(4);
	myGaussCst.myInvBTmp[0] = -8.47351093090;
	myGaussCst.myInvBTmp[1] = 23.08336743743;
	myGaussCst.myInvBTmp[2] = -21.06224101826;
	myGaussCst.myInvBTmp[3] = 3.13082909833;

	myGaussCst.myInvCTmp.resize(9);
	myGaussCst.myInvCTmp[0] = 0.3374754822726147;
	myGaussCst.myInvCTmp[1] = 0.9761690190917186;
	myGaussCst.myInvCTmp[2] = 0.1607979714918209;
	myGaussCst.myInvCTmp[3] = 0.0276438810333863;
	myGaussCst.myInvCTmp[4] = 0.0038405729373609;
	myGaussCst.myInvCTmp[5] = 0.0003951896511919;
	myGaussCst.myInvCTmp[6] = 0.0000321767881768;
	myGaussCst.myInvCTmp[7] = 0.0000002888167364;
	myGaussCst.myInvCTmp[8] = 0.0000003960315187;*/
}

/* Normal density function */
double MG_CdfNormal::NormalDensity(const double &aX) const
{
	return ONEOVERROOTTWOPI * exp(-aX*aX/2);
}

double MG_CdfNormal::CumulativeNormal(const double& aX, const double& aBound) const
{
	double vZ;
	double vY = fabs(aX);

	if (vY >= aBound)
		return aX > 0. ? 1 : 0.;

	if (vY <= 0.46875*ROOTTWO)
	{
		// evaluate erf() for |u| <= sqrt(2)*0.46875 
		vZ = vY * vY;
		vY = aX	*	(	(	(	(	myGaussCst.myA[0] * vZ
								+	myGaussCst.myA[1]) * vZ
							+	myGaussCst.myA[2]) * vZ
						+	myGaussCst.myA[3]) * vZ
					+	myGaussCst.myA[4])
				/	(	(	(	(	myGaussCst.myB[0] * vZ
								+	myGaussCst.myB[1]) * vZ
							+	myGaussCst.myB[2]) * vZ
						+	myGaussCst.myB[3]) * vZ
					+	myGaussCst.myB[4]);
		return 0.5 + vY;
	}

	vZ = 0.5*exp(-0.5*vY*vY);
	if (vY <= 4.0)
	{
		// evaluate erfc() for sqrt(2)*0.46875 <= |u| <= sqrt(2)*4.0 
		vY /= ROOTTWO;
		vY =	(	(	(	(	(	(	(	(	myGaussCst.myC[0] * vY
											+	myGaussCst.myC[1]) * vY
										+	myGaussCst.myC[2]) * vY
									+	myGaussCst.myC[3]) * vY
								+	myGaussCst.myC[4]) * vY
							+	myGaussCst.myC[5]) * vY
						+	myGaussCst.myC[6]) * vY
					+	myGaussCst.myC[7]) * vY
				+	myGaussCst.myC[8])
			/	(	(	(	(	(	(	(	(	myGaussCst.myD[0] * vY
											+	myGaussCst.myD[1]) * vY
										+	myGaussCst.myD[2]) * vY
									+	myGaussCst.myD[3]) * vY
								+	myGaussCst.myD[4]) * vY
							+	myGaussCst.myD[5]) * vY
						+	myGaussCst.myD[6]) * vY
					+	myGaussCst.myD[7]) * vY
				+	myGaussCst.myD[8]);

		vY *= vZ;
	} 
	else
	{
		// evaluate erfc() for |u| > sqrt(2)*4.0 
		vZ *= ROOTTWO / vY;
		vY = 2. / (vY * vY);
		vY = vY	*	(	(	(	(	(	myGaussCst.myP[0] * vY
									+	myGaussCst.myP[1]) * vY
								+	myGaussCst.myP[2]) * vY
							+	myGaussCst.myP[3]) * vY
						+	myGaussCst.myP[4]) * vY
					+	myGaussCst.myP[5])
				/	(	(	(	(	(	myGaussCst.myQ[0] * vY
									+	myGaussCst.myQ[1]) * vY
								+	myGaussCst.myQ[2]) * vY
							+	myGaussCst.myQ[3]) * vY
						+	myGaussCst.myQ[4]) * vY
					+	myGaussCst.myQ[5]);
		vY = vZ * (ONEOVERROOTPI - vY);
	}

	return (aX<0.0 ? vY : 1.-vY);
}

double MG_CdfNormal::InverseNormal(const double& aP) const
{
	double vQ, vT, vU;
	double vError = 1.e-12;

	if (aP > 1 - vError)
		return NORMALCUMULATIVEBOUND;
	
	if (aP < vError)
		return -NORMALCUMULATIVEBOUND;

	vQ = min(aP, 1.-aP);
	if (vQ > 0.02425)
	{
		// Rational approximation for central region. 
		vU = vQ - 0.5;
		vT = vU * vU;
		vU = vU	*	(	(	(	(	(	myGaussCst.myInvA[0] * vT
									+	myGaussCst.myInvA[1]) * vT
								+	myGaussCst.myInvA[2]) * vT
							+	myGaussCst.myInvA[3]) * vT
						+	myGaussCst.myInvA[4]) * vT
					+	myGaussCst.myInvA[5])
				/	(	(	(	(	(	myGaussCst.myInvB[0] * vT
									+	myGaussCst.myInvB[1]) * vT
								+	myGaussCst.myInvB[2]) * vT
							+	myGaussCst.myInvB[3]) * vT
						+	myGaussCst.myInvB[4]) * vT
					+	1.);
	}
	else
	{
		// Rational approximation for tail region.
		vT = sqrt(-2. * log(vQ));
		vU =	(	(	(	(	(	myGaussCst.myInvC[0] * vT
								+	myGaussCst.myInvC[1]) * vT
							+	myGaussCst.myInvC[2]) * vT
						+	myGaussCst.myInvC[3]) * vT
					+	myGaussCst.myInvC[4]) * vT
				+	myGaussCst.myInvC[5])
			/	(	(	(	(	myGaussCst.myInvD[0] * vT
							+	myGaussCst.myInvD[1]) * vT
						+	myGaussCst.myInvD[2]) * vT
					+	myGaussCst.myInvD[3]) * vT
				+	1.);
	}

	// The relative error of the approximation has absolute value less
	// than 1.15e-9.  One iteration of Halley's rational method (third
	// order) gives full machine precision...

	vT = CumulativeNormal(vU) - vQ;			// error
	vT = vT * ROOTTWOPI * exp(0.5*vU*vU);	// f(vU)/df(vU)
	vU = vU - vT/(1.+0.5*vU*vT);			// Halley's method

	return (aP>0.5 ? -vU : vU);
}

/*double MG_CdfNormal::CumulativeNormal_Tmp(const double& aX) const
{
	double vAbsX = abs(aX);
	if (vAbsX >= 15.)
		return aX>0 ? 1. : 0.;

	int vJ = int(min(vAbsX+0.5, 14.));
	double vZ(vJ);
	double vH(vAbsX-vZ);
	double vA(myGaussCst.myV[vJ]);
	double vB(vZ*vA-1);
	double vQ(1.);
	double vS(vA+vH*vB);
	for(int i=2; i<=24-vJ; i+=2)
	{
		vA = (vA+vZ*vB)/i;
		vB = (vB+vZ*vA)/(i+1);
		vQ = vQ*vH*vH;
		vS = vS+vQ*(vA+vH*vB);
	}
	double vY = vS*exp(-0.5*aX*aX-LOGROOTTWOPI);
	if (aX > 0.) return 1.-vY;
	return vY;
}

double MG_CdfNormal::InverseNormal_Tmp(const double& aU) const
{
	assert(aU>=0. && aU<=1.);

	double vY(aU-0.5);
	double vAbsY(abs(vY));
	double vR, vX;
	if (vAbsY < 0.42)
	{
		vR = vY*vY;
		vX = vY * (((myGaussCst.myInvATmp[3]*vR + myGaussCst.myInvATmp[2])*vR + myGaussCst.myInvATmp[1])*vR + myGaussCst.myInvATmp[0]) /
			((((myGaussCst.myInvBTmp[3]*vR + myGaussCst.myInvBTmp[2])*vR + myGaussCst.myInvBTmp[1])*vR + myGaussCst.myInvBTmp[0])*vR + 1);
	}
	else
	{
		vR = aU;
		if (vY > 0.) vR = 1.-aU;
		vR = log(-log(vR));
		vX = myGaussCst.myInvCTmp[0] + vR*(myGaussCst.myInvCTmp[1] + vR*(myGaussCst.myInvCTmp[2] + vR*(myGaussCst.myInvCTmp[3] + vR*(myGaussCst.myInvCTmp[4] +
			vR*(myGaussCst.myInvCTmp[5] + vR*(myGaussCst.myInvCTmp[6] + vR*(myGaussCst.myInvCTmp[7] + vR*myGaussCst.myInvCTmp[8])))))));
		if (vY < 0.) vX = -vX;
	}

	return vX;
}*/
