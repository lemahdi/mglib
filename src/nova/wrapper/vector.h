/*
 * Copyright			: 2011 by MG
 * Version				: 0.2.0
 * Date					: 28 JUL 2011
 * Purpose				: MG_Vector is a wrapper of the gsl_vector type
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "gsl/gsl_vector.h"

#include "nova/glob/typedef.h"
#include "nova/glob/object.h"

#include <vector>


MG_NAMESPACE_BEGIN


/* Forward Declarations */
class MG_Date;


/* Vector Class */
class MG_Vector : public MG_Object
{
public:
	/* Constructors / Destructor */
	MG_Vector();
	MG_Vector(const size_t& aSize, const double& aVal = 0.);
	MG_Vector(const std::vector<double>& aVals);
	MG_Vector(const std::vector<MG_Date>& aDates);

	COPY_CTOR_DECL(MG_Vector)
	ASSIGN_OPERATOR(MG_Vector)
	CLONE_METHOD(MG_Vector)
	SWAP_DECL(MG_Vector)
	
	virtual ~MG_Vector();

public:
	/* Accessors */
	inline double&					operator []	(const size_t& aIdx)	const	{ return *gsl_vector_ptr(myVect, aIdx); }
	inline size_t					Size		()					const	{ return mySize; }
	inline const gsl_vector*	GetPtr()	const	{ return myVect; }
	inline gsl_vector*			GetPtr()			{ return myVect; }

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
	MG_Vector& operator ++();
	MG_Vector& operator --();
	/* Postfix */
	MG_Vector operator ++(int);
	MG_Vector operator --(int);

	/* Arithmetic functions */
	double Sum() const;
	double SumProduct(const MG_Vector& aRight) const;

	/* Comparison */

	/* Utility Functions */
	inline double		& Front	()			{ return *gsl_vector_ptr(myVect, 0); }
	inline const double	& Front	() const	{ return *gsl_vector_ptr(myVect, 0); }
	inline double		& Back	()			{ return *gsl_vector_ptr(myVect, mySize-1); }
	inline const double	& Back	() const	{ return *gsl_vector_ptr(myVect, mySize-1); }
	void Clear();
	void Resize(const size_t& aSize, const double& aVal = 0.);

private:
	size_t		mySize;
	gsl_vector*	myVect;

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
