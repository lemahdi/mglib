#include "genpricer/genpricer/genpricer.h"
#include "genpricer/gensec/gensecurity.h"
#include "genpricer/genmod/pricingmodel.h"

#include "genpricer/genmod/irpricingmodel.h"

#include "mgnova/numerical/distributions.h"
#include "genpricer/infra/arg.h"

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
		const vector<double>& vVals = *vArg.VDouble();
		for(size_t i=0; i<vSimulNb; ++i)
			vAvg += vVals[i];
		vAvg /= vSimulNb;

		MG_Date vRstDt(2011, 2, 15);
		MG_Date vStDt(2011, 2, 17);
		MG_Date vEdDt(2011, 5, 17);
		
		double vMat = MG_Date(2011, 2, 15) - MG_Date(2010, 2, 15);
		vMat /= 365.;
		double vVol = 0.2;
		double vFwd = dynamic_cast<MG_IRPricingModel&>(*myPricingModel).Libor(vStDt, vEdDt, vEdDt, 0.25);
		double vStrike = 0.01955;

		double vD1 = 0.5*vVol*sqrt(vMat);
		double vD2 = -vD1;
		
		double vMC = vAvg/vFwd;
		double vBS = (vFwd*MG_NormalDist::CdfFunc(vD1) - vStrike*MG_NormalDist::CdfFunc(vD2)) / vFwd;
		cout << "MC: " << vMC/**exp(-0.02*vMat)*/ << endl;
		cout << "Proxy: " << 0.4*vFwd/**exp(-0.02*vMat)*/*vVol*sqrt(vMat)/vFwd << endl;
		cout << "BS: " << vBS << endl;

		cout << "MC-BS: " << (vMC-vBS)*10000. << " bp" << endl;
	}
}
