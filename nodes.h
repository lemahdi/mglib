#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <assert.h>
#ifdef WIN32
#include <io.h>
#define isatty _isatty
#endif

#include "typedef.h"
#include "singleton.hpp"


/* class FileError
 * keeping a trace of Lexer and Paser errors
 */
class FileError
{
	template<class T> friend class Singleton;

private:
	FileError(void) {}

public:
	void Init(void);

private:
	std::ofstream myFile;
};
typedef Singleton<FileError> SFileError;


/* Table Walker class
 * Simple class providing current cell coordinates
 * and table column names
 */
class TableWalker
{
public:
	TableWalker(void);
	virtual ~TableWalker(void) {}

public:
	/* accessors */
	inline unsigned int				GetCurrentRow	(void) const { return myCurrentRow; }
	inline unsigned int				GetCurrentCol	(void) const { return myCurrentCol; }
	inline std::vector<std::string>	GetColumnNames	(void) const { return myColumnNames; }

	inline unsigned int				IncCurrentRow	(void) { return ++myCurrentRow; }
	inline unsigned int				IncCurrentCol	(void) { assert(myCurrentCol<MAX_DESC_TABLE_COLUMNS); return ++myCurrentCol; }
	inline unsigned int				DecCurrentRow	(void) { return --myCurrentRow; }
	inline unsigned int				DecCurrentCol	(void) { return --myCurrentCol; }

	unsigned int GetColumn(const std::string& aColName) const;

	inline void SetColumnNames(const std::vector<std::string>& aColNames) { myColumnNames = aColNames; }

private:
	unsigned int myCurrentRow;
	unsigned int myCurrentCol;

	std::vector<std::string> myColumnNames;
};


/* class Node
 * a syntaxic tree of all tokens
 */
class Node
{
public:
	Node(void);
	Node(const NODE_TYPE& aNodeType, const Coord& aCoord, Node* aL, Node* aR);
	virtual ~Node(void);

	inline NODE_TYPE	GetNodeType	(void) const { return myNodeType; }
	inline Node*		GetL		(void) const { return myL; }
	inline Node*		GetR		(void) const { return myR; }

protected:
	NODE_TYPE	myNodeType;
	Coord		myCoord;

	Node*		myL;
	Node*		myR;
};

/* class NumNode
 * specific to a double cell
 */
class NumNode : public Node
{
public:
	NumNode(const Coord& aCoord, const double& aValue);

	inline double GetValue(void) const { return myValue; }

protected:
	double myValue;
};

/* class RefNode
 * specific to a reference cell
 */
class RefNode : public Node
{
public:
	RefNode(const Coord& aCoord, Node* aL);
	virtual ~RefNode(void);
};


/* A class for managing nodes
 ** Initializing
 ** Accessing
 ** Building
 ** Evaluating
 */
class NodeManager
{
public:
	NodeManager(void) {}
	virtual ~NodeManager(void) {}

	/* Accessing */
public:
	Node*			GetNode		(const Coord& aC);
	Node*			GetChildNode(const TableWalker& walker, const char* ref);
	unsigned int	Hash		(const Coord& aC);
	void			Insert		(const Coord& aC, Node* aN);

	/* Building */
	Node* BuildNode	(const TableWalker& walker, const NODE_TYPE& aNodeType, Node* aL = NULL, Node* aR = NULL);
	Node* BuildNum	(const TableWalker& walker, const double& aNum);
	Node* BuildRef	(const TableWalker& walker, Node* aN);

	/* Evaluating */
	double Eval(Node* aN);

private:
	/* Map of all nodes */
	CoordNodeMap AllNodes;
};
