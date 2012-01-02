/*
 * Copyright			: 2011 by MG
 * File					: matrix.h
 * Version				: 0.2.0
 * Last changed			: 30 JUL 2011
 * Purpose				: MG_Matrix is a wrapper of the gsl_matrix type
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include "gsl/gsl_matrix.h"

#include "mgnova/typedef.h"
#include "mgnova/wrapper/vector.h"


MG_NAMESPACE_BEGIN


/* Vector Class */
class MG_Matrix : public MG_Object
{
public:
	/* Constructors / Destructor */
	MG_Matrix(void);
	MG_Matrix(const size_t& aRows, const size_t& aCols, const double& aVal = 0.);
	MG_Matrix(const MG_Vector& aVect);

	COPY_CTOR_DECL(MG_Matrix)
	ASSIGN_OPERATOR(MG_Matrix)
	CLONE_METHOD(MG_Matrix)
	SWAP_DECL(MG_Matrix)

	virtual ~MG_Matrix(void);

public:
	/* Accessors */
	inline double&	Elt	(const size_t& aI, const size_t& aJ)	const { return *gsl_matrix_ptr(myMatrix, aI, aJ); }
	inline double&	operator ()(const size_t& aI, const size_t& aJ)	const { return *gsl_matrix_ptr(myMatrix, aI, aJ); }

	inline size_t	Rows(void)									const { return myRows; }
	inline size_t	Cols(void)									const { return myCols; }
	inline size_t	Size(void)									const { return myRows*myCols; }
	
	inline const gsl_matrix* const	GetPtr(void) const	{ return myMatrix; }
	inline gsl_matrix* const		GetPtr(void)		{ return myMatrix; }

	void Transpose(MG_Matrix& aDest);

public:
	/* Assignment operators */
	MG_Matrix& operator +=(const double& aRight);
	MG_Matrix& operator -=(const double& aRight);
	MG_Matrix& operator *=(const double& aRight);
	MG_Matrix& operator /=(const double& aRight);
	MG_Matrix& operator +=(const MG_Matrix& aRight);
	MG_Matrix& operator -=(const MG_Matrix& aRight);
	MG_Matrix& operator *=(const MG_Matrix& aRight);
	MG_Matrix& operator /=(const MG_Matrix& aRight);

	/* Arithmetic operators */
	/* Prefix */
	MG_Matrix& operator ++(void);
	MG_Matrix& operator --(void);
	/* Postfix */
	MG_Matrix operator ++(int);
	MG_Matrix operator --(int);

private:
	gsl_matrix*	myMatrix;
	size_t		myRows;
	size_t		myCols;

public:
	/* friend functions */
	MG_FRIEND_ARITH_OP_LEFT(MG_Matrix, +, double)
	MG_FRIEND_ARITH_OP_RIGHT(MG_Matrix, +, double)
	MG_FRIEND_ARITH_OP(MG_Matrix, +)

	MG_FRIEND_ARITH_OP_LEFT(MG_Matrix, -, double)
	MG_FRIEND_ARITH_OP_RIGHT(MG_Matrix, -, double)
	MG_FRIEND_ARITH_OP(MG_Matrix, -)

	MG_FRIEND_ARITH_OP_LEFT(MG_Matrix, *, double)
	MG_FRIEND_ARITH_OP_RIGHT(MG_Matrix, *, double)
	MG_FRIEND_ARITH_OP(MG_Matrix, *)

	MG_FRIEND_ARITH_OP_LEFT(MG_Matrix, /, double)
	MG_FRIEND_ARITH_OP_RIGHT(MG_Matrix, /, double)
	MG_FRIEND_ARITH_OP(MG_Matrix, /)
};


MG_NAMESPACE_END
