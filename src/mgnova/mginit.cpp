#include "mgnova/mginit.h"
#include "mgnova/market/calendar.h"
#include "mggenpricer/infra/func.h"


using namespace MG;


void MG_Initializator::Init()
{
	MG_Calendar::LoadCalendars();
	MG_SFuncBuilder::Instance()->Init();
}

