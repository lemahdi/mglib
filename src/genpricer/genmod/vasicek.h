/*
 * Copyright			: 2012 by MG
 * Version				: 0.1.23
 * Date					: 30 SEP 2012
 * Purpose				: MG_Vasicek is the Vasicek model
 * Author				: MM Akkouh
 * Notes				: write here the diffusion
 */


#pragma once


#include "genpricer/genmod/irpricingmodel.h"

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

	MG_Vasicek(const MG_ZeroCurvePtr& aZC);

	virtual ~MG_Vasicek(void);

public:
	//==> Underlying
	MG_StdVectDblPtr Libor(const MG_Date& aRstDt, const MG_Date& aStDt, const MG_Date& aEdDt, const MG_Date& aPayDt
						, const double& aDelta, const double& aTenor, const double& aSpread
						, const std::vector<double>& aStates);

	//==> One coupon pricing
	MG_StdVectDblPtr Caplet(const MG_Date& aMatDt
						, const MG_Date& aRstDt, const MG_Date& aStDt, const MG_Date& aEdDt, const MG_Date& aPayDt
						, const double& aDelta, const double& aTenor, const double& aSpread
						, const double& aStrike, const std::vector<double>& aStates);
	MG_StdVectDblPtr Floorlet(const MG_Date& aMatDt
						, const MG_Date& aRstDt, const MG_Date& aStDt, const MG_Date& aEdDt, const MG_Date& aPayDt
						, const double& aDelta, const double& aTenor, const double& aSpread
						, const double& aStrike, const std::vector<double>& aStates);
	MG_StdVectDblPtr DigitalUp(const MG_Date& aMatDt
						, const MG_Date& aRstDt, const MG_Date& aStDt, const MG_Date& aEdDt, const MG_Date& aPayDt
						, const double& aDelta, const double& aTenor, const double& aSpread
						, const double& aStrike, const double& aAlpha
						, const std::vector<double>& aStates);
	MG_StdVectDblPtr DigitalDown(const MG_Date& aMatDt
						, const MG_Date& aRstDt, const MG_Date& aStDt, const MG_Date& aEdDt, const MG_Date& aPayDt
						, const double& aDelta, const double& aTenor, const double& aSpread
						, const double& aStrike, const double& aAlpha
						, const std::vector<double>& aStates);

	//==> Set of coupons pricing
	MG_StdVectDblPtr Cap(const MG_Date& aMatDt
						, const MG_Schedule& aSched, const double& aTenor, const std::vector<double>& aSpread
						, const double& aStrike
						, const std::vector<double>& aStates);
	MG_StdVectDblPtr Floor(const MG_Date& aMatDt
						, const MG_Schedule& aSched, const double& aTenor, const std::vector<double>& aSpread
						, const double& aStrike
						, const std::vector<double>& aStates);

	//==> Other
	MG_StdVectDblPtr Swaption(const MG_Date& aMatDt, const MG_Schedule& aSched, const double& aTenor
						, const double& aStrike
						, const std::vector<double>& aStates);

private:
	// here we'll add model parameters
};


MG_NAMESPACE_END
