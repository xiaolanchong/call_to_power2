

#ifdef HAVE_PRAGMA_ONCE
#pragma once
#endif
#ifndef __GAME_OVER_H__
#define __GAME_OVER_H__

enum GAME_OVER
{
	
	GAME_OVER_LOST_OUT_OF_TIME,
	GAME_OVER_LOST_CONQUERED,
	GAME_OVER_LOST_SCIENCE,
	GAME_OVER_LOST_DIPLOMACY,
	GAME_OVER_LOST_INEPT,
	GAME_OVER_LOST_SCENARIO,
	
	GAME_OVER_WON_OUT_OF_TIME,
	GAME_OVER_WON_SCENARIO,
	GAME_OVER_WON_CONQUERED_WORLD,
	GAME_OVER_WON_SCIENCE,
	GAME_OVER_WON_DIPLOMACY
};

#endif
