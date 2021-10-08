#pragma once
#include <Engine/Script.h>

class CPlayerScript;


enum class KUNGFUMASTER_STATE
{
	_CC_DOWN,
	_CC_GROGGY,
	_CC_STUN,
	_CC_AIR,
	_CC_MOUNT,
	_CC_KNOCK_BACK,
	_CC_PULL,
	NON_COMBAT,
	COMBAT_STATE,
	END,
};

enum class KUNGFUMASTER_DIR
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


class CAIScript :
	public CScript
{
private:
	Ptr<CMaterial>		m_pOriginMtrl;
	Ptr<CMaterial>		m_pCloneMtrl;

private:
	float m_fSpeed;

	KUNGFUMASTER_STATE		AI_Position;

	KUNGFUMASTER_FSM		AI_FSM;
	KUNGFUMASTER_FSM		Prev_FSM;
	KUNGFUMASTER_FSM		Changed_FSM;
	KUNGFUMASTER_FSM		Changed_Prev_FSM;

	KUNGFUMASTER_DIR		KFM_DIR;

	bool AnimationExe;

	bool m_bDash;
	bool m_bGlide;

	bool OnTerrain;
	bool Active_skill;	// 이것이 false가 되야 idle로 돌아감
						// 스킬 액티브로써 이 값을 false 시키려면 다른 스킬들 
						//	Idle, 자세전환 이런것들 다 true여야함.

	bool m_bBackStep;
	bool m_bViolence_Active;
	bool RedDragon_Fist;
	bool m_bTargetting;

	UINT RedDragonFistCount;
	UINT DragonFistCount;

public:
	const wstring * m_strClipName;

	bool m_bCounter;			// 반격 성공하여 Active 스킬
	bool _Counterattacking;		// 현재 반격 중인가

	void _Distance_calculating();
	void Animation_Setting_FSM();

	bool GetDash() { return m_bDash; }
	void SetDash(bool _Dash) { m_bDash = _Dash; }

	bool GetGlide() { return m_bGlide; }
	void SetGlide(bool _Glide) { m_bGlide = _Glide; };

	CPlayerScript* _PlayerScript;
	void GetPlayerScript();

	bool _Has_Resisted;
	bool _Ready_resist;

	float _CC_Holdingtime;

public:
	virtual void awake();
	virtual void update();

public:
	CLONE(CAIScript);

public:
	void _AnimateFrame_Event();


	void SortBoolDir();

	void AI_MovingSetting();
	void AI_SkillSetting();

	DISTANCE	m_eDistance;
	float		m_fDistance;

	void SetFrameAnimationFunc(int _SetFrame, void (CAIScript::*functiontype)());

	static CGameObject* AIObject;

	void DefineAI();

	const wstring& GetCurPlayingAnimationName();

public:
	void MovingSettingPosition();
	void MovingSetting_FSM();
	void SkillCancelling_FSM();
	void SkillEndSetting_FSM();

	void SocketState_Confirm();

	KUNGFUMASTER_FSM GetPlayingAnimationFSM();

public:
	void SetKungfuMaster_FSM(KUNGFUMASTER_FSM _SettingFSM) { AI_FSM = _SettingFSM; };
	KUNGFUMASTER_FSM GetKungfuMaster_FSM() { return AI_FSM; };

	void SetKungfuMaster_STATE(KUNGFUMASTER_STATE _Setting_STATE) { AI_Position = _Setting_STATE; };
	KUNGFUMASTER_STATE GetKungfuMaster_STATE() { return AI_Position; };

public:
	CAIScript();
	virtual ~CAIScript();
};