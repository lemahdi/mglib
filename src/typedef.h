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
#define CoordNode std::pair<Coord,MG_Node*>

/* Map of CoordNode
 * Maximum of 9997 elements
 */
#define NHASH 9997
#define CoordNodeMap std::map<unsigned int,CoordNode*>


/* some useful definitions */
#define MAX_DESC_TABLE_COLUMNS 9999

#define NameFuncMap		std::map<std::string,MG_FuncPtr>
#define NameFuncPair	std::pair<std::string,MG_FuncPtr>

//#define MG_Date Date


/* enum for node direction */
enum NODE_DIRECTION {
	BACKWARD_NODE,
	NODIR_NODE,
	FORWARD_NODE
};


/* enum for nodes types */
enum NODE_TYPE {
	NODEF_NODE,
	NUM_NODE,
	NEG_NODE,
	DATE_NODE,
	REF_NODE,
	ADD_NODE,
	SUB_NODE,
	MUL_NODE,
	DIV_NODE,
	ARG_NODE,
	FUNC_NODE
};


/* enum for date displaying form */
enum DATE_DISPLAY {
	ENG_DATE,		// 2010/12/25
	FR_DATE,		// 25/12/2010
	ENG_M3L_DATE,	// 2010-Dec-25
	FR_M3L_DATE		// 25-Dec-2010
};
