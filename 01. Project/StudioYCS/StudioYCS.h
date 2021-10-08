
// StudioYCS.h: StudioYCS 응용 프로그램의 기본 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // 주 기호입니다.


// CStudioYCSApp:
// 이 클래스의 구현에 대해서는 StudioYCS.cpp을(를) 참조하세요.
//
class CSplashScreen;
class CStudioYCSApp : public CWinAppEx
{
public:
	CMDIFrameWnd*		pFrameWnd;
	CStudioYCSApp() noexcept;

public:
	void TestCode();
	void CreateTestScene();

	CSplashScreen*	 pSplash;

	// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	// 구현입니다.
	COleTemplateServer m_server;
	// 문서 만들기에 대한 서버 개체입니다.
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	afx_msg void OnAppPause();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern CStudioYCSApp theApp;
