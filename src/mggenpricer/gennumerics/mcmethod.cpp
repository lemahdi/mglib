#include "mggenpricer/gennumerics/mcmethod.h"
#include "mgnova/numerical/random.h"


using namespace std;
using namespace MG;


/*
 * IR Pricing Model Base Class
 */
MG_MonteCarloMethod::MG_MonteCarloMethod(	const MG_MonteCarloMethod& aRight)
										:	MG_NumericalMethod(aRight)
										,	myTimeSteps	(aRight.myTimeSteps)
										,	mySimulNb	(aRight.mySimulNb)
{}

void MG_MonteCarloMethod::Swap(MG_MonteCarloMethod& aRight)
{
	MG_NumericalMethod::Swap(aRight);
	std::swap(myTimeSteps, aRight.myTimeSteps);
	std::swap(mySimulNb, aRight.mySimulNb);
	myRandGen.Swap(aRight.myRandGen);
	mySimulations.Swap(aRight.mySimulations);
}

MG_MonteCarloMethod::MG_MonteCarloMethod(	const size_t		& aTimeSteps
										,	const size_t		& aSimulNb
										,	const MG_RandomPtr& aRndGen)
										:	MG_NumericalMethod()
										,	myTimeSteps		(aTimeSteps)
										,	mySimulNb		(aSimulNb)
										,	myRandGen		(aRndGen)
										,	mySimulations	(aSimulNb, aTimeSteps)
{}

MG_MonteCarloMethod::~MG_MonteCarloMethod()
{}

void MG_MonteCarloMethod::Simulate()
{
	for(size_t i=0; i<mySimulNb; ++i)
		for(size_t j=0; j<myTimeSteps; ++j)
			mySimulations(i, j) = myRandGen->DrawUniform();
}
