// SplashScreen.cpp: 구현 파일
//

#include "pch.h"
#include "SplashScreen.h"
#include "afxdialogex.h"

#include "resource.h"

#include "StudioYCS.h"

#include <Engine/PathMgr.h>
// CSplashScreen 대화 상자

IMPLEMENT_DYNAMIC(CSplashScreen, CDialogEx)

CSplashScreen::CSplashScreen(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SplashScreen, pParent)
{

}

CSplashScreen::~CSplashScreen()
{
}

void CSplashScreen::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GIF, _gif_control);
}


BEGIN_MESSAGE_MAP(CSplashScreen, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSplashScreen 메시지 처리기


BOOL CSplashScreen::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	CPathMgr::init();

	wstring ResPath = CPathMgr::GetResPath();
	ResPath  += L"SplashScreen\\Screen.gif";

	_gif_control.load(ResPath.c_str());
	_gif_control.play();

	SetTimer(0, 4000, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSplashScreen::DestroySplash()
{
	delete this;
}

void CSplashScreen::OnOK() {}
void CSplashScreen::OnCancel() {}

void CSplashScreen::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	EndDialog(0);
	CDialogEx::OnTimer(nIDEvent);
}
