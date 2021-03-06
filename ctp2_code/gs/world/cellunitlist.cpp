//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C++ source
// Description  : Handling of a list of Units in the same cell
// Id           : $Id$
//
//----------------------------------------------------------------------------
//
// Disclaimer
//
// THIS FILE IS NOT GENERATED OR SUPPORTED BY ACTIVISION.
//
// This material has been developed at apolyton.net by the Apolyton CtP2 
// Source Code Project. Contact the authors at ctp2source@apolyton.net.
//
//----------------------------------------------------------------------------
//
// Compiler flags
// 
// USE_STOP_ZERO_MOVEMENT
// - When defined, prevents unit without movement points from moving.
//
//----------------------------------------------------------------------------
//
// Modifications from the original Activision code:
//
// - Ships no longer get an underwater tunnel movement bonus, based on
//   suggestions by NelsonAndBronte.
// - Make unit types with 0 movement stand still (compiler option).
// - Handled crashes with invalid units.
// - Initialized local variables. (Sep 9th 2005 Martin G�hmann)
// - Standartized army strength computation. (30-Apr-2008 Martin G�hmann)
// - Position strength can now be calculated independently from position. (13-Aug-2008 Martin G�hmann)
// - Added LowestMoveBonusUnit to return movement required if cellunitlist contains 
//	 only movebonus units, otherwise regular move cost check is used. (15-Mar-2009 Maq)
// - Changed occurances of UnitRecord::GetMaxHP to
//   UnitData::CalculateTotalHP. (Aug 3rd 2009 Maq)
//
//----------------------------------------------------------------------------

#include "c3.h"
#include "cellunitlist.h"

#include "World.h"
#include "player.h"
#include "UnitRec.h"
#include "DB.h"
#include "WonderRecord.h"
#include "SelItem.h"
#include "Army.h"
#include "network.h"
#include "SlicEngine.h"
#include "Cell.h"
#include "UnitDynArr.h"
#include "UnitData.h"
#include "UnitRecord.h"
#include "MoveFlags.h"
#include "wonderutil.h"
#include "GameEventManager.h"
#include "TerrainRecord.h"	    // TerrainRecord
#include "RandGen.h"            // g_rand

// Visibility cheat flags
extern sint32 g_god;
extern sint32 g_fog_toggle;

bool CellUnitList::Insert(Unit id)
{
	Assert(m_nElements < k_MAX_ARMY_SIZE);
	if(m_nElements < k_MAX_ARMY_SIZE) {
		m_array[m_nElements].m_id = id.m_id;
		m_nElements++;
		UpdateMoveIntersection();
		return true;
	}
	return false;
}

bool CellUnitList::CanEnter(const MapPoint &point) const
{	//emod?
	//TODO - add impassable terrain
	//TODO - add impassable tileimp
	/// @todo Check next if: ghost army?
	if (m_nElements < 1)
	{
		return true;
	}

	Assert(point.IsValid());

	if(!point.IsValid()) return false;

	Cell * cell = g_theWorld->GetCell(point);
	if (!cell) return false;

	if (cell->GetCity().IsValid() &&
	   cell->GetCity().GetOwner() == m_array[0].GetOwner()) {
		return true;
	}
	
	uint32 env = cell->GetEnv();

	if(m_moveIntersection) {
		if((env & m_moveIntersection))
			return true;

		if((m_moveIntersection & k_BIT_MOVEMENT_TYPE_AIR) && 
		   !(env & k_BIT_MOVEMENT_TYPE_SPACE)) {
			return true;
		}

		if((m_moveIntersection & k_BIT_MOVEMENT_TYPE_LAND) &&
		   (env & k_MASK_ENV_ROAD)) {
			return true;
		}

		if((m_moveIntersection & k_BIT_MOVEMENT_TYPE_SHALLOW_WATER) &&
		   !(m_moveIntersection & k_BIT_MOVEMENT_TYPE_WATER) &&
		   (env & k_BIT_MOVEMENT_TYPE_WATER) &&
		   wonderutil_GetAllBoatsDeepWater(g_player[m_array[0].GetOwner()]->m_builtWonders)) {
			return true;
		}

		return false;
	} else {
		
		for(sint32 i = 0; i < m_nElements; i++) {
			uint32 moveType = m_array[i].GetMovementType();
			
			if((env & (moveType << k_SHIFT_ENV_MOVEMENT_TYPE)) ||
			   ((moveType & k_BIT_MOVEMENT_TYPE_AIR)) ||
			   ((moveType & k_BIT_MOVEMENT_TYPE_LAND) && (env & k_MASK_ENV_ROAD))) {
				// wonderutil_GetAllBoatsDeepWater(g_player[GetOwner()]->m_builtWonders)) { //add allunits beach or land wonder option?
				
				continue;
			} else {
				if((moveType & k_BIT_MOVEMENT_TYPE_SHALLOW_WATER) &&
				   !(moveType & k_BIT_MOVEMENT_TYPE_WATER) &&
				   (env & k_BIT_MOVEMENT_TYPE_WATER) &&
				   wonderutil_GetAllBoatsDeepWater(g_player[GetOwner()]->m_builtWonders)) {
					continue;
				}
				return false;
			}
		}
		return true;
	}
}

//----------------------------------------------------------------------------
//
// Name       : CellUnitList::GetMovementTypeLand
//
// Description: Determines whether all units can move on land.
//
// Parameters : -
//
// Globals	  : -
//
// Returns    : bool			: all units can move on land
//
// Remark(s)  : -
//
//----------------------------------------------------------------------------
bool CellUnitList::GetMovementTypeLand() const 
{ 
    for (int i = 0; i < m_nElements; ++i) 
	{ 
        if (!m_array[i].GetMovementTypeAir()) 
		{
            return false; 
        }
    }
    
    return true;
}

bool CellUnitList::HasWormholeProbe() const
{
	for (sint32 i = 0; i < m_nElements; i++) {
		if(m_array[i].GetDBRec()->GetWormholeProbe()) {
			return true;
		}
	}
	return false;
}

PLAYER_INDEX CellUnitList::GetOwner() const
{
    Assert (m_nElements > 0); 
    return (m_nElements > 0) ? m_array[0].GetOwner() : PLAYER_UNASSIGNED;
}

void CellUnitList::GetPos(MapPoint &pos) const
{
    Assert(m_nElements > 0); 
    if (m_nElements > 0) 
    {
        m_array[0].GetPos(pos); 
    }
    else
    {
        pos.x = -1;
        pos.y = -1;
    }
}

bool CellUnitList::IsAtLeastOneMoveLand() const
{
    for (sint32 i = 0; i < m_nElements; i++) 
    { 
        if (m_array[i].GetMovementTypeLand())
        {
            return true; 
        }
    }
    return false; 
}

bool CellUnitList::IsAtLeastOneMoveWater() const
{
    for (sint32 i = 0; i < m_nElements; i++) 
    { 
        if (m_array[i].GetMovementTypeSea())
        {
            return true; 
        }
    }
    return false;
}


bool CellUnitList::IsAtLeastOneMoveShallowWater() const
{
    for (sint32 i = 0; i < m_nElements; i++) 
    {
		if(m_array[i].GetMovementTypeShallowWater()) {
			return true;
		}
    }
    return false;
}

bool CellUnitList::IsAtLeastOneMoveAir() const
{
    for (sint32 i = 0; i < m_nElements; i++) { 
        if (m_array[i].GetMovementTypeAir()) {
            return true; 
        }
    }
    
    return false;
}

bool CellUnitList::IsAtLeastOneMoveSpace() const
{
    for (sint32 i = 0; i < m_nElements; i++) { 
        if (m_array[i].GetMovementTypeSpace()) {
            return true; 
        }
    }
    
    return false;
}

bool CellUnitList::IsAtLeastOneMoveMountain() const
{
    for (sint32 i = 0; i < m_nElements; i++) { 
        if (m_array[i].GetMovementTypeMountain()) {
            return true; 
        }
    }
    
    return false;
}

bool CellUnitList::IsEnemy(PLAYER_INDEX owner) const
{
	sint32 myOwner = GetOwner();
	if(g_network.IsActive() && g_network.TeamsEnabled() &&
	   g_player[myOwner] && g_player[owner] &&
	   g_player[myOwner]->m_networkGroup == g_player[owner]->m_networkGroup) {
		return false;
	}

	uint32 a = ~(g_player[m_array[0].GetOwner()]->GetMaskAlliance()); 
	uint32 b = (0x00000001 << owner); 

	return (a & b) != 0;
}

bool CellUnitList::IsEnemy( Unit defender) const
{
	return player_isEnemy(GetOwner(), defender.GetOwner());
}

bool CellUnitList::IsEnemy(CellUnitList &defender) const
{
	return (defender.Num() > 0) && IsEnemy(defender[0]);
}

bool CellUnitList::CanAtLeastOneCaptureCity() const
{
	for (sint32 i = 0; i < m_nElements; i++)
	{
		if(!m_array[i].GetDBRec()->GetCantCaptureCity())
		{
			return true;
		}
	}

	return false;
}

bool CellUnitList::IsVisible(PLAYER_INDEX player) const
{
	for (sint32 i = 0; i < m_nElements; i++)
	{
		if(m_array[i].GetVisibility() & (1 << player))
			return true;
	}

	return false;
}


bool CellUnitList::CanBeExpelled()
{
	for (sint32 i = 0; i < m_nElements; i++)
	{
		if(!m_array[i].CanBeExpelled())
			return false;
	}

	return true;
}


//----------------------------------------------------------------------------
//
// Name       : CellUnitList::GetTopVisibleUnitOfMoveType
//
// Description: Determine which unit of a stack to display.
//
// Parameters : looker				: the observing player
//              moveType			: a bit set of movement types 
//				isResyncReported	: invalid units do not have to be reported
//
// Globals    : g_selected_item		: currently selected item on screen
//				g_player			: players
//				g_theWorld			: map information
//				g_network			: network handler (for multiplayer)
//				g_god				: when set, everything is visible
//				g_fog_toggle		: when set, everything is visible
//
// Returns    : bool				: some unit is visible
//				maxi				: index of the unit to display
//				isResyncReported	: updated when a (new) missing unit has
//                                    triggered a resync request
//
// Remark(s)  : - When returning false, the value of maxi has not been updated.
//              - Units in a city are only visible when awake or currently
//                selected.
//              - When there are multiple units in a stack, the top unit is 
//				  determined on either movement points (own unit) or 
//                "visibility class" + strength (enemy unit).
//
//----------------------------------------------------------------------------
bool CellUnitList::GetTopVisibleUnitOfMoveType
(
	PLAYER_INDEX const	looker,
	uint32 const		moveTypes,
	sint32 &			maxi,
	bool &				isResyncReported
) const
{ 
    bool	is_found_unit	= false; 
    double	maxStrength		= -100000.0;
    uint32	min_vis			= 0xffffffff; 
    double	minmove			= -1;

	Army	selectedArmy;
	g_selected_item->GetSelectedArmy(selectedArmy);

	for (sint32 i = 0; i < m_nElements; ++i)
	{
		Unit const &	u	= m_array[i];

		if (u.IsValid())
		{
			if (u.IsSameMovementType(moveTypes)	&&	// move types match
				// cell visible
				((u.GetVisibility() & (0x01 << looker))						||
				 (g_player[looker] && g_player[looker]->m_hasGlobalRadar)	||
				 g_god || g_fog_toggle
			    )								&&
			    // selected, awake, or out in the open
			    ((u.GetArmy().m_id == selectedArmy.m_id)					||
			     !(u.IsAsleep() || u.IsEntrenched() || u.IsEntrenching())	||
				 !g_theWorld->HasCity(u.RetPos())
			    )
			   )
			{
				if (u.GetOwner() == looker) 
				{
					// Looking at own units: order on movement points
					if (u.GetMovementPoints() > minmove) 
					{
						maxi			= i;
						minmove			= u.GetMovementPoints();
						is_found_unit	= true;
					}
				} 
				else 
				{
					// Looking at "enemy" units: order on visibility class, 
					// with strength as tie-breaker.
					uint32 const	vis = u.GetVisibilityClass();
					
					if (vis <= min_vis) 
					{ 
						double const	strength	= 
							u.GetAttack() + u.GetDefense();
					
						if (strength > maxStrength) 
						{
							maxStrength		= strength; 
							maxi			= i; 
							min_vis			= vis; 
							is_found_unit	= true; 
						} 
					}
				}
			}
		}
		else if (!isResyncReported)
		{
			g_network.RequestResync(RESYNC_INVALID_UNIT);
			isResyncReported = true;
		}
    }  
	
	return is_found_unit;
}










Unit CellUnitList::GetTopVisibleUnit(PLAYER_INDEX const looker) const
{
    uint32	move_union			= 0x0000;
	bool	stopResyncReport	= !g_network.IsClient();

    for (sint32 i = 0; i < m_nElements; ++i) 
	{ 
		if (m_array[i].IsValid())
		{
			move_union |= m_array[i].GetMovementType(); 
		}
		else 
		{
			// Would like to do DelIndex(i), but this is a const method.

			if (!stopResyncReport) 
			{
				g_network.RequestResync(RESYNC_INVALID_UNIT);
				stopResyncReport = false;
			}
		}
    } 
    
    sint32	maxi; 

    if (move_union & k_Unit_MovementType_Space_Bit)
	{
        if (GetTopVisibleUnitOfMoveType
				(looker, k_Unit_MovementType_Space_Bit, maxi, stopResyncReport)
		   ) 
		{
            return m_array[maxi]; 
        }
    }        
    if (move_union & k_Unit_MovementType_Air_Bit) 
	{
        if (GetTopVisibleUnitOfMoveType
				(looker, k_Unit_MovementType_Air_Bit, maxi, stopResyncReport)
		   ) 
		{
            return m_array[maxi]; 
        }
    }
    if (move_union & k_Unit_MovementType_Sea_Bit) 
	{
        if (GetTopVisibleUnitOfMoveType
				(looker, k_Unit_MovementType_Sea_Bit, maxi, stopResyncReport)
		   ) 
		{
            return m_array[maxi]; 
        }
    }
    if (move_union & k_Unit_MovementType_ShallowWater_Bit) 
	{
        if (GetTopVisibleUnitOfMoveType
				(looker, k_Unit_MovementType_ShallowWater_Bit, maxi, stopResyncReport)
		   ) 
		{
            return m_array[maxi]; 
        }
    }
    if (move_union & k_Unit_MovementType_Land_Bit) 
	{
        if (GetTopVisibleUnitOfMoveType
				(looker, k_Unit_MovementType_Land_Bit, maxi, stopResyncReport)
		   ) 
		{
            return m_array[maxi]; 
        }
    }                        
    if (move_union & k_Unit_MovementType_Mountain_Bit) 
	{
        if (GetTopVisibleUnitOfMoveType
				(looker,k_Unit_MovementType_Mountain_Bit, maxi, stopResyncReport)
		   ) 
		{
            return m_array[maxi]; 
        }
    }                            
    if (move_union & k_Unit_MovementType_Trade_Bit) 
	{
        if (GetTopVisibleUnitOfMoveType
				(looker, k_Unit_MovementType_Trade_Bit, maxi, stopResyncReport)
		   )
		{
            return m_array[maxi]; 
        }
    }                                
    
    return Unit(); 
}

bool CellUnitList::CanBeSued() const
{
	for (sint32 i = 0; i < m_nElements; i++) {
		if(m_array[i].GetDBRec()->GetCanBeSued())
			return true;
	}
	return false;
}

void CellUnitList::ForceVisibleThisTurn(const PLAYER_INDEX to_me)
{
    for (sint32 i = 0; i < m_nElements; i++) {
        m_array[i].ForceVisibleThisTurn(to_me); 
    }
}

double CellUnitList::GetHPModifier() const
{
	return (m_nElements > 0) ? m_array[0].GetHPModifier() : 0.0;
}

void CellUnitList::DoVictoryEnslavement(sint32 origOwner)
{
	double success = 0;

	for (sint32 i = 0; i < m_nElements; i++) {
		if(m_array[i].GetHP() > 0 &&
			m_array[i].GetDBRec()->GetVictoryEnslavement()) {
#ifdef WIN32	
			if(!m_array[i].GetDBRec()->GetVictoryEnslavementChance(success))
#endif
				success = 1.0;

			if(g_rand->Next(100) < sint32(success * 100.0)) {
				MapPoint slpos;
				GetPos(slpos);
							
				Unit hc;
				sint32 r = g_player[m_array[0].GetOwner()]->
					GetSlaveCity(slpos, hc);
				
				Assert(r && hc.IsValid());
				if (!hc.IsValid())
					break;

				if(g_network.IsHost()) {
					g_network.Block(hc.GetOwner());
				}
				
				g_gevManager->AddEvent(GEV_INSERT_Tail, GEV_MakePop,
									   GEA_City, hc.m_id,
									   GEA_Player, origOwner,
									   GEA_End);

				
				g_slicEngine->RunVictoryEnslavementTriggers(m_array[i],
															origOwner, hc);
				if(g_network.IsHost()) {
					g_network.Unblock(hc.GetOwner());
				}
				break;
			}
		}
	}
}

bool CellUnitList::ExertsZOC() const
{
	for(sint32 i = 0; i < m_nElements; i++)
	{
		if(!m_array[i].IsNoZoc())
			return true;
	}
	return false;
}

bool CellUnitList::CanSee(const Army & army) const
{
	for(sint32 i = 0; i < m_nElements; i++)
	{
		if(m_array[i].CanSee(army))
			return true;
	}
	return false;
}

uint32 CellUnitList::GetVisionClass() const
{
	uint32 visionClass = 0;

	for(sint32 i = 0; i < m_nElements; i++)
	{
		visionClass |= m_array[i]->GetDBRec()->GetVisionClass();
	}

	return visionClass;
}

uint32 CellUnitList::GetCanSee() const
{
	uint32 canSee = 0;

	for(sint32 i = 0; i < m_nElements; i++)
	{
		canSee |= m_array[i]->GetDBRec()->GetCanSee();
	}

	return canSee;
}

bool CellUnitList::CanMoveIntoCell(const MapPoint &pos, 
								   bool &zocViolation,
								   bool ignoreZoc,
								   bool &alliedCity)
{
	Cell *         cell = g_theWorld->GetCell(pos);
	CellUnitList * ul   = cell ? cell->UnitArmy() : NULL;

	zocViolation = false;
	alliedCity = false;

	if (ul && (ul->Num() + m_nElements) > k_MAX_ARMY_SIZE)
		return false;

	MapPoint myPos;
	GetPos(myPos);

	if (!CanEnter(pos)) {
		return false;
	}

	sint32 myOwner = GetOwner();
	if ((!(m_flags & k_CULF_IGNORES_ZOC)) && !ignoreZoc) {
		if (g_theWorld->IsMoveZOC(myOwner, myPos, pos, false)) {
			zocViolation = true;
			return false;
		}
	}

    if (cell->HasCity()) 
    {
        sint32 cityOwner = cell->GetCity().GetOwner();
        if (myOwner != cityOwner) 
        {
			if(m_flags & k_CULF_CANT_CAPTURE_CITY)
				return false;
			
			if(myOwner == PLAYER_INDEX_VANDALS && wonderutil_GetProtectFromBarbarians(g_player[cityOwner]->m_builtWonders)) {
				return false;
			}

			if (!IsEnemy(cityOwner) ||
			   g_player[myOwner]->WillViolateCeaseFire(cityOwner) ||
			   g_player[myOwner]->WillViolatePact(cityOwner)) {
				alliedCity = true;
				return false;
			}
		}
	}

    return true;
}

bool CellUnitList::IsMovePointsEnough(const double cost) const
{
	for (sint32 i = 0; i < m_nElements; ++i)
	{
		double const mp	= m_array[i].GetMovementPoints();

#if defined(USE_STOP_ZERO_MOVEMENT)
		if (mp == 0.0)
            {
                return false;
            }
#endif

		if ((mp < cost) && !m_array[i].GetFirstMoveThisTurn())
		{
			return false;
		}
		// else: unit has enough movement points, or has not moved this turn
    }

    return true; 
}

bool CellUnitList::IsMovePointsEnough(const MapPoint &pos) const
{

    double cost; 
    if (GetMovementTypeAir()) { 
        cost = k_MOVE_AIR_COST; 
	// Prevent ships from diving under and using tunnels.
	} else if (g_theWorld->IsTunnel(pos) && !GetMovementTypeLand()) {
		sint32 icost;
		(void) g_theWorld->GetTerrain(pos)->GetEnvBase()->GetMovement(icost);
		cost = icost;
    } else { 
        cost = g_theWorld->GetMoveCost(pos); 
    }
	sint32 value;
	if (HighestMoveBonusUnit(value)) { cost = value; }

    return IsMovePointsEnough(cost); 
}
 
bool CellUnitList::HighestMoveBonusUnit(sint32 & value) const 
{ 
	sint32 highvalue = 0;// highest movebonus unit bonus
	sint32 tmp;
    for (sint32 i = 0; i < m_nElements; i++)
	{ 
		UnitRecord const * rec = m_array[i].GetDBRec();
        if (!(rec->GetMoveBonus(tmp))) {
            return false; 
        } else if (rec->GetMoveBonus(tmp)) {
			if (tmp > highvalue) {
				highvalue = tmp;
			}
		}
    }
    value = highvalue;
    return true;
}

bool CellUnitList::GetMovementTypeAir() const 
{ 
    for (sint32 i = 0; i < m_nElements; i++) { 
        if (!m_array[i].GetMovementTypeAir()) {
            return false; 
        }
    }
    
    return true;
}

bool CellUnitList::CanBeCargoPodded() const
{
	sint32 i;
	sint32 cargoPodType = -1;
	sint32 gov = g_player[GetOwner()]->GetGovernmentType();
	for(i = 0; i < g_theUnitDB->NumRecords(); i++) {
		if(g_theUnitDB->Get(i, gov)->GetCargoPod()) {
			cargoPodType = i;
			break;
		}
	}
	Assert(i < g_theUnitDB->NumRecords());
	if(i >= g_theUnitDB->NumRecords())
		return false;
	
	const UnitRecord *cargoRec = g_theUnitDB->Get(cargoPodType, gov);
	extern bool UnitCanCarry(sint32 src, sint32 dest, sint32 government);
	if(!cargoRec->GetCargoDataPtr())
		return false;

	for(i = 0; i < m_nElements; i++) {
		if(cargoRec->GetCargoDataPtr()->GetMaxCargo() < m_nElements ||
		   !UnitCanCarry(cargoPodType, m_array[i].GetType(), gov)) {
			return false;
		}
	}
	return true;
}

bool CellUnitList::CanSpaceLand() const
{
    for (sint32 i = 0; i < m_nElements; i++) 
    { 
	  UnitRecord const * rec = m_array[i].GetDBRec();
        if (!(rec && rec->GetSpaceLand())) 
        {
            return false; 
        }
    }
    
    return true;
}

bool CellUnitList::CanSpaceLaunch() const
{
    for (sint32 i = 0; i < m_nElements; i++) {
	  UnitRecord const * rec = m_array[i].GetDBRec();
        if (!(rec && rec->HasSpaceLaunch())) 
        {
            return false;
        }
    }
    
    return true;
}


bool CellUnitList::IsIgnoresZOC() const
{ 
    for (sint32 i = 0; i < m_nElements; i++) { 
        if (!m_array[i].IsIgnoresZOC())
            return false; 
    } 
    return true; 
}


void CellUnitList::Serialize(CivArchive &archive)
{
	if(archive.IsStoring()) {
		archive.StoreChunk((uint8*)&m_array[0], (uint8*)&m_nElements + sizeof(m_nElements));
	} else {
		archive.LoadChunk((uint8*)&m_array[0], (uint8*)&m_nElements + sizeof(m_nElements));
	}
}

CellUnitList::CellUnitList(const DynamicArray<Unit> &copyme)
{
	Assert(copyme.Num() < k_MAX_ARMY_SIZE);
	if(copyme.Num() < k_MAX_ARMY_SIZE) {
		memcpy(m_array, copyme.m_array, sizeof(Unit) * copyme.Num());
		m_nElements = copyme.Num();
		UpdateMoveIntersection();
	}
}

sint32 CellUnitList::Del(const Unit &id)
{
	for(sint32 i = 0; i < m_nElements; i++) {
		if(m_array[i].m_id == id.m_id) {
			memmove(&m_array[i], &m_array[i+1], (m_nElements - i - 1) * sizeof(Unit));
			m_nElements--;
			UpdateMoveIntersection();
			return 1;
		}
	}
	return 0;
}

sint32 CellUnitList::DelIndex(const sint32 index)
{
	Assert(index >= 0);
	Assert(index < m_nElements);
	if(index >= 0 && index < m_nElements) {
		memmove(&m_array[index], &m_array[index+1], (m_nElements - index - 1) * sizeof(Unit));
		m_nElements--;
		UpdateMoveIntersection();
		return 1;
	}
	return 0;
}

void CellUnitList::KillList(CAUSE_REMOVE_ARMY cause, PLAYER_INDEX killedBy)
{
	sint32 i;
	
	Unit tmpArray[k_MAX_ARMY_SIZE];
	for(i = m_nElements - 1; i >= 0; i--) {
		tmpArray[i].m_id = m_array[i].m_id;
	}
	sint32 n = m_nElements;
	for(i = n - 1; i >= 0; i--) {
		tmpArray[i].Kill(cause, killedBy);
	}
}

bool CellUnitList::IsPresent(const Unit &u)
{
	for (sint32 i = 0; i < m_nElements; i++) {
		if(m_array[i].m_id == u.m_id)
			return true;
	}
	return false;
}

void CellUnitList::UpdateMoveIntersection()
{
	m_moveIntersection = 0xffffffff;
	m_flags = k_CULF_IGNORES_ZOC | 
		k_CULF_CANT_CAPTURE_CITY | 
		k_CULF_CAN_SPACE_LAUNCH | 
		k_CULF_CAN_SPACE_LAND;

	for (sint32 i = 0; i < m_nElements; i++) {
		
		if (!m_array[i].IsValid()) {
			continue;
		}

		uint32 oldMoveBits = 0;
		uint32 newMoveBits = m_array[i].GetMovementType();

		if(newMoveBits & k_Unit_MovementType_Land_Bit)
			oldMoveBits |= k_BIT_MOVEMENT_TYPE_LAND;	   
		if(newMoveBits & k_Unit_MovementType_Sea_Bit)
			oldMoveBits |= k_BIT_MOVEMENT_TYPE_WATER;
		if(newMoveBits & k_Unit_MovementType_Air_Bit)
			oldMoveBits |= k_BIT_MOVEMENT_TYPE_AIR;
		if(newMoveBits & k_Unit_MovementType_Mountain_Bit)
			oldMoveBits |= k_BIT_MOVEMENT_TYPE_MOUNTAIN;
		if(newMoveBits & k_Unit_MovementType_Trade_Bit)
			oldMoveBits |= k_BIT_MOVEMENT_TYPE_TRADE;
		if(newMoveBits & k_Unit_MovementType_ShallowWater_Bit)
			oldMoveBits |= k_BIT_MOVEMENT_TYPE_SHALLOW_WATER;
		if(newMoveBits & k_Unit_MovementType_Space_Bit)
			oldMoveBits |= k_BIT_MOVEMENT_TYPE_SPACE;

		m_moveIntersection &= oldMoveBits;
		if(!m_array[i].IsIgnoresZOC()) {
			m_flags &= ~(k_CULF_IGNORES_ZOC);
		}
		if(!m_array[i].IsCantCaptureCity()) {
			m_flags &= ~(k_CULF_CANT_CAPTURE_CITY);
		}
	}
}

//----------------------------------------------------------------------------
//
// Name       : CellUnitList::ComputeStrength
//
// Description: Get data for army and fill in given parameters.
//
// Parameters : sint32 & attack            : sum of units Attack * hitpoints * fire_power
//              sint32 & defense           : sum of units Defense * hitpoints * fire_power
//              sint32 & ranged            : sum of units ZBRangeAttack * hitpoints * fire_power
//              sint32 & defend_unit_count : how many defenders (Defense>0)
//              sint32 & ranged_unit_count : how many ranged (ZBRangeAttack>0)
//              sint32 & ranged            : sum of units Attack * hitpoints * fire_power if unit can bombard land or mountain
//              sint32 & ranged            : sum of units Attack * hitpoints * fire_power if unit can bombard sea
//              sint32 & ranged            : sum of units Attack * hitpoints * fire_power if unit can bombard air
//              sint32 & total_value       : sum of units ShieldCost
//              bool     noCargo           : Does not count strength from cargo
//
// Globals    : -
//
// Returns    : -
//
// Remark(s)  : includes units in transports if noCargo is false
//
//----------------------------------------------------------------------------
void CellUnitList::ComputeStrength(float & attack,
                                   float & defense,
                                   float & ranged,
                                   sint8 & defend_unit_count,
                                   sint8 & ranged_unit_count,
                                   float & land_bombard,
                                   float & water_bombard,
                                   float & air_bombard,
                                   float & total_value,
                                   const bool terrainIndependent,
                                   bool noCargo) const
{
	attack            = 0.0f;
	defense           = 0.0f;
	ranged            = 0.0f;
	defend_unit_count = 0;
	ranged_unit_count = 0;
	
	land_bombard      = 0.0f;
	water_bombard     = 0.0f;
	air_bombard       = 0.0f;
	total_value       = 0.0f;
	
	for(sint32 i = 0; i < m_nElements; i++)
	{
		if (!m_array[i].IsValid())
			continue;

		UnitRecord const * rec = m_array[i]->GetDBRec();
		if (!rec) continue;

		if (rec->GetDefense() > 0.0)
			defend_unit_count++;

		if (rec->GetZBRangeAttack() > 0)
			ranged_unit_count++;

		float firepower = static_cast<float>(rec->GetFirepower());
		float hitpoints = static_cast<float>(m_array[i]->CalculateTotalHP());//rec->GetMaxHP());

		attack   += static_cast<float>(  rec->GetAttack()
		                               * hitpoints
		                               * firepower);

		if(terrainIndependent)
		{
			defense  += static_cast<float>(  rec->GetDefense()
			                               * hitpoints
			                               * firepower);
		}
		else
		{
			defense  += static_cast<float>(  m_array[i]->GetPositionDefense(Unit()) // Expensive
			                               * hitpoints
			                               * firepower);
		}

		ranged   += static_cast<float>(  rec->GetZBRangeAttack()
		                               * hitpoints
		                               * firepower);

		if(rec->GetCanBombardLand()
		|| rec->GetCanBombardMountain()
		){
			land_bombard += static_cast<float>(  rec->GetAttack()
			                                   * hitpoints
			                                   * firepower);
		}

		if(rec->GetCanBombardWater())
		{
			water_bombard += static_cast<float>(  rec->GetAttack()
			                                    * hitpoints
			                                    * firepower);
		}

		if(rec->GetCanBombardAir())
		{
			air_bombard += static_cast<float>(  rec->GetAttack()
			                                  * hitpoints
			                                  * firepower);
		}

		total_value += static_cast<float>(rec->GetShieldCost());

		if(noCargo)
			continue;

		UnitDynamicArray * cargo_list = m_array[i]->GetCargoList();
		sint32             cargoCount = cargo_list ? cargo_list->Num() : 0;

		for(sint32 j = 0; j < cargoCount; j++)
		{
			rec = cargo_list->Access(j)->GetDBRec();
			if (!rec) continue;

			firepower = static_cast<float>(rec->GetFirepower());
			hitpoints = static_cast<float>(cargo_list->Access(j)->CalculateTotalHP());//rec->GetMaxHP());

			if (rec->GetDefense() > 0.0)
				defend_unit_count++;

			if (rec->GetZBRangeAttack() > 0)
				ranged_unit_count++;

			attack   += static_cast<float>(  rec->GetAttack()
			                               * hitpoints
			                               * firepower);
			defense  += static_cast<float>(  rec->GetDefense()
			                               * hitpoints
			                               * firepower);
			ranged   += static_cast<float>(  rec->GetZBRangeAttack()
			                               * hitpoints
			                               * firepower);

			if(rec->GetCanBombardLand()
			|| rec->GetCanBombardMountain()
			){
				land_bombard  += static_cast<float>(  rec->GetAttack()
				                                    * hitpoints
				                                    * firepower);
			}

			if(rec->GetCanBombardWater())
			{
				water_bombard  += static_cast<float>(  rec->GetAttack()
				                                     * hitpoints
				                                     * firepower);
			}

			if(rec->GetCanBombardAir())
			{
				air_bombard    += static_cast<float>(  rec->GetAttack()
				                                     * hitpoints
				                                     * firepower);
			}

			total_value += static_cast<float>(rec->GetShieldCost());
		}
	}
}

//----------------------------------------------------------------------------
//
// Name       : CellUnitList::ComputeStrength
//
// Description: Get data for army and fill in given parameters.
//
// Parameters : sint32 & attack            : sum of units Attack * hitpoints * fire_power
//              sint32 & defense           : sum of units Defense * hitpoints * fire_power
//              sint32 & ranged            : sum of units ZBRangeAttack * hitpoints * fire_power
//              sint32 & defend_unit_count : how many defenders (Defense>0)
//              sint32 & ranged_unit_count : how many ranged (ZBRangeAttack>0)
//              sint32 & ranged            : sum of units Attack * hitpoints * fire_power if unit can bombard land or mountain
//              sint32 & ranged            : sum of units Attack * hitpoints * fire_power if unit can bombard sea
//              sint32 & ranged            : sum of units Attack * hitpoints * fire_power if unit can bombard air
//              sint32 & total_value       : sum of units ShieldCost
//
// Globals    : -
//
// Returns    : -
//
// Remark(s)  : includes units in transports if noCargo is false
//
//----------------------------------------------------------------------------
void CellUnitList::ComputeCargoStrength(float & attack,
                                        float & defense,
                                        float & ranged,
                                        sint8 & defend_unit_count,
                                        sint8 & ranged_unit_count,
                                        float & land_bombard,
                                        float & water_bombard,
                                        float & air_bombard,
                                        float & total_value,
                                        const bool terrainIndependent) const
{
	attack            = 0.0f;
	defense           = 0.0f;
	ranged            = 0.0f;
	defend_unit_count = 0;
	ranged_unit_count = 0;
	
	land_bombard      = 0.0f;
	water_bombard     = 0.0f;
	air_bombard       = 0.0f;
	total_value       = 0.0f;
	
	for(sint32 i = 0; i < m_nElements; i++)
	{
		if (!m_array[i].IsValid())
			continue;

		UnitDynamicArray * cargo_list = m_array[i]->GetCargoList();
		sint32             cargoCount = cargo_list ? cargo_list->Num() : 0;

		for(sint32 j = 0; j < cargoCount; j++)
		{
			const UnitRecord* rec = cargo_list->Access(j)->GetDBRec();
			if (!rec) continue;

			float firepower = static_cast<float>(rec->GetFirepower());
			float hitpoints = static_cast<float>(cargo_list->Access(j)->CalculateTotalHP());//rec->GetMaxHP());

			if (rec->GetDefense() > 0.0)
				defend_unit_count++;

			if (rec->GetZBRangeAttack() > 0)
				ranged_unit_count++;

			attack   += static_cast<float>(  rec->GetAttack()
			                               * hitpoints
			                               * firepower);
			defense  += static_cast<float>(  rec->GetDefense()
			                               * hitpoints
			                               * firepower);
			ranged   += static_cast<float>(  rec->GetZBRangeAttack()
			                               * hitpoints
			                               * firepower);

			if(rec->GetCanBombardLand()
			|| rec->GetCanBombardMountain()
			){
				land_bombard  += static_cast<float>(  rec->GetAttack()
				                                    * hitpoints
				                                    * firepower);
			}

			if(rec->GetCanBombardWater())
			{
				water_bombard  += static_cast<float>(  rec->GetAttack()
				                                     * hitpoints
				                                     * firepower);
			}

			if(rec->GetCanBombardAir())
			{
				air_bombard    += static_cast<float>(  rec->GetAttack()
				                                     * hitpoints
				                                     * firepower);
			}

			total_value += static_cast<float>(rec->GetShieldCost());
		}
	}
}

double CellUnitList::GetAverageHealthPercentage() const
{
    double totalRatio = 0.0;

    for (sint32 i = 0; i < m_nElements; i++) 
    {
        if (m_array[i].IsValid() && m_array[i].GetDBRec()) 
        {
            double maxHp = m_array[i]->CalculateTotalHP();//.GetDBRec()->GetMaxHP();
            totalRatio += 
                (maxHp) ? (m_array[i].GetHP() / maxHp) : m_array[i].GetHP();
        }
    }

    return (m_nElements) ? (totalRatio / m_nElements) : 0.0;
}

//----------------------------------------------------------------------------
//
// Name       : CellUnitList::IsCivilian
//
// Description: Returns true if all units are civilians 
//
// Parameters : - 
//
// Globals    : -
//
// Returns    : bool
//
// Remark(s)  : -
//
//---------------------------------------------------------------------------- 
bool CellUnitList::IsCivilian() const
{
	for(sint32 i = 0; i < m_nElements; i++) 
	{
		if (!m_array[i].GetDBRec()->GetCivilian())
			return false;
	}
	return true;
}

//----------------------------------------------------------------------------
//
// Name       : CellUnitList::CanBombardTargetType
//
// Description: Returns true if some member of this army can bombard some member
//              of the CellUnitList units.
//
// Parameters : CellUnitList & units  : the list of units on the defending cell
//
// Globals    : -
//
// Returns    : bool
//
// Remark(s)  : -
//
//----------------------------------------------------------------------------
bool CellUnitList::CanBombardTargetType(const CellUnitList & units) const
{
	for(sint32 i = m_nElements - 1; i>= 0; i--)
	{
		for(sint32 j = 0; j < units.Num(); j++)
		{
			if (m_array[i]->CanBombardType(units[j]))
				return true;
		}
	}

	return false;
}
