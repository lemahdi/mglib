#include "nodes.h"
#include "func.h"
#include <algorithm>


using namespace std;
using namespace MG;


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
#ifdef MEMORY_CONTROL_MODE
int MG_Node::ourCounter = 0;
#endif

MG_Node::MG_Node(void) : myNodeType(NODEF_NODE), myL(NULL), myR(NULL)
{
#ifdef MEMORY_CONTROL_MODE
	MG_Node::ourCounter++;
#endif
}

MG_Node::MG_Node(	const NODE_TYPE& aNodeType
				,	const Coord& aC
				,	MG_Node* aL
				,	MG_Node* aR)
	: myNodeType(aNodeType), myL(aL), myR(aR), myCoord(aC)
{
#ifdef MEMORY_CONTROL_MODE
	MG_Node::ourCounter++;
#endif
}

MG_Node::~MG_Node()
{
	if (myL) delete myL;
	if (myR) delete myR;
	
#ifdef MEMORY_CONTROL_MODE
	MG_Node::ourCounter--;
	cout << "Nodes remaining: " << MG_Node::ourCounter << endl;
	if (MG_Node::ourCounter == 0)
	{
		char c;
		cin >> c;
	}
#endif
}

MG_CmpNode::MG_CmpNode	(	const NODE_TYPE& aNodeType
						,	const Coord& aC
						,	const COMPARISON_OP& aCmpOp
						,	MG_Node* aL
						,	MG_Node* aR)
						:	MG_Node(aNodeType, aC, aL, aR)
						,	myOperator(aCmpOp)
{}

MG_NumNode::MG_NumNode(const Coord& aC, const double& aVal)
	: MG_Node(NUM_NODE, aC)
	, myValue(aVal)
{}

MG_DateNode::MG_DateNode(const Coord& aC, const long& aJD)
	: MG_Node(DATE_NODE, aC)
	, myDate(aJD)
{}

MG_RefNode::MG_RefNode(const Coord& aC, MG_Node* aN, const Coord& aCC, const NODE_DIRECTION& aD)
	: MG_Node(REF_NODE, aC, aN)
	, myChildCoord	(aCC)
	, myDirection	(aD)
{}

MG_RefNode::~MG_RefNode()
{
	// myL is already referenced in AllNodes
	myL = NULL;
}

void MG_RefNode::Refresh(MG_Node* aN)
{
	myL = aN;
}

MG_ArgNode::MG_ArgNode(const Coord& aC, MG_Node* aN, MG_Node* aArgN)
	: MG_Node(ARG_NODE, aC, aN, aArgN)
{}

MG_FuncNode::MG_FuncNode(const Coord& aC, MG_FuncPtr aF, MG_Node* aArgN)
	: MG_Node(FUNC_NODE, aC, aArgN)
	, myFunc(aF)
{}


/* Implementation
 ** class MG_NodeManager
 */

/* Destructor */
MG_NodeManager::~MG_NodeManager()
{
	CoordNodeMap::iterator itMap = AllNodes.begin();
	while(itMap != AllNodes.end())
	{
		delete itMap->second->second;
		delete itMap->second;
		itMap++;
	}
}

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
		if (vCN && vCN->first.first==aC.first && vCN->first.second==aC.second)
		{
			vCN->second = aN;
			return;
		}

		if (vHash >= NHASH)
			vCN = AllNodes[0];
	}

	cerr << __FILE__ << "-" << __LINE__ << ": ERROR, cannot insert node into parsing tree." << endl;
	return;
}

bool MG_NodeManager::CheckIndex(const char* aIdx)
{
	if (strcmp(aIdx, "i"))
	{
		cerr << __FILE__ << "-" << __LINE__ << ": ERROR, index to use should be i." << endl;
		return false;
	}

	return true;
}

long MG_NodeManager::ToJulianDay(const char* aDate)
{
	return MG_Date::ToJulianDay(aDate, '/', FR_DATE);
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

MG_Node* MG_NodeManager::BuildCmpNode	(	const MG_TableWalker& walker
										,	const NODE_TYPE& aNodeType
										,	const unsigned int& aCmpOp
										,	MG_Node* aL, MG_Node* aR)
{
	Coord vC(walker.GetCurrentRow(), walker.GetCurrentCol());
	MG_Node* vN = new MG_CmpNode(aNodeType, vC, (COMPARISON_OP)aCmpOp, aL, aR);
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

MG_Node* MG_NodeManager::BuildDate(const MG_TableWalker& walker, const long& aJD)
{
	Coord vC(walker.GetCurrentRow(), walker.GetCurrentCol());
	MG_Node* vN = new MG_DateNode(vC, aJD);
	Insert(vC, vN);
	return vN;
}

MG_Node* MG_NodeManager::BuildRef(const MG_TableWalker& walker, const char* aRef, const int& aIdx)
{
	Coord vC(walker.GetCurrentRow(), walker.GetCurrentCol());
	MG_Node* vN = GetChildNode(walker, aRef, aIdx);
	NODE_DIRECTION vD = (aIdx==0 ? NODIR_NODE : (aIdx<0 ? BACKWARD_NODE : FORWARD_NODE));
	Coord vCC(walker.GetCurrentRow() + aIdx, walker.GetColumn(string(aRef)));
	assert(vC != vCC);
	MG_Node* vNRef = new MG_RefNode(vC, vN, vCC, vD);
	Insert(vC, vNRef);
	RefNodes.push_back((MG_RefNode*)vNRef);
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
	MG_FuncPtr vF = MG_SFuncBuilder::Instance()->GetFunc(string(aFuncName));
	MG_Node* vFuncN = new MG_FuncNode(vC, vF, aArgN);
	Insert(vC, vFuncN);
	return vFuncN;
}

void
MG_NodeManager::PostProcess()
{
	MG_RefNode* vRefN = NULL;
	MG_Node* vN = NULL;

	Coord vParentC, vChildC;
	PairCoord vParentChild;
	vector< Coord > vParentV;
	map< Coord,bool > vCheck;
	MMCoord vPCMM;

	for(unsigned int i=0; i<RefNodes.size(); i++)
	{
		vRefN = RefNodes[i];
		vParentC = vRefN->GetCoord();
		vChildC = vRefN->GetChildCoord();
		if (vRefN->GetDirection()==FORWARD_NODE || vChildC.second>vParentC.second)
		{
			vN = MG_NodeManager::GetNode(vChildC);
			vRefN->Refresh(vN);
		}

		vParentChild = PairCoord(vParentC,vChildC);
		vParentV.push_back(vParentC);
		vCheck.insert(pair< Coord,bool >(vParentC,false));
		vPCMM.insert(vParentChild);
	}

	for(unsigned int i=0; i<vParentV.size(); i++)
	{
		map< Coord,bool > vCheckCopy(vCheck);
		CheckCircularReference(vPCMM, vParentV[i], vCheckCopy);
	}
}

void MG_NodeManager::CheckCircularReference(MMCoord& aPCMM, const Coord& vWalk, map< Coord,bool >& aCheck)
{
	assert(aCheck[vWalk] != true);
	aCheck[vWalk] = true;
	pair< MMCoord::iterator,MMCoord::iterator > vPMM = aPCMM.equal_range(vWalk);
	MMCoord::iterator itMMC;
	for(itMMC=vPMM.first; itMMC!=vPMM.second; ++itMMC)
		CheckCircularReference(aPCMM, itMMC->second, aCheck);
}

double
MG_NodeManager::Eval(MG_Node *aN)
{
	double vVal = 0;

	if (!aN) {
		cerr << __FILE__ << "-" << __LINE__ << ": ERROR, cannot evaluate null node." << endl;
		return 0;
	}

	switch(aN->GetNodeType()) {
		/* constant */
		case NUM_NODE: vVal = ((MG_NumNode *)aN)->GetValue(); break;
		case DATE_NODE: vVal = ((MG_DateNode *)aN)->GetDate().GetJulianDay(); break;

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

		/* comparison */
		case CMP_NODE:
		{
			switch(((MG_CmpNode*)aN)->GetOperator())
			{
			case GT_OP: vVal = Eval(aN->GetL()) > Eval(aN->GetR()); break;
			case LT_OP: vVal = Eval(aN->GetL()) < Eval(aN->GetR()); break;
			case NE_OP: vVal = Eval(aN->GetL()) != Eval(aN->GetR()); break;
			case EQ_OP: vVal = Eval(aN->GetL()) == Eval(aN->GetR()); break;
			case GE_OP: vVal = Eval(aN->GetL()) >= Eval(aN->GetR()); break;
			case LE_OP: vVal = Eval(aN->GetL()) <= Eval(aN->GetR()); break;

			default: cout << "internal error: bad node " << aN->GetNodeType() << endl;
			}
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

