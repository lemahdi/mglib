#include "matrix.h"


using namespace std;
using namespace MG;


/* Matrix class */
MG_Matrix::MG_Matrix(	const MG_ROW& aRows
					,	const MG_COL& aCols
					,	const double& aVal)
					:	MG_Object()
					,	myRows(aRows)
					,	myCols(aCols)
{
	assert(aRows>0 && aCols>0);
	myValues.resize(myRows*myCols);
	this->operator +=(aVal);
}

MG_Matrix::MG_Matrix(	const MG_Matrix &aRight)
					:	MG_Object()
					,	myRows	(aRight.myRows)
					,	myCols	(aRight.myCols)
					,	myValues(aRight.myValues)
{}

void MG_Matrix::Swap(MG_Matrix& aRight)
{
	swap(myRows, aRight.myRows);
	swap(myCols, aRight.myCols);
	swap(myValues, aRight.myValues);
}

MG_Matrix::~MG_Matrix()
{}

/*
 * Assignment operators
 */
MG_Matrix& MG_Matrix::operator+= (const double& aRight)
{
	vector<double>::iterator vIt = myValues.begin();
	while (vIt != myValues.end())
	{
		*vIt += aRight;
		vIt++;
	}
	return *this;
}
MG_Matrix& MG_Matrix::operator-= (const double& aRight)
{
	vector<double>::iterator vIt = myValues.begin();
	while (vIt != myValues.end())
	{
		*vIt -= aRight;
		vIt++;
	}
	return *this;
}
MG_Matrix& MG_Matrix::operator+= (const MG_Matrix& aRight)
{
	assert(myRows==aRight.myRows && myCols==aRight.myCols);
	vector<double>::iterator vIt = myValues.begin();
	vector<double>::const_iterator vItR = aRight.myValues.begin();
	while (vIt != myValues.end())
	{
		*vIt += *vItR;
		vIt++;
		vItR++;
	}
	return *this;
}
MG_Matrix& MG_Matrix::operator-= (const MG_Matrix& aRight)
{
	assert(myRows==aRight.myRows && myCols==aRight.myCols);
	vector<double>::iterator vIt = myValues.begin();
	vector<double>::const_iterator vItR = aRight.myValues.begin();
	while (vIt != myValues.end())
	{
		*vIt -= *vItR;
		vIt++;
		vItR++;
	}
	return *this;
}

/*
 * Arithmetic operators
 */
MG_Matrix& MG_Matrix::operator++ ()
{
	vector<double>::iterator vIt = myValues.begin();
	while (vIt != myValues.end())
	{
		(*vIt)++;
		vIt++;
	}
	return *this;
}
MG_Matrix& MG_Matrix::operator-- ()
{
	vector<double>::iterator vIt = myValues.begin();
	while (vIt != myValues.end())
	{
		(*vIt)--;
		vIt++;
	}
	return *this;
}
MG_Matrix MG_Matrix::operator++ (int)
{
	MG_Matrix vOld(*this);
	++*this;
	return vOld;
}
MG_Matrix MG_Matrix::operator-- (int)
{
	MG_Matrix vOld(*this);
	--*this;
	return vOld;
}

