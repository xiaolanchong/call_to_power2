//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C++ header
// Description  : Surface (part of the screen) 
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
// USE_SDL
//
//----------------------------------------------------------------------------
//
// Modifications from the original Activision code:
//
// - Corrected a reported memory leak.
// - Added back buffering capability. (1-Jan-2010 Martin Gühmann)
//
//----------------------------------------------------------------------------

#ifndef __AUI_SURFACE_H__
#define __AUI_SURFACE_H__


#include "aui_base.h"
#ifdef USE_SDL
#include <SDL.h>
#include <SDL_thread.h>
#endif

enum AUI_SURFACE_PIXELFORMAT
{
	AUI_SURFACE_PIXELFORMAT_FIRST = 0,
	AUI_SURFACE_PIXELFORMAT_UNKNOWN = 0,
	AUI_SURFACE_PIXELFORMAT_332,
	AUI_SURFACE_PIXELFORMAT_555,
	AUI_SURFACE_PIXELFORMAT_565,
	AUI_SURFACE_PIXELFORMAT_888,
	AUI_SURFACE_PIXELFORMAT_LAST
};

struct aui_SurfaceSubset
{
	LPVOID	buffer;
	RECT	rect;
};

#define k_SURFACE_MAXLOCK 4

enum AUI_SURFACE_LOCKOP
{
	AUI_SURFACE_LOCKOP_FIRST = 0,
	AUI_SURFACE_LOCKOP_CHECK = 0,
	AUI_SURFACE_LOCKOP_ADD,
	AUI_SURFACE_LOCKOP_REMOVE,
	AUI_SURFACE_LOCKOP_LAST
};


class aui_Surface
{
public:
	virtual ~aui_Surface() = default;

	virtual BOOL IsThisA(uint32 classId) = 0;

	virtual sint32 Width(void) const = 0;
	virtual sint32 Height(void) const = 0;
	virtual sint32 BitsPerPixel(void) const = 0;
	virtual sint32 BytesPerPixel(void) const = 0;
	virtual sint32 Bytewidth(void) const = 0;
	virtual sint32 Pitch(void) const = 0;
	virtual sint32 Size(void) const = 0;
	virtual uint8 *Buffer(void) const = 0;
	virtual AUI_SURFACE_PIXELFORMAT PixelFormat(void) const = 0;

	virtual uint32 GetChromaKey(void) const = 0;
	virtual uint32 SetChromaKey(uint8 red, uint8 green, uint8 blue) = 0;

	/// @param flags depends on implementation (e.g. DDLOCK_* flags)
	virtual AUI_ERRCODE Lock(RECT *rect, LPVOID *buffer, DWORD flags) = 0;
	virtual AUI_ERRCODE Unlock(LPVOID buffer) = 0;

#ifndef USE_SDL
	virtual AUI_ERRCODE GetDC(HDC *hdc) = 0;
	virtual AUI_ERRCODE ReleaseDC(HDC hdc) = 0;
#endif
	virtual BOOL IsOK() const = 0;

	virtual AUI_ERRCODE Blank(const uint32 &color) = 0;

	virtual void Flip() = 0;
};


class aui_SurfaceImpl : public aui_Surface, protected aui_Base
{
protected:
	
	aui_SurfaceImpl(
		AUI_ERRCODE *retval,
		sint32 width,
		sint32 height,
		sint32 bpp,
		sint32 pitch = 0,
		uint8 *buffer = NULL,
		BOOL isPrimary = FALSE,
		HDC hdc = NULL);
	virtual ~aui_SurfaceImpl();

protected:
	aui_SurfaceImpl() : aui_Base() {}

protected:
	AUI_ERRCODE InitCommon( sint32 width, sint32 height, sint32 bpp, BOOL isPrimary );


	virtual BOOL IsThisA( uint32 classId ) override
	{	
		return classId == m_surfaceClassId;
	}
protected:
	sint32 Width( void ) const override { return m_width; }
	sint32 Height( void ) const override { return m_height; }
	sint32 BitsPerPixel( void ) const override { return m_bpp; }
	sint32 BytesPerPixel( void ) const override { return m_Bpp; }
	sint32 Bytewidth( void ) const override { return m_bytewidth; }
	sint32 Pitch( void ) const override { return m_pitch; }
	sint32 Size( void ) const { return m_size; }
	uint8 *Buffer( void ) const override { return m_buffer; }
	AUI_SURFACE_PIXELFORMAT PixelFormat( void ) const override { return m_pixelFormat; }

	virtual uint32 GetChromaKey( void ) const override { return m_chromaKey; }
protected:
	uint32 SetChromaKey( uint32 color );
private:
	virtual uint32 SetChromaKey( uint8 red, uint8 green, uint8 blue ) override;
protected:
	BOOL IsPrimary( void ) const { return m_isPrimary; }
private:
	
	
	virtual AUI_ERRCODE Lock( RECT *rect, LPVOID *buffer, DWORD flags ) override;
	virtual AUI_ERRCODE Unlock( LPVOID buffer ) override;

#ifndef USE_SDL
	virtual AUI_ERRCODE GetDC( HDC *hdc ) override;
	virtual AUI_ERRCODE ReleaseDC( HDC hdc ) override;
#endif

#ifdef USE_SDL
	SDL_mutex *LPCS( void ) const { return m_cs; };
#else
	LPCRITICAL_SECTION LPCS( void ) const { return &m_cs; };
#endif
	
	virtual BOOL IsOK( void ) const override { return m_saveBuffer != NULL; }

private:
	// not used
	virtual AUI_ERRCODE BlankRGB(const uint8 &red, const uint8 &green, const uint8 &blue);

	virtual AUI_ERRCODE Blank(const uint32 &color) override;

	virtual void Flip(){};
	virtual void ReverseFlip(){};

	static uint32 m_surfaceClassId;

private:
	static sint32 m_surfaceRefCount;
#ifdef USE_SDL
	static SDL_mutex *m_cs;
#else
	static	CRITICAL_SECTION	m_cs;
#endif
protected:
	AUI_ERRCODE ManipulateLockList( RECT *rect, LPVOID *buffer, AUI_SURFACE_LOCKOP op );

private:
	sint32	m_width;		
	sint32	m_height;		
	sint32	m_bpp;			
	sint32	m_Bpp;			
	sint32	m_bytewidth;
protected:
	sint32	m_pitch;		
	sint32	m_size;			
private:
	uint8	*m_buffer;		
	
	HDC		m_hdc;			
protected:
	bool	m_dcIsGot;
private:
	HBITMAP	m_hbitmap;
	HBITMAP	m_holdbitmap;
protected:
	AUI_SURFACE_PIXELFORMAT m_pixelFormat; 
	uint32	m_chromaKey;	

private:
	BOOL	m_isPrimary;	
protected:
	BOOL	m_allocated;	
	uint8	*m_saveBuffer;
private:
	aui_SurfaceSubset m_locklist[ k_SURFACE_MAXLOCK ]; 
	sint32	m_locksRemain;	

private:
	
	BOOL IsLocked( RECT *rect );
	BOOL IsLocked( LPVOID buffer );
};

class aui_MemSurface : public aui_SurfaceImpl
{
public:
	aui_MemSurface(AUI_ERRCODE *retval,
		sint32 width,
		sint32 height,
		sint32 bpp,
		sint32 pitch = 0,
		uint8 *buffer = NULL,
		BOOL isPrimary = FALSE
	) : aui_SurfaceImpl(retval, width, height, bpp, pitch, buffer, isPrimary) {}
};


#endif 
