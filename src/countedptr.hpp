/*
 * Copyright			: 2010 by MG
 * File					: countedptr.hpp
 * Version				: 0.1.15
 * Last changed			: 22 DEC 2010
 * Purpose				: CountedPtr is an implementation of the smart pointer
 *						design pattern
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include <iostream>

#include "typedef.h"


MG_NAMESPACE_BEGIN


template<class T>
class CountedPtr;

class CopyCountedPtr
{
public:
	template<class T,class U>
	static void CopyOperator(const CountedPtr<T>& aTo, const CountedPtr<U>& aFrom);
};


template<class T,class U>
void CopyCountedPtr::CopyOperator(const CountedPtr<T>& aTo, const CountedPtr<U>& aFrom)
{
	if (dynamic_cast<T*>(aFrom.myPtr))
	{
		if (aTo.myPtr)
		{
			if (aTo.myPtr != aFrom.myPtr)
			{
				aTo.Release();
				aTo.myCount = aFrom.myCount;
				++*aTo.myCount;
				aTo.myPtr = aFrom.myPtr;
			}
		}
		else
		{
			delete aTo.myCount;
			aTo.myCount = aFrom.myCount;
			++*aTo.myCount;
		}
	}
}

template<class T>
class CountedPtr
{
	friend class CopyCountedPtr;

public:
	CountedPtr(void) : myPtr(NULL), myCount(new long(1)) {}
	~CountedPtr(void)
	{
		Release();
	}
	CountedPtr(T* aPtr) : myPtr(aPtr), myCount(new long(1)) {}

	CountedPtr(const CountedPtr<T>& aCPtr)
	{
		myCount = aCPtr.myCount;
		++*myCount;
		myPtr = aCPtr.myPtr;
	}

	CountedPtr<T>& operator= (const CountedPtr<T>& aCPtr)
	{
		CopyCountedPtr::CopyOperator(*this, aCPtr);
		return *this;
	}

	template<class U>
	CountedPtr<T>& operator= (const CountedPtr<U>& aCPtr)
	{
		CopyCountedPtr::CopyOperator(*this, aCPtr);
		return *this;
	}

	T* operator-> (void)
	{
		return myPtr;
	}

	T& operator* (void)
	{
		return *myPtr;
	}

	bool operator== (const CountedPtr& aCPtr)
	{
		return myPtr==aCPtr.myPtr;
	}

	bool operator!= (const CountedPtr& aCPtr)
	{
		return myPtr!=aCPtr.myPtr;
	}

	bool IsNull(void)
	{
		return myPtr==NULL ? true : false;
	}

private:
	void Release(void) const
	{
		if (--*myCount == 0)
		{
			delete myCount;
			delete myPtr;
		}
	}

private:
	mutable T*		myPtr;
	mutable long*	myCount;
};


MG_NAMESPACE_END
