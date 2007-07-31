
#ifndef __STARTING_POSITION_H__
#define __STARTING_POSITION_H__

enum STARTINFOTYPE {
	STARTINFOTYPE_NONE,
	STARTINFOTYPE_CIVSFIXED,      
	STARTINFOTYPE_POSITIONSFIXED, 
	STARTINFOTYPE_CIVS,           
	                              
	STARTINFOTYPE_NOLOCS,		
};

struct StartingPosition {
	MapPoint point;
	sint32 civIndex;
};

#define k_MAX_START_POINTS 64

#endif
