#include "mgnova/wrapper/vector.h"


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
	gsl_vector_swap(myVect, aRight.myVect);
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

