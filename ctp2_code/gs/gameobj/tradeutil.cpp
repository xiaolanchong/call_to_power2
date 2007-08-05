//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C++ source
// Description  : Trade utilities
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
// - None
//
//----------------------------------------------------------------------------
//
// Modifications from the original Activision code:
//
// - Devided caravan costs by 10 so that we don't have to update const.txt.
//   This allows better mod compatibility. - May 7th 2005 Martin G�hmann
// - Removed .NET warnings - May 7th 2005 Martin G�hmann
// - Standardized trade route cost calculation. - June 5th 2005 Martin G�hmann
// - added note that trade pact deals shouldn't be hardcoded 6.13.2007
// - Added some functionality from the old const database. (5-Aug-2007 Martin G�hmann)
//
//----------------------------------------------------------------------------

#include "c3.h"
#include "tradeutil.h"
#include "World.h"
#include "ResourceRecord.h"
#include "Unit.h"
#include "citydata.h"
#include "TradeAstar.h"
#include "Path.h"
#include "AgreementMatrix.h"
#include "ConstRecord.h"

extern TradeAstar g_theTradeAstar; 

sint32 tradeutil_GetTradeValue(const sint32 owner, Unit const & destination, sint32 resource)
{
	Assert(destination.IsValid());
	if(!destination.IsValid()) return 0;

	Assert(resource >= 0 && resource < g_theResourceDB->NumRecords());
	if(resource < 0 || resource >= g_theResourceDB->NumRecords())
		return 0;


	double baseValue = g_theWorld->GetGoodValue(resource);
	double distance = static_cast<double>(destination.GetCityData()->GetDistanceToGood(resource));
	sint32 totalValue = sint32(baseValue * distance);
	
	
    PLAYER_INDEX const  tradePartner    = destination.GetOwner();

    if (    (owner != tradePartner)
         && AgreementMatrix::s_agreements.HasAgreement
                (owner, tradePartner, PROPOSAL_TREATY_TRADE_PACT)
       )
    {
		totalValue = (sint32) (totalValue * 1.05); //- shouldn't trade pact values be set in the ConstDB instead of 1.05? - E 6.13.2007
	}

	return totalValue;
}

sint32 tradeutil_GetAccurateTradeDistance(Unit &source, Unit &destination)
{
	Path    path;
	float   cost;

	if (g_theTradeAstar.FindPath
            (source.GetOwner(), source.RetPos(), destination.RetPos(), path, cost, FALSE)
       )
    {
        return static_cast<sint32>(std::max(tradeutil_GetNetTradeCosts(cost), 1.0));
    }

	return DISTANCE_UNKNOWN;
}

sint32 tradeutil_GetTradeDistance(Unit &source, Unit &destination)
{
	double cost = g_theWorld->CalcTerrainFreightCost(source.RetPos()) *
                  static_cast<double>
                    (source.RetPos().NormalizedDistance(destination.RetPos()));
	
    return static_cast<sint32>(std::max(tradeutil_GetNetTradeCosts(cost), 1.0));
}

void constutil_y2meridian(const sint32 y, sint32 &k)
{
	// Actually, the values from the ConstDB are unsued
	for (sint32 i = 0; i < k_NUM_MERIDIANS; i++)
	{
		if (y < 11 * (i + 1))
		{
			k = i;
			return;
		}
	}
	k = i;
}

void constutil_GetMapSizeMapPoint(MAPSIZE size, MapPoint &mapPoint)
{
	// Can be removed, when the map size only depends on the map database
	switch (size) {
	case MAPSIZE_SMALL:
		mapPoint.x = static_cast<sint32>(g_theConstDB->Get(0)->GetMapSizeSmall(0));
		mapPoint.y = static_cast<sint32>(g_theConstDB->Get(0)->GetMapSizeSmall(1));
#if !defined(_SMALL_MAPPOINTS)
		mapPoint.z = static_cast<sint32>(g_theConstDB->Get(0)->GetMapSizeSmall(2));
#endif
		break;
	case MAPSIZE_MEDIUM:
		mapPoint.x = static_cast<sint32>(g_theConstDB->Get(0)->GetMapSizeMedium(0));
		mapPoint.y = static_cast<sint32>(g_theConstDB->Get(0)->GetMapSizeMedium(1));
#if !defined(_SMALL_MAPPOINTS)
		mapPoint.z = static_cast<sint32>(g_theConstDB->Get(0)->GetMapSizeMedium(2));
#endif
		break;
	case MAPSIZE_LARGE:
		mapPoint.x = static_cast<sint32>(g_theConstDB->Get(0)->GetMapSizeLarge(0));
		mapPoint.y = static_cast<sint32>(g_theConstDB->Get(0)->GetMapSizeLarge(1));
#if !defined(_SMALL_MAPPOINTS)
		mapPoint.z = static_cast<sint32>(g_theConstDB->Get(0)->GetMapSizeLarge(2));
#endif
		break;
	case MAPSIZE_GIGANTIC:
		mapPoint.x = static_cast<sint32>(g_theConstDB->Get(0)->GetMapSizeGigantic(0));
		mapPoint.y = static_cast<sint32>(g_theConstDB->Get(0)->GetMapSizeGigantic(1));
#if !defined(_SMALL_MAPPOINTS)
		mapPoint.z = static_cast<sint32>(g_theConstDB->Get(0)->GetMapSizeGigantic(2));
#endif
		break;
	default :
		Assert(FALSE);
	}
}