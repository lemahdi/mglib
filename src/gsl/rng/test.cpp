#include <iostream>


using namespace std;


template<bool b>
struct If_Equal {};

template<>
struct If_Equal<true> { typedef int CHECK; };


struct UnknownParam     { enum { NB = 0 }; void Print(void) { cout << "ZERO" << endl; } };
struct OneParam         { enum { NB = 1 }; typedef UnknownParam PREV; double a; void Print(void) { cout << "ONE" << endl; } };
struct TwoParam         { enum { NB = 2 }; typedef OneParam PREV; double a; double b; void Print(void) { cout << "TWO" << endl; } };
struct ThreeParam		{ enum { NB = 3 }; typedef TwoParam PREV; double a; double b; double c; void Print(void) { cout << "THREE" << endl; } };
 
 
template<unsigned int n, typename P, bool b>
struct Until {};
 
template<unsigned int n, typename P>
struct Until<n,P,true> { typedef P TYPE; };
 
template<unsigned int n,typename P>
struct Until<n,P,false> { typedef typename Until<n,typename P::PREV,P::PREV::NB==n>::TYPE TYPE; };
 
template<unsigned int n>
struct Param
{
	typedef ThreeParam MaxParam;
	typedef typename If_Equal<n <= MaxParam::NB>::CHECK CHECK;

	typedef typename Until<n,MaxParam,MaxParam::NB==n>::TYPE P;
};
 
 
template<unsigned int n>
class Func
{
public:
      typename Param<n>::P myParam;
};
 
int main(void)
{
      Func<2> f2;
      f2.myParam.Print();
 
      Func<3> f3;
      f3.myParam.Print();
     
      Func<0> f0;
      f0.myParam.Print();
     
      Func<1> f1;
      f1.myParam.Print();
	  
      /*Func<4> f4;
      f4.myParam.Print();*/

      char c;
      cin >> c;
 
      return 0;
}

