#include "stdafx.h"
#include "BlendState.h"

#include "Device.h"

CBlendState::CBlendState()
	: m_pState(nullptr)
	, m_tDesc{}
{
}

CBlendState::~CBlendState()
{
	SAFE_RELEASE(m_pState);
}

void CBlendState::Create(const D3D11_BLEND_DESC & _tDesc)
{
	m_tDesc = _tDesc;

	if (FAILED(DEVICE->CreateBlendState(&m_tDesc, &m_pState)))
		assert(nullptr);
}

void CBlendState::UpdateData()
{
	CONTEXT->OMSetBlendState(m_pState, nullptr, 0xffffffff);
}
