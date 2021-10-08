#include "stdafx.h"
#include "FontMgr.h"

#include "Device.h"

CFontMgr::CFontMgr()
{
}

CFontMgr::~CFontMgr()
{
	SAFE_RELEASE(m_pFW1Factory);
	SAFE_RELEASE(m_pFontWrapper);
}


void CFontMgr::init()
{
	if (FAILED(FW1CreateFactory(FW1_VERSION, &m_pFW1Factory)))
	{
		assert(NULL);
	}

	if (FAILED(m_pFW1Factory->CreateFontWrapper(DEVICE, L"Arial", &m_pFontWrapper)))
	{
		assert(NULL);
	}
}

void CFontMgr::DrawFont(wchar_t * _pStr, float _fPosX, float _fPosY, float _fSize, UINT _Color)
{
	m_pFontWrapper->DrawString(
		CONTEXT,
		_pStr, // String
		_fSize,// Font size
		_fPosX,// X position
		_fPosY,// Y position
		_Color,// Text color, 0xAaBbGgRr
		0      // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
	);
}

