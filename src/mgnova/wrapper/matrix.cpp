#include "mgnova/wrapper/matrix.h"


using namespace std;
using namespace MG;


/* Vector Wrapper Class */

/*
 * Constructors / Destructor
 */
MG_Matrix::MG_Matrix()	:	MG_Object()
						,	myRows(0), myCols(0), myMatrix(NULL)
{}

MG_Matrix::MG_Matrix(	const size_t& aRows
					,	const size_t& aCols
					,	const double& aVal)
					:	MG_Object()
					,	myRows(aRows)
					,	myCols(aCols)
{
	assert(aRows*aCols != 0);
	myMatrix = gsl_matrix_calloc(myRows, myCols);
	gsl_matrix_set_all(myMatrix, aVal);
}

MG_Matrix::MG_Matrix(	const MG_Vector& aVect)
					:	MG_Object()
					,	myRows(aVect.Size())
					,	myCols(1)
{
	myMatrix = gsl_matrix_calloc(myRows, myCols);
	for(size_t i=0; i<myRows; ++i)
		myMatrix->data[i] = aVect[i];
}

MG_Matrix::MG_Matrix(	const vector<double>& aVect)
					:	MG_Object()
					,	myRows(aVect.size())
					,	myCols(1)
{
	myMatrix = gsl_matrix_calloc(myRows, myCols);
	for(size_t i=0; i<myRows; ++i)
		myMatrix->data[i] = aVect[i];
}

MG_Matrix::MG_Matrix(	const MG_Matrix& aRight)
					:	MG_Object(aRight)
					,	myRows(aRight.myRows)
					,	myCols(aRight.myCols)
					,	myMatrix(NULL)
{
	if (myRows*myCols)
	{
		myMatrix = gsl_matrix_calloc(myRows, myCols);
		gsl_matrix_memcpy(myMatrix, aRight.myMatrix);
	}
}

MG_Matrix::~MG_Matrix()
{
	gsl_matrix_free(myMatrix);
}

void MG_Matrix::Swap(MG_Matrix& aRight)
{
	swap(myRows, aRight.myRows);
	swap(myCols, aRight.myCols);
	//gsl_matrix_swap(myMatrix, aRight.myMatrix);
	swap(myMatrix, aRight.myMatrix);
}


/*
 * Assignment operators
 */
MG_Matrix& MG_Matrix::operator +=(const double& aRight)
{
	gsl_matrix_add_constant(myMatrix, aRight);
	return *this;
}

MG_Matrix& MG_Matrix::operator -=(const double& aRight)
{
	gsl_matrix_add_constant(myMatrix, -aRight);
	return *this;
}

MG_Matrix& MG_Matrix::operator *=(const double& aRight)
{
	gsl_matrix_scale(myMatrix, aRight);
	return *this;
}

MG_Matrix& MG_Matrix::operator /=(const double& aRight)
{
	gsl_matrix_scale(myMatrix, 1./aRight);
	return *this;
}

MG_Matrix& MG_Matrix::operator +=(const MG_Matrix& aRight)
{
	assert(myRows*myCols == aRight.myRows*aRight.myCols);
	gsl_matrix_add(myMatrix, aRight.myMatrix);
	return *this;
}

MG_Matrix& MG_Matrix::operator -=(const MG_Matrix& aRight)
{
	assert(myRows*myCols == aRight.myRows*aRight.myCols);
	gsl_matrix_sub(myMatrix, aRight.myMatrix);
	return *this;
}

MG_Matrix& MG_Matrix::operator *=(const MG_Matrix& aRight)
{
	assert(myRows*myCols == aRight.myRows*aRight.myCols);
	gsl_matrix_mul_elements(myMatrix, aRight.myMatrix);
	return *this;
}

MG_Matrix& MG_Matrix::operator /=(const MG_Matrix& aRight)
{
	assert(myRows*myCols == aRight.myRows*aRight.myCols);
	gsl_matrix_div_elements(myMatrix, aRight.myMatrix);
	return *this;
}

/*
 * Arithmetic operators
 * Prefix
 */
MG_Matrix& MG_Matrix::operator ++(void)
{
	gsl_matrix_add_constant(myMatrix, 1.);
	return *this;
}

MG_Matrix& MG_Matrix::operator --(void)
{
	gsl_matrix_add_constant(myMatrix, -1.);
	return *this;
}

/*
 * Arithmetic operators
 * Postfix
 */
MG_Matrix MG_Matrix::operator ++(int)
{
	MG_Matrix vOld(*this);
	gsl_matrix_add_constant(vOld.myMatrix, 1.);
	return vOld;
}

MG_Matrix MG_Matrix::operator --(int)
{
	MG_Matrix vOld(*this);
	gsl_matrix_add_constant(vOld.myMatrix, -1.);
	return vOld;
}

/*
 * Transposition
 */
void MG_Matrix::Transpose(MG_Matrix& aDest) const
{
	aDest = MG_Matrix(myCols, myRows);
	gsl_matrix_transpose_memcpy(aDest.myMatrix, myMatrix);
}

