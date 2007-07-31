
#include "c3.h"
#include "globals.h"
#include "Bset.h"
#include "AiMain.h"

#include "ArmyAgent.h"
#include "CityAgent.h"

#include "OA_Loot.h"

#include "AiMap.h"
#include "IC3BlgDB.h"
#include "IC3Player.h"


#include "Order.h"







Opportunity_Action_Loot::Opportunity_Action_Loot ()
{
}







Opportunity_Action_Loot::~Opportunity_Action_Loot ()
{
}







void Opportunity_Action_Loot::Serialize (CivArchive &archive)
{
    return; 
}









void Opportunity_Action_Loot::Execute(AiMain *ai)
{ 
	BSetID id; 
    ArmyAgent *current_agent=NULL;
    CityAgent *ca;
	uint32 num_buildings;
	uint32 type;

	
	if (ai->m_my_player_id != PLAYER_INDEX_VANDALS)
		return;

	num_buildings = ai->m_buildingDB->GetNumBuildings(); 

	
	for (ca = ai->m_city_set->First(id); 
		 ai->m_city_set->Last(); 
		 ca = ai->m_city_set->Next(id))
		{ 
			Assert(ca);
			sint32 city_id = ca->GetID().GetVal();
			
			
			
			

			
			

			
			for (type = 0; type< num_buildings; type++)
				ai->m_player->SellBuilding(city_id, type);
		}
}
