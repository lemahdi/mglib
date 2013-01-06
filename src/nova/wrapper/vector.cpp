#include "nova/wrapper/vector.h"
#include "nova/glob/date.h"


using namespace std;
using namespace MG;


/* Vector Wrapper Class */

/*
 * Constructors / Destructor
 */
MG_Vector::MG_Vector()	:	MG_Object()
						,	mySize(0), myVect(NULL)
{}

MG_Vector::MG_Vector(	const size_t& aSize
					,	const double& aVal)
					:	MG_Object()
					,	mySize(aSize)
{
	assert(aSize != 0);
	myVect = gsl_vector_calloc(mySize);
	gsl_vector_set_all(myVect, aVal);
}

MG_Vector::MG_Vector(	const vector<double>& aVals)
					:	MG_Object()
					,	mySize(aVals.size())
{
	assert(mySize != 0);
	myVect = gsl_vector_calloc(mySize);
	for(size_t i=0; i<mySize; ++i)
		myVect->data[i] = aVals[i];
}

MG_Vector::MG_Vector(	const vector<MG_Date>& aDates)
					:	MG_Object()
					,	mySize(aDates.size())
{
	assert(mySize != 0);
	myVect = gsl_vector_calloc(mySize);
	for(size_t i=0; i<mySize; ++i)
		myVect->data[i] = aDates[i].GetJulianDay();
}

MG_Vector::MG_Vector(	const MG_Vector& aRight)
					:	MG_Object(aRight)
					,	mySize(aRight.mySize)
					,	myVect(NULL)
{
	if (mySize)
	{
		myVect = gsl_vector_calloc(mySize);
		gsl_vector_memcpy(myVect, aRight.myVect);
	}
}

MG_Vector::~MG_Vector()
{
	gsl_vector_free(myVect);
}

void MG_Vector::Swap(MG_Vector& aRight)
{
	swap(mySize, aRight.mySize);
	//gsl_vector_swap(myVect, aRight.myVect);
	swap(myVect, aRight.myVect);
}


/*
 * Assignment operators
 */
MG_Vector& MG_Vector::operator +=(const double& aRight)
{
	gsl_vector_add_constant(myVect, aRight);
	return *this;
}

MG_Vector& MG_Vector::operator -=(const double& aRight)
{
	gsl_vector_add_constant(myVect, -aRight);
	return *this;
}

MG_Vector& MG_Vector::operator *=(const double& aRight)
{
	gsl_vector_scale(myVect, aRight);
	return *this;
}

MG_Vector& MG_Vector::operator /=(const double& aRight)
{
	gsl_vector_scale(myVect, 1./aRight);
	return *this;
}

MG_Vector& MG_Vector::operator +=(const MG_Vector& aRight)
{
	assert(mySize == aRight.mySize);
	gsl_vector_add(myVect, aRight.myVect);
	return *this;
}

MG_Vector& MG_Vector::operator -=(const MG_Vector& aRight)
{
	assert(mySize == aRight.mySize);
	gsl_vector_sub(myVect, aRight.myVect);
	return *this;
}

MG_Vector& MG_Vector::operator *=(const MG_Vector& aRight)
{
	assert(mySize == aRight.mySize);
	gsl_vector_mul(myVect, aRight.myVect);
	return *this;
}

MG_Vector& MG_Vector::operator /=(const MG_Vector& aRight)
{
	assert(mySize == aRight.mySize);
	gsl_vector_div(myVect, aRight.myVect);
	return *this;
}

/*
 * Arithmetic operators
 * Prefix
 */
MG_Vector& MG_Vector::operator ++(void)
{
	gsl_vector_add_constant(myVect, 1.);
	return *this;
}

MG_Vector& MG_Vector::operator --(void)
{
	gsl_vector_add_constant(myVect, -1.);
	return *this;
}

/*
 * Arithmetic operators
 * Postfix
 */
MG_Vector MG_Vector::operator ++(int)
{
	MG_Vector vOld(*this);
	gsl_vector_add_constant(vOld.myVect, 1.);
	return vOld;
}

MG_Vector MG_Vector::operator --(int)
{
	MG_Vector vOld(*this);
	gsl_vector_add_constant(vOld.myVect, -1.);
	return vOld;
}

/*
 * Arithmetic functions
 */
double MG_Vector::Sum(void) const
{
	double vSum(0.);
	const double* vElts = gsl_vector_const_ptr(myVect, 0);
	for(size_t i=0; i<mySize; ++i)
		vSum += vElts[i];
	return vSum;
}

double MG_Vector::SumProduct(const MG_Vector& aRight) const
{
	assert(mySize==aRight.mySize && " : Vector's SumProduct operation should be done on vectors of the same size.");
	MG_Vector vTmp = *this;
	vTmp *= aRight;
	return vTmp.Sum();
}

/*
 * Utility functions
 */
void MG_Vector::Clear()
{
	gsl_vector_free(myVect);
	mySize = 0;
	myVect = NULL;
}

void MG_Vector::Resize(const size_t& aSize, const double& aVal)
{
	assert(aSize != 0);
	gsl_vector_free(myVect);
	mySize = aSize;
	myVect = gsl_vector_calloc(mySize);
	gsl_vector_set_all(myVect, aVal);
}

