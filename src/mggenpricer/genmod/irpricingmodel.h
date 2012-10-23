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

#include <vector>


MG_NAMESPACE_BEGIN


class MG_Date;
class MG_Schedule;


/* Base class for models */
class MG_IRPricingModel : public MG_PricingModel
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_IRPricingModel)

	ASSIGN_OPERATOR(MG_IRPricingModel)
	SWAP_DECL(MG_IRPricingModel)
	//CLONE_METHOD(MG_IRPricingModel)

	MG_IRPricingModel(void);

	virtual ~MG_IRPricingModel(void);

public:
	//==> Underlying
	// Libor + m
	virtual std::vector<double> Libor(const MG_Date& aRstDt, const MG_Date& aStDt, const MG_Date& aEdDt, const MG_Date& aPayDt
						, const double& aDelta, const double& aTenor, const double& aSpread
						, const std::vector<double>& aStates) = 0;

	//==> One coupon pricing
	// MAX(Libor+m-K, 0)
	virtual std::vector<double> Caplet(const MG_Date& aMatDt
						, const MG_Date& aRstDt, const MG_Date& aStDt, const MG_Date& aEdDt, const MG_Date& aPayDt
						, const double& aDelta, const double& aTenor, const double& aSpread
						, const double& aStrike, const std::vector<double>& aStates) = 0;
	// MAX(K-(Libor+m), 0)
	virtual std::vector<double> Floorlet(const MG_Date& aMatDt
						, const MG_Date& aRstDt, const MG_Date& aStDt, const MG_Date& aEdDt, const MG_Date& aPayDt
						, const double& aDelta, const double& aTenor, const double& aSpread
						, const double& aStrike, const std::vector<double>& aStates) = 0;
	// Alpha * Ind(Libor+m>K)
	virtual std::vector<double> DigitalUp(const MG_Date& aMatDt
						, const MG_Date& aRstDt, const MG_Date& aStDt, const MG_Date& aEdDt, const MG_Date& aPayDt
						, const double& aDelta, const double& aTenor, const double& aSpread
						, const double& aStrike, const double& aAlpha
						, const std::vector<double>& aStates) = 0;
	// Alpha * Ind(Libor+m<K)
	virtual std::vector<double> DigitalDown(const MG_Date& aMatDt
						, const MG_Date& aRstDt, const MG_Date& aStDt, const MG_Date& aEdDt, const MG_Date& aPayDt
						, const double& aDelta, const double& aTenor, const double& aSpread
						, const double& aStrike, const double& aAlpha
						, const std::vector<double>& aStates) = 0;

	//==> Set of coupons pricing
	virtual std::vector<double> Cap(const MG_Date& aMatDt
						, const MG_Schedule& aSched, const double& aTenor, const std::vector<double>& aSpread
						, const double& aStrike
						, const std::vector<double>& aStates) = 0;
	virtual std::vector<double> Floor(const MG_Date& aMatDt
						, const MG_Schedule& aSched, const double& aTenor, const std::vector<double>& aSpread
						, const double& aStrike
						, const std::vector<double>& aStates) = 0;

	//==> Other
	virtual std::vector<double> Swaption(const MG_Date& aMatDt, const MG_Schedule& aSched, const double& aTenor
						, const double& aStrike
						, const std::vector<double>& aStates) = 0;
};


MG_NAMESPACE_END
