#pragma once


/* Some pre-declarations from the parser */
#define yyconst const
typedef size_t yy_size_t;
typedef struct yy_buffer_state *YY_BUFFER_STATE;
YY_BUFFER_STATE yy_scan_string (yyconst char *yy_str  );
void yy_flush_buffer (YY_BUFFER_STATE b  );
void yy_delete_buffer (YY_BUFFER_STATE b  );


/* Error file for Lexer and Parser */
#define ERROR_FILE "error_log.txt"


/* Coord is is a pair.
 * This pair is the coordinates for a cell in a table.
 * The first element coordinates are (0,0).
 */
#define Coord std::pair<unsigned int,unsigned int>

/* Coord / Node */
#define CoordNode std::pair<Coord,NodePtr>
#define CoordNodePtr CountedPtr<CoordNode>

/* Map of CoordNode
 * Maximum of 9997 elements
 */
#define NHASH 9997
#define CoordNodeMap std::map<unsigned int,CoordNodePtr>


/* some useful definitions */
#define MAX_DESC_TABLE_COLUMNS 9999
#define MAX_DOUBLE 1.7976931348623158e+308


/* enum for nodes types */
enum NODE_TYPE
{
	NODEF_NODE,
	NUM_NODE,
	NEG_NODE,
	REF_NODE,
	ADD_NODE,
	SUB_NODE,
	MUL_NODE,
	DIV_NODE,
	ARG_NODE,
	FUNC_NODE
};


/* useful shortcuts */
#define NameFuncMap		std::map<std::string,FuncPtr>
#define NameFuncPair	std::pair<std::string,FuncPtr>

/* smart pointers */
#include "countedptr.hpp"
class Func;
class Node;

typedef CountedPtr<Func> FuncPtr;
typedef CountedPtr<Node> NodePtr;
