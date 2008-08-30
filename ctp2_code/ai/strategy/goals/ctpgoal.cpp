//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C++ source
// Description  : CTP2 specific Goal handling
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
// _DEBUG_SCHEDULER
// _DEBUG
// - Generate debug version
//
// USE_LOGGING
// - Enable logging when set, even when not a debug version. This is not
//   original Activision code.
//
//----------------------------------------------------------------------------
//
// Modifications from the original Activision code:
//
// - Resolved ambiguous sqrt calls.
// - Added the Target Position (X,Y) to The ArmyText (the text that is
//   shown on the screen in Debug mode, with the command 'armytext') - calvitix
//   update : it now show if the Army is grouping or going to board...
// - Added an attribute for the goal subtask (used by armytext display)
// - Changed the task forces for goals (based on objectif's threat)
// - Added a Debug Log (activated with k_dbg_scheduler_all) to see the goal priority computing
//   (raw priority, plus value of each modifier)
// - Added an Ungroup condition that can be associated to goals (as it exists RallyFirst) - Calvitix
// - Added consitions for Treaspassing units : (to favorise their Retreat) - Calvitix
// - Added conditions for wounded units : IsWoundedbonus (see goals.txt) - Calvitix
// - Added conditions for territory owner and IsVisible (see goals.txt) - Calvitix
// - Correct the determination of Empire Center and foreign Empire center - Calvitix
// - Invalid goals if in territory with nontrespassing treaty - Calvitix
// - Allow incursion permission to stealth units
// - Rollback the determination of check_dest (problem with special units that couldn't find
//   their path to goal anymore
// - Added other conditions to determine the RETREAT goals (and set max of 2 units per city
//   for that goals, hardcoded (to not depend on threat)
// - Forbid to army with settlers to perform ATTACK or SEIGE goals
// - Allow Units that are grouping to both move (if they are far enough)
// - Corrected condition to give unowned bonus in Compute_Raw_Priority, now it is
//   given if the according territory has owner -1 instead of 0
//   - Feb. 21st 2005 Martin G�hmann
// - Removed unnecessary reinitialization in Compute_Needed_Troop_Flow for trade
//   routes and tileimps. - Feb. 21st 2005 Martin G�hmann
// - Started to reimplement the refuel order. - May 7th 2005 Martin G�hmann
// - Removed .NET warnings - May 7th 2005 Martin G�hmann
// - Initialized local variables. (Sep 9th 2005 Martin G�hmann)
// - Added checks and Asserts to try to prevent crashes.
// - Improved AI debug goal report. (Jan 2nd 2007 Martin G�hmann)
// - Matching value computation now uses the distance measurement from CTPAgent. (25-Jan-2008 Martin G�hmann)
// - Added goody hut goal bonuses, so that there is a bonus if a goody hut is
//   in vision range of the agent and whether there could Barbarians pop up from
//   the goody hut and the goody hut opeing army can defend against such Barbarians. (25-Jan-2008 Martin G�hmann)
// - Fixed Goal subtask handling. (26-Jan-2008 Martin G�hmann)
// - Improved transporter cargo loading. (30-Jan-2008 Martin G�hmann)
// - The AI does not use settlers for attack anymore. (3-Feb-2008 Martin G�hmann)
// - The AI now uses the closest transport to the unit transported, even if
//   there is only one army to be transported. (3-Feb-2008 Martin G�hmann)
// - Use more than one transporter if the goal needs more than one. (8-Feb-2008 Martin G�hmann)
// - Standartized army strength computation. (30-Apr-2008 Martin G�hmann)
// - AI force matches are now based on attack, defense, ranged, land bombard,
//   sea bombard, and air bombard. (30-Apr-2008 Martin G�hmann)
// - USE_LOGGING now works in a final version. (30-Jun-2008 Martin G�hmann)
// - Redesigned AI, so that the matching algorithm is now a greedy algorithm. (13-Aug-2008 Martin G�hmann)
// - Now the goals are used for the matching process, the goal match value
//   is the avarage match value of the matches needed for the goal.
// - Strength calcualtion for offensive goals is now based on the average of
//   grid values and target position vales. For Barbarian targets it is
//   only the position strength. (13-Aug-2008 Martin G�hmann)
// - Slavers now go to cities with enough population. (13-Aug-2008 Martin G�hmann)
// - Rally is now complete if there is only one stack left with less than
//   twelve units. (13-Aug-2008 Martin G�hmann)
// - Added new rally algorithm. (13-Aug-2008 Martin G�hmann)
//
//----------------------------------------------------------------------------

#include "c3.h"
#include "ctpgoal.h"

#include "Squad.h"
#include "gfx_options.h"
#include "GoalRecord.h"
#include "StrategyRecord.h"
#include "OrderRecord.h"
#include "Cell.h"
#include "World.h"
#include "player.h"             // g_player, PLAYER_UNASSIGNED
#include "Order.h"
#include "ArmyData.h"
#include "UnitData.h"
#include "UnitPool.h"
#include "ArmyPool.h"
#include "MoveFlags.h"
#include "Diplomat.h"
#include "ctpagent.h"
#include "mapanalysis.h"
#include "settlemap.h"
#include "UnitRecord.h"
#include "AgreementMatrix.h"
#include "ctpaidebug.h"
#include "CityInfluenceIterator.h"
#include "ctpai.h"
#include "terrainutil.h"
#include "WonderRecord.h"
#include "advanceutil.h"
#include "Barbarians.h"
#include "wonderutil.h"

extern MapPoint                  g_mp_size;
extern UnitPool                 *g_theUnitPool;
extern ArmyPool                 *g_theArmyPool;

CTPGoal::CTPGoal()
:
    Goal            (),
    m_target_pos    (),
    m_target_city   (),
    m_target_army   (),
    m_sub_task      (SUB_TASK_GOAL)
{ ; }

CTPGoal::~CTPGoal()
{ ; }

bool CTPGoal::operator == (const CTPGoal & rval) const
{
	return (m_playerId    == rval.m_playerId)       &&
	       (m_goal_type   == rval.m_goal_type)      &&
	       (m_target_army == rval.m_target_army)    &&
	       (m_target_city == rval.m_target_city)    &&
	       (m_target_pos  == rval.m_target_pos);
}

// TODO: find out what this operator is used for. The relation between this 
// definition and the one in Goal is completely unclear.
//
// A: Probably none. However objects of type Goal are just instantiated
//    by objects of CTPGoal as base objects.
bool CTPGoal::operator < (const CTPGoal & rval) const
{
	Assert(false);
	return (m_playerId     < rval.m_playerId)       &&
	       (m_goal_type    < rval.m_goal_type)      &&
	       (m_target_army  < rval.m_target_army)    &&
	       (m_target_city  < rval.m_target_city)    &&
	       (m_target_pos.x < rval.m_target_pos.x)   &&
	       (m_target_pos.y < rval.m_target_pos.y);
}

void CTPGoal::Set_Target_Pos(const MapPoint & pos)
{
	m_target_pos = pos;
}

void CTPGoal::Set_Target_Army(const Army & army)
{
	m_target_army = army;
}

void CTPGoal::Set_Target_City(const Unit & city)
{
	m_target_city = city;
}

const MapPoint CTPGoal::Get_Target_Pos(const Army & army) const
{
	const GoalRecord *rec               = g_theGoalDB->Get(m_goal_type);
	sint32            max_squared_dist  = (g_theWorld->GetWidth() * g_theWorld->GetHeight());
	                  max_squared_dist *= max_squared_dist;
	sint32            best_squared_dist = max_squared_dist;
	sint32            tmp_squared_dist;
	MapPoint          best_target_pos;
	MapPoint          army_pos          = army->RetPos();

	if (rec->GetTargetTypeTradeRoute())
	{
		Assert( m_target_city.m_id != 0);

		const TradeDynamicArray* trade_routes = 
			m_target_city.GetCityData()->GetTradeSourceList();
		for(sint32 i = 0; i < trade_routes->Num(); i++) 
		{
			const DynamicArray<MapPoint>* path = (*trade_routes)[i].GetPath();

			for (sint32 j = 1; j < path->Num()-1; j++)
			{
				Cell *cell = g_theWorld->GetCell(path->Get(j));

				if (cell->HasCity())
					continue;

				if (cell->CanEnter(army->GetMovementType()))
				{
					tmp_squared_dist = 
					MapPoint::GetSquaredDistance(path->Get(j), army_pos);
					if (tmp_squared_dist < best_squared_dist)
					{
						best_squared_dist = tmp_squared_dist;
						best_target_pos = path->Get(j);
					}
				}
			}

			if (best_squared_dist < max_squared_dist)
				return best_target_pos;
			else
			{
				bool NO_TRADE_ROUTE_TARGET_POS_FOUND = true;
				Assert(NO_TRADE_ROUTE_TARGET_POS_FOUND);
			}
		}
	}
	else if (rec->GetTargetTypeImprovement())
	{
		Assert( m_target_city.m_id != 0);

		if (m_target_city->GetCityData()->WasTerrainImprovementBuilt())
		{
			CityInfluenceIterator it(m_target_city.RetPos(), m_target_city.GetCityData()->GetSizeIndex());
			for(it.Start(); !it.End(); it.Next()) 
			{
				Cell *cell = g_theWorld->GetCell(it.Pos());

				if (!(cell->GetCityOwner() == m_target_city))
					continue;

				if (cell->GetNumDBImprovements() <= 0)
					continue;

				if (m_target_city.RetPos() == it.Pos())
					continue;

				if (cell->CanEnter(army->GetMovementType()))
				{
					tmp_squared_dist = MapPoint::GetSquaredDistance(it.Pos(), army_pos);
					if (tmp_squared_dist < best_squared_dist)
					{
						best_squared_dist = tmp_squared_dist;
						best_target_pos = it.Pos();
					}
				}
			}

			if (best_squared_dist < max_squared_dist)
				return best_target_pos;
			else
			{
				bool NO_TILE_IMPROVEMENT_TARGET_POS_FOUND = true;
				Assert(NO_TILE_IMPROVEMENT_TARGET_POS_FOUND);
			}
		}
	}
	else if (rec->GetTargetTypePetrolStation()){
		sint32 distance_to_refuel;
		MapPoint refuel_pos;
		CtpAi::GetNearestRefuel(army, army_pos, refuel_pos, distance_to_refuel);
		return refuel_pos;
	}

	return Get_Target_Pos();
}

const MapPoint & CTPGoal::Get_Target_Pos() const
{
	static MapPoint pos;    // ugly life-time extension

	if (m_target_army != ID()) 
	{
		if (m_target_army.IsValid())
		{
			m_target_army->GetPos(pos);
		}
		else
		{
			pos.x = -1;
			pos.y = -1;
		}
	}
	else if (m_target_city != ID())
	{
		if (m_target_city.IsValid())
		{
			m_target_city.GetPos(pos);
		}
		else
		{
			pos.x = -1;
			pos.y = -1;
		}
	}
	else
	{
		return m_target_pos;
	}

	if (pos.x == -1 || pos.y == -1)
	{
		Assert(0); // For some reason I had removed this.

		pos.x = 0;
		pos.y = 0;
	}

	return pos;
}

const Army & CTPGoal::Get_Target_Army() const
{
	return m_target_army;
}

void CTPGoal::Set_Sub_Task(const SUB_TASK_TYPE & sub_task)
{
	m_sub_task = sub_task;
}

const SUB_TASK_TYPE & CTPGoal::Get_Sub_Task() const
{
	return m_sub_task;
}

const Unit & CTPGoal::Get_Target_City() const
{
	return m_target_city;
}

sint32 CTPGoal::Get_Target_Value() const
{
	sint32              value   = 0;
	const GoalRecord *  rec     = g_theGoalDB->Get(m_goal_type);
	Assert(rec);

	if(rec->GetTargetTypeAttackUnit()
	|| rec->GetTargetTypeSpecialUnit()
	){
		const Army &    army = Get_Target_Army();
		sint16          tmpCount;
		double          tmp;
		double          tmpValue;
		army->ComputeStrength(tmp,tmp,tmp,tmpCount,tmpCount,tmp,tmp,tmp,tmpValue, false);
		value = static_cast<sint32>(tmpValue);
	}
	else if(rec->GetTargetTypeCity())
	{
		if(m_target_city.IsValid())
		{
			value = m_target_city->GetCityData()->GetValue();
		}
	}
	else if( rec->GetTargetTypeTradeRoute() )
	{
		const Unit & city = Get_Target_City();
		value = city->GetCityData()->GetGoldFromTradeRoutes();
	}

	return value;
}


PLAYER_INDEX CTPGoal::Get_Target_Owner() const
{
	PLAYER_INDEX       target_owner = PLAYER_UNASSIGNED;
	GoalRecord const * goal_record  = g_theGoalDB->Get(m_goal_type);
	Assert(goal_record);
	
	if(goal_record->GetTargetTypeAttackUnit()
	|| goal_record->GetTargetTypeSpecialUnit()
	)
	{
		target_owner = m_target_army.GetOwner();
	}
	else if(goal_record->GetTargetTypePetrolStation()){
		if(m_target_city != ID() || g_theWorld->IsAirfield(Get_Target_Pos())){
			target_owner = g_theWorld->GetOwner(Get_Target_Pos());
		}
		else if(m_target_army != ID()){
			target_owner = m_target_army.GetOwner();
		}
	}
	else{
		MapPoint pos(Get_Target_Pos());
		if(pos.x >= 0){
			target_owner = g_theWorld->GetOwner(Get_Target_Pos());
		}
	}

	return target_owner;
}

void CTPGoal::Rollback_Agent(Agent_ptr agent_ptr)
{
	CTPAgent_ptr ctpagent_ptr = static_cast<CTPAgent_ptr>(agent_ptr);
	Assert(ctpagent_ptr);

#ifdef _DEBUG_SCHEDULER
	if (ctpagent_ptr->Get_Army().IsValid())
	{
//		Assert(ctpagent_ptr->Get_Army()->m_theAgent == ctpagent_ptr);
//		Assert(ctpagent_ptr->Get_Army()->m_theGoal == this);
	}
#endif // _DEBUG_SCHEDULER

	if(!ctpagent_ptr->Get_Is_Dead()
	&&  g_player[m_playerId]
	&&  g_player[m_playerId]->IsRobot()
	){
		ctpagent_ptr->ClearOrders();
	}

	Goal::Rollback_Agent(agent_ptr);
}

bool CTPGoal::Is_Execute_Incrementally() const
{
	return g_theGoalDB->Get(m_goal_type)->GetExecuteIncrementally();
}

void CTPGoal::Compute_Needed_Troop_Flow()
{
	const MapPoint pos         = Get_Target_Pos();
	const sint32   threat      = MapAnalysis::GetMapAnalysis().GetThreat          (m_playerId, pos);
	const sint32   attack      = MapAnalysis::GetMapAnalysis().GetEnemyAttack     (m_playerId, pos);
	const sint32   defense     = MapAnalysis::GetMapAnalysis().GetEnemyDefense    (m_playerId, pos);
	const sint32   ranged      = MapAnalysis::GetMapAnalysis().GetEnemyRanged     (m_playerId, pos);
	const sint32   bombardLand = MapAnalysis::GetMapAnalysis().GetEnemyBombardLand(m_playerId, pos);
	const sint32   bombardSea  = MapAnalysis::GetMapAnalysis().GetEnemyBombardSea (m_playerId, pos);
	const sint32   bombardAir  = MapAnalysis::GetMapAnalysis().GetEnemyBombardAir (m_playerId, pos);
	const sint32   value       = MapAnalysis::GetMapAnalysis().GetEnemyValue      (m_playerId, pos);

	m_current_needed_strength   = Squad_Strength(1);
		// why only one unit ? Why then zero units? - Martin G�hmann
		// by bringing a real army to pirate or pillage, it can be ready for seige or attack
		// a single unit is quite defenseless - Calvitix

	const GoalRecord *goal_record = g_theGoalDB->Get(m_goal_type);
	if(goal_record->GetTargetTypeChokePoint())
	{
		// Need also attack and ranged strength - Calvitix
		m_current_needed_strength.Set_Attack      (attack       / 2);
		m_current_needed_strength.Set_Defense     (defense      / 2);
		m_current_needed_strength.Set_Ranged      (ranged       / 2);
		m_current_needed_strength.Set_Bombard_Land(bombardLand  / 2);
		m_current_needed_strength.Set_Bombard_Sea (bombardSea   / 2);
		m_current_needed_strength.Set_Bombard_Air (bombardAir   / 2);
		m_current_needed_strength.Set_Value       (value        / 2);
	}
	else if(goal_record->GetTargetTypeImprovement()
	     || goal_record->GetTargetTypeTradeRoute()
	     ){

		m_current_needed_strength.Set_Attack(attack  / 2);
		m_current_needed_strength.Set_Ranged(defense / 2);
		m_current_needed_strength.Set_Value(value);
	}
	else if(goal_record->GetTargetTypeEndgame())
	{
		if(goal_record->GetTargetOwnerSelf())
			m_current_needed_strength.Set_Defense(threat);
		
		else
			m_current_needed_strength.Set_Attack(threat);

		//to be sure that the global force of the army will be enough
		// (not only a wounded unit for example)
		m_current_needed_strength.Set_Value(value);

	}
	else if(goal_record->GetTargetTypeCity()
	     && goal_record->GetTargetOwnerSelf()
	     ){

		// tweak to obtain RETREAT Goal definition : TO DO - 'cleaner' method - Calvitix
		// Set to 2 to so that units with no goals will retreat to the nearest city
		// (I Prefer that method than the GARRISON troops, that are not able to leave the city)
		// cities will be better defended if their is enough units, otherwise units will be 
		// affected to relevant goals
		if(g_theGoalDB->Get(m_goal_type)->GetTargetTypeCity()
		&& g_theGoalDB->Get(m_goal_type)->GetTargetOwnerSelf()
		&& goal_record->GetTreaspassingArmyBonus() > 0
		){
			m_current_needed_strength.Set_Agent_Count(2);
		}
		else
		{
			const StrategyRecord & strategy = 
				Diplomat::GetDiplomat(m_playerId).GetCurrentStrategy();

			sint32 offensive_garrison;
			sint32 defensive_garrison;
			sint32 ranged_garrison;
			strategy.GetOffensiveGarrisonCount(offensive_garrison);
			strategy.GetDefensiveGarrisonCount(defensive_garrison);
			strategy.GetRangedGarrisonCount(ranged_garrison);

			// why only defensive units ?
			// added ranged units - Calvitix
			m_current_needed_strength.Set_Defense(threat * 2 / 3);
			m_current_needed_strength.Set_Ranged(threat / 3);
			m_current_needed_strength.Set_Value(value);

			//not used for the moment (only attack or defense strength is considerated
			//(see army_strength > operator) - Calvitix
			m_current_needed_strength.Set_Defenders(static_cast<sint16>(defensive_garrison + offensive_garrison));
			m_current_needed_strength.Set_Ranged_Units(static_cast<sint16>(ranged_garrison));
		}
	}
	else if
	       (
	        (    goal_record->GetTargetTypeAttackUnit()
	          || goal_record->GetTargetTypeCity()
	        )
	        && !goal_record->GetTargetOwnerSelf()
	        && !goal_record->GetTargetTypeSpecialUnit()
	       )
	{
		// A real Attack force, depending on threat
		m_current_needed_strength.Set_Attack      (attack);
		m_current_needed_strength.Set_Defense     (defense);
		m_current_needed_strength.Set_Ranged      (ranged);
		m_current_needed_strength.Set_Bombard_Land(bombardLand);
		m_current_needed_strength.Set_Bombard_Sea (bombardSea);
		m_current_needed_strength.Set_Bombard_Air (bombardAir);
		m_current_needed_strength.Set_Value       (value);

		Squad_Strength strength;
		strength.Set_Pos_Strength(pos);
		//
		if
		  (
		       Get_Target_Owner() != 0
		    || strength.Get_Agent_Count() > 0
		  )
		{
			// Set this to zero, since the units we need doesn't depent on the number of units at the target.
			strength.Set_Agent_Count(0);
			strength.Set_Defenders(0);
			strength.Set_Ranged_Units(0);
			m_current_needed_strength += strength;
			m_current_needed_strength.Set_Force_Matching(0.5,0.5,0.5,0.5,0.5);
		}
	}

	else if(goal_record->GetTargetTypeBorder())
	{
		// assuming threat is the global strength to use (to be coherent with other changes) - Calvitix
		m_current_needed_strength.Set_Attack      (attack       / 2);
		m_current_needed_strength.Set_Defense     (defense      / 2);
		m_current_needed_strength.Set_Ranged      (ranged       / 2);
		m_current_needed_strength.Set_Bombard_Land(bombardLand  / 2);
		m_current_needed_strength.Set_Bombard_Sea (bombardSea   / 2);
		m_current_needed_strength.Set_Bombard_Air (bombardAir   / 2);
		m_current_needed_strength.Set_Value       (value        / 2); // Actually this stuff should go to the force matches
	}
	else if (   goal_record->GetTargetTypeSettleLand()
	        ||  goal_record->GetTargetTypeSettleSea()
	        ||  goal_record->GetTargetTypePetrolStation()
	        )
	{
		// No strength is needed
	}
	else
	{
		m_current_needed_strength.Set_Pos_Strength(pos);
	}

	const StrategyRecord & strategy =
		Diplomat::GetDiplomat(m_playerId).GetCurrentStrategy();
	const StrategyRecord::ForceMatch *force_match = NULL;

	switch (goal_record->GetForceMatch())
	{
	case k_Goal_ForceMatch_Offensive_Bit:
		force_match = strategy.GetOffensivePtr();
		break;
	case k_Goal_ForceMatch_Defensive_Bit:
		force_match = strategy.GetDefensivePtr();
		break;
	case k_Goal_ForceMatch_StealthAttack_Bit:
		force_match = strategy.GetStealthAttackPtr();
		break;
	case k_Goal_ForceMatch_Bombard_Bit:
		force_match = strategy.GetBombardPtr();
		break;
	case k_Goal_ForceMatch_Special_Bit:
		force_match = strategy.GetSpecialPtr();
		break;
	case k_Goal_ForceMatch_Harass_Bit:
		force_match = strategy.GetHarassPtr();
		break;
	default:

		Assert(false);
	}

	Assert(force_match);
	m_current_needed_strength.Set_Force_Matching(
		force_match->GetAttackMatch(),
		force_match->GetDefenseMatch(),
		force_match->GetRangedMatch(),
		force_match->GetBombardMatch(),
		force_match->GetValueMatch());

	if (Needs_Transporter())
	{
		sint16 const    dest_cont       = 
		    g_theWorld->GetContinent(pos);
		bool            need_transport  = false;

		for
		(
		    Agent_List::iterator agent_iter  = m_agents.begin(); 
		                         agent_iter != m_agents.end() && !need_transport;
		                       ++agent_iter
		)
		{
			CTPAgent_ptr ctpagent_ptr = (CTPAgent_ptr) *agent_iter;
			Assert(ctpagent_ptr);

			need_transport = 
			    dest_cont != g_theWorld->GetContinent(ctpagent_ptr->Get_Pos());
		}

		if (!need_transport)
		{
			// Has to be modified
			m_current_needed_strength.Set_Transport(0);
		}
	}
}

Utility CTPGoal::Compute_Matching_Value(const Agent_ptr agent) const
{
	CTPAgent_ptr ctpagent_ptr = (CTPAgent_ptr) agent;

#if defined(_DEBUG)
	Player *player_ptr = g_player[ m_playerId ];
	Assert(player_ptr && ctpagent_ptr);
#endif

	if(ctpagent_ptr->Get_Is_Dead())
	{
		return Goal::BAD_UTILITY;
	}

	MapPoint dest_pos = Get_Target_Pos(ctpagent_ptr->Get_Army());

	if(!Pretest_Bid(ctpagent_ptr, dest_pos))
	{
		return Goal::BAD_UTILITY;
	}

	PLAYER_INDEX target_owner = Get_Target_Owner();
	const Diplomat & diplomat = Diplomat::GetDiplomat(m_playerId);

	// Don't attack as Barbarian a target that is protected by the Great Wall
	if(ctpagent_ptr->Get_Army()->GetOwner() == PLAYER_INDEX_VANDALS
	&& wonderutil_GetProtectFromBarbarians(g_player[target_owner]->m_builtWonders)
	){
		return Goal::BAD_UTILITY;
	}

	if( target_owner > 0 &&
		(!diplomat.IncursionPermission(target_owner)))
	{
		bool isspecial, cancapture, haszoc, canbombard;
		bool isstealth;
		sint32 maxattack, maxdefense;
		ctpagent_ptr->Get_Army()->CharacterizeArmy( isspecial,
			isstealth,
			maxattack,
			maxdefense,
			cancapture,
			haszoc,
			canbombard);
		if (!isspecial || maxattack > 0 || haszoc)
		{
			return Goal::BAD_UTILITY;
		}
	}

	sint8 new_garrison = 0;
	double new_garrison_strength = 0.0;
	if(NeededForGarrison(ctpagent_ptr, dest_pos, new_garrison, new_garrison_strength))
	{
		// Needs to be modified
		return Goal::BAD_UTILITY;
	}

#if defined(_DEBUG) || defined(USE_LOGGING)
	// Maybe this is of some use later
	bool is_transporter = false;
#endif

	sint32 transports, max,empty;
	if(Needs_Transporter()
	&& ctpagent_ptr->Get_Army()->GetCargo(transports, max, empty)
	&& empty > 0
	&& m_agents.size() > 0
	){
		Utility transport_utility = 0;
		Utility           utility = 0;

		sint32            count   = 0;

		for
		(
		    Agent_List::const_iterator agent_iter  = m_agents.begin(); 
		                               agent_iter != m_agents.end();
		                             ++agent_iter
		)
		{
			CTPAgent_ptr agent_ptr = static_cast<CTPAgent_ptr>(*agent_iter);

			if(agent_ptr->Get_Needs_Transporter())
			{
				agent_ptr->EstimateTransportUtility(ctpagent_ptr, utility);
				transport_utility += utility;

				++count;
			}
		}

		if(count > 0)
		{
			Utility match = (transport_utility / count);
			return match + Get_Raw_Priority();
		}
		else
		{
			return Goal::BAD_UTILITY;
		}

#if defined(_DEBUG) || defined(USE_LOGGING)
// Maybe this is of some use later
//		is_transporter = true;
#endif
	}

	Utility bonus = 0;

	//Set if unit is wounded and it is a retreat of defense goal, add bonus 
	//to goalpriority + matching

	MapPoint armyPos      = ctpagent_ptr->Get_Pos();
	PLAYER_INDEX PosOwner = g_theWorld->GetOwner(armyPos);

	if(g_theGoalDB->Get(m_goal_type)->GetTargetTypeCity() 
	&& g_theGoalDB->Get(m_goal_type)->GetTargetOwnerSelf()
	){
		// For Defend or Retreat goals
		if(ctpagent_ptr->Get_Army()->IsWounded()
		&&!ctpagent_ptr->Get_Army()->IsObsolete()
		){
			bonus+= g_theGoalDB->Get(m_goal_type)->GetWoundedArmyBonus();
		}
		if(PosOwner != m_playerId
		&&!diplomat.IncursionPermission(PosOwner)
		){
			bonus+= g_theGoalDB->Get(m_goal_type)->GetTreaspassingArmyBonus();
		}
	}
	else if((g_theGoalDB->Get(m_goal_type)->GetTargetOwnerColdEnemy()
	     ||  g_theGoalDB->Get(m_goal_type)->GetTargetOwnerHotEnemy())
	     && (g_theGoalDB->Get(m_goal_type)->GetTargetTypeAttackUnit()
	     ||  g_theGoalDB->Get(m_goal_type)->GetTargetTypeCity())
	     ){  //For Attack goals (unit or city)

		if(ctpagent_ptr->Get_Army()->CanSettle())
		{
			// If there is a settler in the army...
			return Goal::BAD_UTILITY;
		}
		
		if(ctpagent_ptr->Get_Army()->IsWounded() && !ctpagent_ptr->Get_Army()->IsObsolete())
		{
			bonus+= g_theGoalDB->Get(m_goal_type)->GetWoundedArmyBonus();
		}
		
		if(PosOwner != m_playerId && !diplomat.IncursionPermission(PosOwner))
		{
			bonus += g_theGoalDB->Get(m_goal_type)->GetTreaspassingArmyBonus();
		}
	}

#if defined(_DEBUG) || defined(USE_LOGGING)  // Add a debug report of goal computing (raw priority and all modifiers)
	double report_wounded = bonus;
#endif //_DEBUG
	
	if(ctpagent_ptr->Get_Army()->IsObsolete())
		bonus += g_theGoalDB->Get(m_goal_type)->GetObsoleteArmyBonus();

#if defined(_DEBUG) || defined(USE_LOGGING)  // Add a debug report of goal computing (raw priority and all modifiers)
	double report_obsolete = bonus - report_wounded;
#endif //_DEBUG

	sint32 squared_distance = 0;
	double eta = ctpagent_ptr->GetRoundsPrecise(dest_pos, squared_distance);
	double cell_dist = sqrt(static_cast<double>(squared_distance));

	Utility raw_priority = Get_Raw_Priority();

	const StrategyRecord & strategy = 
		Diplomat::GetDiplomat(m_playerId).GetCurrentStrategy();

	sint32 distance_modifier = 1;
	strategy.GetDistanceModifierFactor(distance_modifier);
	Utility time_term = static_cast<Utility>( (eta * distance_modifier) + cell_dist);

	if(g_theGoalDB->Get(m_goal_type)->GetTreaspassingArmyBonus() > 0)
	{
		PLAYER_INDEX pos_owner = g_theWorld->GetCell(ctpagent_ptr->Get_Pos())->GetOwner();

		bool incursion_permissin = Diplomat::GetDiplomat(m_playerId).IncursionPermission(pos_owner);
		if (pos_owner >= 0 && !(incursion_permissin))
		{
			bonus += g_theGoalDB->Get(m_goal_type)->GetTreaspassingArmyBonus();
		}
	}
#if defined(_DEBUG) || defined(USE_LOGGING) // Add a debug report of goal computing (raw priority and all modifiers)
	double report_Treaspassing = bonus - report_obsolete;
#endif //_DEBUG

	if(ctpagent_ptr->Get_Army()->IsInVisionRangeAndCanEnter(dest_pos))
	{
		/// @ToDo: Use the actual path cost, to check whether the goody hut really so close.
		bonus += g_theGoalDB->Get(m_goal_type)->GetInVisionRangeBonus();

		if (!Barbarians::InBarbarianPeriod()
		||  wonderutil_GetProtectFromBarbarians(g_player[ctpagent_ptr->Get_Army()->GetOwner()]->m_builtWonders)
		){
			bonus += g_theGoalDB->Get(m_goal_type)->GetNoBarbarianBonus();
		}
		else if ((ctpagent_ptr->Get_Squad_Class() & k_Goal_SquadClass_CanAttack_Bit) != 0x0)
		{
			bonus += g_theGoalDB->Get(m_goal_type)->GetCanAttackBonus();
		}
	}
#if defined(_DEBUG) || defined(USE_LOGGING) // Add a debug report of goal computing (raw priority and all modifiers)
	double report_InVisionRange  = bonus - report_Treaspassing;
	double report_NoBarbsPresent = bonus - report_InVisionRange;
#endif //_DEBUG

	if(!ctpagent_ptr->Get_Army()->HasCargo())
	{
		if(!g_theWorld->IsOnSameContinent(dest_pos, ctpagent_ptr->Get_Pos()) // Same continent problem
		&& !ctpagent_ptr->Get_Army()->GetMovementTypeAir()
		&& g_player[m_playerId]->GetCargoCapacity() <= 0
		&& m_current_attacking_strength.Get_Transport() <= 0
		){
			return Goal::BAD_UTILITY;
		}
	}

	Utility match = bonus + time_term + raw_priority;

#if 0
	// Well we have to see whether this is good:
	const ArmyData* army = ctpagent_ptr->Get_Army().GetData();

	Utility tieBreaker = 0;
	sint32 govType = g_player[m_playerId]->GetGovernmentType();

	for(sint32 i = 0; i < army->Num(); ++i)
	{
		sint32 type = army->Get(i).GetType();
		const UnitRecord* rec = g_theUnitDB->Get(type, govType);
		tieBreaker += static_cast<Utility>(rec->GetAttack());
		tieBreaker += static_cast<Utility>(rec->GetDefense());
		tieBreaker +=                      rec->GetZBRangeAttack();
		tieBreaker +=                      rec->GetFirepower();
		tieBreaker += static_cast<Utility>(rec->GetArmor());
		tieBreaker +=                      rec->GetMaxHP();
	}

	Assert(army->Num() > 0);

	tieBreaker /= army->Num();

	match += tieBreaker;
#endif

#if defined(_DEBUG) || defined(USE_LOGGING) // Add a debug report of goal computing (raw priority and all modifiers) - Calvitix
	MapPoint target_pos = Get_Target_Pos();

	AI_DPRINTF(k_DBG_SCHEDULER_DETAIL, this->Get_Player_Index(), m_goal_type, -1,
	("\t\t%9x,\t %9x,\t%9x (%3d,%3d),\t%s (%3d,%3d) (%3d,%3d),\t%8d,\t%8d,\t%8f,\t%8f,\t%8d,\t%8f,\t%8f,\t%8f,\t%8d,\t%8f,\t%8f,\t%8d,\t%9x,\t%s \n",
	this,                                          // This goal
	ctpagent_ptr->Get_Army().m_id,                 // The army
	ctpagent_ptr,                                  // The agent
	ctpagent_ptr->Get_Pos().x,                     // Agent pos.x
	ctpagent_ptr->Get_Pos().y,                     // Agent pos.y
	g_theGoalDB->Get(m_goal_type)->GetNameText(),  // Goal name
	target_pos.x,                                  // Target pos.x
	target_pos.y,                                  // Target pos.y
	dest_pos.x,                                    // Sub target pos.x
	dest_pos.y,                                    // Sub target pos.y
	match,                                         // Computed match value
	raw_priority,                                  // Raw match value
	cell_dist,                                     // Distance to target (Quare rooted quare distance), not identical with path distance
	eta,                                           // Rounds to target
	bonus,                                         // Total bonus
	report_wounded,                                // Whether there wounded units
	report_obsolete,                               // Whether there are obselete units
	report_Treaspassing,                           // Whether we are treaspassing
	time_term,                                     // Time needed to goal, if we would follow a bee line
	report_InVisionRange,                          // In vision range bonus
	report_NoBarbsPresent,                         // If no Barbarian are present bonus
	is_transporter,                                // Whether the agent is a transporter
	ctpagent_ptr->Get_Goal(),                      // The goal to that this agent is asigned to
	(g_theWorld->HasCity(target_pos) ? g_theWorld->GetCity(target_pos).GetName() : "field")
	));
#endif //_DEBUG

	return match;
}

Utility CTPGoal::Compute_Raw_Priority()
{
	Player *    player_ptr = g_player[m_playerId];
	Assert(player_ptr);

	if (!player_ptr || Get_Totally_Complete())
	{
		m_raw_priority = Goal::BAD_UTILITY;
		return m_raw_priority;
	}

	MapPoint target_pos = Get_Target_Pos();

	if(!player_ptr->CanUseSeaTab()
	&& (g_theWorld->IsWater(target_pos) || g_theWorld->IsShallowWater(target_pos))
	){
		AI_DPRINTF(k_DBG_SCHEDULER_DETAIL, this->Get_Player_Index(), this->Get_Goal_Type(), -1, ("\t  No sea tab\n"));
		m_raw_priority = Goal::BAD_UTILITY;
		return m_raw_priority;
	}

	const GoalRecord * goal_rec = g_theGoalDB->Get(m_goal_type);
	const MapAnalysis & map = MapAnalysis::GetMapAnalysis();
	PLAYER_INDEX target_owner = Get_Target_Owner();

	//alway compute a foreign center (even if the target is owned by the player
	// otherwise it compute with coords (0,0) !!
	MapPoint empire_center;
	MapPoint foreign_empire_center;
	empire_center = map.GetEmpireCenter(m_playerId);
	if (target_owner > 0 && m_playerId != target_owner)
		foreign_empire_center = map.GetEmpireCenter(target_owner);
	else
		foreign_empire_center = map.GetNearestForeigner(m_playerId, target_pos);

	const Diplomat       & diplomat = Diplomat::GetDiplomat(m_playerId);
	const StrategyRecord & strategy = diplomat.GetCurrentStrategy();

	Assert(m_goal_type < strategy.GetNumGoalElement());
	Assert(strategy.GetGoalElement(m_goal_type) != NULL);

	double cell_value = (m_goal_type < strategy.GetNumGoalElement())
	                    ? strategy.GetGoalElement(m_goal_type)->GetPriority()
	                    : 0.0;

#if defined(_DEBUG) || defined(USE_LOGGING) // Add a debug report of goal computing (raw priority and all modifiers) - Calvitix
	double report_cell_value            = cell_value;
	double report_cell_lastvalue        = cell_value;
	double report_cell_threat           = 0.0;
	double report_cell_EnemyValue       = 0.0;
	double report_cell_AlliedValue      = 0.0;
	double report_cell_HomeDistance     = 0.0;
	double report_cell_EnemyDistance    = 0.0;
	double report_cell_Settle           = 0.0;
	double report_cell_Chokepoint       = 0.0;
	double report_cell_MaxPower         = 0.0;
	double report_cell_Unexplored       = 0.0;
	double report_cell_NotVisible       = 0.0;
	double report_cell_NoOwnerTerritory = 0.0;
	double report_cell_InHomeTerritory  = 0.0;
	double report_cell_InEnemyTerritory = 0.0;
#endif //_DEBUG

	double maxThreat = static_cast<double>(map.GetMaxThreat(m_playerId));
	if ( maxThreat > 0 )
	{
		cell_value += 
		( ( static_cast<double>(map.GetThreat(m_playerId, target_pos)) /
		     maxThreat) * 
		  g_theGoalDB->Get(m_goal_type)->GetThreatBonus() );

#if defined(_DEBUG) || defined(USE_LOGGING) // Add a debug report of goal computing (raw priority and all modifiers) - Calvitix
		report_cell_threat    = cell_value - report_cell_lastvalue;
		report_cell_lastvalue = cell_value;
#endif //_DEBUG
	}

	double maxEnemyValue = static_cast<double>(map.GetMaxEnemyValue(m_playerId));
	if (maxEnemyValue)
	{
		cell_value += 
		( ( static_cast<double>(map.GetEnemyValue( m_playerId, target_pos)) /
		    maxEnemyValue ) * 
		  goal_rec->GetEnemyValueBonus() );

#if defined(_DEBUG) || defined(USE_LOGGING) // Add a debug report of goal computing (raw priority and all modifiers) - Calvitix
		report_cell_EnemyValue = cell_value - report_cell_lastvalue;
		report_cell_lastvalue  = cell_value;
#endif //_DEBUG
	}

	double maxAlliedValue = static_cast<double>(map.GetMaxAlliedValue(m_playerId));
	if ( maxAlliedValue > 0)
	{
		cell_value += 
		( ( static_cast<double>(map.GetAlliedValue(m_playerId, target_pos)) /
		    maxAlliedValue ) * 
		  goal_rec->GetAlliedValueBonus() );

#if defined(_DEBUG) || defined(USE_LOGGING) // Add a debug report of goal computing (raw priority and all modifiers) - Calvitix
		report_cell_AlliedValue = cell_value - report_cell_lastvalue;
		report_cell_lastvalue   = cell_value;
#endif //_DEBUG
	}
	double maxPower = static_cast<double>(map.GetMaxPower(m_playerId)); // Get Max Power of all Allies
	if (maxPower > 0)
	{
		cell_value += 
		( ( static_cast<double>(map.GetPower( m_playerId, target_pos)) /  // Allies' Power at target_pos
		    maxPower ) * 
		goal_rec->GetPowerBonus() );

#if defined(_DEBUG) || defined(USE_LOGGING) // Add a debug report of goal computing (raw priority and all modifiers) - Calvitix
		report_cell_MaxPower  = cell_value - report_cell_lastvalue;
		report_cell_lastvalue = cell_value;
#endif //_DEBUG
	}

	cell_value += sqrt(static_cast<double>
	(MapPoint::GetSquaredDistance(target_pos, empire_center))) * goal_rec->GetDistanceToHomeBonus();

#if defined(_DEBUG) || defined(USE_LOGGING) // Add a debug report of goal computing (raw priority and all modifiers) - Calvitix
	report_cell_HomeDistance = cell_value - report_cell_lastvalue;
	report_cell_lastvalue    = cell_value;
#endif //_DEBUG

	if (foreign_empire_center.x != 0 && foreign_empire_center.y != 0)//Dangerious if the empire center has coords (0,0)
	{
		cell_value += sqrt(static_cast<double>
		(MapPoint::GetSquaredDistance(target_pos, foreign_empire_center))) * goal_rec->GetDistanceToEnemyBonus();
	}

#if defined(_DEBUG) || defined(USE_LOGGING) // Add a debug report of goal computing (raw priority and all modifiers) - Calvitix
	report_cell_EnemyDistance = cell_value - report_cell_lastvalue;
	report_cell_lastvalue     = cell_value;
#endif //_DEBUG

	if (g_theWorld->GetCell( target_pos )->GetIsChokePoint())
	{
		cell_value += goal_rec->GetChokePointBonus();
	}

#if defined(_DEBUG) || defined(USE_LOGGING) // Add a debug report of goal computing (raw priority and all modifiers) - Calvitix
	report_cell_Chokepoint = cell_value - report_cell_lastvalue;
	report_cell_lastvalue  = cell_value;
#endif //_DEBUG

	if (!player_ptr->IsExplored(target_pos))
	{
		cell_value += goal_rec->GetUnexploredBonus();
	}

#if defined(_DEBUG) || defined(USE_LOGGING) // Add a debug report of goal computing (raw priority and all modifiers) - Calvitix
	report_cell_Unexplored = cell_value - report_cell_lastvalue;
	report_cell_lastvalue  = cell_value;
#endif //_DEBUG

	if (!player_ptr->IsVisible(target_pos))
	{
		cell_value += goal_rec->GetNotVisibleBonus();
	}

#if defined(_DEBUG) || defined(USE_LOGGING) // Add a debug report of goal computing (raw priority and all modifiers) - Calvitix
	report_cell_NotVisible = cell_value - report_cell_lastvalue;
	report_cell_lastvalue  = cell_value;
#endif //_DEBUG

	PLAYER_INDEX territoryOwner = g_theWorld->GetCell( target_pos )->GetOwner();
	if (m_playerId == territoryOwner)
	{
		cell_value += goal_rec->GetInHomeTerritoryBonus();
	}

#if defined(_DEBUG) || defined(USE_LOGGING) // Add a debug report of goal computing (raw priority and all modifiers) - Calvitix
	report_cell_InHomeTerritory = cell_value - report_cell_lastvalue;
	report_cell_lastvalue       = cell_value;
#endif //_DEBUG

	if (m_playerId != territoryOwner && territoryOwner >= 0) // 0: Barbarian player
	{
		cell_value += goal_rec->GetInEnemyTerritoryBonus();
	}

#if defined(_DEBUG) || defined(USE_LOGGING) // Add a debug report of goal computing (raw priority and all modifiers) - Calvitix
	report_cell_InEnemyTerritory = cell_value - report_cell_lastvalue;
	report_cell_lastvalue        = cell_value;
#endif //_DEBUG

	if (territoryOwner == PLAYER_UNASSIGNED)
	{
		cell_value += goal_rec->GetNoOwnerTerritoryBonus();
	}

#if defined(_DEBUG) || defined(USE_LOGGING) // Add a debug report of goal computing (raw priority and all modifiers) - Calvitix
	report_cell_NoOwnerTerritory = cell_value - report_cell_lastvalue;
	report_cell_lastvalue        = cell_value;
#endif //_DEBUG

	if ( goal_rec->GetTargetTypeSettleLand() ||
		 goal_rec->GetTargetTypeSettleSea() )
	{
		cell_value += SettleMap::s_settleMap.GetValue(target_pos);

#if defined(_DEBUG) || defined(USE_LOGGING)
		report_cell_Settle    = cell_value - report_cell_lastvalue;
		report_cell_lastvalue = cell_value;
#endif //_DEBUG

	}

	sint32 threaten_bonus = GetThreatenBonus();

	m_raw_priority = (Utility) cell_value + threaten_bonus;

	Assert(m_raw_priority <  Goal::MAX_UTILITY);
	Assert(m_raw_priority >= Goal::BAD_UTILITY);
	if (m_raw_priority > Goal::MAX_UTILITY)
		m_raw_priority = Goal::MAX_UTILITY-1;
	else if (m_raw_priority < Goal::BAD_UTILITY)
		m_raw_priority = Goal::BAD_UTILITY;

#if defined(_DEBUG) || defined(USE_LOGGING) // Add a debug report of goal computing (raw priority and all modifiers) - Calvitix
	char buff[256];
	sprintf(buff, "\t %9x,\t%s,\t%i, \t\trc(%3d,%3d),\t%8f,\t%8f,\t%8f,\t%8f,\t%8f,\t%8f,\t%8f, rc(%3d,%3d),\t%8f, rc(%3d,%3d), \t%8f,\t%8f,\t%8f,\t%8f,\t%8f,",
	        this,
	        goal_rec->GetNameText(),
	        m_raw_priority,
	        target_pos.x,
	        target_pos.y,
	        report_cell_value,
	        report_cell_lastvalue,
	        report_cell_threat,
	        report_cell_EnemyValue,
	        report_cell_AlliedValue,
	        report_cell_MaxPower,
	        report_cell_HomeDistance,
	        empire_center.x,
	        empire_center.y,
	        report_cell_EnemyDistance,
	        foreign_empire_center.x,
	        foreign_empire_center.y,
	        report_cell_Settle,
	        report_cell_Chokepoint,
	        report_cell_Unexplored,
	        report_cell_NotVisible,
	        threaten_bonus
	       );

	AI_DPRINTF(k_DBG_SCHEDULER_DETAIL, this->Get_Player_Index(), this->Get_Goal_Type(), -1,("%s\t%s\n", buff, (g_theWorld->HasCity(target_pos) ? g_theWorld->GetCity(target_pos).GetName() : "field")));

	// For some reason the following does not work in VC6:
/*	AI_DPRINTF(k_DBG_SCHEDULER_DETAIL, this->Get_Player_Index(), this->Get_Goal_Type(), -1,
	("\t %9x,\t%s,\t%i, \t\trc(%3d,%3d),\t%8f,\t%8f,\t%8f,\t%8f,\t%8f,\t%8f,\t%8f, rc(%3d,%3d),\t%8f, rc(%3d,%3d), \t%8f,\t%8f,\t%8f,\t%8f,\t%8f,\t%s \n",
	this,
	goal_rec->GetNameText(),
	m_raw_priority,
	target_pos.x,
	target_pos.y,
	report_cell_value,
	report_cell_lastvalue,
	report_cell_threat,
	report_cell_EnemyValue,
	report_cell_AlliedValue,
	report_cell_MaxPower,
	report_cell_HomeDistance,
	empire_center.x,
	empire_center.y,
	report_cell_EnemyDistance,
	foreign_empire_center.x,
	foreign_empire_center.y,
	report_cell_Settle,
	report_cell_Chokepoint,
	report_cell_Unexplored,
	report_cell_NotVisible,
	threaten_bonus,
	(g_theWorld->HasCity(target_pos) ? g_theWorld->GetCity(target_pos).GetName() : "field")
	));*/
#endif //_DEBUG

	return m_raw_priority;
}

GOAL_RESULT CTPGoal::Execute_Task()
{
	if(Get_Totally_Complete())
		return GOAL_COMPLETE;

	if(!Can_Be_Executed())
		return GOAL_ALREADY_MOVED;

	Assert(m_agents.begin() != m_agents.end());

	      CTPAgent_ptr ctpagent_ptr = (CTPAgent_ptr) *m_agents.begin();
	      MapPoint     goto_pos     = Get_Target_Pos(ctpagent_ptr->Get_Army());
	const GoalRecord * goal_record  = g_theGoalDB->Get(m_goal_type);
	      sint32       cells;

	Set_Sub_Task(SUB_TASK_GOAL);

	// Add this condition to avoid that a 12 units army with SEIGE goal retreat 
	// at 1 tile near the city, because it has left 1 unit and has to group with 
	// another one. I Think it is better to go on an seige the city (if there is 
	// more than 2/3 left, if more than 8 units).
	/// @ToDo: Reconsider NeverSatisfied
	bool hastogowithoutgrouping = (goal_record->GetNeverSatisfied() && ctpagent_ptr->GetRounds(goto_pos,cells) <= 1)
	                              && m_current_attacking_strength.Get_Agent_Count() > (2*k_MAX_ARMY_SIZE/3);

	if(Is_Satisfied() || Is_Execute_Incrementally() || hastogowithoutgrouping)
	{
		if(goal_record->GetRallyFirst())
		{
			if(Goal_Too_Expensive())
				return GOAL_FAILED;

			if(!RallyComplete())
			{
				Set_Sub_Task(SUB_TASK_RALLY);

				if(Ok_To_Rally())
				{
					if(!RallyTroops())
					{
						return GOAL_FAILED;
					}
					else
					{
						// If hastogowithoutgrouping is true, execute the goal 
						// even if the rally is not complete
						if(!hastogowithoutgrouping)
						{
							return GOAL_IN_PROGRESS;
						}
						else
						{
							Set_Sub_Task(SUB_TASK_GOAL);
						}
					}
				}
			}
			else
			{
				Set_Sub_Task(SUB_TASK_GOAL);
			}
		}

		// Added an Ungroup method (sometimes, for example to explore,
		// it is more interessant to have a lot of small units rather
		// than a huge army
		else if(goal_record->GetUnGroupFirst()) 
		{
			Set_Sub_Task(SUB_TASK_UNGROUP);
			if (!UnGroupTroops())
			{
				return GOAL_FAILED;
			}

			if (!UnGroupComplete())
			{
				return GOAL_IN_PROGRESS;
			}
			else
			{
				Set_Sub_Task(SUB_TASK_GOAL);
			}
		}
/*		else
		{
			Set_Sub_Task(SUB_TASK_GOAL);
		}*/

		for
		(
		    Agent_List::iterator agent_iter  = m_agents.begin();
		                         agent_iter != m_agents.end();
		                       ++agent_iter
		)
		{
			ctpagent_ptr = (CTPAgent_ptr) *agent_iter;

			if(ctpagent_ptr->Get_Is_Dead())
				continue;

			if(!ctpagent_ptr->Get_Can_Be_Executed())
				continue;

			if(!GotoGoalTaskSolution(ctpagent_ptr, goto_pos))
			{
				if(Needs_Transporter())
				{
					ctpagent_ptr->Set_Needs_Transporter(true);
					return GOAL_NEEDS_TRANSPORT;
				}
				else
				{
					return GOAL_FAILED;
				}
			}
		}
	}

	return GOAL_IN_PROGRESS; // Maybe this is good as it is
}

bool CTPGoal::Get_Totally_Complete() const
{
	if (Get_Invalid())
		return true;

	const GoalRecord *goal_record = g_theGoalDB->Get(m_goal_type);
	Diplomat & diplomat           = Diplomat::GetDiplomat(m_playerId);
	PLAYER_INDEX target_owner     = Get_Target_Owner();
	MapPoint target_pos           = Get_Target_Pos();

	Player *player_ptr = g_player[ m_playerId ];
	Assert(player_ptr != NULL);

	if (m_playerId > 0 && target_owner > 0 && goal_record->HasTargetProtectionWonder())
	{
		const WonderRecord *wonder_rec = goal_record->GetTargetProtectionWonderPtr();

		if ((AgreementMatrix::s_agreements.TurnsAtWar(m_playerId, target_owner) < 0) &&
			g_player[target_owner] &&
			(g_player[target_owner]->GetBuiltWonders() & ((uint64)1 << (uint64)(wonder_rec->GetIndex()))))
			return true;
	}

	if(!MapAnalysis::GetMapAnalysis().PlayerCanEnter(m_playerId, target_pos))
		return true;

	bool isspecial;
	bool isstealth = false;
	sint32 maxattack = 0;
	bool iscivilian = false;
	if ( goal_record->GetTargetTypeAttackUnit() ||
		goal_record->GetTargetTypeSpecialUnit() ) 
	{
		if(g_theWorld->GetCity(m_target_army->RetPos()).m_id != 0)
			return true;

		sint32 maxdefense;
		bool cancapture;
		bool haszoc;
		bool canbombard;

		m_target_army->CharacterizeArmy
		                               (
		                                isspecial,
		                                isstealth,
		                                maxattack,
		                                maxdefense,
		                                cancapture,
		                                haszoc,
		                                canbombard
		                               );

		iscivilian = m_target_army->IsCivilian();

		if(isspecial && !m_target_army->IsVisible(m_playerId))
			return true;
	}

	if( goal_record->GetTargetTypeSpecialUnit()
	&& !goal_record->GetTargetTypeAttackUnit()
	&& maxattack > 0
	){
		return true;
	}

	if(goal_record->GetTargetTypeUnexplored())
	{
		Unit city = g_theWorld->GetCity(target_pos);
		CellUnitList army;
		g_theWorld->GetArmy(target_pos,army);
		if (army.Num() > 0 || city.m_id != 0x0)
			return true;
	}

	if(m_playerId != 0 && target_owner > 0  && target_owner != m_playerId)
	{
		bool regard_checked = false;
		bool diplomacy_match = true;

		if ( player_ptr->HasContactWith(target_owner))
		{
			if (iscivilian &&
				goal_record->GetTargetOwnerHotEnemy() &&
				(diplomat.GetPersonality()->GetAlignmentGood() ||
				 diplomat.GetPersonality()->GetAlignmentNeutral()))
				 return true;

			if (goal_record->GetTargetOwnerNeutral())
			{
				diplomacy_match =
					diplomat.TestEffectiveRegard(target_owner, NEUTRAL_REGARD);

				regard_checked = true;
			}

			if ( (!regard_checked || !diplomacy_match) &&
				goal_record->GetTargetOwnerColdEnemy() )
			{
				diplomacy_match = 
					diplomat.TestEffectiveRegard(target_owner, COLDWAR_REGARD);

				regard_checked = true;
			}

			if ( (!regard_checked || !diplomacy_match) &&
				goal_record->GetTargetOwnerHotEnemy() )
			{
				diplomacy_match = 
					diplomat.TestEffectiveRegard(target_owner, HOTWAR_REGARD);

				regard_checked = true;
			}

			if ( (!regard_checked || !diplomacy_match) &&
				goal_record->GetTargetOwnerAlly() )
			{
				diplomacy_match = 
					diplomat.TestEffectiveRegard(target_owner, ALLIED_REGARD);

				regard_checked = true;
			}
			// If the goal is not executed by stealth units, forbid to 
			// execute it if there is no incursion permission 
			// (depending on alignement) - Calvitix
			if ((!diplomat.IncursionPermission(target_owner) &&
				(diplomat.GetPersonality()->GetAlignmentGood() ||
				 diplomat.GetPersonality()->GetAlignmentNeutral()))
				 && !goal_record->GetSquadClassStealth())
			{
				AI_DPRINTF(k_DBG_SCHEDULER, m_playerId, m_goal_type, 0,
				    ("\t  GOAL %x (%s) (%3d,%3d): Diplomacy match failed : No permission to enter territory\n", this, g_theGoalDB->Get(m_goal_type)->GetNameText(),target_pos.x,target_pos.y));
				return true;
			}

			if(!diplomacy_match)
			{
				AI_DPRINTF(k_DBG_SCHEDULER, m_playerId, m_goal_type, 0,
				    ("\t  GOAL %x (%s) (%3d,%3d): Diplomacy match failed.\n", this, g_theGoalDB->Get(m_goal_type)->GetNameText(),target_pos.x,target_pos.y));

				return true;
			}
		}
		else if(!goal_record->GetTargetOwnerNoContact())
		{
			AI_DPRINTF(k_DBG_SCHEDULER, m_playerId, m_goal_type, 0,
			    ("\t  GOAL %x (%s) (%3d,%3d): Target owner not contacted.\n", this, g_theGoalDB->Get(m_goal_type)->GetNameText(),target_pos.x,target_pos.y));

			return true;
		}
	}

	const OrderRecord * order_record = goal_record->GetExecute();

	switch(order_record->GetTargetPretest())
	{
		case k_Order_TargetPretest_EnemyArmy_Bit:
		case k_Order_TargetPretest_EnemySpecialUnit_Bit:
		case k_Order_TargetPretest_EnemySettler_Bit:
		case k_Order_TargetPretest_EnemyTradeUnit_Bit:
		case k_Order_TargetPretest_AttackPosition_Bit:
			if ((m_target_army.m_id != 0) && (m_target_army.GetOwner() == m_playerId))
			{
				Assert(false);
				return true;
			}
			break;
		case k_Order_TargetPretest_TradeRoute_Bit:

			Assert( m_target_city.m_id != 0);
			if ((m_target_city.m_id == 0) ||
				 (m_target_city.GetCityData()->GetTradeSourceList() == NULL) ||
				 (m_target_city.GetCityData()->GetTradeSourceList()->Num() <= 0))
				return true;
			break;
		case k_Order_TargetPretest_TerrainImprovement_Bit:

			if (goal_record->GetTargetTypeImprovement())
			{
				Assert( m_target_city.m_id != 0);
				if (m_target_city.m_id == 0 ||
					!m_target_city->GetCityData()->WasTerrainImprovementBuilt())
					return true;
			}
			else
			{
				if (g_theWorld->GetCell(target_pos)->GetNumDBImprovements() <= 0)
					return true;
			}
			break;
	}

	if(g_player[m_playerId]->GetGold() < order_record->GetGold())
	{
		AI_DPRINTF(k_DBG_SCHEDULER, m_playerId, m_goal_type, 0,
		    ("GOAL %x (%s): Not enough gold to perform goal.\n", this, g_theGoalDB->Get(m_goal_type)->GetNameText()));

		return true;
	}

	if(goal_record->GetAvoidWatchfulCity())
	{
		Assert(m_target_city.m_id != 0);
		if(m_target_city->IsWatchful())
			return true;
	}

	if(goal_record->GetTargetTypeChokePoint())
	{
		CellUnitList army;

		g_theWorld->GetArmy(target_pos, army);
		if(g_theWorld->GetCell(target_pos)->GetNumUnits() > 0
		&& army.GetOwner() != m_playerId)
			return false;

		if(Is_Satisfied() && ArmiesAtGoal())
			return true;
	}

	if(order_record->GetUnitPretest_CanPlantNuke())
	{
		if (!g_player[m_playerId]->HasAdvance(advanceutil_GetNukeAdvance()))
			return true;
	}

	if(order_record->GetTargetPretestEnemySpecialUnit())
	{
		if(g_theWorld->GetOwner(target_pos) != m_playerId)
			return true;
	}

	if(!order_record->GetTargetPretestTerrainImprovement()
	&& !order_record->GetTargetPretestTradeRoute()
	){
		if(!ArmyData::TargetValidForOrder(order_record, target_pos))
			return true;
	}

	if(order_record->GetTargetPretestEnemySpecialUnit()
	&& m_target_army.m_id != 0
	&& m_target_army->CanBeExpelled()
	){
		return true;
	}

	if (order_record->GetUnitPretest_CanReformCity())
	{
		if (!m_target_city.GetCityData()->IsConverted())
			return true;
	}

	if (order_record->GetUnitPretest_CanSueFranchise())
	{
		if (!m_target_city.GetCityData()->IsFranchised())
			return true;
	}

	if (order_record->GetUnitPretest_EstablishEmbassy())
	{
		if (g_player[m_playerId]->HasEmbassyWith(m_target_city->GetOwner()))
			return true;
	}

	if (order_record->GetUnitPretest_CanCreateFranchise())
	{
		if (m_target_city.GetCityData()->GetFranchiseTurnsRemaining() <= 0)
			return true;
	}

	if (order_record->GetUnitPretest_CanBioTerror())
	{
		if (m_target_city.GetCityData()->IsBioInfected())
			return true;
	}

	if (order_record->GetUnitPretest_CanNanoInfect())
	{
		if (m_target_city.GetCityData()->IsNanoInfected())
			return true;
	}

	if (order_record->GetUnitPretest_CanConvertCity())
	{
		if (m_target_city.GetCityData()->GetConvertedTo() == m_playerId)
			return true;
	}

	if (order_record->GetUnitPretest_CanInterceptTrade())
	{
		Assert( m_target_city.m_id != 0);
		if ((m_target_city.m_id == 0) ||
			((m_target_city.GetCityData()->GetTradeSourceList()) &&
			(m_target_city.GetCityData()->GetTradeSourceList()->Num() <= 0)))
			return true;
	}

	if (order_record->GetUnitPretest_CanInciteRevolution())
	{
		sint32 cost;
		if (ArmyData::GetInciteRevolutionCost(target_pos, cost) &&
			( cost > g_player[m_playerId]->GetGold()))
			return true;
	}

	if (order_record->GetUnitPretest_CanNukeCity())
	{
		if (diplomat.GetNuclearLaunchTarget() == target_owner)
			return true;
	}

	//Try to steal technology only if the other civ has more advances than player
	//Otherwise, spy can do anything else
	if (order_record->GetUnitPretest_CanStealTechnology())
	{
		if (g_player[m_playerId]->NumAdvances() > g_player[target_owner]->NumAdvances())
			return true;
	}

	//Abolisionist has to go to cities with slaves
	if(order_record->GetUnitPretest_CanInciteUprising()
	|| order_record->GetUnitPretest_CanUndergroundRailway ()
	){
		if (m_target_city.GetCityData()->SlaveCount() == 0)
			return true;
	}

	if(order_record->GetUnitPretest_CanSlaveRaid())
	{
		sint32 popCount;
		m_target_city.GetCityData()->GetPop(popCount);

		// Slavers must to go to cities with population to enslave, give an extra point so that the city isn't killed on conquest
		if(popCount <= 2)
			return true;

		// Check city walls
	}

	if(order_record->GetUnitPretest_NoFuelThenCrash())
	{
		if(g_theUnitPool->IsValid(m_target_city)
		&& target_owner != m_playerId
		){
			return true;
		}
		else if(g_theArmyPool->IsValid(m_target_army)
		     && target_owner != m_playerId
		     ){
			return true;
		}
		else if(!g_theWorld->IsAirfield(target_pos)){
			return true;
		}
	}

	return false;
}

bool CTPGoal::Get_Invalid() const
{
	const GoalRecord *goal_record = g_theGoalDB->Get(m_goal_type);

	if(goal_record->GetTargetTypeAttackUnit()
	|| goal_record->GetTargetTypeSpecialUnit()
	){
		if(!g_theArmyPool->IsValid(m_target_army))
			return true;
		else if(m_target_army->Num() <= 0)
			return true;
		else
			return false;
	}

	if(goal_record->GetTargetTypeCity()
	|| goal_record->GetTargetTypeTradeRoute()
	|| goal_record->GetTargetTypeImprovement()
	){
		if(!g_theUnitPool->IsValid(m_target_city))
			return true;

		CityData *city = m_target_city->GetCityData();
		if(city == NULL)
		{
			Assert(false);
			return true;
		}

		if(goal_record->GetTargetOwnerSelf()
		&& city->GetOwner() != m_playerId)
			return true;
		else if(!goal_record->GetTargetOwnerSelf()
		     &&  city->GetOwner() == m_playerId)
			return true;
	}

	if(goal_record->GetTargetTypeEndgame())
	{
		return(!terrainutil_HasEndgame( Get_Target_Pos() ));
	}

	if (goal_record->GetTargetTypeUnexplored() )
		return(g_player[m_playerId]->IsExplored(Get_Target_Pos()));

	if(goal_record->GetTargetTypeSettleLand()
	|| goal_record->GetTargetTypeSettleSea()
	){
		if(!SettleMap::s_settleMap.CanSettlePos(Get_Target_Pos()))
			return true;

		if(g_theWorld->HasCity(Get_Target_Pos()))
			return true;
	}

	if(goal_record->GetTargetTypeGoodyHut())
		return(g_theWorld->GetGoodyHut(Get_Target_Pos()) == NULL);

	// Check whether the target can refuel the given army
	if(goal_record->GetTargetTypePetrolStation())
	{
		if(g_theUnitPool->IsValid(m_target_city))
		{
			CityData *city = m_target_city->GetCityData();
			if(city == NULL){
				Assert(0);
				return true;
			}

			if(goal_record->GetTargetOwnerSelf()
			&& city->GetOwner() != m_playerId)
				return true;
			else if(!goal_record->GetTargetOwnerSelf()
				 &&  city->GetOwner() == m_playerId)
				return true;
		}
//		else if(g_theArmyPool->IsValid(m_target_army)){ // Aircraft carriers are missing
//			return Cannot carry aircrafts;
//		}
		return !g_theWorld->IsAirfield(Get_Target_Pos());
	}

	return false;
}

bool CTPGoal::Get_Removal_Time() const
{
	if ( Get_Invalid() ||
		 (g_theGoalDB->Get(m_goal_type)->GetRemoveWhenComplete() && 
		  Get_Totally_Complete() ) )
		 return true;
	return false;
}

bool CTPGoal::Pretest_Bid(const Agent_ptr agent_ptr) const
{
	return Pretest_Bid(agent_ptr, MapPoint(-1,-1));
}

bool CTPGoal::Pretest_Bid(const Agent_ptr agent_ptr, const MapPoint & cache_pos) const
{
	CTPAgent_ptr ctpagent_ptr = (CTPAgent_ptr) agent_ptr;
	const Army & army = ctpagent_ptr->Get_Army();

	MapPoint target_pos;

	if (cache_pos == MapPoint(-1,-1))
		target_pos = Get_Target_Pos(army);
	else
		target_pos = cache_pos;

	const GoalRecord *goal_rec = g_theGoalDB->Get(m_goal_type);

	const OrderRecord *order_rec = goal_rec->GetExecute();

	if (army->GetMinFuel() != 0x7fffffff)
	{
		sint32 num_tiles_to_half;
		sint32 num_tiles_to_empty;
		army->CalcRemainingFuel(num_tiles_to_half, num_tiles_to_empty);

		num_tiles_to_empty = static_cast<sint32>(num_tiles_to_empty / k_MOVE_AIR_COST);
		num_tiles_to_half = static_cast<sint32>(num_tiles_to_half / k_MOVE_AIR_COST);

		sint32 distance_to_refuel;
		sint32 distance_to_target;
		MapPoint refuel_pos;
		CtpAi::GetNearestRefuel(army, target_pos, refuel_pos, distance_to_refuel);

		distance_to_target = 
		    static_cast<sint32>(sqrt(static_cast<double>
		    (MapPoint::GetSquaredDistance(army->RetPos(), target_pos))
		    ));

		if (num_tiles_to_empty < distance_to_target + distance_to_refuel)
			return false;
	}

	if (goal_rec->GetSquadClassCanBombard())
	{
		static CellUnitList defenders;
		defenders.Clear();
		g_theWorld->GetArmy(target_pos, defenders);
		if(!ctpagent_ptr->Get_Army()->CanBombardTargetType(defenders))
			return false;
	}

	sint32 transports;
	sint32 max_cargo_slots;
	sint32 empty_cargo_slots;

	army->GetCargo(transports, max_cargo_slots, empty_cargo_slots);

	if (transports > 0 )
	{
		if ((empty_cargo_slots < max_cargo_slots) || (army->HasCargo()))
		{
			ORDER_TEST cargo_test = army->CargoTestOrderHere(order_rec, target_pos);
			if ( cargo_test == ORDER_TEST_ILLEGAL ||
				 cargo_test == ORDER_TEST_INVALID_TARGET )
				return false;
			else
				return true;
		}
		else if (Needs_Transporter())
			return true;
	}

	bool needs_land_unit  = goal_rec->GetTargetTypeCity();
	     needs_land_unit &= goal_rec->GetTargetOwnerSelf();
	     needs_land_unit |= goal_rec->GetTargetTypeGoodyHut();
	
	uint32 movement_type  = army->GetMovementType();
	if (army->HasCargo())
	       movement_type |= army->GetCargoMovementType();

	bool land_movement = ((movement_type & 
	        (k_Unit_MovementType_Land_Bit | k_Unit_MovementType_Mountain_Bit)) != 0x0);
	land_movement &= ((movement_type & k_Unit_MovementType_Air_Bit) == 0x0);

	if (needs_land_unit && !land_movement)
	{
		return false;
	}

	if ( army->CanNukeCity() && 
		 (g_theWorld->GetCell(target_pos)->GetOwner() != m_playerId ||
		  g_theWorld->GetCity(target_pos).m_id != 0x0) )
		return false;

	if ( army->TestOrder(order_rec) == ORDER_TEST_ILLEGAL )
		return false;

	if ( !army->CanEnter(target_pos) )
		return false;

	return true;
}

void CTPGoal::Log_Debug_Info(const int &log) const
{
#if defined(_DEBUG) || defined(USE_LOGGING)

	const char *name = g_theGoalDB->Get(m_goal_type)->GetNameText();

	MapPoint pos  = Get_Target_Pos();

	if(m_raw_priority > BAD_UTILITY)
	{
		AI_DPRINTF
		          (
		           log,
		           m_playerId,
		           -1,
		           -1,
		           (
		                "\tGoal %9x,\t%s,\tRaw priority: %8d,\t(%3d,%3d) (%s)\n",
		                this,
		                name,
		                m_raw_priority,
		                pos.x,
		                pos.y,
		                (g_theWorld->HasCity(pos) ? g_theWorld->GetCity(pos).GetName() : "field")
		           )
		          );
	}
	else
	{
		MapPoint pos = Get_Target_Pos();

		AI_DPRINTF
		          (
		           log,
		           m_playerId,
		           -1,
		           -1,
		           (
		                "\tGoal %9x,\t%s,\tBAD_UTILITY,\t(%d,%d) (%s)\n",
		                this,
		                name,
		                pos.x,
		                pos.y,
		                (g_theWorld->HasCity(pos) ? g_theWorld->GetCity(pos).GetName() : "field")
		           )
		          );
	}

	Plan_List::const_iterator plan_iter;
	sint32 count = 0;
	for
	(
	      plan_iter  = m_matches.begin();
	      plan_iter != m_matches.end();
	    ++plan_iter
	)
	{
		Squad_ptr squad = plan_iter->Get_Squad();
		Utility value   = plan_iter->Get_Matching_Value();

		if(value > Goal::BAD_UTILITY)
		{
			AI_DPRINTF(log, m_playerId, -1, -1,
				("\t\t[%d] match=%d %s (squad: %x)\n",
					count++, value, g_theGoalDB->Get(m_goal_type)->GetNameText(), squad));

			AI_DPRINTF(log, m_playerId, -1, -1, ("\t\tSquad:\n"));
			squad->Log_Debug_Info(k_DBG_SCHEDULER_ALL, this);
		}
		else
		{
			AI_DPRINTF
			          (
			           log,
			           m_playerId,
			           -1,
			           -1,
			             (
			              "\t\t[%d] First match with bad utility: In all, there were %d matches with bad utility.\n",
			              count,
			              m_matches.size() - count
			             )
			            );

			break;
		}
	}

	bool log_army = false;
	bool log_goal = CtpAiDebug::DebugLogCheck(m_playerId, m_goal_type, -1);

	Agent_List::const_iterator agent_iter;
	for
	(
	      agent_iter  = m_agents.begin();
	      agent_iter != m_agents.end();
	    ++agent_iter
	)
	{
		CTPAgent_ptr agent_ptr = (CTPAgent_ptr) *agent_iter;

		if(CtpAiDebug::DebugLogCheck(m_playerId, m_goal_type, agent_ptr->Get_Army().m_id))
		{
			log_army = true;

			pos = Get_Target_Pos(agent_ptr->Get_Army());
			break;
		}
	}

	if(!log_army || !log_goal)
	{
		return;
	}

	if (m_agents.size() > 0)
		DPRINTF(log,("\t\t\tCommitted Agents (%d):\n", m_agents.size()));

	for( agent_iter  = m_agents.begin();
		 agent_iter != m_agents.end();
		 agent_iter++
	){
		CTPAgent_ptr agent_ptr = (CTPAgent_ptr) *agent_iter;
		agent_ptr->Log_Debug_Info(log, this);
	}

#endif // _DEBUG
}


bool CTPGoal::ReferencesAgent(const CTPAgent *ctp_agent) const
{
#ifdef _DEBUG
	for(Agent_List::const_iterator agent_iter  = m_agents.begin();
	                               agent_iter != m_agents.end();
	                               agent_iter++)
	{
		
		CTPAgent_ptr tmp_ctp_agent = (CTPAgent_ptr) *agent_iter;
		if (ctp_agent == tmp_ctp_agent)
			return true;
	}
#endif // _DEBUG

	return false;
}

// Garrison needs to be calculated differently
// Probably needs to go the scheduler
bool CTPGoal::NeededForGarrison(CTPAgent_ptr army,
								const MapPoint &dest_pos,
								sint8 & new_garrison,
								double & new_garrison_strength) const
{
	Unit city = g_theWorld->GetCity( army->Get_Pos() );

	if (city.m_id == 0)
		return false;

	// Needs to be computed since the result is used, even if we return false.
	new_garrison = city->GetCityData()->GetCurrentGarrison();
	new_garrison_strength = city->GetCityData()->GetCurrentGarrisonStrength();

	if (( army->Get_Army()->GetMovementType() & 
		  (k_Unit_MovementType_Land_Bit | k_Unit_MovementType_Mountain_Bit)) == 0)
	{
		return false;
	}

	if ( dest_pos == army->Get_Pos() )
		return false;

	if (((army->Get_Squad_Class() & k_Goal_SquadClass_CanDefend_Bit   ) != 0x0 ) &&
		((army->Get_Squad_Class() & k_Goal_SquadClass_Special_Bit     ) == 0x0 ) &&
		((army->Get_Squad_Class() & k_Goal_SquadClass_CanTransport_Bit) == 0x0 ))
	{
		sint16 defense_count;
		double defense_strength;
		sint16 tmpCount;
		double tmp;
		army->Get_Army()->ComputeStrength(tmp,
		                                  defense_strength,
		                                  tmp,
		                                  defense_count,
		                                  tmpCount,
		                                  tmp,
		                                  tmp,
		                                  tmp,
		                                  tmp,
		                                  false
		                                 );

		defense_strength += city->GetCityData()->GetDefendersBonus() * static_cast<double>(defense_count);

		new_garrison -= static_cast<sint8>(defense_count);
		new_garrison_strength -= defense_strength;

		if(new_garrison >= city->GetCityData()->GetNeededGarrison()
		&& new_garrison_strength >= city->GetCityData()->GetNeededGarrisonStrength()
		){
			return false;
		}
		return true;
	}

	return false;
}

bool CTPGoal::FollowPathToTask( CTPAgent_ptr first_army,
                                CTPAgent_ptr second_army,
                                const MapPoint &dest_pos,
                                const Path &found_path)
{
	Unit city = g_theWorld->GetCity(first_army->Get_Pos());
	if(city.m_id != 0)
	{
		if(first_army->Get_Pos() != dest_pos)
		{
			sint8 new_garrison           = 0;
			double new_garrison_strength = 0.0;
			if(NeededForGarrison(first_army, dest_pos, new_garrison, new_garrison_strength))
			{
				AI_DPRINTF(k_DBG_SCHEDULER, m_playerId, m_goal_type, first_army->Get_Army().m_id,
					("GOAL %x (%d): FollowPathToTask::Can not send army needed for garrison to destination (x=%d,y=%d):\n", this,
					m_goal_type, dest_pos.x, dest_pos.y));
				first_army->Log_Debug_Info(k_DBG_SCHEDULER, this);
				uint8 magnitude = 220;
				g_graphicsOptions->AddTextToArmy(first_army->Get_Army(), "GARRISON", magnitude);
				return false;
			}
			else
			{
				city->GetCityData()->SetCurrentGarrison(new_garrison);
				city->GetCityData()->SetCurrentGarrisonStrength(new_garrison_strength);
			}
		}
	}

	const GoalRecord *goal_rec = g_theGoalDB->Get(m_goal_type);
	const OrderRecord *order_rec;
	ORDER_TEST test = ORDER_TEST_OK;

	if (m_sub_task == SUB_TASK_GOAL
	||  m_sub_task == SUB_TASK_TRANSPORT_TO_GOAL) // @ToDo clean this
	{
		order_rec = goal_rec->GetExecute();
		if (first_army->Get_Army()->HasCargo() )
		{
			test = first_army->Get_Army()->CargoTestOrderHere(order_rec, dest_pos );

			if(first_army->Get_Army()->GetMovementTypeAir())
			{
				order_rec = CtpAi::GetUnloadOrder();
			}
			
			if(first_army->Get_Pos() == dest_pos
			&& city.m_id != 0x0
			){
				order_rec = CtpAi::GetUnloadOrder();
			}
		}
		else
		{
			test = first_army->Get_Army()->TestOrderHere(order_rec, dest_pos );
		}
	}
	else
	{
		order_rec = CtpAi::GetMoveOrder();
	}

	if(test == ORDER_TEST_OK
	|| test == ORDER_TEST_NO_MOVEMENT
	){

		//I want to see armytext even in optimized test version - Calvitix
		Utility val = Compute_Matching_Value(first_army);
		uint8 magnitude = (uint8) (((5000000 - val)* 255.0) / 5000000);
		const char * myText = goal_rec->GetNameText();
		MBCHAR * myString   = new MBCHAR[strlen(myText) + 40];
		MBCHAR * goalString = new MBCHAR[strlen(myText) + 20];
		memset(goalString, 0, strlen(myText) + 20);
		memset(myString,   0, strlen(myText) + 40);

		for (uint8 myComp = 0; myComp < strlen(myText) - 5; myComp++)
		{
			goalString[myComp] = myText[myComp + 5];
		}

		MapPoint targetPos = Get_Target_Pos(first_army->Get_Army());
		switch (m_sub_task)
		{
			case SUB_TASK_RALLY:
				sprintf(myString, "Group to (%d,%d), %s (%d,%d)", dest_pos.x, dest_pos.y, goalString, targetPos.x, targetPos.y);
				break;
			case SUB_TASK_TRANSPORT_TO_BOARD:
				sprintf(myString, "Boat to (%d,%d), %s (%d,%d)", dest_pos.x, dest_pos.y, goalString, targetPos.x, targetPos.y);
				break;
			case SUB_TASK_TRANSPORT_TO_GOAL:
				sprintf(myString, "Transp. to (%d,%d), %s (%d,%d)", dest_pos.x, dest_pos.y, goalString, targetPos.x, targetPos.y);
				break;
			case SUB_TASK_CARGO_TO_BOARD:
				sprintf(myString, "Cargo. to (%d,%d), %s (%d,%d)", dest_pos.x, dest_pos.y, goalString, targetPos.x, targetPos.y);
				break;
			case SUB_TASK_AIRLIFT:
				sprintf(myString, "Airlift to (%d,%d), %s (%d,%d)", dest_pos.x, dest_pos.y, goalString, targetPos.x, targetPos.y);
				break;
			case SUB_TASK_GOAL:
			default:
				sprintf(myString, "%s (%d,%d)", goalString, dest_pos.x, dest_pos.y);
				break;
		}

		g_graphicsOptions->AddTextToArmy(first_army->Get_Army(), myString, magnitude, m_goal_type);
		delete[] myString;
		delete[] goalString;

		if (first_army->Get_Can_Be_Executed())
		{
			Assert(order_rec);
			if (order_rec)
			{
				if(m_sub_task != SUB_TASK_CARGO_TO_BOARD)
				{
					first_army->PerformOrderHere(order_rec, (Path *) &found_path);
				}
				else
				{
					first_army->PerformOrderHere(order_rec, (Path *) &found_path, GEV_INSERT_Tail);
				}
			}
		}
		else
		{
			// Nothing
		}

		return true;
	}
	else
	{
		const char * myText = goal_rec->GetNameText();
		MBCHAR * myString = new MBCHAR[strlen(myText) + 40];
		memset(myString, 0, strlen(myText) + 40);
		sprintf(myString, "%s failed at (%d, %d)", goal_rec->GetNameText(), dest_pos.x, dest_pos.y);

		g_graphicsOptions->AddTextToArmy(first_army->Get_Army(), myString, 0, m_goal_type);
		delete[] myString;

		if(test != ORDER_TEST_OK && test == ORDER_TEST_NO_MOVEMENT)
		{
			AI_DPRINTF(k_DBG_SCHEDULER, m_playerId, m_goal_type, first_army->Get_Army().m_id,
				("GOAL %x (%d): FollowPathToTask:: failed TestOrderHere( %s, (%d,%d))\n", this, m_goal_type,
				order_rec->GetNameText(),dest_pos.x,dest_pos.y));
		}

		return false;
	}

	return true;
}

bool CTPGoal::GotoTransportTaskSolution(CTPAgent_ptr the_army, CTPAgent_ptr the_transport, MapPoint & pos)
{
	MapPoint dest_pos;
	Path found_path;
	bool check_dest;

	bool found = false;

	switch (m_sub_task)
	{

	case SUB_TASK_AIRLIFT:
	{
		MapPoint start_pos = the_army->Get_Pos(); 

		sint16 cargo_cont = g_theWorld->GetContinent(start_pos); // Dangerous with transport target can be closer

		Unit nearest_city;
		MapPoint nearest_airfield;
		double airfield_distance = 0.0;
		double     city_distance = 0.0;
		bool airfield_found = g_player[m_playerId]->
		                      GetNearestAirfield(start_pos, nearest_airfield, cargo_cont);
		bool     city_found = g_player[m_playerId]->
		                      GetNearestCity(start_pos, nearest_city, city_distance, false);
		if (airfield_found)
		{
			airfield_distance = MapPoint::GetSquaredDistance(start_pos, nearest_airfield);
		}

		if (airfield_found && city_found && (airfield_distance < city_distance))
			dest_pos = nearest_airfield;
		else if (city_found)
		{
			dest_pos = nearest_city.RetPos();
		}
		else
		{
			return false;
		}

		if (dest_pos == start_pos &&
			dest_pos == the_transport->Get_Army()->RetPos())
		{
			the_army->MoveIntoTransport();

			the_transport->Set_Can_Be_Executed(false);

			return true;
		}

		check_dest = true;

		found = CTPAgent::FindPath(the_army->Get_Army(), dest_pos, check_dest, found_path);

		if (found && FollowPathToTask(the_army, the_transport, dest_pos, found_path) )
		{
			// move_intersection = the_transport->Get_Army().GetMovementType();

			found = CTPAgent::FindPath(the_transport->Get_Army(), dest_pos, check_dest, found_path);

			if (found && FollowPathToTask(the_transport, the_army, dest_pos, found_path) )
				return true;
		}

		break;
	}
	case SUB_TASK_TRANSPORT_TO_BOARD:
	{
		if(pos == MapPoint(-1, -1))
		{
			dest_pos = the_army->Get_Pos();
			check_dest = false;
		}
		else
		{
			dest_pos = pos;
			check_dest = true;
		}

		if(dest_pos == the_transport->Get_Pos()
		|| the_transport->Get_Army()->CheckValidDestination(dest_pos)
		){
			if(pos == MapPoint(-1, -1))
			{
				pos = dest_pos;
			}

			the_transport->Set_Target_Pos(dest_pos);
			return true;
		}

			uint32 move_intersection = 
				the_transport->Get_Army().GetMovementType() | the_army->Get_Army().GetMovementType();

			found = the_transport->FindPathToBoard(move_intersection, dest_pos, check_dest, found_path);

		if (!found)
		{
			AI_DPRINTF(k_DBG_SCHEDULER, m_playerId, m_goal_type, the_transport->Get_Army().m_id,
			        ("GOAL %x (%d):GotoTransportTaskSolution:: No path found from army to destination (x=%d,y=%d) (SUB_TASK_TRANSPORT_TO_BOARD):\n",
			        this, m_goal_type, dest_pos.x, dest_pos.y));
			the_transport->Log_Debug_Info(k_DBG_SCHEDULER, this);
			        uint8 magnitude = 220;
			        MBCHAR * myString = new MBCHAR[40];
			        sprintf(myString, "NO PATH -> BOARD (%d,%d)", dest_pos.x, dest_pos.y);
			        g_graphicsOptions->AddTextToArmy(the_army->Get_Army(), myString, magnitude, m_goal_type);
			        delete[] myString;
		}
		else
		{
			MapPoint last = found_path.SnipEndUntilCanEnter(the_transport->Get_Army());

			if ( (found_path.GetMovesRemaining() > 0) &&
				 !FollowPathToTask(the_transport, the_army, last, found_path) )
				return false;

			if(pos == MapPoint(-1, -1))
			{
				pos = last;
			}

			return true;
		}

		break;
	}
	case SUB_TASK_CARGO_TO_BOARD:
	{
		dest_pos = pos;
		MapPoint start_pos = the_army->Get_Pos();

		Assert(pos != MapPoint(-1, -1));

		if (dest_pos == start_pos)
		{
			the_army->MoveIntoTransport();

			return true;
		}

		bool transport_at_rendezvous;
		if (! the_transport->Get_Army()->AtEndOfPath() )
		{
			// This maybe a problem
			MapPoint next_pos;

			transport_at_rendezvous  =  the_transport->Get_Army()->GetNextPathPoint(next_pos);
			transport_at_rendezvous &= !the_transport->Get_Army()->CanEnter(next_pos);
		}
		else
		{
			transport_at_rendezvous = true;
			
		}

		if (transport_at_rendezvous )
		{
			uint32 move_intersection = the_army->Get_Army().GetMovementType();

			check_dest = true;

			found =  the_army->FindPathToBoard(move_intersection, dest_pos, check_dest, found_path);
		}
		else
		{
			check_dest =  false;

			found = CTPAgent::FindPath(the_army->Get_Army(), dest_pos, check_dest, found_path); 

			if (!found)
			{
				uint32 move_intersection = the_transport->Get_Army().GetMovementType() |
				                                the_army->Get_Army().GetMovementType();

				found =  the_army->FindPathToBoard(move_intersection, dest_pos, check_dest, found_path);
			}
		}

		if (!found)
		{
			AI_DPRINTF(k_DBG_SCHEDULER, m_playerId, m_goal_type, the_army->Get_Army().m_id,
			        ("GOAL %x (%d): GotoTransportTaskSolution: No path found from army to destination (x=%d,y=%d) (SUB_TASK_CARGO_TO_BOARD):\n",
			        this, m_goal_type, dest_pos.x, dest_pos.y));
			the_army->Log_Debug_Info(k_DBG_SCHEDULER, this);
			        uint8 magnitude = 220;
			        MBCHAR * myString = new MBCHAR[40];
			        sprintf(myString, "NO PATH -> BOARD (%d,%d)", dest_pos.x, dest_pos.y);
			        g_graphicsOptions->AddTextToArmy(the_army->Get_Army(), myString, magnitude, m_goal_type);
			        delete[] myString;
		}

		if (found && FollowPathToTask(the_army, the_transport, dest_pos, found_path) )
			return true;

		break;
	}
	default:

		Assert(false);
	}

	return false;
}

bool CTPGoal::GotoGoalTaskSolution(CTPAgent_ptr the_army, const MapPoint & goal_pos)
{
	if (the_army->Get_Army()->CheckValidDestination(goal_pos)) // If we are already moving along a path
		return true;

	Path found_path;

	sint32 range = 0;
	(void) g_theGoalDB->Get(m_goal_type)->GetExecute()->GetRange(range);
	
	
	bool check_dest;
	const GoalRecord * goal_rec = g_theGoalDB->Get(m_goal_type);
	if (range > 0  || 
		goal_rec->GetExecute()->GetTargetPretestAttackPosition() ||
		(goal_rec->GetTargetTypeCity() && goal_rec->GetTargetOwnerSelf()))
		check_dest = false;
	else
		check_dest = true;


	bool     waiting_for_buddies = !Ok_To_Rally()
	                            && m_sub_task == SUB_TASK_RALLY
	                            && g_theWorld->IsOnSameContinent(goal_pos, the_army->Get_Pos())
	                            && g_theWorld->IsLand(goal_pos)
	                            && g_theWorld->IsLand(the_army->Get_Pos());

	bool     found               = false;

	if(the_army->Get_Army()->HasCargo()
	&& the_army->Get_Army()->GetMovementTypeAir()
	&& the_army->Get_Army()->CanSpaceLaunch()
	){
		sint16 target_cont = g_theWorld->GetContinent(goal_pos);

		Unit   nearest_city;
		double city_distance = 0.0;
		bool   city_found    = g_player[m_playerId]->
		                       GetNearestCity(goal_pos, nearest_city, city_distance, false, target_cont);
		
		if (city_found)
		{
			found = CTPAgent::FindPath(the_army->Get_Army(), nearest_city.RetPos(), true, found_path); 
			if (found) Set_Sub_Task(SUB_TASK_AIRLIFT);

			if (the_army->Get_Pos() == nearest_city.RetPos())
			{
				the_army->PerformOrderHere(CtpAi::GetUnloadOrder(), (Path *) &found_path);
			}
			else if(!the_army->Get_Army()->HasLeftMap())
			{
				the_army->PerformOrderHere(CtpAi::GetSpaceLaunchOrder(), (Path *) &found_path);
			}

			return true;
		}
	}
	else if ( the_army->Get_Army()->HasCargo()
	     &&  !the_army->Get_Army()->GetMovementTypeAir())
	{
		// Check if is single squad
		// Return true if we are a transporter and we need transporters
		// SUB_TASK_TRANSPORT_TO_GOAL
		uint32 move_intersection = 
		        the_army->Get_Army()->GetMovementType() | the_army->Get_Army()->GetCargoMovementType();

		found = the_army->FindPathToBoard(move_intersection, goal_pos, false, found_path);

		Assert(found);

		if (found)
		{
			Set_Sub_Task(SUB_TASK_TRANSPORT_TO_GOAL);
		}
	}
	else if ( the_army->Get_Army()->CanTransport()
	     &&  !the_army->Get_Army()->GetMovementTypeAir()
	     &&  !the_army->Get_Army()->CanEnter(goal_pos) )
	{
		// There is still a problem with air transporters
		return true;
	}
	else
	{
		if (!waiting_for_buddies)
		{
			found = CTPAgent::FindPath(the_army->Get_Army(), goal_pos, check_dest, found_path);
		}
	}

	switch (m_sub_task) {
		case SUB_TASK_GOAL:
			if (!found)
			{
				AI_DPRINTF(k_DBG_SCHEDULER, m_playerId, m_goal_type, the_army->Get_Army().m_id,
				           ("GOAL %x (%s): GotoGoalTaskSolution: No path found from army to goal (x=%d,y=%d) (SUB_TASK_GOAL):\n",
				            this, g_theGoalDB->Get(m_goal_type)->GetNameText(), goal_pos.x, goal_pos.y));
				the_army->Log_Debug_Info(k_DBG_SCHEDULER, this);
				uint8 magnitude = 220;
				MBCHAR * myString = new MBCHAR[256];
				sprintf(myString, "NO PATH to (%d,%d) - %s", goal_pos.x, goal_pos.y, g_theGoalDB->Get(m_goal_type)->GetNameText());
				g_graphicsOptions->AddTextToArmy(the_army->Get_Army(), myString, magnitude, m_goal_type);

				delete[] myString;
			}

			break;

		case SUB_TASK_RALLY:

			if (waiting_for_buddies)
			{
				Utility val = Compute_Matching_Value(the_army);
				uint8 magnitude = (uint8)(((5000000 - val) * 255.0) / 5000000);
				MBCHAR * myString = new MBCHAR[40];
				sprintf(myString, "Waiting GROUP to GO (%d,%d)\n", goal_pos.x, goal_pos.y);
				g_graphicsOptions->AddTextToArmy(the_army->Get_Army(), myString, magnitude, m_goal_type);
				delete[] myString;

				return true;
			}

			if (!found)
			{
				AI_DPRINTF(k_DBG_SCHEDULER, m_playerId, m_goal_type, the_army->Get_Army().m_id,
				           ("GOAL %x (%d):GotoGoalTaskSolution: No path found from army to goal (x=%d,y=%d) (SUB_TASK_RALLY):\n",
				           this, m_goal_type, goal_pos.x, goal_pos.y));
				the_army->Log_Debug_Info(k_DBG_SCHEDULER, this);
				uint8 magnitude = 220;
				MBCHAR * myString = new MBCHAR[40];
				sprintf(myString, "NO PATH (GROUP)(%d,%d)", goal_pos.x, goal_pos.y);
				g_graphicsOptions->AddTextToArmy(the_army->Get_Army(), myString, magnitude, m_goal_type);
				delete[] myString;
			}

			break;

		default:
		{
			if (!found)
			{
				AI_DPRINTF(k_DBG_SCHEDULER, m_playerId, m_goal_type, the_army->Get_Army().m_id,
				           ("GOAL %x (%d):GotoGoalTaskSolution: No path found from army to goal (x=%d,y=%d) (SUB_TASK_TRANSPORT:\n",
				           this, m_goal_type, goal_pos.x, goal_pos.y));
				the_army->Log_Debug_Info(k_DBG_SCHEDULER, this);
				uint8 magnitude = 220;
				MBCHAR * myString = new MBCHAR[40];
				sprintf(myString, "NO PATH (TRANSP.)(%d,%d)", goal_pos.x, goal_pos.y);
				g_graphicsOptions->AddTextToArmy(the_army->Get_Army(), myString, magnitude, m_goal_type);
				delete[] myString;
			}
		}
	}

	bool move_success = false;
	if ( found )
	{
		move_success = FollowPathToTask(the_army, NULL, goal_pos, found_path);
	}
	else
	{
		move_success = TryTransport(the_army, goal_pos);
	}

	if (move_success)
	{
		the_army->Set_Can_Be_Executed(false);
	}

	return move_success;
}

bool CTPGoal::Ok_To_Rally() const
{
	if (m_agents.size() == 1)
		return false;

	if (!g_theWorld->IsLand(Get_Target_Pos())) // Problematic if we want to attack a stack of Destroyers
		return true;

	sint32          num_in_water    = 0;
	sint32          num_at_dest     = 0;
	sint32          count           = 0;
	MapPoint        targetPos       = Get_Target_Pos();

	for
	(
	    Agent_List::const_iterator agent_iter  = m_agents.begin(); 
	                               agent_iter != m_agents.end();
	                             ++agent_iter
	)
	{
		const CTPAgent_ptr ctpagent_ptr = (CTPAgent_ptr) *agent_iter;
		Assert(ctpagent_ptr);
		
		if(!ctpagent_ptr->Get_Is_Dead())
		{
			MapPoint const	army_pos		= ctpagent_ptr->Get_Pos();
			
			if (g_theWorld->IsLand(army_pos))
			{
				if(g_theWorld->IsOnSameContinent(army_pos, targetPos))
				{
					++num_at_dest;
				}
				// Would have been removed for the outcommented stuff below
				else
				{
					return false;
				}
			}
			else
			{
				++num_in_water;
			}

			++count;
		}
	}
/*	//So far this isn't good it makes transporters to take part in the rally
	if(m_current_needed_strength.Get_Transport() > 0)
	{
		if(!Needs_Transporter())
		{
			return true;
		}
	}
*/
	return (num_in_water + num_at_dest) == count;
}

bool CTPGoal::RallyComplete() const
{
	if (m_agents.size() == 1)
		return true;

	bool incompleteStackFound   = false;

	for
	(
	    Agent_List::const_iterator agent_iter  = m_agents.begin();
	                               agent_iter != m_agents.end(); 
	                             ++agent_iter
	)
	{
		CTPAgent_ptr ctpagent_ptr   = (CTPAgent_ptr) *agent_iter;
		Assert(ctpagent_ptr);
		
		if (ctpagent_ptr->Get_Is_Dead())
			continue;

		if(!ctpagent_ptr->IsArmyPosFilled())
		{
			if(incompleteStackFound)
			{
				return false;
			}

			incompleteStackFound = true;
		}
	}

	return true;
}

void CTPGoal::GroupTroops()
{
	// Maybe sort the agent list by id
	// so that we use the lower ids.

	for
	(
	    Agent_List::iterator agent1_iter  = m_agents.begin();
	                         agent1_iter != m_agents.end();
	                       ++agent1_iter
	)
	{
		CTPAgent_ptr ctpagent1_ptr = (CTPAgent_ptr) *agent1_iter;

		if
		  (
		        ctpagent1_ptr->GetUnitsAtPos() == ctpagent1_ptr->Get_Army()->Num()
		    ||  ctpagent1_ptr->Get_Is_Dead()
		    || !ctpagent1_ptr->Get_Can_Be_Executed()
		  )
		{
			continue;
		}

		for
		(
		    Agent_List::iterator agent2_iter  = m_agents.begin();
		                         agent2_iter != m_agents.end();
		                       ++agent2_iter
		)
		{
			CTPAgent_ptr ctpagent2_ptr = (CTPAgent_ptr) *agent2_iter;

			if( ctpagent2_ptr->Get_Is_Dead()
			|| !ctpagent2_ptr->Get_Can_Be_Executed()
			||  ctpagent1_ptr == ctpagent2_ptr
			){
				continue;
			}

			if(ctpagent1_ptr->Get_Pos() == ctpagent2_ptr->Get_Pos())
			{
				ctpagent1_ptr->Group_With(ctpagent2_ptr);
			}
		}
	}
}

MapPoint CTPGoal::MoveOutOfCity(CTPAgent_ptr rallyAgent)
{
	MapPoint rallyPos = rallyAgent->Get_Pos();
	if(g_theWorld->GetCity(rallyPos).IsValid())
	{
		MapPoint tempPos;
		for(sint32 i = 0 ; i < NOWHERE; i++)
		{
			bool result = rallyPos.GetNeighborPosition(WORLD_DIRECTION(i), tempPos);
			if(result)
			{
				CellUnitList *the_army = NULL;
				the_army = g_theWorld->GetArmyPtr(tempPos);
				if(!the_army
				&& rallyAgent->Get_Army()->CanEnter(tempPos)
				){	//search for cell without army
					rallyPos = tempPos;
					break;
				}
			}
		}

		if(!GotoGoalTaskSolution(rallyAgent, rallyPos)) // Problem units are lost to garrison
			Assert(false);
	}

	return rallyPos;
}

CTPAgent_ptr CTPGoal::GetRallyAgent() const
{
	MapPoint targetPos                 = Get_Target_Pos();
	CTPAgent_ptr rallyAgent            = NULL;
	CTPAgent_ptr rallyFriendlyAgent    = NULL;
	sint32 minDistance                 = 0x7fffffff;
	sint32 minFriendlyDistance         = 0x7fffffff;

	for
	(
	    Agent_List::const_iterator agent_iter  = m_agents.begin();
	                               agent_iter != m_agents.end();
	                             ++agent_iter
	)
	{
		CTPAgent_ptr ctpagent_ptr = (CTPAgent_ptr) *agent_iter;

		if(ctpagent_ptr->GetUnitsAtPos() >= k_MAX_ARMY_SIZE)
		{
			continue;
		}

		sint32 distance = MapPoint::GetSquaredDistance(ctpagent_ptr->Get_Pos(), targetPos);

		if(distance < minDistance)
		{
			minDistance = distance;
			rallyAgent = ctpagent_ptr;
		}

		if
		  (
		       distance < minFriendlyDistance
		    && g_theWorld->IsOnSameContinent(ctpagent_ptr->Get_Pos(), targetPos)
		  )
		{
			minFriendlyDistance = distance;
			rallyFriendlyAgent  = ctpagent_ptr;
		}
	}

	return (minFriendlyDistance < 0x7fffffff) ? rallyFriendlyAgent : rallyAgent;
}

MapPoint CTPGoal::GetFreeNeighborPos(MapPoint pos) const
{
	MapPoint nextPos(-1, -1);
	MapPoint tempPos;
	for(sint32 i = 0 ; i < NOWHERE; i++)
	{
		bool result = pos.GetNeighborPosition(WORLD_DIRECTION(i), tempPos);
		if(result)
		{
			CellUnitList *the_army = g_theWorld->GetArmyPtr(tempPos);
			if(!the_army)
			{	//search for cell without army
				nextPos = tempPos;
				break;
			}
		}
	}

	// Could happen in that case we have to find something else
	// Assert(nextPos != MapPoint(-1, -1));

	return nextPos;
}

bool CTPGoal::RallyTroops()
{
	Set_Sub_Task(SUB_TASK_RALLY);

// A new rally algorithm, but actually the old one is better in principle
// but unfortunately less complete
#if 1
	// Group armies first
	GroupTroops();
	CTPAgent_ptr rallyAgent = GetRallyAgent();

	if(rallyAgent == NULL)
	{
		return true;
	}

	MapPoint     rallyPos   = MoveOutOfCity(rallyAgent);

	rallyAgent->WaitHere(Get_Target_Pos(rallyAgent->Get_Army()));

	Squad_Strength strength = m_current_attacking_strength;

	sint32 unitsAtRallyPos = (rallyPos == rallyAgent->Get_Pos()) ? rallyAgent->GetUnitsAtPos() : rallyAgent->Get_Army()->Num();
	for
	(
	    Agent_List::iterator agent_iter  = m_agents.begin();
	                         agent_iter != m_agents.end();
	                       ++agent_iter
	)
	{
		CTPAgent_ptr ctpagent_ptr = (CTPAgent_ptr) *agent_iter;
		Assert(ctpagent_ptr);

		if( ctpagent_ptr->Get_Is_Dead()
		|| !ctpagent_ptr->Get_Can_Be_Executed()
		|| !ctpagent_ptr->CanMove()
		||  ctpagent_ptr == rallyAgent
		){
			continue;
		}

		if
		  (
		       ctpagent_ptr->Get_Army()->Num() >= k_MAX_ARMY_SIZE
		    && rallyPos.IsNextTo(ctpagent_ptr->Get_Pos())
		  )
		{
			continue;
		}

		// Send units to rally position
		if(!GotoGoalTaskSolution(ctpagent_ptr, rallyPos))
		{
			// @ToDo: Check for transports, too.
			strength -= ctpagent_ptr->Get_Squad_Strength();
			if(strength.HasEnough(m_current_needed_strength))
			{
				continue;
			}
			else
			{
				return false;
			}
		}

		// Count units at target position or sent to target position
		unitsAtRallyPos += ctpagent_ptr->Get_Army()->Num();

		// If target position is full or will be full
		if(unitsAtRallyPos > k_MAX_ARMY_SIZE)
		{
			unitsAtRallyPos -= k_MAX_ARMY_SIZE;

			// Change target
			if(ctpagent_ptr->Get_Army()->Num() < k_MAX_ARMY_SIZE)
			{
				rallyPos = GetFreeNeighborPos(rallyPos);

				if(rallyPos == MapPoint(-1, -1))
				{
					Assert(false);
					// Find another rally point
				}

				// Outgroup the units over limit
				// Outgrouping is done before the army is send to their target
				ctpagent_ptr->Get_Army()->RemainNumUnits(ctpagent_ptr->Get_Army()->Num() - unitsAtRallyPos);
			}
		}
	}
#else
	Agent_List tmp_agents            = m_agents;
	Agent_List::iterator agent1_iter = tmp_agents.begin();

	/// @ToDo: Check whether this sqaure design is necessary
	while (agent1_iter != tmp_agents.end())
	{
		CTPAgent_ptr ctpagent1_ptr = (CTPAgent_ptr) *agent1_iter;
		Assert(ctpagent1_ptr);
		
		if( ctpagent1_ptr->Get_Is_Dead()
		|| !ctpagent1_ptr->Get_Can_Be_Executed()
		){
			++agent1_iter;
			continue;
		}

		bool agent1_is_partial = (ctpagent1_ptr->Get_Army().Num() < k_MAX_ARMY_SIZE );

		sint32 min_distance = (g_mp_size.x + g_mp_size.y);
		min_distance *= min_distance;
		Agent_List::iterator closest_agent_iter = tmp_agents.end();
		bool partiality_found = false;

		for
		(
		    Agent_List::iterator agent2_iter  = tmp_agents.begin();
		                         agent2_iter != tmp_agents.end();
		                       ++agent2_iter
		)
		{
			CTPAgent_ptr ctpagent2_ptr = (CTPAgent_ptr) *agent2_iter;

			if(ctpagent1_ptr == ctpagent2_ptr)
				continue;

			if(ctpagent2_ptr->Get_Is_Dead())
				continue;
			bool agent2_is_partial = (ctpagent2_ptr->Get_Army().Num() < k_MAX_ARMY_SIZE);
			if( (partiality_found) &&
				(agent2_is_partial != agent1_is_partial) )
				continue;

			sint32 const distance = 
			    MapPoint::GetSquaredDistance(ctpagent1_ptr->Get_Pos(), ctpagent2_ptr->Get_Pos());

			if(distance < min_distance)
			{
				min_distance        = distance;
				closest_agent_iter  = agent2_iter;
			}

			if( agent1_is_partial == agent2_is_partial && 
				!partiality_found)
			{
				partiality_found = true;
				min_distance = distance;
				closest_agent_iter = agent2_iter;
			}
		}

		if(min_distance < 1)
		{
			ctpagent1_ptr->Group_With((CTPAgent_ptr) *closest_agent_iter);
		}
		else if( closest_agent_iter != tmp_agents.end() )
		{
			MapPoint closest_agent_pos;

			CTPAgent_ptr closest_agent_ptr = (CTPAgent_ptr) *closest_agent_iter;

			MapPoint closest_agent_pos;
			if(closest_agent_ptr->Get_Can_Be_Executed())
			{
				closest_agent_pos = closest_agent_ptr->Get_Pos();
			}
			else
			{
				closest_agent_pos = closest_agent_ptr->Get_Target_Pos();
			}

			// To avoid Groups to be blocked when an unit is in a city 
			// (problem with garrison -> not enough room)
			sint32 cells;
			if(!g_theWorld->GetCity(closest_agent_pos).IsValid() 
			||  ctpagent1_ptr->GetRounds(closest_agent_pos, cells) > 2
			){
				// Should be superflous
				Set_Sub_Task(SUB_TASK_RALLY);
				if (!GotoGoalTaskSolution(ctpagent1_ptr, closest_agent_pos))
					return false;
			}
			else
			{
				uint8 magnitude = 220;
				MBCHAR * myString = new MBCHAR[40];
				MapPoint goal_pos;
				goal_pos = Get_Target_Pos(ctpagent1_ptr->Get_Army());
				sprintf(myString, "Waiting GROUP to GO (%d,%d)", goal_pos.x, goal_pos.y);
				g_graphicsOptions->AddTextToArmy(ctpagent1_ptr->Get_Army(), myString, magnitude);
				delete[] myString;
			}

			MapPoint agent1_pos = ctpagent1_ptr->Get_Pos();
			if( g_theWorld->GetCity(closest_agent_pos).IsValid() || closest_agent_ptr->GetRounds(agent1_pos, cells) > 2)
			{
				if (g_theWorld->GetCity(agent1_pos).IsValid() && g_theWorld->GetCity(closest_agent_pos).IsValid()) //two units are in another town
				{
					MapPoint tempPos;
					for(sint32 i = 0 ; i < NOWHERE; i++)
					{
						if(closest_agent_pos.GetNeighborPosition(WORLD_DIRECTION(i),tempPos))
						{
							CellUnitList *the_army = g_theWorld->GetArmyPtr(tempPos);
							if(!the_army)
							{	//search for cell without army
								agent1_pos = tempPos;
								break;
							}
						}
					}
				}

				// Should be superflous
				Set_Sub_Task(SUB_TASK_RALLY);
				if(!GotoGoalTaskSolution(closest_agent_ptr, agent1_pos))
					return false;
			}
			else
			{
				CellUnitList *the_army = g_theWorld->GetArmyPtr(closest_agent_pos);
				if(static_cast<uint32>(the_army->Num()) >= m_agents.size() && m_agents.size() > k_MAX_ARMY_SIZE/2)
				{
					MapPoint tempPos;
					for(sint32 i = 0 ; i < NOWHERE; i++)
					{
						if(closest_agent_pos.GetNeighborPosition(WORLD_DIRECTION(i),tempPos))
						{
							CellUnitList *the_army = g_theWorld->GetArmyPtr(tempPos);
							if(!the_army)
							{	//search for cell without army
								// Should be superflous
								Set_Sub_Task(SUB_TASK_RALLY);
								if(!GotoGoalTaskSolution(closest_agent_ptr, tempPos))
									return false;
								break;
							}
						}
					}
				}
			}

			/// @ToDo: Check whether this is necessary, and what does it do
			tmp_agents.insert(tmp_agents.begin(), *closest_agent_iter);
			tmp_agents.erase(closest_agent_iter);
		}
		else
		{
			Assert(false);
		}

		++agent1_iter;
	}
#endif

	return true;
}


bool CTPGoal::UnGroupTroops()
{
	bool            breturn     = false;

	for
	(
	    Agent_List::iterator agent1_iter  = m_agents.begin();
	                         agent1_iter != m_agents.end();
	                       ++agent1_iter
	)
	{
		CTPAgent_ptr    ctpagent1_ptr   = (CTPAgent_ptr) *agent1_iter;
		Assert(ctpagent1_ptr);

		if (!ctpagent1_ptr->Get_Is_Dead())
		{
			if (ctpagent1_ptr->Get_Army().Num() > 1)
			{
				ctpagent1_ptr->Ungroup_Order();
			}

			breturn = true;
		}
	}
	
	return breturn;
}


bool CTPGoal::UnGroupComplete() const
{
	for
	(
	    Agent_List::const_iterator agent_iter  = m_agents.begin(); 
	                               agent_iter != m_agents.end(); 
	                             ++agent_iter
	)
	{
		CTPAgent_ptr ctpagent_ptr = (CTPAgent_ptr) *agent_iter;
		Assert(ctpagent_ptr);

		if (!ctpagent_ptr->Get_Is_Dead() &&
		    (ctpagent_ptr->Get_Army().Num() > 1)
		   )
		{
			return false;
		}
	}

	return true;
}

bool CTPGoal::TryTransport(CTPAgent_ptr agent_ptr, const MapPoint & goal_pos) 
{
	if (g_theGoalDB->Get(m_goal_type)->GetNoTransport())
		return false;

	// Not so easy, what if a tranport is a shortcut?
	if ( g_theWorld->GetContinent( goal_pos ) == 
		 g_theWorld->GetContinent(agent_ptr->Get_Pos()) )
		return false;

	return LoadTransporters(agent_ptr);
}

bool CTPGoal::FindTransporters(const CTPAgent_ptr & agent_ptr, std::list< std::pair<Utility, CTPAgent_ptr> > & transporter_list)
{
	std::pair<Utility, CTPAgent_ptr> transporter;

	double          max_utility         = Goal::BAD_UTILITY;
	sint32          needed_transport    = agent_ptr->Get_Army()->Num();
	sint16 const    dest_cont           = g_theWorld->GetContinent(Get_Target_Pos());

	m_current_needed_strength.Set_Transport(static_cast<sint16>(needed_transport));

	for
	(
	    Agent_List::iterator agent_iter  = m_agents.begin();
	                         agent_iter != m_agents.end();
	                       ++agent_iter
	)
	{
		CTPAgent_ptr    possible_transport  = (CTPAgent_ptr) *agent_iter;

		sint32          transports          = 0;
		sint32          max_slots           = 0;
		sint32          empty_slots         = 0;
		possible_transport->Get_Army()->GetCargo(transports, max_slots, empty_slots);

		if (max_slots <= 0)
			continue;

		if (empty_slots <= 0)
			continue;

		if (possible_transport == agent_ptr)
			continue;

		if (!possible_transport->Get_Can_Be_Executed())
		{
			MapPoint target_pos = possible_transport->Get_Target_Pos();
			MapPoint army_pos = agent_ptr->Get_Army()->RetPos();
			if (!target_pos.IsNextTo(army_pos))
				continue;
		}

		Utility  utility         = Goal::BAD_UTILITY;
		if(agent_ptr->EstimateTransportUtility(possible_transport, utility))
		{
			transporter.first  = utility;
			transporter.second = possible_transport;

			transporter_list.push_back(transporter);
			needed_transport -= empty_slots;
		}
	}

	// Probably more stuff needs to be done here
//	m_current_needed_strength.Set_Transport(static_cast<sint16>(needed_transport));

	transporter_list.sort(std::greater<std::pair<Utility,class CTPAgent *> >());
	return transporter_list.size() > 0;
}

bool CTPGoal::LoadTransporters(CTPAgent_ptr agent_ptr)
{
	std::list< std::pair<Utility, CTPAgent_ptr> > transporter_list;

	if(!FindTransporters(agent_ptr, transporter_list))
		return false;

	bool success = false;
	sint32 foundSlots = 0;

	MapPoint pos(-1,-1);

	/// @ToDo: Add transport escorts
	for
	(
	    std::list< std::pair<Utility, CTPAgent_ptr> >::iterator
	        transporter  = transporter_list.begin();
	        transporter != transporter_list.end();
	      ++transporter
	)
	{
		CTPAgent_ptr transport_ptr = transporter->second;

		transport_ptr->Log_Debug_Info(k_DBG_SCHEDULER, this);

		if(transport_ptr->Get_Army()->GetMovementTypeAir()
		&& transport_ptr->Get_Army()->CanSpaceLaunch()
		){
			// @ToDo: Check whether this really works
			Set_Sub_Task(SUB_TASK_AIRLIFT);
		}
		else
		{
			Set_Sub_Task(SUB_TASK_TRANSPORT_TO_BOARD);
		}

		if(!GotoTransportTaskSolution(agent_ptr, transport_ptr, pos))
			continue;

		success = true;

		sint32 transporters = 0;
		sint32 max          = 0;
		sint32 empty        = 0;

		agent_ptr->Get_Army()->GetCargo(transporters, max, empty);

		foundSlots += empty;

		if(agent_ptr->Get_Army()->Num() <= foundSlots)
			break;
	}

	if (success)
	{
		CTPAgent_ptr transport_ptr = transporter_list.begin()->second;
		Set_Sub_Task(SUB_TASK_CARGO_TO_BOARD);
		success = GotoTransportTaskSolution(agent_ptr, transport_ptr, pos);

		g_player[m_playerId]->
			AddCargoCapacity(static_cast<sint16>(-1 * agent_ptr->Get_Army().Num()));
	}
	else
	{
		Set_Sub_Task(SUB_TASK_GOAL);
	}

	return success;
}

bool CTPGoal::ArmiesAtGoal() const
{
	MapPoint pos = Get_Target_Pos();

	for
	(
	    Agent_List::const_iterator agent_iter  = m_agents.begin();
	                               agent_iter != m_agents.end();
	                             ++agent_iter
	)
	{
		CTPAgent_ptr ctpagent_ptr = (CTPAgent_ptr) *agent_iter;

		if (ctpagent_ptr->Get_Pos() != pos)
			return false;
	}

	return true;
}

sint32 CTPGoal::GetThreatenBonus() const
{
	const GoalRecord *goal_record = g_theGoalDB->Get(m_goal_type);

	if (goal_record->GetThreatenTypeNone())
		return 0;

	switch (goal_record->GetThreatenType())
	{
		case k_Goal_ThreatenType_DestroyCity_Bit:
		{
			Threat           tmp_threat;
			const Diplomat & diplomat    = Diplomat::GetDiplomat(m_playerId);

			if (diplomat.HasThreat(Get_Target_Owner(), THREAT_DESTROY_CITY, tmp_threat))
			{
				if (tmp_threat.detail.arg.cityId == m_target_city.m_id)
					return goal_record->GetThreatenBonus();
			}
			break;
		}
	}

	return 0;
}

bool CTPGoal::Goal_Too_Expensive() const
{
	return (m_current_attacking_strength.Get_Agent_Count() > k_MAX_ARMY_SIZE) 
	    && (m_current_attacking_strength.Get_Value() > 
	            m_current_needed_strength.Get_Value() * 3
	       );
}

bool CTPGoal::Can_Add_To_Goal(const Agent_ptr agent_ptr) const
{
	CTPAgent_ptr ctpagent_ptr = (CTPAgent_ptr) agent_ptr;

	if(ctpagent_ptr->Get_Is_Dead())
	{
		return false;
	}

	MapPoint dest_pos = Get_Target_Pos(ctpagent_ptr->Get_Army());

	if(!Pretest_Bid(ctpagent_ptr, dest_pos))
	{
		return false;
	}

	return true;
}
