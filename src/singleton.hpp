#pragma once

#include <iostream>

template<class T>
class Singleton
{
private:
	Singleton(void) {}

public:
	static T* Instance(void)
	{
		if (!myInstance)
			Singleton::myInstance = new T;

		return Singleton::myInstance;
	}

	static void Release(void)
	{
		delete Singleton::myInstance;
		Singleton::myInstance = NULL;
	}

private:
	static T* myInstance;
};
