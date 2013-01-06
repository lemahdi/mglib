/*
 * Copyright			: 2010 by MG
 * Version				: 0.1.11
 * Date					: 12 DEC 2010
 * Purpose				: MG_FileError is a singleton class for managing error displays
 *						  MG_Node is a class for each information in the table
 *						  MG_NodeManager is a class for building each node
 *						  MG_TableWalker is a class for keeping the coordinates in the description table, like a compass
 * Author				: MM Akkouh
 * Notes				: 
 */


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

#include "nova/patterns/singleton.hpp"
#include "genpricer/infra/func.h"
#include "nova/glob/date.h"


MG_NAMESPACE_BEGIN


/* class MG_FileError
 * keeping a trace of Lexer and Paser errors
 */
class MG_FileError
{
	template<class T> friend class Singleton;

private:
	MG_FileError(void);

public:
	virtual ~MG_FileError(void);
	void Init(void);

private:
	std::ofstream myFile;
	std::streambuf* mySavedCerr;
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

	SWAP_DECL(MG_TableWalker)

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
	MG_Node	(	const NODE_TYPE& aNodeType
			,	const Coord& aC
			,	MG_Node* aL = NULL
			,	MG_Node* aR = NULL);
	virtual ~MG_Node(void);

	inline NODE_TYPE		GetNodeType	(void) const { return myNodeType; }
	inline Coord			GetCoord	(void) const { return myCoord; }
	inline MG_Node*			GetL		(void) const { return myL; }
	inline MG_Node*			GetR		(void) const { return myR; }

protected:
	NODE_TYPE		myNodeType;
	Coord			myCoord;

	MG_Node*		myL;
	MG_Node*		myR;

#ifdef MEMORY_CONTROL_MODE
public:
	static int ourCounter;
#endif
};

/* class MG_CmpNode
 * specific to comparison
 */
class MG_CmpNode : public MG_Node
{
public:
	MG_CmpNode	(	const NODE_TYPE& aNodeType
				,	const Coord& aC
				,	const COMPARISON_OP& aCmpOp
				,	MG_Node* aL = NULL
				,	MG_Node* aR = NULL);

	inline COMPARISON_OP GetOperator(void) const { return myOperator; }

protected:
	COMPARISON_OP myOperator;
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

/* class MG_DateNode
 * specific to a double cell
 */
class MG_DateNode : public MG_Node
{
public:
	MG_DateNode(const Coord& aC, const long& aJD);

	inline MG_Date GetDate(void) const { return myDate; }

protected:
	MG_Date myDate;
};

/* class MG_RefNode
 * specific to a reference cell
 */
class MG_RefNode : public MG_Node
{
public:
	MG_RefNode	(	const Coord& aC
				,	MG_Node* aN
				,	const Coord& aCC
				,	const NODE_DIRECTION& aD = NODIR_NODE);
	virtual ~MG_RefNode(void);

	inline Coord			GetChildCoord	(void) const { return myChildCoord; }
	inline NODE_DIRECTION	GetDirection	(void) const { return myDirection; }

	void Refresh(MG_Node* vN);

private:
	Coord			myChildCoord;
	NODE_DIRECTION	myDirection;
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
	MG_FuncNode(const Coord& aC, MG_FuncPtr aF, MG_Node* aArgN);

	inline MG_FuncPtr Func(void) const { return myFunc; }

private:
	MG_FuncPtr myFunc;
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
	// self-generated copy-constructor is enough
	MG_NodeManager(void) {}
	virtual ~MG_NodeManager(void);

public:
	/* Accessing */
	MG_Node*			GetNode		(const Coord& aC);
	MG_Node*			GetChildNode(const MG_TableWalker& walker, const char* aRef, const int& aIdx);
	unsigned int		Hash		(const Coord& aC);
	void				Insert		(const Coord& aC, MG_Node* aN);
	bool				CheckIndex	(const char* aIdx);
	long				ToJulianDay	(const char* aDate);

	/* Building */
	MG_Node* BuildNode		(const MG_TableWalker& walker, const NODE_TYPE& aNodeType, MG_Node* aL = NULL, MG_Node* aR = NULL);
	MG_Node* BuildCmpNode	(	const MG_TableWalker& walker
							,	const NODE_TYPE& aNodeType
							,	const unsigned int& aCmpOp
							,	MG_Node* aL = NULL, MG_Node* aR = NULL);
	MG_Node* BuildNum		(const MG_TableWalker& walker, const double& aNum);
	MG_Node* BuildDate		(const MG_TableWalker& walker, const long& aJD);
	MG_Node* BuildRef		(const MG_TableWalker& walker, const char* aRef, const int& aIdx);
	MG_Node* BuildArg		(const MG_TableWalker& walker, MG_Node* aN, MG_Node* aArgN);
	MG_Node* BuildFunc		(const MG_TableWalker& walker, const char* aRef, MG_Node* aArgN);

	/* Rebuilding References */
	void PostProcess(void);

	/* Propagating Model */
	void ModelProcess(const MG_PricingModelPtr& aMdl);

private:
	void CheckCircularReference(MMCoord& aPCMM, const Coord& vWalk, std::map< Coord,bool >& aCheck);

public:
	/* Evaluating */
	MG_Arg Eval(MG_Node* aN);
	MG_Arg Eval(MG_Node* aN, const std::vector<double>& aStates);

private:
	/* Map of all nodes */
	CoordNodeMap				AllNodes;
	/* Vector of ref nodes */
	std::vector<MG_RefNode*>	RefNodes;
	/* Vector of func nodes */
	std::vector<MG_FuncNode*>	FuncNodes;
};


MG_NAMESPACE_END
