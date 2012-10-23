/*
 * Copyright			: 2012 by MG
 * Version				: 0.1.23
 * Date					: 30 SEP 2012
 * Purpose				: MG_Vasicek is the Vasicek model
 * Author				: MM Akkouh
 * Notes				: write here the diffusion
 */


#pragma once


#include "mggenpricer/genmod/irpricingmodel.h"

#include <vector>


MG_NAMESPACE_BEGIN


class MG_Date;
class MG_Schedule;


/* Base class for models */
class MG_Vasicek : public MG_IRPricingModel
{
public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_Vasicek)

	ASSIGN_OPERATOR(MG_Vasicek)
	SWAP_DECL(MG_Vasicek)
	CLONE_METHOD(MG_Vasicek)

	MG_Vasicek(void);

	virtual ~MG_Vasicek(void);

public:
	//==> Underlying
	std::vector<double> Libor(const MG_Date& aRstDt, const MG_Date& aStDt, const MG_Date& aEdDt, const MG_Date& aPayDt
						, const double& aDelta, const double& aTenor, const double& aSpread
						, const std::vector<double>& aStates);

	//==> One coupon pricing
	std::vector<double> Caplet(const MG_Date& aMatDt
						, const MG_Date& aRstDt, const MG_Date& aStDt, const MG_Date& aEdDt, const MG_Date& aPayDt
						, const double& aDelta, const double& aTenor, const double& aSpread
						, const double& aStrike, const std::vector<double>& aStates);
	std::vector<double> Floorlet(const MG_Date& aMatDt
						, const MG_Date& aRstDt, const MG_Date& aStDt, const MG_Date& aEdDt, const MG_Date& aPayDt
						, const double& aDelta, const double& aTenor, const double& aSpread
						, const double& aStrike, const std::vector<double>& aStates);
	std::vector<double> DigitalUp(const MG_Date& aMatDt
						, const MG_Date& aRstDt, const MG_Date& aStDt, const MG_Date& aEdDt, const MG_Date& aPayDt
						, const double& aDelta, const double& aTenor, const double& aSpread
						, const double& aStrike, const double& aAlpha
						, const std::vector<double>& aStates);
	std::vector<double> DigitalDown(const MG_Date& aMatDt
						, const MG_Date& aRstDt, const MG_Date& aStDt, const MG_Date& aEdDt, const MG_Date& aPayDt
						, const double& aDelta, const double& aTenor, const double& aSpread
						, const double& aStrike, const double& aAlpha
						, const std::vector<double>& aStates);

	//==> Set of coupons pricing
	std::vector<double> Cap(const MG_Date& aMatDt
						, const MG_Schedule& aSched, const double& aTenor, const std::vector<double>& aSpread
						, const double& aStrike
						, const std::vector<double>& aStates);
	std::vector<double> Floor(const MG_Date& aMatDt
						, const MG_Schedule& aSched, const double& aTenor, const std::vector<double>& aSpread
						, const double& aStrike
						, const std::vector<double>& aStates);

	//==> Other
	std::vector<double> Swaption(const MG_Date& aMatDt, const MG_Schedule& aSched, const double& aTenor
						, const double& aStrike
						, const std::vector<double>& aStates);

private:
	// here we'll add model parameters
};


MG_NAMESPACE_END
