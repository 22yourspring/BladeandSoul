#pragma once

// 해당 프레임에서는 모두가 같은 키에대해서 동일한 상태값을 가져가야 한다.
// 최초 눌린 시점, 키를 최로로 뗀 시점
enum class KEY_TYPE
{
	KEY_1 = 0,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_0,
	KEY_Q,
	KEY_W,
	KEY_E,
	KEY_R,
	KEY_T,
	KEY_Y,
	KEY_U,
	KEY_I,
	KEY_O,
	KEY_P,
	KEY_A,
	KEY_S,
	KEY_D,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_Z,
	KEY_X,
	KEY_C,
	KEY_V,
	KEY_B,
	KEY_N,
	KEY_M,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,

	KEY_CTRL,
	KEY_ALT,
	KEY_LSHIFT,
	KEY_RSHIFT,
	KEY_LTAB,
	KEY_CAP,
	KEY_ENTER,
	KEY_ESC,
	KEY_SPACE,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,

	KEY_NUM1,
	KEY_NUM2,
	KEY_NUM3,
	KEY_NUM4,
	KEY_NUM5,
	KEY_NUM6,
	KEY_NUM7,
	KEY_NUM8,
	KEY_NUM9,
	KEY_NUM0,

	KEY_LBTN,
	KEY_RBTN,

	KEY_END,
};

enum class KEY_STATE
{
	STATE_NONE,		// 아무일도 없다.
	STATE_TAB,		// 키가 처음 눌린 상태
	STATE_HOLD,		// 눌리고 있는 상태
	STATE_AWAY,		// 키를 뗀 상태
};

struct tKeyState
{
	KEY_STATE	eState;
	bool		bPrevPress;
};

class CKeyMgr
{
	SINGLE(CKeyMgr);
private:
	vector<tKeyState>	m_vecKey;
	POINT				m_ptMouse;
	POINT				m_ptOldMouse;

	Vec2				m_vDragDir;
	   	 
public:
	void init();
	void update();	   
	   
public:
	KEY_STATE GetKeyState(KEY_TYPE _eKey) { return m_vecKey[(UINT)_eKey].eState; }
	const POINT & GetMousePos() { return m_ptMouse; }
	const POINT & GetPrevMousePos() { return m_ptOldMouse; }
	Vec2 GetDragDir() { return m_vDragDir; }
};

