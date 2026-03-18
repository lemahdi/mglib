/*
 * Copyright			: 2011 by MG
 * Version				: 0.1.24
 * Date					: 15 NOV 2011
 * Purpose				: 
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include <vector>

#include "nova/glob/object.h"
#include "nova/glob/argconvdef.h"
#include "nova/wrapper/vector.h"
#include "nova/wrapper/matrix.h"

#include "gsl/gsl_multifit.h"


MG_NAMESPACE_BEGIN


class MG_Regression : public MG_XLObject
{

public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_Regression)

	SWAP_DECL(MG_Regression)

	MG_Regression(const size_t& aSize, const MG_Vector& aY, const MG_Vector& aW);
	virtual ~MG_Regression() {}

	/* Accessors */

	/* Support */

	/* Engine */
	virtual void ProcessLinearReg	() = 0;
	virtual void ProcessWLinearReg	() = 0;

protected:
	const MG_Vector myY;
	const MG_Vector myW;

	MG_Vector myTheta;
	MG_Matrix myCov;
	double myChiSquared;

};


class MG_LinearReg : public MG_Regression
{

public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_LinearReg)

	ASSIGN_OPERATOR(MG_LinearReg)
	SWAP_DECL(MG_LinearReg)
	CLONE_METHOD(MG_LinearReg)

	virtual ~MG_LinearReg() {}

	MG_LinearReg(const MG_Vector& aX, const MG_Vector& aY, const MG_Vector& aW = MG_Vector());

	/* Accessors */

	/* Support */

	/* Engine */
	void ProcessLinearReg	() override;
	void ProcessWLinearReg	() override;

	double Estimate(const double& aX);

protected:
	const size_t mySize;
	const MG_Vector myX;

};


class MG_MultiReg : public MG_Regression
{

public:
	/* Constructors / Destructor */
	COPY_CTOR_DECL(MG_MultiReg)

	ASSIGN_OPERATOR(MG_MultiReg)
	SWAP_DECL(MG_MultiReg)
	CLONE_METHOD(MG_MultiReg)

	virtual ~MG_MultiReg();

	MG_MultiReg(const MG_Matrix& aX, const MG_Vector& aY, const MG_Vector& aW = MG_Vector());

	/* Accessors */

	/* Support */

	/* Engine */
	void ProcessLinearReg	() override;
	void ProcessWLinearReg	() override;

	double Estimate(const MG_Vector& aX);
	MG_Vector Residuals();

protected:
	const size_t myFeaturesNb;
	const size_t myExplesNb;
	const MG_Matrix myX;

	gsl_multifit_linear_workspace* myWorkSpace;

};


MG_NAMESPACE_END
