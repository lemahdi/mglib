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

#include "singleton.hpp"


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


private:
	GaussCst myGaussCst;

};
typedef Singleton<MG_CdfNormal> MG_SCdfNormal;


MG_NAMESPACE_END
