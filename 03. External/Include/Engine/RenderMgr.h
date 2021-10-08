#pragma once

class CBlendState;
class CDepthStencilState;
class CCamera;
class CLight3D;

class CRenderTarget24;
class CMRT;

#include "Ptr.h"
#include "Texture.h"

class CRenderMgr
{
	SINGLE(CRenderMgr);

private:
	IDXGISwapChain*			m_pSwapChain;

	D3D11_VIEWPORT			m_tViewportDesc;
	tResolution				m_tResolution;

	CRenderTarget24*		m_arrRT[(UINT)RT_TYPE::END];
	CMRT*					m_arrMRT[(UINT)MRT_TYPE::END];

	CBlendState*			m_arrBlendState[(UINT)BLEND_TYPE::END];
	CDepthStencilState*		m_arrDepthStencilState[(UINT)DEPTH_STENCIL_STATE::END];

	ID3D11SamplerState*		m_pSampler0;
	ID3D11SamplerState*		m_pSampler1;

	tLight2D				m_arrLight2D[100];
	UINT					m_iLight2DCount;

	vector<CLight3D*>		m_vecLight3D;

	vector<CCamera*>		m_vecCam;
	CCamera*				m_pToolCam;
	CCamera*				m_pMainCam;


	bool					m_bWindowed;

public:
	void init(HWND _hWnd, const tResolution & _res, bool _bWindow);
	void render();
	void render_tool();

	void render_shadowmap();
	void render_lights();
	void merge_light();

	void CopySwapToPosteffect();

	void Clear(float(&_arrFloat)[4]);
	void UpdateSampler();
	void Present() { m_pSwapChain->Present(0, 0); }

	CMRT* GetMRT(MRT_TYPE _eType) { return m_arrMRT[(UINT)_eType]; }

private:
	void CreateSwapChain();

	// ViewPort ¸¸µé±â
	void CreateViewPort();

	void CreateRenderTarget();
	void CreateMultiRenderTarget();

	void CreateSamplerState();
	void CreateBlendState();
	void CreateDepthStencilState();

	void UpdateLight2D();
	void UpdateLight3D();

public:
	CBlendState* GetBlendState(BLEND_TYPE _eType) { return m_arrBlendState[(UINT)_eType]; }
	CDepthStencilState* GetDepthStencilState(DEPTH_STENCIL_STATE _eType) { return m_arrDepthStencilState[(UINT)_eType]; }

	void RegisterLight2D(const tLight2D& _Light2D)
	{
		if (100 <= m_iLight2DCount)
		{
			return;
		}

		m_arrLight2D[m_iLight2DCount++] = _Light2D;
	}

	int RegisterLight3D(CLight3D* _pLight3D) {
		if (m_vecLight3D.size() >= 100)
			return -1;
		m_vecLight3D.push_back(_pLight3D);
		return (int)m_vecLight3D.size() - 1;
	}

	CCamera* GetCamera(int _iIdx) { return m_vecCam[_iIdx]; }
	vector<CCamera*>* GetCamera() { return &m_vecCam; }
	CCamera* GetMainCam();

	void RegisterCamera(CCamera* _pCam) { m_vecCam.push_back(_pCam); }
	void RegisterToolCamera(CCamera* _pToolCam) { m_pToolCam = _pToolCam; }
	void RegisterMainCamera(CCamera* _pMainCam) { m_pMainCam = _pMainCam; }
	void ClearCamera() { m_vecCam.clear(); }

	CCamera* GetToolCamera() { return m_pToolCam; };
	CCamera* GetMainCamera() { return m_pMainCam; }

	tResolution GetResolution() { return m_tResolution; }

	friend class CSceneMgr;
};


