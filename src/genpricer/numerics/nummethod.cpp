#include "genpricer/numerics/nummethod.h"


using namespace std;
using namespace MG;


/*
 * IR Pricing Model Base Class
 */
MG_NumericalMethod::MG_NumericalMethod(const MG_NumericalMethod& aRight) : MG_XLObject(aRight)
{}

void MG_NumericalMethod::Swap(MG_NumericalMethod& aRight)
{
	MG_XLObject::Swap(aRight);
}

MG_NumericalMethod::MG_NumericalMethod() : MG_XLObject()
{}

MG_NumericalMethod::~MG_NumericalMethod()
{}
