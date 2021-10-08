#include "stdafx.h"
#include "RenderTarget24.h"

#include "Device.h"

CRenderTarget24::CRenderTarget24()
{
}

CRenderTarget24::~CRenderTarget24()
{
}

void CRenderTarget24::Create(Ptr<CTexture> _pTargetTex, Vec4  _vClearColor)
{
	m_pTargetTex = _pTargetTex;
	SetClearColor(_vClearColor);
}

void CRenderTarget24::Clear()
{
	CONTEXT->ClearRenderTargetView(m_pTargetTex->GetRTV(), m_ClearColor);
}
