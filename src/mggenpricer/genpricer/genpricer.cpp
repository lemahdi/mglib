#include "mggenpricer/genpricer/genpricer.h"
#include "mggenpricer/gensec/gensecurity.h"
#include "mggenpricer/genmod/pricingmodel.h"


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

	for(unsigned int i=0; i<vWalker.GetRows(); i++)
	{
		Coord c(i,(unsigned int)vWalker.GetCols()-1);
		MG_Node* n = vManager.GetNode(c);
		cout << vManager.Eval(n).Double() << endl;
	}

	//for(size_t j=0; j<myPricingModel->NumMethod()->
}
