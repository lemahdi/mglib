//
//
//                          MJmatrices.cpp
//
//
/*
 Copyright (C) 2006 Mark Joshi
 Copyright (C) 2009 Narinder S Claire

 This file is part of XLW, a free-software/open-source C++ wrapper of the
 Excel C API - http://xlw.sourceforge.net/

 XLW is free software: you can redistribute it and/or modify it under the
 terms of the XLW license.  You should have received a copy of the
 license along with this program; if not, please email xlw-users@lists.sf.net

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/


#pragma warning (disable : 4996)

#include <xlw/MJmatrices.h>
#include <algorithm>
#include <assert.h>
#include <math.h>

xlw::MJMatrix&
xlw::MJMatrix::resize(size_t rows, size_t columns)
{
  if(rows==Rows && columns == Columns)
    return *this;
  else
  {
    if(Start>0)
    {
      delete [] Start;
      delete [] RowStarts;
    }
    Create(rows,columns);
    return *this;
  }

}



void xlw::MJMatrix::Create()
{
    if (Rows*Columns>0)
    {
        Start = new double[Rows*Columns];
        RowStarts = new double*[Rows];

        for (unsigned long i=0; i < Rows; i++)
            RowStarts[i] = Start+i*Columns;
    }
    else
        Start=0;

    for (unsigned long i=0; i < Rows; i++)
        for (unsigned long j=0; j < Columns; j++)
            RowStarts[i][j]=0.0;
}


void xlw::MJMatrix::Create(size_t rows, size_t cols)
{
  Rows = rows;
  Columns = cols;
  Create();
}


xlw::MJMatrix::MJMatrix(size_t Rows_, size_t Columns_)
                :       Rows(Rows_),
                        Columns(Columns_)
{
	Start = NULL;
	RowStarts = NULL;
    Create();
}

xlw::MJMatrix::MJMatrix(const MJMatrix& original)
                :       Rows(original.Rows),
                        Columns(original.Columns)
{
    Create();

    std::copy(original.Start, original.Start+Rows*Columns, Start);

}

xlw::MJMatrix& xlw::MJMatrix::operator=(const MJMatrix& original)
{
    if (this != &original)
    {
        if (Rows != original.Rows || Columns != original.Columns)
        {
            if (Start>0)
            {
                delete[] Start;
                delete[] RowStarts;
            }

            Rows = original.Rows;
            Columns = original.Columns;

            Create();
        }

        std::copy(original.Start, original.Start+Rows*Columns, Start);
    }

    return *this;
}


xlw::MJMatrix::~MJMatrix()
{
    if (Start >0)
    {
        delete[] Start;
        delete[] RowStarts;
    }
}


void xlw::MJMatrix::Swap(xlw::MJMatrix& aRight)
{
	std::swap(Rows, aRight.Rows);
	std::swap(Columns, aRight.Columns);
	std::swap(RowStarts, aRight.RowStarts);
	std::swap(Start, aRight.Start);
}


xlw::MJMatrix xlw::MJMatrix::Cholesky() const
{
	assert(Columns == Rows);

	double vTmp;
	MJMatrix vTri(Rows, Columns);
	for(size_t j=0; j<Rows; ++j)
	{
		for(size_t i=j; i<Rows; ++i)
		{
			vTmp = this->operator ()(i, j);
			for(size_t k=0; k<j-1; ++k)
			{
				vTmp -= vTri(j, k)*vTri(i, k);
			}
			vTri(i, j) = vTmp / sqrt(vTmp); // could be sqrt(vTmp), but if useful when vTmp=0 when the input matrix is not symmetric definite positive
		}
	}
	return vTri;
}