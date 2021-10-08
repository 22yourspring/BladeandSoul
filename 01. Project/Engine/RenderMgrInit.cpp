#include "stdafx.h"
#include "RenderMgr.h"

#include "Device.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "ResMgr.h"

#include "RenderTarget24.h"
#include "MRT.h"
#include "Core.h"

void CRenderMgr::init(HWND _hWnd, const tResolution & _res, bool _bWindow)
{
	m_tResolution = _res;
	m_bWindowed = _bWindow;

	g_global.vResolution = m_tResolution;

	// SwapChain �����
	CreateSwapChain();

	// ViewPort �����
	CreateViewPort();

	CreateRenderTarget();

	CreateMultiRenderTarget();

	CreateSamplerState();

	CreateBlendState();

	CreateDepthStencilState();
}


void CRenderMgr::CreateSwapChain()
{
	// ��� �����츦 �����Ͽ�, ȭ�� ���ſ����� ���
	DXGI_SWAP_CHAIN_DESC tDesc = {};

	tDesc.BufferCount = 1; // ���� ī��Ʈ�� 1�� �־��ش�

	tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // ������ �ȼ� ����(�ȼ��� 4����Ʈ
	tDesc.BufferDesc.Width = (UINT)m_tResolution.fWidth;  // ������ �ػ�(������ �ػ󵵶� ��ġ���ѳ���)
	tDesc.BufferDesc.Height = (UINT)m_tResolution.fHeight;// ������ �ػ�(������ �ػ󵵶� ��ġ���ѳ���)
	tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING::DXGI_MODE_SCALING_UNSPECIFIED; // ������� �ػ� ����ġ�� ȭ�� �����ϸ��� �Ұ�����
	tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // �ȼ��� ��ĵ���� ����
	tDesc.BufferDesc.RefreshRate.Denominator = 60; // ȭ�� ���� ����
	tDesc.BufferDesc.RefreshRate.Numerator = 1;    // ȭ�� ���� ���� 

	tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // ��� Ÿ�� �뵵�� ���۸� �����.
	tDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	tDesc.OutputWindow = CCore::GetInst()->GetHwnd();	// ��� ������
	tDesc.Windowed = m_bWindowed;   // â ��� or ��üȭ�� ���
	tDesc.SampleDesc.Count = 1;		// ��Ƽ ���� ��� ����
	tDesc.SampleDesc.Quality = 0;
	tDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD; // ���� �ĸ� ���� ��ü �� ���� ������ ���� ����

	IDXGIDevice* pDXGIDevice = nullptr;
	IDXGIAdapter* pAdapter = nullptr;
	IDXGIFactory* pFactory = nullptr;

	DEVICE->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDXGIDevice);
	pDXGIDevice->GetAdapter(&pAdapter);
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);

	pFactory->CreateSwapChain(DEVICE, &tDesc, &m_pSwapChain);

	SAFE_RELEASE(pDXGIDevice);
	SAFE_RELEASE(pAdapter);
	SAFE_RELEASE(pFactory);
}

void CRenderMgr::CreateViewPort()
{
	m_tViewportDesc.TopLeftX = 0.f;
	m_tViewportDesc.TopLeftY = 0.f;
	m_tViewportDesc.Width = m_tResolution.fWidth;
	m_tViewportDesc.Height = m_tResolution.fHeight;
	m_tViewportDesc.MinDepth = 0.f;
	m_tViewportDesc.MaxDepth = 1.f;

	CONTEXT->RSSetViewports(1, &m_tViewportDesc);
}

void CRenderMgr::CreateRenderTarget()
{
	Ptr<CTexture> pTargetTex = nullptr;

	//=================
	// SwapChain Target
	//=================
	ID3D11Texture2D* pTex2D = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pTex2D);
	SAFE_RELEASE(pTex2D); // GetBuffer ������ ref count ������ ����
	
	pTargetTex = CResMgr::GetInst()->CreateTexture(L"RenderTargetTex", pTex2D);

	m_arrRT[(UINT)RT_TYPE::SWAPCHAIN] = new CRenderTarget24;
	m_arrRT[(UINT)RT_TYPE::SWAPCHAIN]->Create(pTargetTex, Vec4(0.f, 0.f, 0.f, 0.f));
		
	
	//================
	// Diffuse Target
	//================
	pTargetTex = CResMgr::GetInst()->CreateTexture(L"DiffuseTargetTex"
		, m_tResolution.fWidth, m_tResolution.fHeight
		, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
		, DXGI_FORMAT_R8G8B8A8_UNORM);

	m_arrRT[(UINT)RT_TYPE::DIFFUSE] = new CRenderTarget24;
	m_arrRT[(UINT)RT_TYPE::DIFFUSE]->Create(pTargetTex, Vec4(0.f, 0.f, 0.f, 1.f));
	m_arrRT[(UINT)RT_TYPE::DIFFUSE]->Clear();


	// ==============
	// Normal Target
	// ==============
	pTargetTex = CResMgr::GetInst()->CreateTexture(L"NormalTargetTex"
		, m_tResolution.fWidth, m_tResolution.fHeight
		, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
		, DXGI_FORMAT_R32G32B32A32_FLOAT);

	m_arrRT[(UINT)RT_TYPE::NORMAL] = new CRenderTarget24;
	m_arrRT[(UINT)RT_TYPE::NORMAL]->Create(pTargetTex, Vec4(0.f, 0.f, 0.f, 1.f));
	m_arrRT[(UINT)RT_TYPE::NORMAL]->Clear();


	// ================
	// Position Target
	// ================
	pTargetTex = CResMgr::GetInst()->CreateTexture(L"PositionTargetTex"
		, m_tResolution.fWidth, m_tResolution.fHeight
		, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
		, DXGI_FORMAT_R32G32B32A32_FLOAT);

	m_arrRT[(UINT)RT_TYPE::POSITION] = new CRenderTarget24;
	m_arrRT[(UINT)RT_TYPE::POSITION]->Create(pTargetTex, Vec4(0.f, 0.f, 0.f, 1.f));
	m_arrRT[(UINT)RT_TYPE::POSITION]->Clear();

	// ================
	// SpecularMtrl Target
	// ================
	pTargetTex = CResMgr::GetInst()->CreateTexture(L"SpecularMtrlTargetTex"
		, m_tResolution.fWidth, m_tResolution.fHeight
		, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
		, DXGI_FORMAT_R32G32B32A32_FLOAT);

	m_arrRT[(UINT)RT_TYPE::SPECULARMTRL] = new CRenderTarget24;
	m_arrRT[(UINT)RT_TYPE::SPECULARMTRL]->Create(pTargetTex, Vec4(0.f, 0.f, 0.f, 1.f));
	m_arrRT[(UINT)RT_TYPE::SPECULARMTRL]->Clear();

	// ================
	// DepthBuffer Target
	// ================
	pTargetTex = CResMgr::GetInst()->CreateTexture(L"DepthBufferTargetTex"
		, m_tResolution.fWidth, m_tResolution.fHeight
		, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
		, DXGI_FORMAT_R32G32B32A32_FLOAT);

	m_arrRT[(UINT)RT_TYPE::DEPTH_BUFFER] = new CRenderTarget24;
	m_arrRT[(UINT)RT_TYPE::DEPTH_BUFFER]->Create(pTargetTex, Vec4(0.f, 0.f, 0.f, 1.f));
	m_arrRT[(UINT)RT_TYPE::DEPTH_BUFFER]->Clear();

	// ================
	// Light Target
	// ================
	pTargetTex = CResMgr::GetInst()->CreateTexture(L"LightTargetTex"
		, m_tResolution.fWidth, m_tResolution.fHeight
		, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
		, DXGI_FORMAT_R8G8B8A8_UNORM);

	m_arrRT[(UINT)RT_TYPE::LIGHT] = new CRenderTarget24;
	m_arrRT[(UINT)RT_TYPE::LIGHT]->Create(pTargetTex, Vec4(0.f, 0.f, 0.f, 1.f));
	m_arrRT[(UINT)RT_TYPE::LIGHT]->Clear();

	// ================
	// Specular Target
	// ================
	pTargetTex = CResMgr::GetInst()->CreateTexture(L"SpecularTargetTex"
		, m_tResolution.fWidth, m_tResolution.fHeight
		, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
		, DXGI_FORMAT_R8G8B8A8_UNORM);

	m_arrRT[(UINT)RT_TYPE::SPECULAR] = new CRenderTarget24;
	m_arrRT[(UINT)RT_TYPE::SPECULAR]->Create(pTargetTex, Vec4(0.f, 0.f, 0.f, 1.f));
	m_arrRT[(UINT)RT_TYPE::SPECULAR]->Clear();

	// ================
	// Shdowmap Target
	// ================
	pTargetTex = CResMgr::GetInst()->CreateTexture(L"ShadowMapTargetTex"
		, 8192, 8192
		, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE
		, DXGI_FORMAT_R32_FLOAT);

	m_arrRT[(UINT)RT_TYPE::SHADOWMAP] = new CRenderTarget24;
	m_arrRT[(UINT)RT_TYPE::SHADOWMAP]->Create(pTargetTex, Vec4(0.f, 0.f, 0.f, 0.f));
	m_arrRT[(UINT)RT_TYPE::SHADOWMAP]->Clear();

	// ================
	// PostEffectTarget
	// ================
	pTargetTex = CResMgr::GetInst()->CreateTexture(L"PosteffectTargetTex"
		, m_tResolution.fWidth, m_tResolution.fHeight
		, D3D11_BIND_SHADER_RESOURCE
		, DXGI_FORMAT_R8G8B8A8_UNORM);

	m_arrRT[(UINT)RT_TYPE::POSTEFFECT] = new CRenderTarget24;
	m_arrRT[(UINT)RT_TYPE::POSTEFFECT]->Create(pTargetTex, Vec4(0.f, 0.f, 0.f, 0.f));
	//m_arrRT[(UINT)RT_TYPE::POSTEFFECT]->Clear();
}

void CRenderMgr::CreateMultiRenderTarget()
{
	Ptr<CTexture> pTexture = nullptr;

	// ===============
	// SwapChain MRT
	// ===============
	{
		CRenderTarget24* arrRT[8] = { m_arrRT[(UINT)RT_TYPE::SWAPCHAIN] , };

		pTexture = CResMgr::GetInst()->CreateTexture(L"DepthStencilTex"
			, m_tResolution.fWidth, m_tResolution.fHeight
			, D3D11_BIND_DEPTH_STENCIL
			, DXGI_FORMAT_D24_UNORM_S8_UINT);

		m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->Create(1, arrRT, pTexture);
		m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->OMSet();
	}

	// ==============
	// Deferred MRT
	// ==============
	{
		CRenderTarget24* arrRT[8] = { m_arrRT[(UINT)RT_TYPE::DIFFUSE]
									, m_arrRT[(UINT)RT_TYPE::NORMAL]
									, m_arrRT[(UINT)RT_TYPE::POSITION]
									, m_arrRT[(UINT)RT_TYPE::SPECULARMTRL]
									, m_arrRT[(UINT)RT_TYPE::DEPTH_BUFFER] };

		m_arrMRT[(UINT)MRT_TYPE::DEFERRED] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->Create(5, arrRT, nullptr);
		m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->OMSet();
	}

	// ===========
	// Light MRT
	// ===========
	{
		CRenderTarget24* arrRT[8] = { m_arrRT[(UINT)RT_TYPE::LIGHT]
									, m_arrRT[(UINT)RT_TYPE::SPECULAR]};

		m_arrMRT[(UINT)MRT_TYPE::LIGHT] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::LIGHT]->Create(2, arrRT, nullptr);		
	}

	// ==============
	// ShadowMap MRT
	// ==============
	{
		pTexture = CResMgr::GetInst()->CreateTexture(L"ShadowmapDepthTex"
			, 8192, 8192
			, D3D11_BIND_DEPTH_STENCIL
			, DXGI_FORMAT_D32_FLOAT);

		CRenderTarget24* arrRT[8] = { m_arrRT[(UINT)RT_TYPE::SHADOWMAP] };

		m_arrMRT[(UINT)MRT_TYPE::SHADOWMAP] = new CMRT;
		m_arrMRT[(UINT)MRT_TYPE::SHADOWMAP]->Create(1, arrRT, pTexture);
	}
}

void CRenderMgr::CreateSamplerState()
{
	// Sampler �ϳ� �����
	D3D11_SAMPLER_DESC tSamDesc = {};

	tSamDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tSamDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tSamDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tSamDesc.Filter = D3D11_FILTER_ANISOTROPIC;

	DEVICE->CreateSamplerState(&tSamDesc, &m_pSampler0);

	CONTEXT->VSSetSamplers(0, 1, &m_pSampler0);
	CONTEXT->HSSetSamplers(0, 1, &m_pSampler0);
	CONTEXT->DSSetSamplers(0, 1, &m_pSampler0);
	CONTEXT->GSSetSamplers(0, 1, &m_pSampler0);
	CONTEXT->CSSetSamplers(0, 1, &m_pSampler0);
	CONTEXT->PSSetSamplers(0, 1, &m_pSampler0);

	tSamDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tSamDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tSamDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tSamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;

	DEVICE->CreateSamplerState(&tSamDesc, &m_pSampler1);

	CONTEXT->VSSetSamplers(1, 1, &m_pSampler1);
	CONTEXT->HSSetSamplers(1, 1, &m_pSampler1);
	CONTEXT->DSSetSamplers(1, 1, &m_pSampler1);
	CONTEXT->GSSetSamplers(1, 1, &m_pSampler1);
	CONTEXT->CSSetSamplers(1, 1, &m_pSampler1);
	CONTEXT->PSSetSamplers(1, 1, &m_pSampler1);
}


void CRenderMgr::CreateBlendState()
{
	// ============ 
	// Alpha Blend
	// ============
	{
		D3D11_BLEND_DESC tDesc = {};
		m_arrBlendState[(UINT)BLEND_TYPE::ALPHABLEND] = new CBlendState;

		tDesc.IndependentBlendEnable = false; // �� Ÿ�� ���� Blending ����� �����Ѵ�. (false, 0 �� Ÿ�� �ɼ��� �ϰ� ���� �Ѵ�)
		tDesc.AlphaToCoverageEnable = false;  // ��Ƽ ���ø��� �̿��Ͽ� ��Ȯ�� ���İ����� Blending �� �Ѵ�.

		tDesc.RenderTarget[0].BlendEnable = true;
		tDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		// RenderTarget pixel �� RGB ���� ��Ʈ ȥ�� ����
		// PixelShader ��� Color �� Alpha ���� Src ���
		// PixelShader ��� Color �� (1 - Alpha) ���� Target RGB �� ����� ����
		tDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		tDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA;
		tDesc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;

		// ��ġ�� ������ ����.(RenderTarget pixel �� Alpha �ڸ��� �� ���� ���� ����)
		tDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		tDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
		tDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;

		m_arrBlendState[(UINT)BLEND_TYPE::ALPHABLEND]->Create(tDesc);
	}

	// ==============
	// One-One Blend
	// ==============
	{
		D3D11_BLEND_DESC tDesc = {};
		m_arrBlendState[(UINT)BLEND_TYPE::ONEBLEND] = new CBlendState;

		tDesc.IndependentBlendEnable = false; // �� Ÿ�� ���� Blending ����� �����Ѵ�. (false, 0 �� Ÿ�� �ɼ��� �ϰ� ���� �Ѵ�)
		tDesc.AlphaToCoverageEnable = false;  // ��Ƽ ���ø��� �̿��Ͽ� ��Ȯ�� ���İ����� Blending �� �Ѵ�.

		tDesc.RenderTarget[0].BlendEnable = true;
		tDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		// RenderTarget pixel �� RGB ���� ��Ʈ ȥ�� ����
		// PixelShader ��� Color �� Alpha ���� Src ���
		// PixelShader ��� Color �� (1 - Alpha) ���� Target RGB �� ����� ����
		tDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		tDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_ONE;
		tDesc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_ONE;

		// ��ġ�� ������ ����.(RenderTarget pixel �� Alpha �ڸ��� �� ���� ���� ����)
		tDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
		tDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
		tDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;

		m_arrBlendState[(UINT)BLEND_TYPE::ONEBLEND]->Create(tDesc);
	}


}

void CRenderMgr::CreateDepthStencilState()
{
	m_arrDepthStencilState[(UINT)DEPTH_STENCIL_STATE::DEFAULT] = nullptr;

	CDepthStencilState*	pDSState = nullptr;
	D3D11_DEPTH_STENCIL_DESC tDesc = {};
	
	//====================================
	// Depth : LessEqual, Stencil = false
	//====================================
	tDesc.StencilEnable = false;
	tDesc.DepthEnable = true;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	tDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	pDSState = new CDepthStencilState;
	pDSState->Create(tDesc);

	m_arrDepthStencilState[(UINT)DEPTH_STENCIL_STATE::LESS_EQUAL] = pDSState;

	//====================================
	// Depth : Greater, Stencil = false
	//====================================
	tDesc.StencilEnable = false;
	tDesc.DepthEnable = true;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	tDesc.DepthFunc = D3D11_COMPARISON_GREATER;

	pDSState = new CDepthStencilState;
	pDSState->Create(tDesc);

	m_arrDepthStencilState[(UINT)DEPTH_STENCIL_STATE::GRATER] = pDSState;

	//====================================
	// Depth : Greater_eaual, Stencil = false
	//====================================
	tDesc.StencilEnable = false;
	tDesc.DepthEnable = true;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	tDesc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;

	pDSState = new CDepthStencilState;
	pDSState->Create(tDesc);

	m_arrDepthStencilState[(UINT)DEPTH_STENCIL_STATE::GRATER_EQUAL] = pDSState;

	//====================================================
	// Depth : false, DepthWrite = false, Stencil = false
	//====================================================
	tDesc.StencilEnable = false;
	tDesc.DepthEnable = false;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	tDesc.DepthFunc = D3D11_COMPARISON_NEVER;

	pDSState = new CDepthStencilState;
	pDSState->Create(tDesc);

	m_arrDepthStencilState[(UINT)DEPTH_STENCIL_STATE::NO_TEST_NO_WIRTE] = pDSState;

	//====================================================
	// DepthTest : true, DepthWrite = false, Stencil = false
	//====================================================
	tDesc.StencilEnable = false;
	tDesc.DepthEnable = true;
	tDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	tDesc.DepthFunc = D3D11_COMPARISON_LESS;

	pDSState = new CDepthStencilState;
	pDSState->Create(tDesc);

	m_arrDepthStencilState[(UINT)DEPTH_STENCIL_STATE::NO_WRITE] = pDSState;
}

