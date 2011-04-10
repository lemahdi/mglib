#pragma once

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
#include "func.h"
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
	TableWalker(const std::vector<std::string>& aColNames, const std::vector<std::string>& aFlows);
	virtual ~TableWalker(void) {}

public:
	/* accessors */
	inline unsigned int				GetCurrentRow	(void) const { return myCurrentRow; }
	inline unsigned int				GetCurrentCol	(void) const { return myCurrentCol; }
	inline std::vector<std::string>	GetColumnNames	(void) const { return myColumnNames; }
	inline unsigned int				GetRows			(void) const { return myRows; }
	inline unsigned int				GetCols			(void) const { return myCols; }

	inline unsigned int				IncCurrentRow	(void) { assert(myCurrentRow<myRows); return ++myCurrentRow; }
	inline unsigned int				IncCurrentCol	(void) { assert(myCurrentCol<myCols); ++myCurrentCol; myCurrentCol%=myCols; return myCurrentCol; }
	inline unsigned int				DecCurrentRow	(void) { assert(myCurrentRow>=0); return --myCurrentRow; }
	inline unsigned int				DecCurrentCol	(void) { assert(myCurrentCol>=0); return --myCurrentCol; }

	unsigned int GetColumn(const std::string& aColName) const;

private:
	unsigned int myCurrentRow;
	unsigned int myCurrentCol;

	std::vector<std::string> myColumnNames;
	std::vector<std::string> myFlows;
	unsigned int myRows;
	unsigned int myCols;
};


/* class Node
 * a syntaxic tree of all tokens
 */
class Node
{
public:
	Node(void);
	Node(const NODE_TYPE& aNodeType, const Coord& aC, Node* aL, Node* aR);
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
	NumNode(const Coord& aC, const double& aVal);

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
	RefNode(const Coord& aC, Node* aN);
	virtual ~RefNode(void);
};

/* class ArgNode
 * specific to a function argument
 */
class ArgNode : public Node
{
public:
	ArgNode(const Coord& aC, Node* aN, Node* aArgN);

	inline double GetValue(void) const { return myValue; }
	inline void SetValue(const double& vVal) { myValue = vVal; }

private:
	double myValue;
};

/* class FuncNode
 * specific to a user defined function
 */
class FuncNode : public Node
{
public:
	FuncNode(const Coord& aC, Func* aF, Node* aArgN);

	inline Func* GetFunc(void) const { return myFunc; }

private:
	Func* myFunc;
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
	Node*			GetChildNode(const TableWalker& walker, const char* aRef, const int& aIdx);
	unsigned int	Hash		(const Coord& aC);
	void			Insert		(const Coord& aC, Node* aN);
	bool			CheckIndex	(const char* aIdx);

	/* Building */
	Node* BuildNode		(const TableWalker& walker, const NODE_TYPE& aNodeType, Node* aL = NULL, Node* aR = NULL);
	Node* BuildNum		(const TableWalker& walker, const double& aNum);
	Node* BuildRef		(const TableWalker& walker, const char* aRef, const int& aIdx);
	Node* BuildArg		(const TableWalker& walker, Node* aN, Node* aArgN);
	Node* BuildFunc		(const TableWalker& walker, const char* aRef,Node* aArgN);

	/* Evaluating */
	double Eval(Node* aN);

private:
	/* Map of all nodes */
	CoordNodeMap AllNodes;
};
