#include "nova/mginit.h"
#include "nova/market/calendar.h"
#include "genpricer/infra/func.h"


using namespace MG;


void MG_Initializator::Init()
{
	MG_Calendar::LoadCalendars();
}

