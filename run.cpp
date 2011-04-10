#include "nodes.h"
#include "my_parser.tab.hpp"

using namespace std;

int main()
{
	dorsal::columns_names.push_back("Date");
	dorsal::columns_names.push_back("Spot");
	dorsal::columns_names.push_back("Strike");
	dorsal::columns_names.push_back("Spot-Strike");
	
	vector<string> flows;
	flows.push_back("2123");
	flows.push_back("5");
	flows.push_back("3");
	flows.push_back("Spot-Strike");

	coord cd;
	//cout << "> ";
	yy::my_parser parser(cd);	// make a cpp calc parser

	for(unsigned int i=0; i<flows.size(); i++)
	{
		yyconst char* text = flows[i].c_str();
		YY_BUFFER_STATE bp = yy_scan_string(text);
		yy_switch_to_buffer(bp);
		parser.parse();		// and run it
		yy_flush_buffer(bp);
		yy_delete_buffer(bp);
		dorsal::current_col++;
	}

	coord c(dorsal::current_row,dorsal::current_col-1);
	node* n = dorsal::get(c);
	cout << node_eval::eval(n) << endl;

	for(int i=0; i<=dorsal::current_row; i++)
	{
		for(int j=0; j<dorsal::current_col; j++)
		{
			c = coord(i,j);
			n = dorsal::get(c);
			delete n;
		}
	}

	char ch;
	cin >> ch;

	return 0;
}
