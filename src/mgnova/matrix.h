/*
 * Copyright			: 2011 by MG
 * File					: matrix.h
 * Version				: 0.1.21
 * Last changed			: 25 MAR 2011
 * Purpose				: MG_Matrix is a matrix
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include <vector>

#include "mgnova/typedef.h"
#include "mgnova/object.h"


MG_NAMESPACE_BEGIN


/* Base class matrix */
class MG_Matrix : public MG_Object
{
	typedef size_t MG_ROW;
	typedef size_t MG_COL;
	typedef size_t MG_SIZE;

public:
	/* Constructors / Destructor */
	MG_Matrix(const MG_ROW& aRows = 1, const MG_COL& aCols = 1, const double& aVal = 0);
	MG_Matrix(const MG_Matrix& aRight);
	ASSIGN_OPERATOR(MG_Matrix)
	CLONE_METHOD(MG_Matrix)
	SWAP_DECL(MG_Matrix)
	virtual ~MG_Matrix(void);

public:
	/* Accessors */
	inline MG_ROW GetRows(void) const { return myRows; }
	inline MG_COL GetCols(void) const { return myCols; }

	inline double Elt(const MG_ROW& aRow, const MG_COL& aCol) const { return myValues[aRow*myCols+aCol]; }
	inline double& Elt(const MG_ROW& aRow, const MG_COL& aCol) { return myValues[aRow*myCols+aCol]; }

	inline MG_SIZE Size(void) const { return myRows * myCols; }

public:
	/* Assignment operators */
	MG_Matrix& operator+= (const double& aRight);
	MG_Matrix& operator-= (const double& aRight);
	MG_Matrix& operator+= (const MG_Matrix& aRight);
	MG_Matrix& operator-= (const MG_Matrix& aRight);

	/* Arithmetic operators */
	MG_Matrix operator+ (const double& aRight);
	MG_Matrix operator- (const double& aRight);
	/* Prefix */
	MG_Matrix& operator++ (void);
	MG_Matrix& operator-- (void);
	/* Postfix */
	MG_Matrix operator++ (int);
	MG_Matrix operator-- (int);

private:
	MG_ROW myRows;
	MG_COL myCols;
	std::vector<double> myValues;

public:
	/* friend functions */
	friend MG_Matrix operator+ (MG_Matrix vLeft, const long& vRight)
	{
		return vLeft += vRight;
	}
	friend MG_Matrix operator+ (const long& vLeft, MG_Matrix vRight)
	{
		return vRight += vLeft;
	}
	friend MG_Matrix operator+ (const MG_Matrix& vLeft, MG_Matrix vRight)
	{
		return vRight += vLeft;
	}
	friend MG_Matrix operator- (MG_Matrix vLeft, const long& vRight)
	{
		return vLeft -= vRight;
	}
	friend MG_Matrix operator- (const long& vLeft, MG_Matrix vRight)
	{
		return vRight -= vLeft;
	}
	friend MG_Matrix operator- (const MG_Matrix& vLeft, MG_Matrix vRight)
	{
		return vRight -= vLeft;
	}
};


MG_NAMESPACE_END
