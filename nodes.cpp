#include "nodes.h"
#include "func.h"
#include <algorithm>


using namespace std;


/* class MG_FileError */
#ifndef WIN32
template<>
#endif
MG_FileError* MG_SFileError::myInstance = NULL;

void MG_FileError::Init()
{
	myFile.open(ERROR_FILE);
	streambuf *vCerr = myFile.rdbuf();
	cerr.rdbuf(vCerr);
}


/* Implentation:
 ** class MG_Node
 ** class MG_NumNode
 ** class MG_RefNode
 */
MG_Node::MG_Node(void) : myNodeType(NODEF_NODE), myL(NULL), myR(NULL)
{}

MG_Node::MG_Node(const NODE_TYPE& aNodeType, const Coord& aC, MG_Node* aL, MG_Node* aR)
	: myNodeType(aNodeType), myL(aL), myR(aR), myCoord(aC)
{}

MG_Node::~MG_Node()
{
	if (myL) delete myL;
	if (myR) delete myR;
}

MG_NumNode::MG_NumNode(const Coord& aC, const double& aVal)
	: MG_Node(NUM_NODE, aC, NULL, NULL)
	, myValue(aVal)
{}

MG_RefNode::MG_RefNode(const Coord& aC, MG_Node* aN)
	: MG_Node(REF_NODE, aC, aN, NULL)
{}

MG_RefNode::~MG_RefNode()
{
	// myL is already referenced in AllNodes
	myL = NULL;
}

MG_ArgNode::MG_ArgNode(const Coord& aC, MG_Node* aN, MG_Node* aArgN)
	: MG_Node(ARG_NODE, aC, aN, aArgN)
{}

MG_FuncNode::MG_FuncNode(const Coord& aC, MG_Func* aF, MG_Node* aArgN)
	: MG_Node(FUNC_NODE, aC, aArgN, NULL)
	, myFunc(aF)
{}


/* Implementation
 ** class MG_NodeManager
 */

/* Accessing */
unsigned int MG_NodeManager::Hash(const Coord& aC)
{
	unsigned int vHash = 0;

	vHash = 1;
	vHash = vHash*9 ^ aC.first;
	vHash = vHash*9 ^ aC.second;

	return vHash;
}

void MG_NodeManager::Insert(const Coord& aC, MG_Node* aN)
{
	unsigned int vHash = Hash(aC)%NHASH;
	CoordNode* vCN = AllNodes[vHash];

	int vNCount = NHASH;
	while (--vNCount >= 0) {
		if (!vCN)
		{
			vCN = new CoordNode();
			AllNodes[vHash] = vCN;

			vCN->first	= aC;
			vCN->second	= aN;
			return;
		}
		if (vCN->second && vCN->first.first==aC.first && vCN->first.second==aC.second)
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

bool MG_NodeManager::CheckIndex(const char* aIdx)
{
	if (strcmp(aIdx, "i"))
	{
		cerr << "ERROR: index to use should be i";
		return false;
	}

	return true;
}

MG_Node* MG_NodeManager::GetNode(const Coord& aC)
{
	MG_Node* vN = NULL;
	unsigned int vHash = Hash(aC)%NHASH;

	CoordNodeMap::iterator itMap = AllNodes.find(vHash);
	if (itMap == AllNodes.end())
		Insert(aC, vN);
	else
		vN = AllNodes[vHash]->second;

	return vN;
}

MG_Node* MG_NodeManager::GetChildNode(const MG_TableWalker& walker, const char* aRef, const int& aIdx)
{
	unsigned int vCIdx = walker.GetColumn(string(aRef));
	if (vCIdx != MAX_DESC_TABLE_COLUMNS)
	{
		int vRIdx = walker.GetCurrentRow() + aIdx;
		assert(vRIdx >= 0);
		Coord vC(walker.GetCurrentRow() + aIdx, vCIdx);
		MG_Node* vN = GetNode(vC);
		return vN;
	}

	return NULL;
}

/* Building */
MG_Node* MG_NodeManager::BuildNode(const MG_TableWalker& walker, const NODE_TYPE& aNodeType, MG_Node* aL, MG_Node* aR)
{
	Coord vC(walker.GetCurrentRow(), walker.GetCurrentCol());
	MG_Node* vN = new MG_Node(aNodeType, vC, aL, aR);
	Insert(vC, vN);
	return vN;
}

MG_Node* MG_NodeManager::BuildNum(const MG_TableWalker& walker, const double& aNum)
{
	Coord vC(walker.GetCurrentRow(), walker.GetCurrentCol());
	MG_Node* vN = new MG_NumNode(vC, aNum);
	Insert(vC, vN);
	return vN;
}

MG_Node* MG_NodeManager::BuildRef(const MG_TableWalker& walker, const char* aRef, const int& aIdx)
{
	Coord vC(walker.GetCurrentRow(), walker.GetCurrentCol());
	MG_Node* vN = GetChildNode(walker, aRef, aIdx);
	MG_Node* vNRef = new MG_RefNode(vC, vN);
	Insert(vC, vNRef);
	return vNRef;
}

MG_Node* MG_NodeManager::BuildArg(const MG_TableWalker &walker, MG_Node *aN, MG_Node *aArgN)
{
	Coord vC(walker.GetCurrentRow(), walker.GetCurrentCol());
	MG_Node* vArgN = new MG_ArgNode(vC, aN, aArgN);
	Insert(vC, vArgN);
	return vArgN;
}

MG_Node* MG_NodeManager::BuildFunc(const MG_TableWalker& walker, const char* aFuncName, MG_Node* aArgN)
{
	Coord vC(walker.GetCurrentRow(), walker.GetCurrentCol());
	MG_Func* vF = MG_SFuncBuilder::Instance()->GetFunc(string(aFuncName));
	MG_Node* vFuncN = new MG_FuncNode(vC, vF, aArgN);
	Insert(vC, vFuncN);
	return vFuncN;
}

double
MG_NodeManager::Eval(MG_Node *aN)
{
	double vVal = 0;

	if (!aN) {
		cerr << "ERROR" << endl;
		return 0;
	}

	switch(aN->GetNodeType()) {
		/* constant */
		case NUM_NODE: vVal = ((MG_NumNode *)aN)->GetValue(); break;

		/* name reference */
		case REF_NODE: vVal = Eval(aN->GetL()); break;

		/* function argument */
		case ARG_NODE:
			vVal = Eval(aN->GetL());
			((MG_ArgNode*)aN)->SetValue(vVal);
			if (aN->GetR()) Eval(aN->GetR());
			break;

		/* function */
		case FUNC_NODE:
			{
			MG_ArgNode* vArgN = (MG_ArgNode*)(((MG_FuncNode *)aN)->GetL());
			Eval(vArgN);
			vector<double> vArgVals;
			while (vArgN != NULL)
			{
				vArgVals.push_back(vArgN->GetValue());
				vArgN = (MG_ArgNode*)(vArgN->GetR());
			}
			vVal = ((MG_FuncNode *)aN)->GetFunc()->Eval(vArgVals);
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
 ** class MG_TableWalker
 */
MG_TableWalker::MG_TableWalker()
	: myCurrentRow(0), myCurrentCol(0), myRows(0), myCols(0)
{}


MG_TableWalker::MG_TableWalker	(	const vector<string>& aColNames
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

unsigned int MG_TableWalker::GetColumn(const string& aColName) const
{
	vector<string>::const_iterator it = find(myColumnNames.begin(), myColumnNames.end(), aColName);
	if (it == myColumnNames.end())
		return MAX_DESC_TABLE_COLUMNS;

	unsigned int index = (unsigned int)(it - myColumnNames.begin());
	return index;
}

