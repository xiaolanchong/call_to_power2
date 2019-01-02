//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C++ header
// Description  : User interface DirectX surface
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
// __AUI_USE_DIRECTX__
// Use DirectX
//
//----------------------------------------------------------------------------
//
// Modifications from the original Activision code:
//
// - Initialised pointer in default constructor to prevent destructor crash.
// - Added back buffering capability. (1-Jan-2010 Martin Gühmann)
//
//----------------------------------------------------------------------------

#ifndef __AUI_DIRECTSURFACE_H__
#define __AUI_DIRECTSURFACE_H__

#ifdef __AUI_USE_DIRECTX__


#include "aui_surface.h"
#include "aui_directx.h"



class aui_DirectSurface : public aui_SurfaceImpl, private aui_DirectX
{
public:
	
	aui_DirectSurface(
		AUI_ERRCODE *retval,
		sint32 width,
		sint32 height,
		sint32 bpp,
		LPDIRECTDRAW lpdd,
		LPDIRECTDRAWSURFACE lpdds = NULL,
		BOOL isPrimary = FALSE,
		BOOL useVideoMemory = FALSE,
		LPDIRECTDRAWSURFACE back = NULL
		);

private:
	virtual ~aui_DirectSurface();

	virtual BOOL IsThisA( uint32 classId ) override
	{
		return classId == m_directSurfaceClassId
		||     aui_SurfaceImpl::IsThisA( classId )
		||     aui_DirectX::IsThisA( classId );
	}

	virtual uint32 SetChromaKey( uint32 color );

	
	virtual AUI_ERRCODE Lock( RECT *rect, LPVOID *buffer, DWORD flags );
	virtual AUI_ERRCODE Unlock( LPVOID buffer );

	
	virtual AUI_ERRCODE GetDC( HDC *hdc );
	virtual AUI_ERRCODE ReleaseDC( HDC hdc );

	virtual AUI_ERRCODE Blank(const uint32 &color);
public:	
	LPDIRECTDRAWSURFACE BUFFER ( void ) const { return (m_back == NULL) ? m_lpdds : m_back; }
	LPDIRECTDRAWSURFACE DDS    ( void ) const { return m_lpdds; }
private:
	BOOL				IsDCGot( void ) const { return m_dcIsGot; }

	
	virtual BOOL IsOK( void ) const;
	virtual void Flip();

public:
	static uint32 m_directSurfaceClassId;

private:
	aui_DirectSurface()
	:
		aui_SurfaceImpl (),
		m_lpdds     (NULL),
		m_back      (NULL)
	{};

	AUI_ERRCODE InitCommon( void );

	LPDIRECTDRAWSURFACE m_lpdds;
	LPDIRECTDRAWSURFACE m_back;
};

typedef aui_DirectSurface aui_NativeSurface;

#endif 

#endif 
