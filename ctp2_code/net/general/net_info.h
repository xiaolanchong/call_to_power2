//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C++ header file
// Description  : Multiplayer server to client information messages
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
//----------------------------------------------------------------------------
//
// Modifications from the original Activision code:
//
// - Propagate PW each turn update
// - Added NET_INFO_CODE_DISBANDED_CITY_SETTLER message
// - Added NET_INFO_CODE_ACCOMPLISHED_FEAT message
//
//----------------------------------------------------------------------------

#if defined(_MSC_VER) && (_MSC_VER > 1000)
#pragma once
#endif

#ifndef _NET_INFO_H_
#define _NET_INFO_H_

#include "net_packet.h"

enum NET_INFO_CODE {
	NET_INFO_CODE_PLAYER_INDEX,					// 0x0000
	NET_INFO_CODE_BEGIN_TURN,
	NET_INFO_CODE_MAP_DONE,
	NET_INFO_CODE_START_UNITS,
	NET_INFO_CODE_END_UNITS,
	NET_INFO_CODE_KILL_UNIT,
	NET_INFO_CODE_TAX_RATE,
	NET_INFO_CODE_GOLD_SCIENCE,
	NET_INFO_CODE_ADVANCE,
	NET_INFO_CODE_SUPPORT_UNIT,
	NET_INFO_CODE_KILL_POP,
	NET_INFO_CODE_KILL_TRADE_ROUTE,
	NET_INFO_CODE_BUILDING_UNIT,
	NET_INFO_CODE_KILL_TRADE_OFFER,
	NET_INFO_CODE_ACK_OBJECT,
	NET_INFO_CODE_NAK_OBJECT,
	NET_INFO_CODE_KILL_IMPROVEMENT,				// 0x0010
	NET_INFO_CODE_KILL_INSTALLATION,
	NET_INFO_CODE_GOLD,
	NET_INFO_CODE_MATERIALS_TAX,
	NET_INFO_CODE_WORKDAY_LEVEL,
	NET_INFO_CODE_WAGES_LEVEL,
	NET_INFO_CODE_RATIONS_LEVEL,
	NET_INFO_CODE_NEW_CIVILIZATION,
	NET_INFO_CODE_BUILT_FRONT,
	NET_INFO_CODE_KILL_MESSAGE,
	NET_INFO_CODE_KILL_DIP_REQUEST,
	NET_INFO_CODE_RESET_CITY_OWNER,
	NET_INFO_CODE_RESEARCH,
	NET_INFO_CODE_REMOVE_HUT,
	NET_INFO_CODE_GLOBAL_WARMING,
	NET_INFO_CODE_OZONE_DEPLETION,
	NET_INFO_CODE_BATTLE,						// 0x0020
	NET_INFO_CODE_KILL_PLAYER,
	NET_INFO_CODE_BUILD_IMP,
	NET_INFO_CODE_CHANGE_BUILD,
	NET_INFO_CODE_YEAR,
	NET_INFO_CODE_BEGIN_SLICE,
	NET_INFO_CODE_REQUEST_SLICE,
	NET_INFO_CODE_SET_GOVERNMENT,
	NET_INFO_CODE_ENACT_REQUEST,
	NET_INFO_CODE_REJECT_REQUEST,

	NET_INFO_CODE_CLASSIC_STYLE,
	NET_INFO_CODE_UNIT_MOVES_STYLE,
	NET_INFO_CODE_SPEED_STYLE,
	NET_INFO_CODE_TIMED_STYLE,
	NET_INFO_CODE_SIMULTANEOUS_STYLE,
	NET_INFO_CODE_TURN_SYNC,
	NET_INFO_CODE_CARRYOVER_STYLE,				// 0x0030
	NET_INFO_CODE_SET_SETUP_MODE,
	NET_INFO_CODE_SET_SETUP_AREA,
	NET_INFO_CODE_POWER_POINTS,
	NET_INFO_CODE_CHOOSE_RESEARCH,
	NET_INFO_CODE_SET_OPTIMAL_OUTPUT,
	NET_INFO_CODE_END_TURN_FOR,
	NET_INFO_CODE_POP_TYPE,
	NET_INFO_CODE_ACTUALLY_SET_GOVERNMENT,
	NET_INFO_CODE_CHANGE_INSTALLATION_OWNER,
	NET_INFO_CODE_REMOVE_BUILD_ITEM,
	NET_INFO_CODE_HAPPINESS_ATTACK,
	NET_INFO_CODE_CLEAR_ORDERS,
	NET_INFO_CODE_EXECUTE_ORDERS,
	NET_INFO_CODE_ADD_ARMY,
	NET_INFO_CODE_SET_MOVEMENT_TO_ZERO,
	NET_INFO_CODE_SET_ARMY,						// 0x0040
	NET_INFO_CODE_CHANGE_ARMY,
	NET_INFO_CODE_ACHIEVEMENTS,
	NET_INFO_CODE_BUILT_WONDERS,
	NET_INFO_CODE_BUILD_WONDER,
	NET_INFO_CODE_SEND_TRADE_BID,
	NET_INFO_CODE_SEND_SLAVE_TO,
	NET_INFO_CODE_ACK_POP_MOVE,
	NET_INFO_CODE_CLEAR_ORDERS_EXCEPT_GROUP,
	NET_INFO_CODE_BUILD_CAPITALIZATION,
	NET_INFO_CODE_BUILD_INFRASTRUCTURE,
	NET_INFO_CODE_BUILD_END_GAME,
	NET_INFO_CODE_PROGRESS,
	NET_INFO_CODE_TAKE_TRADE_OFFER,
	NET_INFO_CODE_SOLD_BUILDING,
	NET_INFO_CODE_KILL_AGREEMENT,
	NET_INFO_CODE_BREAK_CEASE_FIRE,				// 0x0050
	NET_INFO_CODE_BREAK_ALLIANCE,
	NET_INFO_CODE_CLEAR_QUEUE,
	NET_INFO_CODE_RESYNC,
	NET_INFO_CODE_WONDER_BUILT,
	NET_INFO_CODE_SEND_OFFER_ACCEPT_MESSAGE,

	NET_INFO_CODE_ATTACH_ROBOT,
	NET_INFO_CODE_DETACH_ROBOT,

	NET_INFO_CODE_SET_DIP_STATE,
	NET_INFO_CODE_SET_TURN,
	NET_INFO_CODE_RESET_UNIT_OWNER,
	NET_INFO_CODE_CELL_OWNER,
	NET_INFO_CODE_ENACT_REQUEST_NEED_ACK,
	NET_INFO_CODE_NAK_ENACT,
	NET_INFO_CODE_REMOVE_ILLEGAL_ITEMS,
	NET_INFO_CODE_MAKE_UNIT_PERMANENT,
	NET_INFO_CODE_CLEAR_QUEUE_EXCEPT_HEAD,		// 0x0060
	NET_INFO_CODE_REMOVE_DEAD_PLAYERS,

	NET_INFO_CODE_WONDER_STARTED,
	NET_INFO_CODE_WONDER_OBSOLETE,
	NET_INFO_CODE_BEGIN_TURN_ENEMY_UNITS,
	NET_INFO_CODE_WONDER_ALMOST_DONE,
	NET_INFO_CODE_WONDER_STOPPED,
	NET_INFO_CODE_ALL_PLAYERS_READY,
	NET_INFO_CODE_SET_FRANCHISE_PRODUCTION,
	NET_INFO_CODE_REVOLT_NOTICES,
	NET_INFO_CODE_ADD_UNSEEN,
	NET_INFO_CODE_OFFER_REJECTED_MESSAGE,
	NET_INFO_CODE_WON_END_GAME,
	NET_INFO_CODE_RECOVERED_PROBE,

	NET_INFO_CODE_OTHER_CIV_LAB_MSG,
	NET_INFO_CODE_OTHER_CIV_SEQUENCE_MSG,
	NET_INFO_CODE_ALIEN_ALMOST_DONE_OTHERS_MSG,	// 0x0070
	NET_INFO_CODE_ALIEN_SCRAPPED_OWNER,
	NET_INFO_CODE_CATACLYSM_OTHER,

	NET_INFO_CODE_GAME_OVER_OUT_OF_TIME,

	NET_INFO_CODE_ACTIVATE_SPACE_BUTTON,
	NET_INFO_CODE_TIMES_ALMOST_UP,
	NET_INFO_CODE_VIOLATE_AGREEMENT,

	NET_INFO_CODE_SET_MAYOR,
	NET_INFO_CODE_FINISH_AI_TURN,
	NET_INFO_CODE_CITIES_DONE,
	NET_INFO_CODE_SET_ROUND,
	NET_INFO_CODE_BEGIN_SCHEDULER,

	NET_INFO_CODE_REMOTE_GROUP,
	NET_INFO_CODE_REMOTE_UNGROUP,
	NET_INFO_CODE_REENTER,

	NET_INFO_CODE_PLAYER_TRADE_DATA,
	NET_INFO_CODE_CREATED_WONDER,				// 0x0080

	NET_INFO_CODE_GAME_OVER,
	NET_INFO_CODE_SET_EMBASSIES,

	// Notify the accomplishment of a feat
	NET_INFO_CODE_ACCOMPLISHED_FEAT				= 0x0083,
	// Mark a settler as created by disbanding a city
	NET_INFO_CODE_DISBANDED_CITY_SETTLER		= 0x0084,	
	// Propagate PW each turn update
	NET_INFO_CODE_MATERIALS,					// unconfirmed value

	NET_INFO_CODE_NULL
};

class NetInfo : public Packetizer
{
public:
	NetInfo(NET_INFO_CODE code, uint32 data = 0, uint32 data2 = 0, uint32 data3 = 0, uint32 data4 = 0, uint32 data5 = 0) :
		m_type(code),
		m_data(data),
		m_data2(data2),
		m_data3(data3),
		m_data4(data4),
		m_data5(data5)
	{
	}

	NetInfo() {};

	void Packetize(uint8* buf, uint16& size);
	void Unpacketize(uint16 id, uint8* buf, uint16 size);
private:
	NET_INFO_CODE m_type;
	uint32        m_data;
	uint32        m_data2;
	uint32        m_data3;
	uint32        m_data4;
	uint32        m_data5;
	static const uint32 m_args[NET_INFO_CODE_NULL];
};

#else
class NetInfo;
#endif
