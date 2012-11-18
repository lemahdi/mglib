#include "mggenpricer/gensec/gensecurity.h"
#pragma warning(push)
#pragma warning(disable:4512)
#include "mggenpricer/infra/my_parser.tab.hpp"
#pragma warning(pop)


using namespace std;
using namespace MG;


/* Base Model class */
/*MG_GenSecurity::MG_GenSecurity() : MG_XLObject()
{}*/

void MG_GenSecurity::Swap(MG_GenSecurity& aRight)
{
	MG_XLObject::Swap(aRight);
	myHeader.swap(aRight.myHeader);
	myDealDesc.swap(aRight.myDealDesc);
	swap(myRows, aRight.myRows);
	swap(myCols, aRight.myCols);
	myWalker.Swap(aRight.myWalker);
}

MG_GenSecurity::MG_GenSecurity	(	const vector<string>& aDealDesc
								,	const size_t		& aCols)
								:	MG_XLObject()
								,	myHeader	(aDealDesc.begin(), aDealDesc.begin()+aCols)
								,	myDealDesc	(aDealDesc.begin()+aCols, aDealDesc.end())
								,	myCols		(aCols)
								,	myWalker	(myHeader, myDealDesc)
{
	myXLName = MG_GENSEC_XL_NAME;
	myRows = (aDealDesc.size()/myCols) - 1;

	yy::my_parser vParser(myWalker, myManager);	// make a cpp calc parser
	for(unsigned int i=0; i<myDealDesc.size(); i++)
	{
		const char* vText = myDealDesc[i].c_str();
		YY_BUFFER_STATE vBP = yy_scan_string(vText);
		vParser.parse();		// and run it
		yy_flush_buffer(vBP);
		yy_delete_buffer(vBP);
		if (myWalker.GetCurrentCol() == myWalker.GetCols()-1)
			myWalker.IncCurrentRow();
		myWalker.IncCurrentCol();
	}
	myManager.PostProcess();
}

MG_GenSecurity::~MG_GenSecurity()
{}

void MG_GenSecurity::PropagateModel(const MG_PricingModelPtr& aMdl)
{
	myManager.ModelProcess(aMdl);
}

