#include "nodes.h"
#include <math.h>

using namespace std;

int dorsal::current_row = 0;
int dorsal::current_col = 0;
vector<string> dorsal::columns_names = vector<string>();
int dummy = dorsal::init();
int dorsal::init(void)
{
	for(unsigned int i=0; i<NHASH; i++)
		coord_node_tab[i] = NULL;

	return 0;
}


node::node(void) : nodetype(0), l(NULL), r(NULL)
{}

node::node(coord& _c) : nodetype(0), l(NULL), r(NULL), c(_c)
{}

node::node(int _nodetype, node* _l, node* _r, coord& _c)
	: nodetype(_nodetype), l(_l), r(_r), c(_c)
{}

node::~node(void)
{
	if (l) delete l;
	if (r) delete r;
}

num_node::num_node(double _val, coord& _c)
	: node(_c), val(_val)
{
	nodetype = 'K';
}

ref_node::ref_node(node* _l, coord& _c)
	: node(_c)
{
	nodetype = 'R';
	l = _l;
}

ref_node::~ref_node(void)
{
	l = NULL; // l is already referenced by the coord_node_tab
}


node* node_builder::build_node(int nodetype, node* l, node* r)
{
	coord c(dorsal::current_row, dorsal::current_col);
	node* n = new node(nodetype, l, r, c);
	dorsal::insert(c, n);
	return n;
}

node* node_builder::build_num(double num)
{
	coord c(dorsal::current_row, dorsal::current_col);
	node* n = new num_node(num, c);
	dorsal::insert(c, n);
	return n;
}

node* node_builder::build_ref(node* n)
{
	coord c(dorsal::current_row, dorsal::current_col);
	node* n_ref = new ref_node(n, c);
	dorsal::insert(c, n_ref);
	return n_ref;
}


double
node_eval::eval(node *n)
{
	double v;

	if (!n) {
		cerr << "ERROR" << endl;
		return 0;
	}

	switch(n->nodetype) {
		/* constant */
		case 'K': v = ((num_node *)n)->val; break;

		/* name reference */
		case 'R': v = eval(n->l); break;

		/* expressions */
		case '+': v = eval(n->l) + eval(n->r); break;
		case '-': v = eval(n->l) - eval(n->r); break;
		case '*': v = eval(n->l) * eval(n->r); break;
		case '/': v = eval(n->l) / eval(n->r); break;
		case '|': v = fabs(eval(n->l)); break;
		case 'M': v = -eval(n->r); break;

		default: cout << "internal error: bad node " << n->nodetype << endl;
	}
	return v;
}


unsigned int dorsal::hash(coord& c)
{
	unsigned int hash = 0;

	hash = 1;
	hash = hash*9 ^ c.first;
	hash = hash*9 ^ c.second;

	return hash;
}

void dorsal::insert(coord& c, node* n)
{
	unsigned int hash = dorsal::hash(c)%NHASH;
	coord_node* cn = coord_node_tab[hash];

	int scount = NHASH;
	while (--scount >= 0) {
		if (!cn)
		{
			cn = new coord_node();
			coord_node_tab[hash] = cn;

			cn->first = c;
			cn->second = n;
			return;
		}
		if (cn->second && cn->first.first==c.first && cn->first.second==c.second)
		{
			cn->second = n;
			return;
		}

		if (++cn >= *coord_node_tab+NHASH)
			cn = *coord_node_tab;
	}

	cerr << "ERROR" << endl;
	return;
}

node* dorsal::get(coord& c)
{
	unsigned int hash = dorsal::hash(c)%NHASH;
	node* n = coord_node_tab[hash]->second;
	return n;
}

node* dorsal::child_coord(char* s)
{
	for(unsigned int i=0; i<dorsal::columns_names.size(); i++)
	{
		if (!strcmp(dorsal::columns_names[i].c_str(), s))
		{
			coord c(dorsal::current_row, i);
			node* n = dorsal::get(c);
			return n;
		}
	}

	cerr << "ERROR" << endl;
	return NULL;
}
