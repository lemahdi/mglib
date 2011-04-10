#include "nodes.h"
#include "my_parser.tab.hpp"

using namespace std;

int main()
{
	MG_SFileError::Instance()->Init();

	// Deal Description
	vector<string> vColNames;
	vColNames.push_back("Date");
	vColNames.push_back("Spot");
	vColNames.push_back("Strike");
	vColNames.push_back("Payoff");

	vector<string> vFlows;
	vFlows.push_back("1");
	vFlows.push_back("5");
	vFlows.push_back("3");
	vFlows.push_back("EXP(-0.05*Date[i])*MAX(Spot[i]-Strike[i],0)");
	vFlows.push_back("2");
	vFlows.push_back("6");
	vFlows.push_back("3");
	vFlows.push_back("EXP(-0.05*Date[i])*MAX(Spot[i+1]-Strike[i],0)");
	vFlows.push_back("3");
	vFlows.push_back("2");
	vFlows.push_back("3");
	vFlows.push_back("EXP(-0.05*Date[i])*MAX(Spot[i-1]-Strike[i],0)");
	vFlows.push_back("4");
	vFlows.push_back("3");
	vFlows.push_back("3");
	vFlows.push_back("EXP(-0.05*Date[i])*MAX(Spot[i-1]-Strike[i],0)");

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

	for(unsigned int i=0; i<walker.GetRows(); i++)
	{
		Coord c(i,(unsigned int)walker.GetCols()-1);
		MG_Node* n = manager.GetNode(c);
		cout << manager.Eval(n) << endl;
	}

	for(unsigned int i=0; i<walker.GetRows(); i++)
	{
		for(unsigned int j=0; j<walker.GetCols(); j++)
		{
			Coord c(i,j);
			MG_Node* n = manager.GetNode(c);
			delete n;
		}
	}

	MG_SFuncBuilder::Release();
	MG_SFileError::Release();

	char ch;
	cin >> ch;

	return 0;
}
