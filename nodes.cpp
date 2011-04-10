#include "nodes.h"
#include <algorithm>


using namespace std;


/* class FileError */
#ifndef WIN32
template<>
#endif
FileError* SFileError::myInstance = NULL;

void FileError::Init()
{
	myFile.open(ERROR_FILE);
	streambuf *vCerr = myFile.rdbuf();
	cerr.rdbuf(vCerr);
}


/* Implentation:
 ** class Node
 ** class NumNode
 ** class RefNode
 */
Node::Node(void) : myNodeType(NODEF_NODE), myL(NULL), myR(NULL)
{}

Node::Node(const NODE_TYPE& aNodeType, const Coord& aC, NodePtr aL, NodePtr aR)
	: myNodeType(aNodeType), myL(aL), myR(aR), myCoord(aC)
{}

NumNode::NumNode(const Coord& aC, const double& aVal)
	: Node(NUM_NODE, aC, NodePtr(NULL), NodePtr(NULL))
	, myValue(aVal)
{}

RefNode::RefNode(const Coord& aC, NodePtr aN)
	: Node(REF_NODE, aC, aN, NodePtr(NULL))
{}

ArgNode::ArgNode(const Coord& aC, NodePtr aN, NodePtr aArgN)
	: Node(ARG_NODE, aC, aN, aArgN)
{}

FuncNode::FuncNode(const Coord& aC, FuncPtr aF, NodePtr aArgN)
	: Node(FUNC_NODE, aC, aArgN, NodePtr(NULL))
	, myFunc(aF)
{}


/* Implementation
 ** class NodeManager
 */

/* Accessing */
unsigned int NodeManager::Hash(const Coord& aC)
{
	unsigned int vHash = 0;

	vHash = 1;
	vHash = vHash*9 ^ aC.first;
	vHash = vHash*9 ^ aC.second;

	return vHash;
}

void NodeManager::Insert(const Coord& aC, NodePtr aN)
{
	unsigned int vHash = Hash(aC)%NHASH;
	CoordNodePtr vCN = AllNodes[vHash];

	int vNCount = NHASH;
	while (--vNCount >= 0) {
		if (vCN.IsNull())
		{
			vCN = CoordNodePtr(new CoordNode());
			AllNodes[vHash] = vCN;

			vCN->first	= aC;
			vCN->second	= aN;
			return;
		}
		if (!vCN->second.IsNull() && vCN->first.first==aC.first && vCN->first.second==aC.second)
		{
			vCN->second = aN;
			return;
		}

		if (vHash >= NHASH)
			vCN = AllNodes[0];
	}

	cerr << "ERROR: cannot insert node into parsing tree" << endl;
	return;
}

bool NodeManager::CheckIndex(const char* aIdx)
{
	if (strcmp(aIdx, "i"))
	{
		cerr << "ERROR: index to use should be i";
		return false;
	}

	return true;
}

NodePtr NodeManager::GetNode(const Coord& aC)
{
	NodePtr vN = NULL;
	unsigned int vHash = Hash(aC)%NHASH;

	CoordNodeMap::iterator itMap = AllNodes.find(vHash);
	if (itMap == AllNodes.end())
		Insert(aC, vN);
	else
		vN = AllNodes[vHash]->second;

	return vN;
}

NodePtr NodeManager::GetChildNode(const TableWalker& walker, const char* aRef, const int& aIdx)
{
	unsigned int vCIdx = walker.GetColumn(string(aRef));
	if (vCIdx != MAX_DESC_TABLE_COLUMNS)
	{
		int vRIdx = walker.GetCurrentRow() + aIdx;
		assert(vRIdx >= 0);
		Coord vC(walker.GetCurrentRow() + aIdx, vCIdx);
		NodePtr vN = GetNode(vC);
		return vN;
	}

	return NodePtr(NULL);
}

/* Building */
NodePtr NodeManager::BuildNode(const TableWalker& walker, const NODE_TYPE& aNodeType, NodePtr aL, NodePtr aR)
{
	Coord vC(walker.GetCurrentRow(), walker.GetCurrentCol());
	NodePtr vN = NodePtr(new Node(aNodeType, vC, aL, aR));
	Insert(vC, vN);
	return vN;
}

NodePtr NodeManager::BuildNum(const TableWalker& walker, const double& aNum)
{
	Coord vC(walker.GetCurrentRow(), walker.GetCurrentCol());
	NodePtr vN = NodePtr(new NumNode(vC, aNum));
	Insert(vC, vN);
	return vN;
}

NodePtr NodeManager::BuildRef(const TableWalker& walker, const char* aRef, const int& aIdx)
{
	Coord vC(walker.GetCurrentRow(), walker.GetCurrentCol());
	NodePtr vN = GetChildNode(walker, aRef, aIdx);
	NodePtr vNRef = NodePtr(new RefNode(vC, vN));
	Insert(vC, vNRef);
	return vNRef;
}

NodePtr NodeManager::BuildArg(const TableWalker &walker, NodePtr aN, NodePtr aArgN)
{
	Coord vC(walker.GetCurrentRow(), walker.GetCurrentCol());
	NodePtr vArgN = NodePtr(new ArgNode(vC, aN, aArgN));
	Insert(vC, vArgN);
	return vArgN;
}

NodePtr NodeManager::BuildFunc(const TableWalker& walker, const char* aFuncName, NodePtr aArgN)
{
	Coord vC(walker.GetCurrentRow(), walker.GetCurrentCol());
	FuncPtr vF = SFuncBuilder::Instance()->GetFunc(string(aFuncName));
	NodePtr vFuncN = NodePtr(new FuncNode(vC, vF, aArgN));
	Insert(vC, vFuncN);
	return vFuncN;
}

double
NodeManager::Eval(NodePtr aN)
{
	double vVal = 0;

	if (aN.IsNull()) {
		cerr << "ERROR" << endl;
		return 0;
	}

	switch(aN->GetNodeType()) {
		/* constant */
		case NUM_NODE: vVal = aN->GetValue(); break;

		/* name reference */
		case REF_NODE: vVal = Eval(aN->GetL()); break;

		/* function argument */
		case ARG_NODE:
			vVal = Eval(aN->GetL());
			aN->SetValue(vVal);
			if (!aN->GetR().IsNull()) Eval(aN->GetR());
			break;

		/* function */
		case FUNC_NODE:
			{
			NodePtr vArgN = aN->GetL();
			Eval(vArgN);
			vector<double> vArgVals;
			while (vArgN != NULL)
			{
				vArgVals.push_back(vArgN->GetValue());
				vArgN = vArgN->GetR();
			}
			vVal = aN->GetFunc()->Eval(vArgVals);
			break;
			}

		/* expressions */
		case ADD_NODE: vVal = Eval(aN->GetL()) + Eval(aN->GetR()); break;
		case SUB_NODE: vVal = Eval(aN->GetL()) - Eval(aN->GetR()); break;
		case MUL_NODE: vVal = Eval(aN->GetL()) * Eval(aN->GetR()); break;
		case DIV_NODE: vVal = Eval(aN->GetL()) / Eval(aN->GetR()); break;
		case NEG_NODE: vVal = -Eval(aN->GetR()); break;

		default: cout << "internal error: bad node " << aN->GetNodeType() << endl;
	}

	return vVal;
}


/* Implementation
 ** class TableWalker
 */
TableWalker::TableWalker()
	: myCurrentRow(0), myCurrentCol(0), myRows(0), myCols(0)
{}


TableWalker::TableWalker(	const vector<string>& aColNames
						,	const vector<string>& aFlows)
						:	myColumnNames	(aColNames)
						,	myFlows			(aFlows)
						,	myCols			(aColNames.size())
						,	myCurrentCol	(0)
						,	myCurrentRow	(0)
{
	myRows = aFlows.size() / myCols;
	assert(aFlows.size() % myCols == 0);
	assert(myCols < MAX_DESC_TABLE_COLUMNS);
}

unsigned int TableWalker::GetColumn(const string& aColName) const
{
	vector<string>::const_iterator it = find(myColumnNames.begin(), myColumnNames.end(), aColName);
	if (it == myColumnNames.end())
		return MAX_DESC_TABLE_COLUMNS;

	unsigned int index = (unsigned int)(it - myColumnNames.begin());
	return index;
}

