#ifdef HAVE_PRAGMA_ONCE
#pragma once
#endif
#ifndef __PASSWORDSCREEN_H__
#define __PASSWORDSCREEN_H__



enum PASSWORDSCREEN_MODE
{
	PASSWORDSCREEN_MODE_ASK,	
	PASSWORDSCREEN_MODE_JOIN,	
	PASSWORDSCREEN_MODE_DENY,	
	PASSWORDSCREEN_MODE_FULL,	
	PASSWORDSCREEN_MODE_NOLOBBY,
	PASSWORDSCREEN_MODE_CONNECTIONLOST, 
	PASSWORDSCREEN_MODE_NODIALUP, 
	PASSWORDSCREEN_MODE_CONNECTIONERR, 
	PASSWORDSCREEN_MODE_SCENARIO_NOT_FOUND, 
	PASSWORDSCREEN_MODE_MAX
};


sint32 passwordscreen_displayMyWindow( PASSWORDSCREEN_MODE m );
sint32 passwordscreen_removeMyWindow( void );
AUI_ERRCODE passwordscreen_Initialize( void );
AUI_ERRCODE passwordscreen_Cleanup( void );


void PasswordScreenCallback( aui_Control *, uint32, uint32, void *);


#endif 
