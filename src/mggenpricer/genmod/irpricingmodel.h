/*
 * Copyright			: 2011 by MG
 * Version				: 0.1.23
 * Date					: 01 NOV 2011
 * Purpose				: MG_IRPricingModel is a base class for interest rate models
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "mggenpricer/genmod/pricingmodel.h"
#include "mgnova/patterns/countedptr.hpp"

#include <vector>


MG_NAMESPACE_BEGIN


class MG_Date;
class MG_Schedule;
class MG_ZeroCurve;


/* Base class for models */
class MG_IRPricingModel : public MG_PricingModel
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_IRPricingModel)

	ASSIGN_OPERATOR(MG_IRPricingModel)
	SWAP_DECL(MG_IRPricingModel)
	//CLONE_METHOD(MG_IRPricingModel)

	MG_IRPricingModel(const MG_ZeroCurvePtr& aZC);

	virtual ~MG_IRPricingModel(void);

	//==> Accessors
	const MG_Date& AsOf(void) const;
	const MG_ZeroCurvePtr& ZcCurve(void) const;

public:
	//==> Underlying
	// Libor + m
	double Libor(const MG_Date& aStDt, const MG_Date& aEdDt, const MG_Date& aPayDt, const double& aDelta);
	virtual MG_StdVectDblPtr Libor(const MG_Date& aRstDt, const MG_Date& aStDt, const MG_Date& aEdDt, const MG_Date& aPayDt
						, const double& aDelta, const double& aTenor, const double& aSpread
						, const std::vector<double>& aStates) = 0;

	//==> One coupon pricing
	// MAX(Libor+m-K, 0)
	virtual MG_StdVectDblPtr Caplet(const MG_Date& aMatDt
						, const MG_Date& aRstDt, const MG_Date& aStDt, const MG_Date& aEdDt, const MG_Date& aPayDt
						, const double& aDelta, const double& aTenor, const double& aSpread
						, const double& aStrike, const std::vector<double>& aStates) = 0;
	// MAX(K-(Libor+m), 0)
	virtual MG_StdVectDblPtr Floorlet(const MG_Date& aMatDt
						, const MG_Date& aRstDt, const MG_Date& aStDt, const MG_Date& aEdDt, const MG_Date& aPayDt
						, const double& aDelta, const double& aTenor, const double& aSpread
						, const double& aStrike, const std::vector<double>& aStates) = 0;
	// Alpha * Ind(Libor+m>K)
	virtual MG_StdVectDblPtr DigitalUp(const MG_Date& aMatDt
						, const MG_Date& aRstDt, const MG_Date& aStDt, const MG_Date& aEdDt, const MG_Date& aPayDt
						, const double& aDelta, const double& aTenor, const double& aSpread
						, const double& aStrike, const double& aAlpha
						, const std::vector<double>& aStates) = 0;
	// Alpha * Ind(Libor+m<K)
	virtual MG_StdVectDblPtr DigitalDown(const MG_Date& aMatDt
						, const MG_Date& aRstDt, const MG_Date& aStDt, const MG_Date& aEdDt, const MG_Date& aPayDt
						, const double& aDelta, const double& aTenor, const double& aSpread
						, const double& aStrike, const double& aAlpha
						, const std::vector<double>& aStates) = 0;

	//==> Set of coupons pricing
	virtual MG_StdVectDblPtr Cap(const MG_Date& aMatDt
						, const MG_Schedule& aSched, const double& aTenor, const std::vector<double>& aSpread
						, const double& aStrike
						, const std::vector<double>& aStates) = 0;
	virtual MG_StdVectDblPtr Floor(const MG_Date& aMatDt
						, const MG_Schedule& aSched, const double& aTenor, const std::vector<double>& aSpread
						, const double& aStrike
						, const std::vector<double>& aStates) = 0;

	//==> Other
	virtual MG_StdVectDblPtr Swaption(const MG_Date& aMatDt, const MG_Schedule& aSched, const double& aTenor
						, const double& aStrike
						, const std::vector<double>& aStates) = 0;

protected:
	MG_ZeroCurvePtr myZC;
};


MG_NAMESPACE_END
