#include "nodes.h"
#include "my_parser.tab.hpp"

using namespace std;

int main()
{
	SFileError::Instance()->Init();

	/* Deal Description */
	vector<string> vColNames;
	vColNames.push_back("Date");
	vColNames.push_back("Spot");
	vColNames.push_back("Strike");
	vColNames.push_back("Payoff");
	
	vector<string> vFlows;
	vFlows.push_back("2010");
	vFlows.push_back("5");
	vFlows.push_back("3");
	vFlows.push_back("Spot[i]-Strike[i]");
	vFlows.push_back("2011");
	vFlows.push_back("6");
	vFlows.push_back("3");
	vFlows.push_back("Spot[i-1]-Strike[i]");
	vFlows.push_back("2012");
	vFlows.push_back("4");
	vFlows.push_back("2");
	vFlows.push_back("Spot[i-1]-Strike[i]");
	vFlows.push_back("2013");
	vFlows.push_back("3");
	vFlows.push_back("3");
	vFlows.push_back("Spot[i-1]-Strike[i-1]+Payoff[i-1]");

	TableWalker walker(vColNames, vFlows);;
	NodeManager manager;
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
		Coord c(i,walker.GetCols()-1);
		Node* n = manager.GetNode(c);
		cout << manager.Eval(n) << endl;
	}

	for(unsigned int i=0; i<walker.GetRows(); i++)
	{
		for(unsigned int j=0; j<walker.GetCols(); j++)
		{
			Coord c(i,j);
			Node* n = manager.GetNode(c);
			delete n;
		}
	}

	SFileError::Release();

	char ch;
	cin >> ch;

	return 0;
}
