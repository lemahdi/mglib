/*
 * Copyright			: 2011 by MG
 * File					: vector.h
 * Version				: 0.2.0
 * Last changed			: 28 JUL 2011
 * Purpose				: MG_Vector is a wrapper of the gsl_vector type
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "gsl/gsl_vector.h"

#include "mgnova/typedef.h"
#include "mgnova/object.h"


MG_NAMESPACE_BEGIN


/* Vector Class */
class MG_Vector : public MG_Object
{
public:
	/* Constructors / Destructor */
	MG_Vector(void);
	MG_Vector(const size_t& aSize, const double& aVal = 0.);

	COPY_CTOR_DECL(MG_Vector)
	ASSIGN_OPERATOR(MG_Vector)
	CLONE_METHOD(MG_Vector)
	SWAP_DECL(MG_Vector)
	
	virtual ~MG_Vector(void);

public:
	/* Accessors */
	inline double&					operator []	(const size_t& aIdx)	const	{ return *gsl_vector_ptr(myVect, aIdx); }
	inline size_t					Size		(void)					const	{ return mySize; }
	inline const gsl_vector* const	GetPtr		(void)					const	{ return myVect; }
	inline gsl_vector* const		GetPtr		(void)							{ return myVect; }

public:
	/* Assignment operators */
	MG_Vector& operator +=(const double& aRight);
	MG_Vector& operator -=(const double& aRight);
	MG_Vector& operator *=(const double& aRight);
	MG_Vector& operator /=(const double& aRight);
	MG_Vector& operator +=(const MG_Vector& aRight);
	MG_Vector& operator -=(const MG_Vector& aRight);
	MG_Vector& operator *=(const MG_Vector& aRight);
	MG_Vector& operator /=(const MG_Vector& aRight);

	/* Arithmetic operators */
	/* Prefix */
	MG_Vector& operator ++(void);
	MG_Vector& operator --(void);
	/* Postfix */
	MG_Vector operator ++(int);
	MG_Vector operator --(int);

	/* Comparison */

	/* Utility Functions */
	inline double		& Front	(void)			{ return *gsl_vector_ptr(myVect, 0); }
	inline const double	& Front	(void) const	{ return *gsl_vector_ptr(myVect, 0); }
	inline double		& Back	(void)			{ return *gsl_vector_ptr(myVect, mySize-1); }
	inline const double	& Back	(void) const	{ return *gsl_vector_ptr(myVect, mySize-1); }

private:
	gsl_vector*	myVect;
	size_t		mySize;

public:
	/* friend functions */
	MG_FRIEND_ARITH_OP_LEFT(MG_Vector, +, double)
	MG_FRIEND_ARITH_OP_RIGHT(MG_Vector, +, double)
	MG_FRIEND_ARITH_OP(MG_Vector, +)

	MG_FRIEND_ARITH_OP_LEFT(MG_Vector, -, double)
	MG_FRIEND_ARITH_OP_RIGHT(MG_Vector, -, double)
	MG_FRIEND_ARITH_OP(MG_Vector, -)

	MG_FRIEND_ARITH_OP_LEFT(MG_Vector, *, double)
	MG_FRIEND_ARITH_OP_RIGHT(MG_Vector, *, double)
	MG_FRIEND_ARITH_OP(MG_Vector, *)

	MG_FRIEND_ARITH_OP_LEFT(MG_Vector, /, double)
	MG_FRIEND_ARITH_OP_RIGHT(MG_Vector, /, double)
	MG_FRIEND_ARITH_OP(MG_Vector, /)
};


MG_NAMESPACE_END
