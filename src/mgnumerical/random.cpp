#include "mgnumerical/random.h"

#include <assert.h>
#include <math.h>

#include "mgnumerical/normal.h"

using namespace std;
using namespace MG;


/* Base Random class */
MG_Random::MG_Random()
{}


/* Park Miller*/
const long MG_ParkMillerRand::ourA = 16807;
const long MG_ParkMillerRand::ourM = 2147483647;
const long MG_ParkMillerRand::ourQ = 127773;
const long MG_ParkMillerRand::ourR = 2836;

MG_ParkMillerRand::MG_ParkMillerRand(const long& aSeed)
	:	MG_Random()
	,	myInitialSeed(aSeed), mySeed(aSeed)
{
	assert(myInitialSeed != 0);
}

MG_ParkMillerRand::MG_ParkMillerRand(	const MG_ParkMillerRand& aRight)
									:	MG_Random()
									,	myInitialSeed	(aRight.myInitialSeed)
									,	mySeed			(aRight.mySeed)
{}

MG_ParkMillerRand::~MG_ParkMillerRand()
{}

double MG_ParkMillerRand::DrawUniform()
{
	long vK = mySeed / ourQ;
	mySeed = ourA * (mySeed - vK * ourQ) - ourR * vK;
	if (mySeed < 0)
		mySeed += ourM;

	return double(mySeed) / ourM;
}

void MG_ParkMillerRand::Reset()
{
	mySeed = myInitialSeed;
}


/* Base Sampler */
MG_Sampler::MG_Sampler	(	const unsigned int& aDim, const MG_RandomPtr &aRG, const MG_SamplerPtr& aSampler)
						:	myDim			(aDim)
						,	myRandGen		(aRG)
						,	myComposition	(aSampler)
{}

MG_Sampler::~MG_Sampler()
{}

void MG_Sampler::Compose(const MG_SamplerPtr& aComp)
{
	myComposition = aComp;
}


/* Ordinary Sampler */
MG_UniformSampler::MG_UniformSampler(	const unsigned int		& aDim
									,	const MG_RandomPtr		& aRG
									,	const int				& aMin
									,	const int				& aMax)
									:	MG_Sampler(aDim, aRG)
									,	myMin(aMin)
									,	myMax(aMax)
{
	assert(myMin < myMax);
}

MG_UniformSampler::~MG_UniformSampler()
{}

vector<double> MG_UniformSampler::GenerateSample()
{
	vector<double> vSample;
	vSample.resize(myDim);
	if (myMin==0 && myMax==1)
	{
		for(unsigned int i=0; i<myDim; i++)
			vSample[i] = myRandGen->DrawUniform();
	}
	else
	{
		for(unsigned int i=0; i<myDim; i++)
			vSample[i] = (myRandGen->DrawUniform() * (myMax - myMin) + myMin) - myMin;
	}
	return vSample;
}

vector<double> MG_UniformSampler::GetSampleAntithetic(const vector<double>& aSample)
{
	vector<double> vSample;
	vSample.resize(myDim);

	for(unsigned int i=0; i<myDim; i++)
	{
		vSample[i] = 1.-aSample[i];
	}
	return vSample;
}

/* Box Muller Sampler */
MG_BoxMullerSampler::MG_BoxMullerSampler(	const unsigned int& aDim
										,	const MG_RandomPtr& aRG)
										:	MG_Sampler(aDim, aRG)
{}

MG_BoxMullerSampler::~MG_BoxMullerSampler()
{}

vector<double> MG_BoxMullerSampler::GenerateSample()
{
	vector<double> vSample;
	vSample.resize(myDim);
	double /*vSquared,*/ vX/*, vY*/;
	for(unsigned int i=0; i<myDim; i++)
	{
		//do
		//{
		//	vX = 2. * myRandGen->DrawUniform()-1;
		//	vY = 2. * myRandGen->DrawUniform()-1;
		//	vSquared = vX*vX + vY*vY;
		//} while (vSquared >= 1.);
		//vSample[i] = vX * sqrt(-2 * log(vSquared) / vSquared);
		// GEB : test 
		vX = myRandGen->DrawUniform();
		vSample[i] = MG_SCdfNormal::Instance()->InverseNormal(vX);

	}
	return vSample;
}

vector<double> MG_BoxMullerSampler::GetSampleAntithetic(const vector<double>& aSample)
{
	vector<double> vSample;
	vSample.resize(myDim);

	for(unsigned int i=0; i<myDim; i++)
	{
		vSample[i] = -aSample[i];
	}
	return vSample;
}


///* Antithetic Sampler */
//MG_AntitheticSampler::MG_AntitheticSampler	(	const unsigned int& aDim
//											,	const MG_SamplerPtr& aSampler)
//											:	MG_Sampler(aDim, aSampler->GetRandGen(), aSampler)
//{}
//
//MG_AntitheticSampler::~MG_AntitheticSampler()
//{}
//
//vector<double> MG_AntitheticSampler::GenerateSample()
//{
//	//vector<double> vUniforms1, vUniforms2;
//	//vUniforms1.resize(myDim/2);
//	//vUniforms2.resize(myDim/2);
//
//	//vector<double> vUniforms = myComposition->GenerateMinUniformsForSample();
//	//for(unsigned int i=0; i<myDim/2; i++)
//	//{
//	//	vU1 = myRandGen->DrawUniform();
//	//	vU2 = 1. - vU1;
//	//	vUniforms1[i] = vU1;
//	//	vUniforms2[i] = vU2;
//	//}
//	//if (myDim%2 == 1)
//	//{
//	//	vU1 = myRandGen->DrawUniform();
//	//	vUniforms1.push_back(vU1);
//	//}
//	vector<double> vSample1 = myComposition->GenerateSample();
//	vector<double> vSample2 = myComposition->GetSampleAntithetic(vSample1);
//	vSample1.insert(vSample1.end(), vSample2.begin(), vSample2.end());
//
//	return vSample1;
//}
//
