#include "stdafx.h"
#include "DepthStencilState.h"

#include "Device.h"

CDepthStencilState::CDepthStencilState()
	: m_pState(nullptr)
	, m_tDesc{}
{
}

CDepthStencilState::~CDepthStencilState()
{
	SAFE_RELEASE(m_pState);
}

void CDepthStencilState::Create(const D3D11_DEPTH_STENCIL_DESC & _tDesc)
{
	m_tDesc = _tDesc;

	if (FAILED(DEVICE->CreateDepthStencilState(&_tDesc, &m_pState)))
		assert(nullptr);
}

void CDepthStencilState::UpdateData(UINT _iRef)
{
	CONTEXT->OMSetDepthStencilState(m_pState, _iRef);
}
