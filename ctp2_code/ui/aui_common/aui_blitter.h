#ifndef __AUI_BLITTER_H__
#define __AUI_BLITTER_H__


#include "aui_base.h"


class aui_Surface;
class aui_DirtyList;
struct aui_SpanList;



#define k_AUI_BLITTER_FLAG_COPY			0x00000001  /// copy src to dst
#define k_AUI_BLITTER_FLAG_CHROMAKEY	0x00000002  /// masking color of a src surface, src pixel not copied if the color equals to the given one
#define k_AUI_BLITTER_FLAG_BLEND		0x00000004  /// blend between src and dst with factor=1/2
#define k_AUI_BLITTER_FLAG_STIPPLE		0x00000008  /// never used
#define k_AUI_BLITTER_FLAG_FAST			0x00000010  /// fast blt, only secondary -> primary surface


#define k_AUI_BLITTER_FLAG_OUT			0x00010000  /// outer bevel, never used
#define k_AUI_BLITTER_FLAG_IN			0x00020000  /// inner bevel

struct aui_Stencil;
aui_Stencil *aui_CreateStencil(aui_Surface *pSurface);

namespace aui
{

	class IBlitter
	{
	public:
		virtual ~IBlitter() = default;


		// k_AUI_BLITTER_FLAG_COPY
		// k_AUI_BLITTER_FLAG_CHROMAKEY
		// k_AUI_BLITTER_FLAG_BLEND
		// k_AUI_BLITTER_FLAG_CHROMAKEY & k_AUI_BLITTER_FLAG_BLEND
		typedef AUI_ERRCODE(RobustBltFunc)(
			aui_Surface *destSurf,
			sint32 destx,
			sint32 desty,
			aui_Surface *srcSurf,
			RECT *srcRect,
			uint32 flags);

		virtual RobustBltFunc Blt = 0;

		// k_AUI_BLITTER_FLAG_COPY
		// k_AUI_BLITTER_FLAG_CHROMAKEY
		typedef AUI_ERRCODE(RobustTileBltFunc)(
			aui_Surface *destSurf,
			RECT *destRect,
			aui_Surface *srcSurf,
			RECT *srcRect,
			sint32 anchorx,
			sint32 anchory,
			uint32 flags);

		virtual RobustTileBltFunc TileBlt = 0;

		// k_AUI_BLITTER_FLAG_IN
		// k_AUI_BLITTER_FLAG_OUT
		typedef AUI_ERRCODE(RobustBevelBltFunc)(
			aui_Surface *destSurf,
			RECT *destRect,
			RECT *bevelRect,
			sint32 bevelThickness,
			sint32 lightx,
			sint32 lighty,
			uint32 flags);

		// 1 call
		virtual RobustBevelBltFunc BevelBlt = 0;

		// no flag
		typedef AUI_ERRCODE(RobustColorBltFunc)(
			aui_Surface *destSurf,
			RECT *destRect,
			COLORREF color,
			uint32 flags);

		virtual RobustColorBltFunc ColorBlt = 0;

		typedef AUI_ERRCODE(NakedColorBltFunc)(
			aui_Surface *destSurf,
			RECT *destRect,
			uint32 color,
			uint32 flags);

		// TODO(EG): implement with ColorBlt and COLORREF -> Pixel16
		virtual NakedColorBltFunc ColorBlt16 = 0;

		// 1 call
		virtual AUI_ERRCODE StencilMixBlt16(
			aui_Surface *destSurf,
			RECT *destRect,
			aui_Surface *topSurf,
			RECT *topRect,
			aui_Surface *bottmSurf,
			RECT *bottomRect,
			aui_Stencil *stencil,
			RECT *stencilRect) = 0;

		// k_AUI_BLITTER_FLAG_COPY
		// k_AUI_BLITTER_FLAG_CHROMAKEY
		typedef AUI_ERRCODE(RobustStretchBltFunc)(
			aui_Surface *destSurf,
			RECT *destRect,
			aui_Surface *srcSurf,
			RECT *srcRect,
			uint32 flags);

		virtual RobustStretchBltFunc StretchBlt = 0;
	};

}


class aui_Blitter : public aui::IBlitter, protected aui_Base
{

protected:
	aui_Blitter() {}
	virtual ~aui_Blitter() {}

protected:

	virtual AUI_ERRCODE Blt(
		aui_Surface *destSurf,
		sint32 destx,
		sint32 desty,
		aui_Surface *srcSurf,
		RECT *srcRect,
		uint32 flags) override;

	virtual RobustTileBltFunc TileBlt;
	virtual RobustBevelBltFunc BevelBlt;
	virtual RobustColorBltFunc ColorBlt;
	virtual NakedColorBltFunc ColorBlt16;

	virtual AUI_ERRCODE StencilMixBlt16(
		aui_Surface *destSurf,
		RECT *destRect,
		aui_Surface *topSurf,
		RECT *topRect,
		aui_Surface *bottmSurf,
		RECT *bottomRect,
		aui_Stencil *stencil,
		RECT *stencilRect) override;

	virtual AUI_ERRCODE StretchBlt(
		aui_Surface *destSurf,
		RECT *destRect,
		aui_Surface *srcSurf,
		RECT *srcRect,
		uint32 flags) override;


protected:

	typedef AUI_ERRCODE(NakedBltFunc)(
		aui_Surface *destSurf,
		RECT *destRect,
		aui_Surface *srcSurf,
		RECT *srcRect,
		uint32 flags);
	virtual NakedBltFunc Blt8To8;
	virtual NakedBltFunc Blt16To16;
	virtual NakedBltFunc Blt24To24;

private:

	typedef AUI_ERRCODE(NakedTileBltFunc)(
		aui_Surface *destSurf,
		RECT *destRect,
		aui_Surface *srcSurf,
		RECT *srcRect,
		sint32 anchorx,
		sint32 anchory,
		uint32 flags);
	virtual NakedTileBltFunc TileBlt8To8;
	virtual NakedTileBltFunc TileBlt16To16;
	virtual NakedTileBltFunc TileBlt24To24;


private:

	typedef AUI_ERRCODE(NakedBevelBltFunc)(
		aui_Surface *destSurf,
		RECT *destRect,
		RECT *bevelRect,
		sint32 bevelThickness,
		sint32 lightx,
		sint32 lighty,
		uint32 flags);

	virtual NakedBevelBltFunc BevelBlt8;
	virtual NakedBevelBltFunc BevelBlt16;
	virtual NakedBevelBltFunc BevelBlt24;

protected:



	virtual NakedColorBltFunc ColorBlt8;
	
	virtual NakedColorBltFunc ColorBlt24;

protected:
	typedef AUI_ERRCODE (RobustColorStencilBltFunc)(
		aui_Surface *destSurf,
		RECT *destRect,
		aui_Surface *stencilSurf,
		RECT *stencilRect,
		COLORREF color,
		uint32 flags );

	typedef AUI_ERRCODE (NakedColorStencilBltFunc)(
		aui_Surface *destSurf,
		RECT *destRect,
		aui_Surface *stencilSurf,
		RECT *stencilRect,
		uint32 color,
		uint32 flags );

	
	virtual RobustColorStencilBltFunc ColorStencilBlt;

	virtual NakedColorStencilBltFunc ColorStencilBlt8;
	virtual NakedColorStencilBltFunc ColorStencilBlt16;
	virtual NakedColorStencilBltFunc ColorStencilBlt24;

private:
	AUI_ERRCODE StencilBlt16(
		aui_Surface *destSurf,
		RECT *destRect,
		aui_Surface *stencilSurf,
		RECT *stencilRect,
		aui_Surface *sourceSurf,
		RECT *sourceRect,
		uint32 flags);

protected:


	typedef AUI_ERRCODE(NakedStretchBltFunc)(
		aui_Surface *destSurf,
		RECT *destRect,
		aui_Surface *srcSurf,
		RECT *srcRect,
		uint32 flags);

	virtual NakedStretchBltFunc StretchBlt8To8;
	virtual NakedStretchBltFunc StretchBlt16To16;
	virtual NakedStretchBltFunc StretchBlt24To24;


	
private:
	AUI_ERRCODE SpanBlt(
		aui_Surface *destSurf,
		sint32 destx,
		sint32 desty,
		aui_Surface *srcSurf,
		aui_DirtyList *srcDirtyList,
		uint32 flags );

	AUI_ERRCODE SpanBlt16To16(
		aui_Surface *destSurf,
		sint32 destx,
		sint32 desty,
		aui_Surface *srcSurf,
		aui_SpanList *srcSpanListArray,
		uint32 flags );
};


#endif 
