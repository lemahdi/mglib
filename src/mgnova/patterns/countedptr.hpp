/*
 * Copyright			: 2010 by MG
 * Version				: 0.1.15
 * Last changed			: 22 DEC 2010
 * Purpose				: CountedPtr is an implementation of the smart pointer
 *						design pattern
 * Author				: MM Akkouh
 * Notes				: 
 */


#pragma once


#include <iostream>

#include "mgnova/glob/typedef.h"


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
	T* vTmp = (T*)aFrom.myPtr;
	if (aTo.myPtr == vTmp)
		return;

	if (vTmp)
	{
		aTo.Release();
		aTo.myCount = aFrom.myCount;
		++*aTo.myCount;
		aTo.myPtr = vTmp;
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
	template<class U>
	CountedPtr(U* aPtr) : myPtr(dynamic_cast<T*>(aPtr)), myCount(new long(1)) {}

	CountedPtr(const CountedPtr<T>& aCPtr)
	{
		myCount = aCPtr.myCount;
		++*myCount;
		myPtr = aCPtr.myPtr;
	}

	template<class U>
	CountedPtr(const CountedPtr<U>& aCPtr) : myPtr(NULL), myCount(new long(1))
	{
		CopyCountedPtr::CopyOperator(*this, aCPtr);
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

	void Swap(CountedPtr<T>& aRight)
	{
		swap(myCount, aRight.myCount);
		swap(myPtr, aRight.myPtr);
	}

	T* operator-> (void)
	{
		return myPtr;
	}

	T* operator-> (void) const
	{
		return myPtr;
	}

	T& operator* (void)
	{
		return *myPtr;
	}

	T& operator* (void) const
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

	/*bool IsNull(void)
	{
		return myPtr==NULL ? true : false;
	}*/

	bool operator() (void)
	{
		return myPtr ? true : false;
	}

	bool operator! (void)
	{
		return myPtr ? false : true;
	}

private:
	void Release(void) const
	{
		if (myCount && --*myCount==0)
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
