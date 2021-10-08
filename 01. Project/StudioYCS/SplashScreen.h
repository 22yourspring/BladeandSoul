#pragma once

#include "gif_control.hpp"

// CSplashScreen 대화 상자

class CSplashScreen : public CDialogEx
{
	DECLARE_DYNAMIC(CSplashScreen)

public:
	CSplashScreen(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CSplashScreen();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SplashScreen };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	gif_control _gif_control;

	virtual void OnOK();
	virtual void OnCancel();
public:
	void DestroySplash();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
