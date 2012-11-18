#include "genpricer/numerics/mcmethod.h"
#include "mgnova/numerical/distributions.h"


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
	myDist.Swap(aRight.myDist);
	mySimulations.Swap(aRight.mySimulations);
}

MG_MonteCarloMethod::MG_MonteCarloMethod(	const size_t		& aTimeSteps
										,	const size_t		& aSimulNb
										,	const MG_RandDistPtr& aDist)
										:	MG_NumericalMethod()
										,	myTimeSteps		(aTimeSteps) // columns
										,	mySimulNb		(aSimulNb) // rows
										,	myDist			(aDist)
										,	mySimulations	(aSimulNb, aTimeSteps)
{}

MG_MonteCarloMethod::~MG_MonteCarloMethod()
{}

void MG_MonteCarloMethod::Simulate(void)
{
	for(size_t i=0; i<mySimulNb; ++i)
		for(size_t j=0; j<myTimeSteps; ++j)
			mySimulations(i, j) = myDist->Draw();
}
