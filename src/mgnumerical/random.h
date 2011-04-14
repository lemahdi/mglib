/*
 * Copyright			: 2010 by MG
 * File					: random.h
 * Version				: 0.1.20
 * Last changed			: 01 JAN 2011
 * Purpose				: 
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include <vector>

#include "object.h"


MG_NAMESPACE_BEGIN


/* Base class for random numbers */
class MG_Random
{
public:
	MG_Random(void);
	MG_Random(const MG_Random& aRight);
	virtual ~MG_Random(void) {}

	/* Engines */
	virtual double DrawUniform(void) = 0;
	virtual void Reset(void) = 0;
};


/* Park Miller */
class MG_ParkMillerRand : public MG_Random
{
private:
	FAKE_ASSIGN_OPERATOR(MG_ParkMillerRand)

public:
	MG_ParkMillerRand(const long& aSeed = -156);
	MG_ParkMillerRand(const MG_ParkMillerRand& aRight);
	virtual ~MG_ParkMillerRand(void);

	/* Support */
	inline long Max(void) const { return ourM - 1; }
	inline long Min(void) const { return 1; }

	/* Engines */
	virtual double DrawUniform(void);
	virtual void Reset(void);

private:
	const long myInitialSeed;
	long mySeed;

	/* some constants */
	static const long ourA;
	static const long ourM;
	static const long ourQ;
	static const long ourR;

};


/* Base Sampler */
class MG_Sampler
{
private:
	FAKE_ASSIGN_OPERATOR(MG_Sampler)

public:
	MG_Sampler(const unsigned int& aDim, const MG_RandomPtr& aRG, const MG_SamplerPtr& aSampler = NULL);
	virtual ~MG_Sampler(void);

	/* Accessors */
	inline MG_SamplerPtr GetComposition(void) const { return myComposition; }
	inline MG_RandomPtr GetRandGen(void) const { return myRandGen; }

	/* Engines */
	virtual std::vector<double> GenerateSample(void) = 0;
	virtual std::vector<double> GetSampleAntithetic(const std::vector<double>& aUniforms) = 0;
	void Compose(const MG_SamplerPtr& aComp);

	virtual void Reset(void) { myRandGen->Reset(); }

protected:
	const unsigned int	myDim;
	MG_RandomPtr		myRandGen;
	MG_SamplerPtr		myComposition;
};

/* Uniform Sampler */
class MG_UniformSampler : public MG_Sampler
{
private:
	FAKE_ASSIGN_OPERATOR(MG_UniformSampler)

public:
	MG_UniformSampler(const unsigned int& aDim, const MG_RandomPtr& aRG, const int& aMin = 0, const int& aMax = 1);
	virtual ~MG_UniformSampler(void);

	virtual std::vector<double> GenerateSample(void);
	virtual std::vector<double> GetSampleAntithetic(const std::vector<double>& Sample);

private:
	int myMin;
	int myMax;

};

/* Box Muller Sampler */
class MG_BoxMullerSampler : public MG_Sampler
{
private:
	FAKE_ASSIGN_OPERATOR(MG_BoxMullerSampler)

public:
	MG_BoxMullerSampler(const unsigned int& aDim, const MG_RandomPtr& aRG);
	virtual ~MG_BoxMullerSampler(void);

	virtual std::vector<double> GenerateSample(void);
	virtual std::vector<double> GetSampleAntithetic(const std::vector<double>& Sample);

};

///* Antithetic Sampler */
//class MG_AntitheticSampler : public MG_Sampler
//{
//private:
//	FAKE_ASSIGN_OPERATOR(MG_AntitheticSampler)
//
//public:
//	MG_AntitheticSampler(const unsigned int& aDim, const MG_SamplerPtr& aSampler);
//	virtual ~MG_AntitheticSampler(void);
//
//	virtual std::vector<double> GenerateSample(void);
//	
//	virtual void Reset(void) { myComposition->GetRandGen()->Reset(); }
//
//};
//

MG_NAMESPACE_END
