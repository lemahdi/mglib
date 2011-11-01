#include "mgnova/wrapper/function.h"


using namespace std;
using namespace MG;


/*
 * Function Base Clas
 */
void MG_Function::Swap(MG_Function& )
{}
	
double MG_Function::F(double aX, void* aEngine)
{
	return ((MG_Function*)aEngine)->F_Func(aX);
}

double MG_Function::Df(double aX, void* aEngine)
{
	return ((MG_Function*)aEngine)->Df_Func(aX);
}

void MG_Function::FDf(double aX, void* aEngine, double* aF, double* aDf)
{
	((MG_Function*)aEngine)->FDf_Func(aX, *aF, *aDf);
}


/*
 * 0 Order Function Class
 */
MG_FFunction::MG_FFunction	(	const MG_FFunction& aRight)
							:	MG_Function(aRight)
{
	myFunc.function = &F;
	myFunc.params = this;
}

void MG_FFunction::Swap(MG_FFunction& aRight)
{
	MG_Function::Swap(aRight);
	swap(myFunc, aRight.myFunc);
}

MG_FFunction::MG_FFunction()
{
	myFunc.function = &F;
	myFunc.params = this;
}

MG_FFunction::~MG_FFunction()
{}


/*
 * 1st Order Function Class
 */
MG_FDfFunction::MG_FDfFunction	(	const MG_FDfFunction& aRight)
								:	MG_Function(aRight)
{
	myFunc.f = &F;
	myFunc.df = &Df;
	myFunc.fdf = &FDf;
	myFunc.params = this;
}

void MG_FDfFunction::Swap(MG_FDfFunction& aRight)
{
	MG_Function::Swap(aRight);
	swap(myFunc, aRight.myFunc);
}

MG_FDfFunction::MG_FDfFunction()
{
	myFunc.f = &F;
	myFunc.df = &Df;
	myFunc.fdf = &FDf;
	myFunc.params = this;
}

MG_FDfFunction::~MG_FDfFunction()
{}

