#include "mggenpricer/genpricer/genpricer.h"
#include "mggenpricer/gensec/gensecurity.h"
#include "mggenpricer/genmod/pricingmodel.h"

#include <math.h>


using namespace std;
using namespace MG;


/* Base Model class */

void MG_GenPricer::Swap(MG_GenPricer& aRight)
{
	MG_XLObject::Swap(aRight);
	myGenSec.Swap(aRight.myGenSec);
	myPricingModel.Swap(aRight.myPricingModel);
}

MG_GenPricer::MG_GenPricer	(	const MG_GenSecurityPtr& aGenSec
							,	const MG_PricingModelPtr& aPricingMdl)
							:	MG_XLObject()
							,	myGenSec		(aGenSec)
							,	myPricingModel	(aPricingMdl)
{
	myXLName = MG_GENPRICER_XL_NAME;

	myGenSec->PropagateModel(myPricingModel);
}

MG_GenPricer::~MG_GenPricer()
{}


void MG_GenPricer::Price() const
{
	const MG_TableWalker& vWalker = myGenSec->Walker();
	MG_NodeManager& vManager = myGenSec->Manager();

	const MG_MonteCarloMethod& vMC = dynamic_cast<MG_MonteCarloMethod&>(*myPricingModel->NumMethod());
	const MG_Matrix& vSimul = vMC.Simulations();
	size_t vSimulNb = vMC.SimulNb();
	vector<double> vStates(vSimulNb);
	for(size_t i=0; i<vSimulNb; ++i)
		vStates[i] = vSimul.Elt(i, 0);

	for(unsigned int i=0; i<vWalker.GetRows(); i++)
	{
		Coord c(i,(unsigned int)vWalker.GetCols()-1);
		MG_Node* n = vManager.GetNode(c);
		//cout << vManager.Eval(n).Double() << endl;
		MG_Arg vArg = vManager.Eval(n, vStates);
		double vAvg(0.);
		const vector<double>& vVals = vArg.VDouble();
		for(size_t i=0; i<vSimulNb; ++i)
			vAvg += vVals[i];
		cout << "MC: " << vAvg/vSimulNb << endl;
		
		double vMat = MG_Date(2011, 2, 15) - MG_Date(2010, 2, 15);
		vMat /= 365.;
		double vVol = 0.2;
		double vFwd = 0.02;
		cout << "Proxy: " << 0.4*vFwd*exp(-0.02*vMat)*vVol*sqrt(vMat) << endl;
	}
}
