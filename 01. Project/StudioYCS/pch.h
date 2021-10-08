// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"

#endif //PCH_H


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


// Game Engine 
#include <Engine/global.h>
#include <afxwin.h>
#include <afxcontrolbars.h>
#include <afxcmn.h>


// SplashScreen
#include <gdiplus.h>
#pragma comment(lib, "gdiplus")

#include "memory"
#include "afxdialogex.h"


#ifdef _DEBUG
#pragma comment(lib, "Engine/Engine_debug.lib")
#else
#pragma comment(lib, "Engine/Engine.lib")
#endif

#ifdef _DEBUG
#pragma comment(lib, "Script/Script_debug.lib")
#else
#pragma comment(lib, "Script/Script.lib")
#endif


// 툴 작업영역 크기
#define GAMEVIEW_X 1920.f
#define GAMEVIEW_Y 1080.f

//#define BTNVIEW_X GAMEVIEW_X
//#define BTNVIEW_Y 40
//
//#define DBGVIEW_X GAMEVIEW_X
//#define DBGVIEW_Y 250
//
//#define HIERACHYVIEW_X 270
//#define HIERACHYVIEW_Y (GAMEVIEW_Y + BTNVIEW_Y + DBGVIEW_Y + 2)
//
//#define INFOVIEW_X 330
//#define INFOVIEW_Y (GAMEVIEW_Y + BTNVIEW_Y + DBGVIEW_Y + 2)
//
//#define FRAME_X	 (GAMEVIEW_X + HIERACHYVIEW_X + INFOVIEW_X + 2 + 1 + 1 + 2) // 2는 경계 1픽셀 2개
//#define FRAME_Y	 (GAMEVIEW_Y + BTNVIEW_Y + DBGVIEW_Y + 2  + 1 + 1 + 2) 


extern int g_int[10];