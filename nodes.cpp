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

Node::Node(const NODE_TYPE& aNodeType, const Coord& aCoord, Node* aL, Node* aR)
	: myNodeType(aNodeType), myL(aL), myR(aR), myCoord(aCoord)
{}

Node::~Node(void)
{
	if (myL) delete myL;
	if (myR) delete myR;
}

NumNode::NumNode(const Coord& aCoord, const double& aValue)
	: Node(NUM_NODE, aCoord, NULL, NULL)
	, myValue(aValue)
{}

RefNode::RefNode(const Coord& aCoord, Node* aL)
	: Node(REF_NODE, aCoord, aL, NULL)
{}

RefNode::~RefNode(void)
{
	// myL is already referenced in AllNodes
	myL = NULL;
}


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

void NodeManager::Insert(const Coord& aC, Node* aN)
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

	cerr << "ERROR" << endl;
	return;
}

Node* NodeManager::GetNode(const Coord& aC)
{
	unsigned int vHash = Hash(aC)%NHASH;
	Node* vN = AllNodes[vHash]->second;
	return vN;
}

Node* NodeManager::GetChildNode(const TableWalker& walker, const char* ref)
{
	unsigned int index = walker.GetColumn(string(ref));
	if (index != MAX_DESC_TABLE_COLUMNS)
	{
		Coord vC(walker.GetCurrentRow(), index);
		Node* vN = GetNode(vC);
		return vN;
	}

	return NULL;
}

/* Building */
Node* NodeManager::BuildNode(const TableWalker& walker, const NODE_TYPE& aNodeType, Node* aL, Node* aR)
{
	Coord vC(walker.GetCurrentRow(), walker.GetCurrentCol());
	Node* vN = new Node(aNodeType, vC, aL, aR);
	Insert(vC, vN);
	return vN;
}

Node* NodeManager::BuildNum(const TableWalker& walker, const double& aNum)
{
	Coord vC(walker.GetCurrentRow(), walker.GetCurrentCol());
	Node* vN = new NumNode(vC, aNum);
	Insert(vC, vN);
	return vN;
}

Node* NodeManager::BuildRef(const TableWalker& walker, Node* aN)
{
	Coord vC(walker.GetCurrentRow(), walker.GetCurrentCol());
	Node* vNRef = new RefNode(vC, aN);
	Insert(vC, vNRef);
	return vNRef;
}


double
NodeManager::Eval(Node *aN)
{
	double vVal;

	if (!aN) {
		cerr << "ERROR" << endl;
		return 0;
	}

	switch(aN->GetNodeType()) {
		/* constant */
		case NUM_NODE: vVal = ((NumNode *)aN)->GetValue(); break;

		/* name reference */
		case REF_NODE: vVal = Eval(aN->GetL()); break;

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
{
	myCurrentRow = 0;
	myCurrentCol = 0;
}

unsigned int TableWalker::GetColumn(const string& aColName) const
{
	vector<string>::const_iterator it = find(myColumnNames.begin(), myColumnNames.end(), aColName);
	if (it == myColumnNames.end())
		return MAX_DESC_TABLE_COLUMNS;

	unsigned int index = it - myColumnNames.begin();
	return index;
}

