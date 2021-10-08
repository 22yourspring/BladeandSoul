#include "stdafx.h"
#include "KeyMgr.h"
#include "Core.h"
#include "Device.h"

// KEY_TYPE 에 대응하는 가상키
int arrVK[(UINT)KEY_TYPE::KEY_END] =
{
	'1', // KEY_1
	'2', // KEY_2
	'3', // KEY_3
	'4', // KEY_4
	'5', // KEY_5
	'6', // KEY_6
	'7', // KEY_7
	'8', // KEY_8
	'9', // KEY_9
	'0', // KEY_0
	'Q', // KEY_Q
	'W', // KEY_W,
	'E', // KEY_E
	'R', // KEY_R,
	'T', // KEY_T,
	'Y', // KEY_Y,
	'U', // KEY_U,
	'I', // KEY_I,
	'O', // KEY_O,
	'P', // KEY_P,
	'A', // KEY_A,
	'S', // KEY_S,
	'D', // KEY_D,
	'F', // KEY_F,
	'G', // KEY_G,
	'H', // KEY_H,
	'J', // KEY_J,
	'K', // KEY_K,
	'L', // KEY_L,
	'Z', // KEY_Z,
	'X', // KEY_X,
	'C', // KEY_C,
	'V', // KEY_V,
	'B', // KEY_B,
	'N', // KEY_N,
	'M', // KEY_M,

	VK_F1,	// KEY_F1,
	VK_F2,	// KEY_F2,
	VK_F3,	// KEY_F3,
	VK_F4,	// KEY_F4,
	VK_F5,	// KEY_F5,
	VK_F6,	// KEY_F6,
	VK_F7,	// KEY_F7,
	VK_F8,	// KEY_F8,
	VK_F9,	// KEY_F9,
	VK_F10,	// KEY_F10,
	VK_F11,	// KEY_F11,
	VK_F12,	// KEY_F12,

	VK_LCONTROL,// KEY_CTRL,
	VK_LMENU,	// KEY_ALT,
	VK_LSHIFT,	// KEY_LSHIFT,
	VK_RSHIFT,	// KEY_RSHIFT,
	VK_TAB,		// KEY_TAB,
	VK_CAPITAL,	// KEY_CAP,
	VK_RETURN,	// KEY_ENTER,
	VK_ESCAPE,	// KEY_ESC,
	VK_SPACE,	// KEY_SPACE,
	VK_UP,		// KEY_UP,
	VK_DOWN,	// KEY_DOWN,
	VK_LEFT,	// KEY_LEFT,
	VK_RIGHT,	// KEY_RIGHT,
	VK_NUMPAD1,	// KEY_NUM1,
	VK_NUMPAD2,	// KEY_NUM2,
	VK_NUMPAD3,	// KEY_NUM3,
	VK_NUMPAD4,	// KEY_NUM4,
	VK_NUMPAD5,	// KEY_NUM5,
	VK_NUMPAD6,	// KEY_NUM6,
	VK_NUMPAD7,	// KEY_NUM7,
	VK_NUMPAD8,	// KEY_NUM8,
	VK_NUMPAD9,	// KEY_NUM9,
	VK_NUMPAD0,	// KEY_NUM0,	

	VK_LBUTTON,// KEY_LBTN
	VK_RBUTTON,// KEY_RBTN
};

CKeyMgr::CKeyMgr()
{
}

CKeyMgr::~CKeyMgr()
{
}

void CKeyMgr::init()
{
	for (UINT i = 0; i < (UINT)KEY_TYPE::KEY_END; ++i)
	{
		m_vecKey.push_back(tKeyState{ KEY_STATE::STATE_NONE, false });
	}
}

void CKeyMgr::update()
{
	// 현재 윈도우가 Active(활성화, 포커싱) 상태가 아니라면, 
	// 모든 키 상태를 초기상태로 변경한다.
	HWND hActive = GetActiveWindow();

	if (nullptr == hActive)
	{
		for (UINT i = 0; i < (UINT)KEY_TYPE::KEY_END; ++i)
		{
			m_vecKey[i].bPrevPress = false;
			m_vecKey[i].eState = KEY_STATE::STATE_NONE;
		}
		return;
	}
	if (CCore::GetInst()->GetHwnd() == GetFocus())
	{
		// 윈도우가 활성상태라면 키를 정상적으로 update 한다.
		for (UINT i = 0; i < (UINT)KEY_TYPE::KEY_END; ++i)
		{
			// 현재 키가 눌려있다.
			if (GetAsyncKeyState(arrVK[i]) & 0x8000)
			{
				if (m_vecKey[i].bPrevPress) // 이전 프레임에 눌려있었다.
				{
					m_vecKey[i].eState = KEY_STATE::STATE_HOLD;
				}
				else
				{
					m_vecKey[i].eState = KEY_STATE::STATE_TAB;
				}
				m_vecKey[i].bPrevPress = true;
			}
			else // 키가 눌리지 않은 상태
			{
				if (m_vecKey[i].bPrevPress) // 이전 프레임에 눌려있었다.
				{
					m_vecKey[i].eState = KEY_STATE::STATE_AWAY;
				}
				else
				{
					m_vecKey[i].eState = KEY_STATE::STATE_NONE;
				}
				m_vecKey[i].bPrevPress = false;
			}
		}

		// 마우스 좌표	
		m_ptOldMouse = m_ptMouse;
		GetCursorPos(&m_ptMouse);
		ScreenToClient(CCore::GetInst()->GetHwnd(), &m_ptMouse);

		m_vDragDir = Vec2((float)(m_ptMouse.x - m_ptOldMouse.x), (float)(m_ptOldMouse.y - m_ptMouse.y));
	}
}