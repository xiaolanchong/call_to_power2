








#ifdef HAVE_PRAGMA_ONCE
#pragma once
#endif
#ifndef _TRADEROUTE_H_
#define _TRADEROUTE_H_

class TradeRoute;

enum CAUSE_KILL_TRADE_ROUTE {
	CAUSE_KILL_TRADE_ROUTE_UNKNOWN,
	CAUSE_KILL_TRADE_ROUTE_SENDER_KILLED,
	CAUSE_KILL_TRADE_ROUTE_RECIPIENT_KILLED,
	CAUSE_KILL_TRADE_ROUTE_PIRATED,
	CAUSE_KILL_TRADE_ROUTE_CITY_CHANGED_OWNER,
	CAUSE_KILL_TRADE_ROUTE_CITY_DIED,
	CAUSE_KILL_TRADE_ROUTE_DIPLOMATIC_AGREEMENT,
	CAUSE_KILL_TRADE_ROUTE_CHANGED_DESTINATION,
	CAUSE_KILL_TRADE_ROUTE_NO_MORE_CARAVANS,
	CAUSE_KILL_TRADE_ROUTE_NO_INITIAL_CARAVANS,
	CAUSE_KILL_TRADE_ROUTE_WAR,
	CAUSE_KILL_TRADE_ROUTE_RESET,
};

enum ROUTE_TYPE {
	ROUTE_TYPE_RESOURCE,
	ROUTE_TYPE_FOOD,
	ROUTE_TYPE_GOLD,
	ROUTE_TYPE_SLAVE, 
};


#include "DB.h"     // StringId
#include "ID.h"     // ID
#include "player.h" // PLAYER_INDEX

template <class T> class DynamicArray;
class TradeRouteData;
class Unit;
class MapPoint;


class TradeRoute : public ID
{
public:
	TradeRoute () : ID() { ; } ;
	TradeRoute (sint32 val) : ID (val) { ; };
	TradeRoute (uint32 val) : ID (val) { ; };

	TradeRouteData *operator -> () const { return AccessData(); }
	bool IsValid() const;

	void KillRoute(CAUSE_KILL_TRADE_ROUTE cause);
	void Kill(CAUSE_KILL_TRADE_ROUTE cause) { KillRoute(cause); }

	void RemoveAllReferences(CAUSE_KILL_TRADE_ROUTE cause);

	const TradeRouteData* GetData() const;
	TradeRouteData* AccessData() const;

	Unit GetSource() const;
	Unit GetDestination() const;

	PLAYER_INDEX GetOwner() const;
	PLAYER_INDEX GetPayingFor() const;

	void GetSourceResource(ROUTE_TYPE &type, sint32 &resource) const;

	TradeRoute GetRecip() const;
	TradeRoute AccessRecip();
	void SetRecip(TradeRoute route);

	BOOL PassesThrough(sint32 player) const;
	void SetTariff(sint32 player, BOOL b);
	
	BOOL CrossesWater() const;

	uint32 GetColor() const;
	uint32 GetOutlineColor() const;
	void SetColor( uint32 color );
	void SetOutlineColor( uint32 color );

	
	void ClearSelectedPath();
	void GenerateSelectedPath(const MapPoint &pos);
	sint32 AddSelectedWayPoint(const MapPoint &pos);
	BOOL InitSelectedData();
	BOOL IsSelectedPathSame();
	BOOL IsPosInSelectedPath(const MapPoint &pos);
	BOOL IsPosInPath(const MapPoint &pos);
	void ClearSelectedCellData(TradeRoute route);
	void UpdateSelectedCellData(TradeRoute route);
	sint32 GetPathSelectionState() const;
	void SetPathSelectionState(sint32 state);

	double GetCost() const;
	const DynamicArray<MapPoint>* GetPath() const;
	const DynamicArray<MapPoint>* GetSelectedPath() const;
	
	BOOL IsActive() const;
	void Activate();
	void Deactivate();

	sint32 GetGoldInReturn() const;

	StringId GetResourceName() const;

	void ReturnPath(const PLAYER_INDEX owner, DynamicArray<MapPoint> &waypoints,
					DynamicArray<MapPoint> &fullpath,
					double &cost);
	void SetPath(DynamicArray<MapPoint> &fullpath,
				 DynamicArray<MapPoint> &waypoints);
	
	void BeginTurn();

	void DontAdjustPointsWhenKilled();
};

#endif
