#include "mginfra/nodes.h"
#pragma warning(push)
#pragma warning(disable:4512)
#include "mginfra/my_parser.tab.hpp"
#pragma warning(pop)
#include "mgnova/date.h"
#include "mgnumerical/random.h"
#include "mgnumerical/normal.h"
#include "mgnova/calendar.h"
#include "mgnumerical/solver.h"
#include "mgnova/matrix.h"


using namespace std;
using namespace MG;

int main()
{
	MG_SFileError::Instance()->Init();
	MG_SFuncBuilder::Instance()->Init();
	MG_SCdfNormal::Instance()->Init();
	MG_RCalendar::Init();

	// Deal Description
	vector<string> vColNames;
	vColNames.push_back("ResetDate");
	vColNames.push_back("StartDate");
	vColNames.push_back("EndDate");
	vColNames.push_back("Spot");
	vColNames.push_back("Strike");
	vColNames.push_back("Fake");
	vColNames.push_back("Payoff");

	vector<string> vFlows;
	vFlows.push_back("15/02/2011");
	vFlows.push_back("17/02/2011");
	vFlows.push_back("17/05/2011");
	vFlows.push_back("5");
	vFlows.push_back("IF(3>2,3,10)");
	vFlows.push_back("0");
	vFlows.push_back("EXP(-0.05)*(EndDate[i]-ResetDate[i])*MAX(Spot[i]-Strike[i],0)");

	vFlows.push_back("15/02/2012");
	vFlows.push_back("17/02/2012");
	vFlows.push_back("17/05/2012");
	vFlows.push_back("6");
	vFlows.push_back("3");
	vFlows.push_back("Spot[i]");
	vFlows.push_back("POW(-0.05*(EndDate[i]-StartDate[i]),2)*MAX(Spot[i+1]-Strike[i],0)");

	vFlows.push_back("15/02/2013");
	vFlows.push_back("17/02/2013");
	vFlows.push_back("17/05/2013");
	vFlows.push_back("2");
	vFlows.push_back("3");
	vFlows.push_back("Fake[i-2]");
	vFlows.push_back("ABS(-0.05)*MAX(Spot[i-1]-Strike[i],0)");

	vFlows.push_back("15/02/2014");
	vFlows.push_back("17/02/2014");
	vFlows.push_back("17/05/2014");
	vFlows.push_back("3");
	vFlows.push_back("3");
	vFlows.push_back("4");
	vFlows.push_back("EXP(-0.05)*MAX(Spot[i-1]-Strike[i],0)");

	MG_TableWalker walker(vColNames, vFlows);;
	MG_NodeManager manager;
	yy::my_parser parser(walker, manager);	// make a cpp calc parser

	//cout << "> ";
	for(unsigned int i=0; i<vFlows.size(); i++)
	{
		const char* text = vFlows[i].c_str();
		YY_BUFFER_STATE bp = yy_scan_string(text);
		parser.parse();		// and run it
		yy_flush_buffer(bp);
		yy_delete_buffer(bp);
		if (walker.GetCurrentCol() == walker.GetCols()-1)
			walker.IncCurrentRow();
		walker.IncCurrentCol();
	}
	manager.PostProcess();

	for(unsigned int i=0; i<walker.GetRows(); i++)
	{
		Coord c(i,(unsigned int)walker.GetCols()-1);
		MG_Node* n = manager.GetNode(c);
		cout << manager.Eval(n) << endl;
	}


	char ch;
	cin >> ch;

	MG_Date vDate;//(25,12,2010);
	cout << vDate.ToString(' ', ENG_M3L_DATE) << endl;
	MG_Date *vFake = (MG_Date*)vDate.Clone();
	delete vFake;

	//MG_ParkMillerRand* vRand = new MG_ParkMillerRand();
	//MG_RandomPtr vRandPtr(vRand);
	//MG_BoxMullerSampler vSampler(100, vRandPtr);
	//vector<double> vRandVars = vSampler.GenerateSample();
	//double vN1, vN2;
	//for(unsigned int i=0; i<100; i++)
	//{
	//	vN1 = vRandVars[i];
	//	cout << vN1 << " ";
	//	vN2 = MG_SCdfNormal::Instance()->CumulativeNormal(vN1);
	//	vN2 = MG_SCdfNormal::Instance()->InverseNormal(vN2);
	//	cout << vN2 << " : " << vN1-vN2 << endl;
	//}

	// GEB : debut test generateur normale
	int nbSimul = 10;
	int nbScenarK = 1001;
	std::vector<double> PrixCalls, ScenraK, PrixBS;
	PrixCalls.resize(nbScenarK,0.);
	ScenraK.resize(nbScenarK,0.);
	PrixBS.resize(nbScenarK,0.);
	double Kmin = 0.;
	double Kmax = 5.;
	double PasK = (Kmax-Kmin)/(nbScenarK-1);
	for (int j = 0; j < nbScenarK; j++) {
		ScenraK[j] = Kmin + j*PasK;
	}

	MG_ParkMillerRand* vRand = new MG_ParkMillerRand();
	MG_RandomPtr vRandPtr(vRand);
	MG_SamplerPtr vSampler(new MG_BoxMullerSampler(1, vRandPtr));
	//vSampler = MG_SamplerPtr(new MG_AntitheticSampler(1, vSampler));
	std::vector<double> x, xanti;
	double esp = 0.;
	double Vol = 0.3;
	double T = 1.;
	double S, Santi, d1, d0;

	for (int i = 0; i < nbSimul; i++) {
		x = vSampler->GenerateSample();
		xanti = vSampler->GetSampleAntithetic(x);
		S = exp(-0.5*Vol*Vol*T + Vol*sqrt(T)*x[0]);
		Santi = exp(-0.5*Vol*Vol*T + Vol*sqrt(T)*xanti[0]);
		for (int j = 0; j < nbScenarK; j++) {
			PrixCalls[j] += 0.5*max(S-ScenraK[j],0.)/nbSimul; 
			PrixCalls[j] += 0.5*max(Santi-ScenraK[j],0.)/nbSimul; 
		}
		//esp += (0.5*x[0] + 0.5*xanti[0])/nbSimul;
	}

	for (int j = 0; j < nbScenarK; j++) {
		d1 = -log(ScenraK[j]) + 0.5*Vol*Vol*T;
		d1 /= Vol*sqrt(T);
		d0 = d1 - Vol*sqrt(T);
		PrixBS[j] = MG_SCdfNormal::Instance()->CumulativeNormal(d1) - ScenraK[j]*MG_SCdfNormal::Instance()->CumulativeNormal(d0);
	}

#ifdef WIN32
	FILE* f(NULL);
	fopen_s(&f,"TestNormal.data","w");
#else
	FILE* f = fopen("TestNormal.data","w");
#endif
	if (f) {
		for (int j = 0; j < nbScenarK; j++) {
			fprintf(f,"%f/n",PrixCalls[j]); 
		}
	}
	fclose(f);









	MG_UnaryFuncPtr vFuncTestPtr(new MG_TestFunc);
	MG_UnaryFuncPtr vFuncTestPrimePtr(new MG_TestFuncPrime);
	MG_NewtonRaphsonSolver vSolver1(vFuncTestPtr, vFuncTestPrimePtr, 1e-10, 20);
	double vRoot = vSolver1.Solve(0, 0, -1, 1);
	cout << "Newton Raphson Solver Exp(x)-x²: " << vRoot << endl;
	double vCheck = exp(vRoot) - vRoot*vRoot;
	cout << "Solver checker: " << vCheck << endl;
	cout << "Number of iterations: " << vSolver1.GetNbIter() << endl;
	MG_BrentSolver vSolver2(vFuncTestPtr, 1e-10, 20);
	vRoot = vSolver2.Solve(0, 0, -1, 1);
	cout << "Brent Solver Exp(x)-x²: " << vRoot << endl;
	vCheck = exp(vRoot) - vRoot*vRoot;
	cout << "Solver checker: " << vCheck << endl;
	cout << "Number of iterations: " << vSolver2.GetNbIter() << endl;

	cin >> ch;

	MG_Matrix vM1(3,4,2);
	MG_Matrix vM2(3,4,-4);
	MG_Matrix vM3 = vM1 + vM2;
	vM1 += vM2;
	vM2--;
	--vM2;


	MG_RCalendar::Release();

	return 0;
}
