/*
 * Copyright			: 2011 by MG
 * File					: normal.h
 * Version				: 0.1.21
 * Last changed			: 05 MAR 2011
 * Purpose				: simple class to deal with normal distribution fucntion
 * Author				: MM Akkouh
 * Notes				: 
 *
 * Updates				: 13 MAR 2011 by G El Boury - more consistent cumulative function
 */


#pragma once


#include <iostream>
#include <vector>

#include "mgnova/patterns/singleton.hpp"


MG_NAMESPACE_BEGIN


class MG_CdfNormal
{
	template<class T> friend class Singleton;

	struct GaussCst
	{
		std::vector<double> myA;
		std::vector<double> myB;
		std::vector<double> myC;
		std::vector<double> myD;
		std::vector<double> myP;
		std::vector<double> myQ;

		std::vector<double> myInvA;
		std::vector<double> myInvB;
		std::vector<double> myInvC;
		std::vector<double> myInvD;

		// Constants for Marsaglia & al. algorithm, "MC Methods in Financial Engineering, Fig. 2.15, p. 70"
		
		std::vector<double> myV;

		std::vector<double> myInvATmp;
		std::vector<double> myInvBTmp;
		std::vector<double> myInvCTmp;
	};

	
private:
	MG_CdfNormal(void) {}

public:
	~MG_CdfNormal(void) {}

	void Init(void);

	// Normal density function
	double NormalDensity(const double& aX) const;

	// Normal cumulative gauss function : Num Recipees
	double CumulativeNormal(const double& aX, const double& aBound = NORMALCUMULATIVEBOUND) const;

	// Normal inverse gauss function : Num Recipees
	double InverseNormal(const double& aP) const;

	// Marsaglia & al. algo 4 Normal function, from "Monte Carlo Methods in Financial Engineering", Fig. 2.15 pageb 70
	//double CumulativeNormal_Tmp(const double& aX) const;

	// Beasley-Springer-Moro algo 4 Inverse Normal function, from "Monte Carlo Methods in Financial Engineering", Fig. 2.13 pageb 68
	//double InverseNormal_Tmp(const double& aU) const;

private:
	GaussCst myGaussCst;

};
typedef Singleton<MG_CdfNormal> MG_SCdfNormal;


MG_NAMESPACE_END
