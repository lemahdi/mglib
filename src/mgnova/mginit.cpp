#include "mgnova/mginit.h"
#include "mgnova/calendar.h"


using namespace MG;


void MG_Initializator::Init()
{
	MG_Calendar::LoadCalendars();
}

