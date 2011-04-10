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


/* class MG_FileError
 * keeping a trace of Lexer and Paser errors
 */
class MG_FileError
{
	template<class T> friend class Singleton;

private:
	MG_FileError(void) {}

public:
	void Init(void);

private:
	std::ofstream myFile;
};
typedef Singleton<MG_FileError> MG_SFileError;


/* Table Walker class
 * Simple class providing current cell coordinates
 * and table column names
 */
class MG_TableWalker
{
public:
	MG_TableWalker(void);
	MG_TableWalker(const std::vector<std::string>& aColNames, const std::vector<std::string>& aFlows);
	virtual ~MG_TableWalker(void) {}

public:
	/* accessors */
	inline unsigned int				GetCurrentRow	(void) const { return myCurrentRow; }
	inline unsigned int				GetCurrentCol	(void) const { return myCurrentCol; }
	inline std::vector<std::string>	GetColumnNames	(void) const { return myColumnNames; }
	inline size_t					GetRows			(void) const { return myRows; }
	inline size_t					GetCols			(void) const { return myCols; }

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
	size_t myRows;
	size_t myCols;
};


/* class MG_Node
 * a syntaxic tree of all tokens
 */
class MG_Node
{
public:
	MG_Node(void);
	MG_Node(const NODE_TYPE& aNodeType, const Coord& aC, MG_Node* aL, MG_Node* aR);
	virtual ~MG_Node(void);

	inline NODE_TYPE	GetNodeType	(void) const { return myNodeType; }
	inline MG_Node*		GetL		(void) const { return myL; }
	inline MG_Node*		GetR		(void) const { return myR; }

protected:
	NODE_TYPE		myNodeType;
	Coord			myCoord;

	MG_Node*		myL;
	MG_Node*		myR;
};

/* class MG_NumNode
 * specific to a double cell
 */
class MG_NumNode : public MG_Node
{
public:
	MG_NumNode(const Coord& aC, const double& aVal);

	inline double GetValue(void) const { return myValue; }

protected:
	double myValue;
};

/* class MG_RefNode
 * specific to a reference cell
 */
class MG_RefNode : public MG_Node
{
public:
	MG_RefNode(const Coord& aC, MG_Node* aN);
	virtual ~MG_RefNode(void);
};

/* class MG_ArgNode
 * specific to a function argument
 */
class MG_ArgNode : public MG_Node
{
public:
	MG_ArgNode(const Coord& aC, MG_Node* aN, MG_Node* aArgN);

	inline double	GetValue(void) const { return myValue; }
	inline void		SetValue(const double& vVal) { myValue = vVal; }

private:
	double myValue;
};

/* class MG_FuncNode
 * specific to a user defined function
 */
class MG_FuncNode : public MG_Node
{
public:
	MG_FuncNode(const Coord& aC, MG_Func* aF, MG_Node* aArgN);

	inline MG_Func* GetFunc(void) const { return myFunc; }

private:
	MG_Func* myFunc;
};


/* A class for managing nodes
 ** Initializing
 ** Accessing
 ** Building
 ** Evaluating
 */
class MG_NodeManager
{
public:
	MG_NodeManager(void) {}
	virtual ~MG_NodeManager(void) {}

	/* Accessing */
public:
	MG_Node*			GetNode		(const Coord& aC);
	MG_Node*			GetChildNode(const MG_TableWalker& walker, const char* aRef, const int& aIdx);
	unsigned int		Hash		(const Coord& aC);
	void				Insert		(const Coord& aC, MG_Node* aN);
	bool				CheckIndex	(const char* aIdx);

	/* Building */
	MG_Node* BuildNode		(const MG_TableWalker& walker, const NODE_TYPE& aNodeType, MG_Node* aL = NULL, MG_Node* aR = NULL);
	MG_Node* BuildNum		(const MG_TableWalker& walker, const double& aNum);
	MG_Node* BuildRef		(const MG_TableWalker& walker, const char* aRef, const int& aIdx);
	MG_Node* BuildArg		(const MG_TableWalker& walker, MG_Node* aN, MG_Node* aArgN);
	MG_Node* BuildFunc		(const MG_TableWalker& walker, const char* aRef,MG_Node* aArgN);

	/* Evaluating */
	double Eval(MG_Node* aN);

private:
	/* Map of all nodes */
	CoordNodeMap AllNodes;
};
