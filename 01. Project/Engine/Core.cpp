#include "stdafx.h"
#include "Core.h"

#include "Device.h"
#include "RenderMgr.h"
#include "KeyMgr.h"
#include "TimeMgr.h"

#include "SceneMgr.h"
#include "PathMgr.h"
#include "ResMgr.h"
#include "FontMgr.h"
#include "CollisionMgr.h"
#include "EventMgr.h"
#include "InstancingBuffer.h"
#include "SSLRManager.h"
#include "MRT.h"
#include "RenderMgr.h"
#include "RenderTarget24.h"
#include "Texture.h"
#include "SSAOManager.h"
#include "PostFX.h"

CCore::CCore()
	: pUpdateFunc(nullptr)
	, pRenderFunc(nullptr)
	, m_hMainHwnd(nullptr)
	, m_eMod(SCENE_MOD::SCENE_STOP)
	
{
}

CCore::~CCore()
{
}

int CCore::init(HWND _hWnd, const tResolution & _resolution, bool _bWindow)
{
	m_hMainHwnd = _hWnd;	
	ShowWindow(_hWnd, true);

	if(FAILED(CDevice::GetInst()->init()))
	{
		return E_FAIL;
	}	

	// 상수버퍼 만들기
	CDevice::GetInst()->CreateConstBuffer(L"Transform Matrix", sizeof(tTransform), 0);
	CDevice::GetInst()->CreateConstBuffer(L"Material Parameter", sizeof(tMtrlParam), 1);
	CDevice::GetInst()->CreateConstBuffer(L"Anim2D", sizeof(tAnim2D), 2);	
	CDevice::GetInst()->CreateConstBuffer(L"Light2D", sizeof(tLight2D) * 100 + 16, 3);
	CDevice::GetInst()->CreateConstBuffer(L"Light3D", sizeof(tLight3D) * 100 + 16, 4);

	CDevice::GetInst()->CreateConstBuffer(L"GlobalValue", sizeof(tGlobalValue), 5);
	CDevice::GetInst()->CreateConstBuffer(L"UNPACK", sizeof(tUnPack), 6);

	g_transform.matWorld = XMMatrixIdentity();
	g_transform.matView = XMMatrixIdentity();
	g_transform.matProj = XMMatrixIdentity();

	CFontMgr::GetInst()->init();
	CRenderMgr::GetInst()->init(_hWnd, _resolution, _bWindow);
	CPathMgr::init();
	CKeyMgr::GetInst()->init();
	CTimeMgr::GetInst()->init();
	CResMgr::GetInst()->init();
	CCollisionMgr::GetInst()->init();
	CSceneMgr::GetInst()->init();
	CInstancingBuffer::GetInst()->init();
	CPostFX::GetInst()->Init(_resolution.fWidth, _resolution.fHeight);
	CSSAOManager::GetInst()->Init(_resolution.fWidth, _resolution.fHeight);
	CSSLRManager::GetInst()->Init(_resolution.fWidth, _resolution.fHeight);
	   
	size_t iSize = sizeof(pUpdateFunc);

	if (iSize == 24)
	{
		MessageBox(m_hMainHwnd, L"사이즈", L"24", MB_OK);
	}

	return S_OK;
}

void CCore::ChangeWindowSize(HWND _hWnd, const tResolution & _resolution)
{
	RECT rt = { 0, 0, (int)_resolution.fWidth, (int)_resolution.fHeight };

	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
	SetWindowPos(_hWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);

	GetClientRect(_hWnd, &rt);
}

void CCore::progress()
{
	CKeyMgr::GetInst()->update();
	CTimeMgr::GetInst()->update();
	CSound::g_pFMOD->update();

	{
		(CSceneMgr::GetInst()->*pUpdateFunc)();
		(CRenderMgr::GetInst()->*pRenderFunc)();
	}
	CEventMgr::GetInst()->update();
}

void CCore::SetSceneMod(SCENE_MOD _eMod)
{
	m_eMod = _eMod;
	if (SCENE_MOD::SCENE_PLAY == m_eMod)
	{
		pUpdateFunc = &CSceneMgr::update;
		pRenderFunc = &CRenderMgr::render;
	}
	else 
	{
		pUpdateFunc = &CSceneMgr::update_tool;
		pRenderFunc = &CRenderMgr::render_tool;
	}
}
