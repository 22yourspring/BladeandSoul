#include "stdafx.h"
#include "MRT.h"
#include "RenderTarget24.h"
#include "Texture.h"

#include "Device.h"

CMRT::CMRT()
	: m_arrRT{}
	, m_pDepthStencilTex(nullptr)
{
}

CMRT::~CMRT()
{
}

void CMRT::Create(UINT _iCount, CRenderTarget24 ** _pRT, Ptr<CTexture> _pDSTex)
{
	m_iCount = _iCount;		
	memcpy(m_arrRT, _pRT, sizeof(void*) * m_iCount);
	m_pDepthStencilTex = _pDSTex;

	m_tViewPortDesc.Width = _pRT[0]->GetRTTex()->Width();
	m_tViewPortDesc.Height = _pRT[0]->GetRTTex()->Height();
	m_tViewPortDesc.MinDepth = 0.f;
	m_tViewPortDesc.MaxDepth = 1.f;
	m_tViewPortDesc.TopLeftX = 0;
	m_tViewPortDesc.TopLeftY = 0;
}

void CMRT::OMSet()
{	
	ID3D11RenderTargetView* arrRTV[8] = {};
	ID3D11DepthStencilView* pDSV = nullptr;

	for (UINT i = 0; i < m_iCount; ++i)
	{		
		arrRTV[i] = m_arrRT[i]->GetRTTex()->GetRTV();
	}

	if (m_pDepthStencilTex != nullptr)
	{
		pDSV = m_pDepthStencilTex->GetDSV();
	}
	else
	{
		CONTEXT->OMGetRenderTargets(0, nullptr, &pDSV);
		SAFE_RELEASE(pDSV); // Get 으로 증가한 REF 줄이기
	}

	CONTEXT->OMSetRenderTargets(m_iCount, arrRTV, pDSV);
	CONTEXT->RSSetViewports(1, &m_tViewPortDesc); // 뷰포트 설정
}

void CMRT::Clear()
{
	for (UINT i = 0; i < m_iCount; ++i)
	{
		m_arrRT[i]->Clear();
	}
	
	if (m_pDepthStencilTex != nullptr)
	{		
		CONTEXT->ClearDepthStencilView(m_pDepthStencilTex->GetDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}	
}
