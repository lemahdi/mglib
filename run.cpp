#include "nodes.h"
#include "my_parser.tab.hpp"
#include "date.h"

using namespace std;

int main()
{
	MG_SFileError::Instance()->Init();
	MG_SFuncBuilder::Instance()->Init();

	// Deal Description
	vector<string> vColNames;
	vColNames.push_back("ResetDate");
	vColNames.push_back("StartDate");
	vColNames.push_back("EndDate");
	vColNames.push_back("Spot");
	vColNames.push_back("Strike");
	vColNames.push_back("Payoff");

	vector<string> vFlows;
	vFlows.push_back("15/02/2011");
	vFlows.push_back("17/02/2011");
	vFlows.push_back("17/05/2011");
	vFlows.push_back("5");
	vFlows.push_back("3");
	vFlows.push_back("EXP(-0.05)*(EndDate[i]-ResetDate[i])*MAX(Spot[i]-Strike[i],0)");
	vFlows.push_back("15/02/2012");
	vFlows.push_back("17/02/2012");
	vFlows.push_back("17/05/2012");
	vFlows.push_back("6");
	vFlows.push_back("3");
	vFlows.push_back("POW(-0.05*(EndDate[i]-StartDate[i]),2)*MAX(Spot[i+1]-Strike[i],0)");
	vFlows.push_back("15/02/2013");
	vFlows.push_back("17/02/2013");
	vFlows.push_back("17/05/2013");
	vFlows.push_back("2");
	vFlows.push_back("3");
	vFlows.push_back("ABS(-0.05)*MAX(Spot[i-1]-Strike[i],0)");
	vFlows.push_back("15/02/2014");
	vFlows.push_back("17/02/2014");
	vFlows.push_back("17/05/2014");
	vFlows.push_back("3");
	vFlows.push_back("3");
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

	MG_SFuncBuilder::Release();
	MG_SFileError::Release();

	char ch;
	//cin >> ch;

	MG_Date vDate;//(25,12,2010);
	cout << vDate.ToString(' ', ENG_M3L_DATE) << endl;

	cin >> ch;
	return 0;
}
