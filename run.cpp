#include "nodes.h"
#include "my_parser.tab.hpp"

using namespace std;

int main()
{
	SFileError::Instance()->Init();

	TableWalker walker;
	NodeManager manager;
	yy::my_parser parser(walker, manager);	// make a cpp calc parser

	vector<string> vColNames;
	vColNames.push_back("Date");
	vColNames.push_back("Spot");
	vColNames.push_back("Strike");
	vColNames.push_back("Payoff");
	walker.SetColumnNames(vColNames);
	
	vector<string> flows;
	flows.push_back("2123");
	flows.push_back("5");
	flows.push_back("3");
	flows.push_back("Spot-Strike");

	//cout << "> ";
	for(unsigned int i=0; i<flows.size(); i++)
	{
		const char* text = flows[i].c_str();
		YY_BUFFER_STATE bp = yy_scan_string(text);
		parser.parse();		// and run it
		yy_flush_buffer(bp);
		yy_delete_buffer(bp);
		walker.IncCurrentCol();
	}

	Coord c(walker.GetCurrentRow(),walker.GetCurrentCol()-1);
	Node* n = manager.GetNode(c);
	cout << manager.Eval(n) << endl;

	for(unsigned int i=0; i<=walker.GetCurrentRow(); i++)
	{
		for(unsigned int j=0; j<walker.GetCurrentCol(); j++)
		{
			c = Coord(i,j);
			n = manager.GetNode(c);
			delete n;
		}
	}

	SFileError::Release();

	char ch;
	cin >> ch;

	return 0;
}
