#pragma once

#include "SceneMgr.h"
#include "RenderMgr.h"

enum SCENE_MOD
{
	SCENE_PLAY,
	SCENE_STOP,
	SCENE_PAUSE,
	SCENE_LOAD,
};

typedef void(CSceneMgr::*SCENEMGR_UPDATE)(void);
typedef void(CRenderMgr::*RENDERMGR_RENDER)(void);

class CCore
{
private:
	CCore(); 
	~CCore();

public:
	static CCore* GetInst()
	{
		static CCore mgr;
		return &mgr;
	}

private:
	SCENEMGR_UPDATE		pUpdateFunc;
	RENDERMGR_RENDER	pRenderFunc;

	HWND				m_hMainHwnd;
	__int64				m_eMod;
	
public:
	int init(HWND _hWnd, const tResolution& _resolution, bool _bWindow);
	void progress();

public:
	void SetSceneMod(SCENE_MOD _eMod);
	__int64 GetSceneMod() { return m_eMod; }
	HWND GetHwnd() { return m_hMainHwnd; };

private:
	void ChangeWindowSize(HWND _hWnd, const tResolution& _resolution);	
};

