#include "mggenpricer/genmod/hw1f.h"


using namespace std;
using namespace MG;


/*
 * IR Pricing Model Base Class
 */
MG_HW1F::MG_HW1F(const MG_HW1F& aRight) : MG_IRPricingModel(aRight)
{}

void MG_HW1F::Swap(MG_HW1F& aRight)
{
	MG_IRPricingModel::Swap(aRight);
}

MG_HW1F::MG_HW1F() : MG_PricingModel()
{
	myXLName = MG_HW1FMODEL_XL_NAME;
}

MG_HW1F::~MG_HW1F()
{}
