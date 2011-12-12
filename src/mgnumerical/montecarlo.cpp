#include "mgnumerical/montecarlo.h"

#include "gsl/gsl_errno.h"
#include "mgnova/exception.h"


using namespace std;
using namespace MG;


/*
 * Monte Carlo Density
 */
double MG_MonteCarlo::MCDensity::Aire(double* aX, size_t aDim, void* aParams)
{
	MCDensity* vDensity = (MCDensity*)(aParams);
	return vDensity->AireFunction(aX, aDim, NULL);
}

void MG_MonteCarlo::MCDensity::Swap(MCDensity& aRight)
{
	swap(myFunc, aRight.myFunc);
}

void MG_MonteCarlo::MCDensity::Load(const size_t& aDim)
{
	myFunc.f = &MCDensity::Aire;
	myFunc.dim = aDim;
	myFunc.params = this;
}


/*
 * Monte Carlo Class
 */
MG_MonteCarlo::MG_MonteCarlo(	const MG_MonteCarlo& aRight)
							:	MG_XLObject(aRight)
							,	myDim	(aRight.myDim)
{}

void MG_MonteCarlo::Swap(MG_MonteCarlo& aRight)
{
	MG_XLObject::Swap(aRight);
	swap(myDim, aRight.myDim);
	myDensity.Swap(aRight.myDensity);
	myRandGen.Swap(aRight.myRandGen);
}

MG_MonteCarlo::~MG_MonteCarlo()
{}

MG_MonteCarlo::MG_MonteCarlo(	const size_t& aDim)
							:	MG_XLObject()
							,	myDim(aDim)
{
	myXLName = MG_MONTECARLO_XL_NAME;
}

void MG_MonteCarlo::Load(const MCDensityPtr& aDensity, const MG_RandomPtr& aRandGen)
{
	myDensity = aDensity;
	myRandGen = aRandGen;

	myDensity->Load(myDim);
}


/*
 * Plain Monte Carlo Class
 */
MG_PlainMC::MG_PlainMC(const MG_PlainMC& aRight) : MG_MonteCarlo(aRight)
{
	myWorkSpace = gsl_monte_plain_alloc(myDim);
	gsl_monte_plain_init(myWorkSpace);
}

void MG_PlainMC::Swap(MG_PlainMC& aRight)
{
	MG_MonteCarlo::Swap(aRight);
	swap(myWorkSpace, aRight.myWorkSpace);
}

MG_PlainMC::~MG_PlainMC()
{
	gsl_monte_plain_free(myWorkSpace);
}

MG_PlainMC::MG_PlainMC(const size_t& aDim) : MG_MonteCarlo(aDim)
{
	myWorkSpace = gsl_monte_plain_alloc(myDim);
	gsl_monte_plain_init(myWorkSpace);
}

double MG_PlainMC::Integrate(double aLow[], double aUp[], double& aErr, const size_t& aCalls)
{
	double vVal;
	gsl_monte_plain_integrate(&myDensity->myFunc, aLow, aUp, myDim, aCalls, myRandGen->GetGenerator(), myWorkSpace, &vVal, &aErr);
	return vVal;
}


/*
 * Miser Monte Carlo Class
 */
MG_MiserMC::MG_MiserMC(const MG_MiserMC& aRight) : MG_MonteCarlo(aRight)
{
	myWorkSpace = gsl_monte_miser_alloc(myDim);
	gsl_monte_miser_init(myWorkSpace);
}

void MG_MiserMC::Swap(MG_MiserMC& aRight)
{
	MG_MonteCarlo::Swap(aRight);
	swap(myWorkSpace, aRight.myWorkSpace);
}

MG_MiserMC::~MG_MiserMC()
{
	gsl_monte_miser_free(myWorkSpace);
}

MG_MiserMC::MG_MiserMC(const size_t& aDim) : MG_MonteCarlo(aDim)
{
	myWorkSpace = gsl_monte_miser_alloc(myDim);
	gsl_monte_miser_init(myWorkSpace);
}

double MG_MiserMC::Integrate(double aLow[], double aUp[], double& aErr, const size_t& aCalls)
{
	double vVal;
	gsl_monte_miser_integrate(&myDensity->myFunc, aLow, aUp, myDim, aCalls, myRandGen->GetGenerator(), myWorkSpace, &vVal, &aErr);
	return vVal;
}


/*
 * Vegas Monte Carlo Class
 */
MG_VegasMC::MG_VegasMC(const MG_VegasMC& aRight) : MG_MonteCarlo(aRight)
{
	myWorkSpace = gsl_monte_vegas_alloc(myDim);
	gsl_monte_vegas_init(myWorkSpace);
}

void MG_VegasMC::Swap(MG_VegasMC& aRight)
{
	MG_MonteCarlo::Swap(aRight);
	swap(myWorkSpace, aRight.myWorkSpace);
}

MG_VegasMC::~MG_VegasMC()
{
	gsl_monte_vegas_free(myWorkSpace);
}

MG_VegasMC::MG_VegasMC(const size_t& aDim) : MG_MonteCarlo(aDim)
{
	myWorkSpace = gsl_monte_vegas_alloc(myDim);
	gsl_monte_vegas_init(myWorkSpace);
}

double MG_VegasMC::Integrate(double aLow[], double aUp[], double& aErr, const size_t& aCalls)
{
	double vVal;
	gsl_monte_vegas_integrate(&myDensity->myFunc, aLow, aUp, myDim, aCalls, myRandGen->GetGenerator(), myWorkSpace, &vVal, &aErr);
	return vVal;
}
