#include <iostream>
#include <vector>
#include <string>
#ifdef WIN32
#include <io.h>
#define isatty _isatty
#endif

/* coordinates */
#define coord std::pair<unsigned int,unsigned int>

#define yyconst const
typedef struct yy_buffer_state *YY_BUFFER_STATE;
YY_BUFFER_STATE yy_scan_string (yyconst char *yy_str  );
void yy_switch_to_buffer (YY_BUFFER_STATE new_buffer  );
void yy_flush_buffer (YY_BUFFER_STATE b  );
void yy_delete_buffer (YY_BUFFER_STATE b  );

/* node types
 * M unary minus
 * K number
 * R reference
 */

class node
{
public:
	node(void);
	node(coord& _c);
	node(int _nodetype, node* _l, node* _r, coord& _c);
	virtual ~node(void);

	int nodetype;
	coord c;

	node* l;
	node* r;
};

class num_node : public node
{
public:
	num_node(double _val, coord& _c);
	double val;
};

class ref_node : public node
{
public:
	ref_node(node* l, coord& _c);
	virtual ~ref_node(void);
};

/* build a node */
class node_builder
{
public:
	static node* build_node(int nodetype, node* l = NULL, node* r = NULL);
	static node* build_num(double num);
	static node* build_ref(node* n);//récupère le node de coord c et le fait pointer par l
};

/* evaluate a node */
class node_eval
{
public:
	static double eval(node* );
};

/* coord / node */
#define coord_node std::pair<coord,node*>

/* simple reference tab of fixed size */
#define NHASH 9997
static coord_node* coord_node_tab[NHASH];

class dorsal
{
public:
	static int dorsal::init(void);
	static unsigned int hash(coord& );
	static void insert(coord& cd, node* );
	static node* get(coord& c);
	static node* child_coord(char* s);

public:
	static int current_row;
	static int current_col;
	static std::vector<std::string> columns_names;
};
