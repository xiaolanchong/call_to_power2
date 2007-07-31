

#ifndef __C3_STATIC_H__
#define __C3_STATIC_H__

#include "aui_Static.h"
#include "PatternBase.h"


#define k_C3_STATIC_LDL_BEVELWIDTH			"bevelwidth"
#define k_C3_STATIC_LDL_BEVELTYPE			"beveltype"

#define k_C3_STATIC_ACTION_LMOUSE 1
#define k_C3_STATIC_ACTION_RMOUSE 2

class aui_Surface;


class c3_Static : public aui_Static, public PatternBase
{
public:
	c3_Static(AUI_ERRCODE *retval,
						uint32 id,
						MBCHAR *ldlBlock );
	c3_Static(AUI_ERRCODE *retval,
						uint32 id,
						sint32 x,
						sint32 y,
						sint32 width,
						sint32 height,
						MBCHAR *pattern,
						MBCHAR *text,
						uint32 maxLength,
						uint32 bevelWidth,
						uint32 bevelType);


	AUI_ERRCODE InitCommonLdl( MBCHAR *ldlBlock );
	AUI_ERRCODE InitCommon(uint32 bevelWidth, uint32 bevelType );

	virtual AUI_ERRCODE DrawThis(aui_Surface *surface,
											sint32 x,
											sint32 y);

	uint32 BevelWidth() { return m_bevelWidth; }

	
	virtual bool IgnoreHighlight() { return true; }
protected:
	virtual MouseEventCallback MouseLGrabInside;
	virtual MouseEventCallback MouseLDropInside;

	virtual MouseEventCallback MouseRGrabInside;
	virtual MouseEventCallback MouseRDropInside;

	uint32 m_bevelWidth;
	uint32 m_bevelType;
};


#endif
