#include "mginfra/nodes.h"
#pragma warning(push)
#pragma warning(disable:4512)
#include "mginfra/my_parser.tab.hpp"
#pragma warning(pop)
#include "mgnova/date.h"
#include "mgnumerical/random.h"
#include "mgnumerical/distributions.h"
#include "mgnova/calendar.h"
#include "mgnumerical/solver.h"
#include "mgnova/wrapper/matrix.h"
#include "mgnumerical/regression.h"

#include "gsl/gsl_rng.h"
#include "gsl/gsl_qrng.h"
#include "gsl/gsl_randist.h"
#include "gsl/gsl_cdf.h"
#include "gsl/gsl_interp.h"
#include "gsl/gsl_nan.h"
#include "gsl/gsl_sys.h"
#include "gsl/gsl_vector.h"


using namespace std;
using namespace MG;

int main()
{
	MG_SFileError::Instance()->Init();
	MG_SFuncBuilder::Instance()->Init();
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

	{
		cout << "DATE" << endl;
		MG_Date vDate;//(25,12,2010);
		cout << vDate.ToString(' ', ENG_M3L_DATE) << endl;
		MG_Date *vFake = (MG_Date*)vDate.Clone();
		delete vFake;
	}

	cin >> ch;

	// GEB : debut test generateur normale
	/*int nbSimul = 10;
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
	fclose(f);*/


	// Regression
	{
		cout << "REGRESSION" << endl;
		
		// Linear Regression
		MG_Vector vDataX(5), vDataY(5), vWeights(5, 1./(2*5));
		vDataX[0]=1.; vDataX[1]=3.; vDataX[2]=4.; vDataX[3]=5.; vDataX[4]=9.;
		vDataY[0]=2.2; vDataY[1]=5.; vDataY[2]=7.; vDataY[3]=8.4; vDataY[4]=16;

		cout << "Linear Regression:" << endl;
		MG_LinearReg vReg(vDataX, vDataY);
		vReg.ProcessLinearReg();
		MG_Vector vXs(3);
		vXs[0]=2.; vXs[1]=4.5; vXs[2]=7.;
		double vX;
		for(size_t i=0; i<vXs.Size(); ++i)
		{
			vX = vXs[i];
			cout << "X=" << vX << " ==> Y=" << vReg.Estimate(vX) << endl;
		}

		cout << "Weighted Linear Regression" << endl;
		MG_LinearReg vWReg(vDataX, vDataY, vWeights);
		vWReg.ProcessWLinearReg();
		for(size_t i=0; i<vXs.Size(); ++i)
		{
			vX = vXs[i];
			cout << "X=" << vX << " ==> Y=" << vWReg.Estimate(vX) << endl;
		}

		// Multi Linear Regression
		MG_Matrix vMDataX(5, 2);
		vMDataX.Elt(0,0)=1.; vMDataX.Elt(1,0)=3.; vMDataX.Elt(2,0)=4.; vMDataX.Elt(3,0)=5.; vMDataX.Elt(4,0)=9.;
		vMDataX.Elt(0,1)=1.; vMDataX.Elt(1,1)=9.; vMDataX.Elt(2,1)=16.; vMDataX.Elt(3,1)=25.; vMDataX.Elt(4,1)=81.;

		cout << "Multi Linear Regression:" << endl;
		MG_MultiReg vMReg(vMDataX, vDataY);
		vMReg.ProcessLinearReg();
		MG_Vector vMX1(2);
		MG_Vector vMX2(2);
		MG_Vector vMX3(2);
		vMX1[0]=2.; vMX1[1]=4.5;
		vMX2[0]=4.5; vMX2[1]=18.5;
		vMX3[0]=3.9; vMX3[1]=13.5;
		cout << "X=(" << vMX1[0] << "," << vMX1[1] << ") ==> Y=" << vMReg.Estimate(vMX1) << endl;
		cout << "X=(" << vMX2[0] << "," << vMX2[1] << ") ==> Y=" << vMReg.Estimate(vMX2) << endl;
		cout << "X=(" << vMX3[0] << "," << vMX3[1] << ") ==> Y=" << vMReg.Estimate(vMX3) << endl;

		cout << "Multi Weighted Linear Regression" << endl;
		MG_MultiReg vWMReg(vMDataX, vDataY, vWeights);
		vWMReg.ProcessWLinearReg();
		cout << "X=(" << vMX1[0] << "," << vMX1[1] << ") ==> Y=" << vWMReg.Estimate(vMX1) << endl;
		cout << "X=(" << vMX2[0] << "," << vMX2[1] << ") ==> Y=" << vWMReg.Estimate(vMX2) << endl;
		cout << "X=(" << vMX3[0] << "," << vMX3[1] << ") ==> Y=" << vWMReg.Estimate(vMX3) << endl;
	}

	cin >> ch;


	// Solver
	{
		cout << "SOLVER" << endl;
		class MG_TestFunc : public MG_FDfFunction
		{
		private:
			double F_Func(const double& aX) { return exp(aX) - aX*aX; }
			double Df_Func(const double& aX) { return exp(aX) - 2*aX; }
			void FDf_Func(const double& aX, double& aF, double& aDf) { aF=F_Func(aX); aDf=Df_Func(aX); }
		};
		MG_FunctionPtr vFunc(new MG_TestFunc);
		MG_FDfSolver vSolver(MG_FDfSolver::NEWTON, 1., 0., 1e-3, 100);
		vSolver.Load(vFunc);
		double vSol = vSolver.Solve();
		cout << "Newton Solver Exp(x)-x²: " << vSol << endl;
	}

	cin >> ch;

	// Matrix
	{
		cout << "MATRIX" << endl;
		MG_Matrix vM1(3,4,2);
		MG_Matrix vM2(3,4,-4);
		MG_Matrix vM3 = vM1 + vM2;
		vM1 += vM2;
		vM2--;
		--vM2;
	}

	// GSL Random Generator
	{
		cout << "Mersenne Twister" << endl;
		MG_Random vRand(MT19937);
		size_t vCount = 100;
		while (--vCount) cout << vRand.DrawOne() << ":" << vRand.DrawUniform() << endl;

		FILE* stream(NULL);
		errno_t vErr = fopen_s(&stream, "C:\\cygwin\\home\\Akkouh\\random_state.txt", "w");
		vRand.ToString(stream);
		fclose(stream);

		const gsl_rng_type **t, **t0;
		t0 = gsl_rng_types_setup();
		cout << "Available Generators:" << endl;
		for(t=t0; *t!=NULL; ++t)
			cout << (*t)->name << endl;

		cout << "GSL Random Generators" << endl;
	}

	cin >> ch;

	// GSL Quasi Random Generator
	{
		cout << "SOBOL" << endl;
		size_t vCount = 100;
		MG_QuasiRandom vRand(SOBOL, 1);
		while (--vCount) cout << vRand.DrawOne() << ":" << vRand.DrawUniform() << endl;

		FILE* stream(NULL);
		errno_t vErr = fopen_s(&stream, "C:\\cygwin\\home\\Akkouh\\quasirandom_state.txt", "w");
		vRand.ToString(stream);
		fclose(stream);

		cout << "GSL Quasi Random Generators" << endl;
	}

	cin >> ch;

	// Gaussian Distribution
	{
		cout << "GAUSSIAN DISTRIBUTION" << endl;
		MG_RandomPtr vRand(new MG_Random(MT19937));
		MG_NormalDist vDist(vRand, MG_NormalDist::ZIGGURAT);
		size_t vCount = 100;
		double x, p, inv_p;
		while (--vCount)
		{
			x=vDist.Draw();
			p=vDist.Cdf(x);
			inv_p = vDist.InvCdf(p);
			cout << x << ":" << p << ":" << inv_p << ":" << x-inv_p << endl;
		}
		cout << "GSL Gaussian Distribution" << endl;
	}

	cin >> ch;

	return 0;
}
