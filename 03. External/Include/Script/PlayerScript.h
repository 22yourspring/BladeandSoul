#pragma once
#include <Engine/Script.h>

class CAIScript;
enum class BLADEMASTER_STATE
{
	_CC_DOWN,
	_CC_GROGGY,
	_CC_STUN,
	_CC_AIR,
	_CC_MOUNT,
	_CC_PULL,
	NON_COMBAT,
	FENCE_STATE,
	BATTO_STATE,
	END,
};

enum class BLADEMASTER_DIR
{
	NONE,
	FRONT,
	FRONT_LEFT,
	LEFT,
	FRONT_RIGHT,
	RIGHT,
	BACK,
	BACK_LEFT,
	BACK_RIGHT,

	RIGHT_TO_LEFT,
	LEFT_TO_RIGHT,

	END,
};


class CPlayerScript :
	public CScript
{
private:
	Ptr<CMaterial>		m_pOriginMtrl;
	Ptr<CMaterial>		m_pCloneMtrl;

private:
	float m_fSpeed;


	BLADEMASTER_STATE	Player_Position;
	
	BLADEMASTER_FSM		Player_FSM;
	BLADEMASTER_FSM		Prev_FSM;
	BLADEMASTER_FSM		Changed_FSM;
	BLADEMASTER_FSM		Changed_Prev_FSM;

	BLADEMASTER_DIR		BM_DIR;

	bool m_bDash;
	bool m_bGlide;
	bool m_bThird_State;

	bool OnTerrain;
	bool Active_skill;	// 이것이 false가 되야 idle로 돌아감
						// 스킬 액티브로써 이 값을 false 시키려면 다른 스킬들 
						//	Idle, 자세전환 이런것들 다 true여야함.

	bool m_bTargetting;

	bool RotHackActive;

	bool m_bGuard;

	bool _Has_Resisted;
	bool _Ready_resist;

public:
	const wstring * m_strClipName;

	void _Distance_calculating();
	bool _Rotating_calculating();

	void Animation_Setting_FSM();
	void SkillActive_Confirm();	// 안씀

	bool GetDash() { return m_bDash; }
	void SetDash(bool _Dash) { m_bDash = _Dash; }

	bool GetGlide() { return m_bGlide; }
	void SetGlide(bool _Glide) { m_bGlide = _Glide; }

	CAIScript* _AIScript;
	void GetAIScript();


	bool GetReadyResist() { return _Ready_resist; };
	void SetReadyResist(bool _SetResist) { _Ready_resist = _SetResist; };
	void SetReadyResist_False() { _Ready_resist = false; };
	void SetReadyResist_True() { _Ready_resist = true; };

public:
	virtual void awake();
	virtual void update();

public:
	CLONE(CPlayerScript);

public:
	void SortBoolDir();

	bool IdleToFront();
	bool IdleToLeft();
	bool IdleToRight();
	bool IdleToBack();

	bool RunToIdle();

	bool IdleToAttack();
	
	void Player_MovingSetting();
	void Player_SkillSetting();

	void BasicAttackEndNotify();
	void FenceIdle_To_ReturnNotify();
	void ReturnToIdleNotify();

	DISTANCE	m_eDistance;
	float		m_fDistance;

	void SetFrameAnimationFunc(int _SetFrame, void (CPlayerScript::*functiontype)());

	static CGameObject* PlayerObject;

	void DefinePlayer();

	const wstring& GetCurPlayingAnimationName();

	float _CC_Holdingtime;

private:
	bool m_bFront;
	bool m_bLeft;
	bool Prev_Left;
	bool m_bRight;
	bool Prev_Right;
	bool m_bBack;

	bool m_bAttack;

public:
	void MovingSettingPosition();
	void MovingSetting_FSM();
	void SkillCancelling_FSM();
	void SkillEndSetting_FSM();

	void SocketState_Confirm();

	void _AnimateFrame_Event();

	void SetSocket_IdlePos();
	void SetSocket_stdStance();
	void SetSocket_hidEndHand();
	void SetSocket_vitStance();
	void SetSocket_hidStance();
	void SetSocket_nonBehind();

	void _SetOpponentScriptFSM(KUNGFUMASTER_FSM _Opponent_FSM, float _Second = 0.f);



	void SetBladeMaster_LandCut() { Player_FSM = BLADEMASTER_FSM::_Z_HID_LAND_CUT_END; };
	void SetBladeMaster_hidStance() { Player_FSM = BLADEMASTER_FSM::_HID_SKL_IDLE; };
	void SetBladeMaster_BattoEnd() { Player_FSM = BLADEMASTER_FSM::_BATTO_SKL_END; };
	void SetBladeMaster_flyingSwordEnd() { Player_FSM = BLADEMASTER_FSM::_Z_STD_FLYING_SWALLOW_SWORD_END; };
	void SetBladeMaster_flyingSwordEndToIdle() { Player_FSM = BLADEMASTER_FSM::_Z_STD_FLYING_SWALLOW_SWORD_ENDTOIDLE; };

	void SetAI_CaptureFire();
	void SetAI_DownFrontKick();
	void SetAI_DownLowSlash();
	void SetAI_StunRush();


	void SetAI_GroggyNear();
	void SetAI_GroggyFar();
	void SetAI_Stun();
	void SetAI_UpperSlash();
	void SetAI_AirComboL();
	void SetAI_AirComboR();
	void SetAI_R_CRASH();
	void SetAI_L_ascension();
	void SetAI_KnockBack();

	void SetAI_Damage();
	void SetAI_DamageFront();
	void SetAI_DamageBack();

	void SetAI_CC_extension();
 
	// 상대가 나를 Setting하게 끔 하는 함수
public:
	void SetBladeMaster_FSM(BLADEMASTER_FSM _SettingFSM) { Player_FSM = _SettingFSM; };
	BLADEMASTER_FSM GetBladeMaster_FSM() { return Player_FSM; };

	void SetBladeMaster_STATE(BLADEMASTER_STATE _Setting_STATE) { Player_Position = _Setting_STATE; };
	BLADEMASTER_STATE GetBladeMaster_STATE() { return Player_Position; };

public:
	CPlayerScript();
	virtual ~CPlayerScript();
};