#include "stdafx.h"
#include "PlayerScript.h"

#include "TestScript.h"
#include "AIScript.h"

#include <Engine/Animator3D.h>

CGameObject* CPlayerScript::PlayerObject = nullptr;


CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
	, m_bFront(false)
	, m_bLeft(false)
	, m_bRight(false)
	, m_bBack(false)
	, m_bAttack(false)
	, m_fSpeed(100.f)
	, m_bDash(false)
	, m_bGlide(false)
	, m_bThird_State(false)
	, m_strClipName(nullptr)
	, Player_Position(BLADEMASTER_STATE::NON_COMBAT)
	, BM_DIR(BLADEMASTER_DIR::FRONT)
	, Active_skill(false)
	, RotHackActive(false)
	, Changed_FSM(BLADEMASTER_FSM::_NON_MOV_IDLE)
	, Changed_Prev_FSM(BLADEMASTER_FSM::_NON_MOV_IDLE)
	, Player_FSM(BLADEMASTER_FSM::_NON_MOV_IDLE)
	, m_bTargetting(false)
	, _AIScript(nullptr)
	, _CC_Holdingtime(0.f)
	, _Has_Resisted(false)
	, _Ready_resist(false)
{
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::SortBoolDir()
{
	if (m_bFront && !m_bLeft && !m_bRight && !m_bBack)
	{
		BM_DIR = BLADEMASTER_DIR::FRONT;
	}
	else if (!m_bFront && m_bLeft && !m_bRight && !m_bBack)
	{
		BM_DIR = BLADEMASTER_DIR::LEFT;
	}
	else if (!m_bFront && !m_bLeft && m_bRight && !m_bBack)
	{
		BM_DIR = BLADEMASTER_DIR::RIGHT;
	}
	else if (!m_bFront && !m_bLeft && !m_bRight && m_bBack)
	{
		BM_DIR = BLADEMASTER_DIR::BACK;
	}
	else if (m_bFront && m_bLeft && !m_bRight && !m_bBack)
	{
		BM_DIR = BLADEMASTER_DIR::FRONT_LEFT;
	}
	else if (m_bFront && !m_bLeft && m_bRight && !m_bBack)
	{
		BM_DIR = BLADEMASTER_DIR::FRONT_RIGHT;
	}
	else if (!m_bFront && m_bLeft && !m_bRight && m_bBack)
	{
		BM_DIR = BLADEMASTER_DIR::BACK_LEFT;
	}
	else if (!m_bFront && !m_bLeft && m_bRight && m_bBack)
	{
		BM_DIR = BLADEMASTER_DIR::BACK_RIGHT;
	}
	else
	{
		BM_DIR = BLADEMASTER_DIR::NONE;
	}

	if (m_bLeft && Prev_Right && !m_bRight)
	{
		BM_DIR = BLADEMASTER_DIR::RIGHT_TO_LEFT;
	}
	else if (m_bRight && Prev_Left && !m_bLeft)
	{
		BM_DIR = BLADEMASTER_DIR::LEFT_TO_RIGHT;
	}

	if (m_bLeft && m_bRight && Prev_Left && !Prev_Right)
	{
		BM_DIR = BLADEMASTER_DIR::LEFT_TO_RIGHT;
	}
	else if (m_bLeft && m_bRight && !Prev_Left && Prev_Right)
	{
		BM_DIR = BLADEMASTER_DIR::RIGHT_TO_LEFT;
	}

	if (!m_bFront && !m_bLeft && !m_bRight && !m_bBack)
	{
		BM_DIR = BLADEMASTER_DIR::NONE;
	}

	if ((Changed_Prev_FSM == BLADEMASTER_FSM::_NON_MOV_RUNLEFT || Changed_Prev_FSM == BLADEMASTER_FSM::_STD_MOV_RUNLEFT || Changed_Prev_FSM == BLADEMASTER_FSM::_HID_MOV_RUNLEFT) && (Changed_FSM == BLADEMASTER_FSM::_NON_MOV_MOVETOIDLE || Changed_FSM == BLADEMASTER_FSM::_STD_MOV_MOVETOIDLE || Changed_FSM == BLADEMASTER_FSM::_HID_MOV_MOVETOIDLE) && m_bRight)
	{
		if ((Player_Position == BLADEMASTER_STATE::NON_COMBAT && Player_FSM != BLADEMASTER_FSM::_NON_MOV_IDLE) || (Player_Position == BLADEMASTER_STATE::FENCE_STATE && Player_FSM != BLADEMASTER_FSM::_STD_MOV_IDLE) || (Player_Position == BLADEMASTER_STATE::BATTO_STATE && Player_FSM != BLADEMASTER_FSM::_HID_SKL_IDLE))
		{
			BM_DIR = BLADEMASTER_DIR::LEFT_TO_RIGHT;
		}
	}
	else if ((Changed_Prev_FSM == BLADEMASTER_FSM::_NON_MOV_RUNRIGHT || Changed_Prev_FSM == BLADEMASTER_FSM::_STD_MOV_RUNRIGHT || Changed_Prev_FSM == BLADEMASTER_FSM::_HID_MOV_RUNRIGHT) && (Changed_FSM == BLADEMASTER_FSM::_NON_MOV_MOVETOIDLE || Changed_FSM == BLADEMASTER_FSM::_STD_MOV_MOVETOIDLE || Changed_FSM == BLADEMASTER_FSM::_HID_MOV_MOVETOIDLE) && (Player_FSM != BLADEMASTER_FSM::_NON_MOV_IDLE || Player_FSM != BLADEMASTER_FSM::_STD_MOV_IDLE || Player_FSM != BLADEMASTER_FSM::_HID_SKL_IDLE) && m_bLeft)
	{
		if ((Player_Position == BLADEMASTER_STATE::NON_COMBAT && Player_FSM != BLADEMASTER_FSM::_NON_MOV_IDLE) || (Player_Position == BLADEMASTER_STATE::FENCE_STATE && Player_FSM != BLADEMASTER_FSM::_STD_MOV_IDLE) || (Player_Position == BLADEMASTER_STATE::BATTO_STATE && Player_FSM != BLADEMASTER_FSM::_HID_SKL_IDLE))
		{
			BM_DIR = BLADEMASTER_DIR::RIGHT_TO_LEFT;
		}
	}

	if ((Player_FSM == BLADEMASTER_FSM::_NON_MOV_LEFTTORIGHT || Player_FSM == BLADEMASTER_FSM::_STD_MOV_LEFTTORIGHT || Player_FSM == BLADEMASTER_FSM::_HID_MOV_LEFTTORIGHT) && m_bLeft)
	{
		BM_DIR = BLADEMASTER_DIR::RIGHT_TO_LEFT;
	}
	else if ((Player_FSM == BLADEMASTER_FSM::_NON_MOV_RIGHTTOLEFT || Player_FSM == BLADEMASTER_FSM::_STD_MOV_RIGHTTOLEFT || Player_FSM == BLADEMASTER_FSM::_HID_MOV_RIGHTTOLEFT) && m_bRight)
	{
		BM_DIR = BLADEMASTER_DIR::LEFT_TO_RIGHT;
	}
}

bool CPlayerScript::IdleToFront()
{
	BM_DIR = BLADEMASTER_DIR::FRONT;
	SortBoolDir();
	return m_bFront;
}

bool CPlayerScript::IdleToLeft()
{
	BM_DIR = BLADEMASTER_DIR::LEFT;
	SortBoolDir();
	return m_bLeft;
}

bool CPlayerScript::IdleToRight()
{
	BM_DIR = BLADEMASTER_DIR::RIGHT;
	SortBoolDir();
	return m_bRight;
}

bool CPlayerScript::IdleToBack()
{
	BM_DIR = BLADEMASTER_DIR::BACK;
	SortBoolDir();
	return m_bBack;
}


bool CPlayerScript::RunToIdle()
{
	return false;
}

bool CPlayerScript::IdleToAttack()
{
	return m_bAttack;
}

void CPlayerScript::SetSocket_IdlePos()
{
	Animator3D()->MoveWeaponFromSocket((GetObj()->GetChild(L"_GonryunBlade")->GetBelongSocketName()).c_str(), L"Idle", GetObj()->GetChild(L"_GonryunBlade"));
	Object()->GetChild(L"_GonryunBlade")->Transform()->SetLocalPos(Vec3(0.f, -100.f, 0.f));	// z 검날 좌우 +일수록 왼쪽
	Object()->GetChild(L"_GonryunBlade")->Transform()->SetLocalRot(Vec3(0.f * _DEGREE, 0.f * _DEGREE, 0.f * _DEGREE));
}

void CPlayerScript::SetSocket_stdStance()
{
	Animator3D()->MoveWeaponFromSocket((GetObj()->GetChild(L"_GonryunBlade")->GetBelongSocketName()).c_str(), L"RightHand", GetObj()->GetChild(L"_GonryunBlade"));
	Object()->GetChild(L"_GonryunBlade")->Transform()->SetLocalPos(Vec3(15.8f, 2.3f, 12.7f));	// z 검날 좌우 +일수록 왼쪽
	Object()->GetChild(L"_GonryunBlade")->Transform()->SetLocalRot(Vec3(25.f * _DEGREE, 10.f * _DEGREE, 5.f * _DEGREE));
}

void CPlayerScript::SetSocket_hidEndHand()
{
	Animator3D()->MoveWeaponFromSocket((GetObj()->GetChild(L"_GonryunBlade")->GetBelongSocketName()).c_str(), L"RightHand", GetObj()->GetChild(L"_GonryunBlade"));
	Object()->GetChild(L"_GonryunBlade")->Transform()->SetLocalPos(Vec3(15.8f, 2.3f, 12.7f));	// z 검날 좌우 +일수록 왼쪽
	Object()->GetChild(L"_GonryunBlade")->Transform()->SetLocalRot(Vec3(190.f * _DEGREE, 20.f * _DEGREE, 40.f * _DEGREE));
}

void CPlayerScript::SetSocket_vitStance()
{
	Animator3D()->MoveWeaponFromSocket((GetObj()->GetChild(L"_GonryunBlade")->GetBelongSocketName()).c_str(), L"RightHand", GetObj()->GetChild(L"_GonryunBlade"));
	Object()->GetChild(L"_GonryunBlade")->Transform()->SetLocalPos(Vec3(15.8f, 2.3f, 12.7f));	// z 검날 좌우 +일수록 왼쪽
	Object()->GetChild(L"_GonryunBlade")->Transform()->SetLocalRot(Vec3(220.f * _DEGREE, 00.f * _DEGREE, 30.f * _DEGREE));
}

void CPlayerScript::SetSocket_hidStance()
{
	Animator3D()->MoveWeaponFromSocket((GetObj()->GetChild(L"_GonryunBlade")->GetBelongSocketName()).c_str(), L"LeftHand", GetObj()->GetChild(L"_GonryunBlade"));
	Object()->GetChild(L"_GonryunBlade")->Transform()->SetLocalPos(Vec3(15.8f, 5.f, 40.0f));	// z 검날 좌우 +일수록 왼쪽
	Object()->GetChild(L"_GonryunBlade")->Transform()->SetLocalRot(Vec3(180.f * _DEGREE, -30.f * _DEGREE, -20.f * _DEGREE));
}

void CPlayerScript::SetSocket_nonBehind()
{
	Animator3D()->MoveWeaponFromSocket((GetObj()->GetChild(L"_GonryunBlade")->GetBelongSocketName()).c_str(), L"WeaponBack", GetObj()->GetChild(L"_GonryunBlade"));
	Object()->GetChild(L"_GonryunBlade")->Transform()->SetLocalPos(Vec3(0.f, 0.f, -2.f));
	Object()->GetChild(L"_GonryunBlade")->Transform()->SetLocalRot(Vec3(7.f * _DEGREE, 0.f * _DEGREE, 10.f * _DEGREE));
}

void CPlayerScript::_SetOpponentScriptFSM(KUNGFUMASTER_FSM _Opponent_FSM, float _Second)
{
	if (_Second == 0.f)
	{
		if (_AIScript && _AIScript->_Ready_resist == false)
		{
			_AIScript->SetKungfuMaster_FSM(_Opponent_FSM);
		}
	}
	else
	{
		if (_AIScript && _AIScript->_Ready_resist == false)
		{
			_AIScript->SetKungfuMaster_FSM(_Opponent_FSM);
			_AIScript->_CC_Holdingtime = _Second;
		}
	}
}

void CPlayerScript::Player_MovingSetting()
{
	MovingSettingPosition();

	SortBoolDir();

	if ((GetBladeMaster_FSM() == BLADEMASTER_FSM::_NON_MOV_IDLE
		|| GetBladeMaster_FSM() == BLADEMASTER_FSM::_STD_MOV_IDLE
		|| GetBladeMaster_FSM() == BLADEMASTER_FSM::_HID_SKL_IDLE)
		&& Prev_FSM == Player_FSM)
	{
		MovingSetting_FSM();
	}
	else if ((UINT)GetBladeMaster_FSM() >= (UINT)BLADEMASTER_FSM::_NON_MOV_IDLETOJUMP_FRONT && Prev_FSM == Player_FSM)
	{
		MovingSetting_FSM();
	}
	else if (Animator3D()->AnimIsFinish() && Prev_FSM == Player_FSM)
 	{
		if (GetBladeMaster_FSM() != BLADEMASTER_FSM::_1_STD_BLOCKING)
		{
			MovingSetting_FSM();
		}
	}
	else if (GetBladeMaster_FSM() == BLADEMASTER_FSM::_1_STD_BLOCKING || GetBladeMaster_FSM() == BLADEMASTER_FSM::_F_HID_BATTO_BLOCKING)
	{
		SkillCancelling_FSM();
	}
}

void CPlayerScript::Player_SkillSetting()
{
	// 1, 2, 3, 4
	if (KEY_TAB(KEY_TYPE::KEY_1))
	{
		if ((GetBladeMaster_STATE() == BLADEMASTER_STATE::NON_COMBAT || GetBladeMaster_STATE() == BLADEMASTER_STATE::FENCE_STATE || GetBladeMaster_STATE() == BLADEMASTER_STATE::BATTO_STATE) && GetBladeMaster_STATE() != BLADEMASTER_STATE::_CC_DOWN)
		{
			SetBladeMaster_FSM(BLADEMASTER_FSM::_1_STD_BLOCKING);
		}
		else if (GetBladeMaster_STATE() == BLADEMASTER_STATE::_CC_DOWN)
		{
			SetBladeMaster_FSM(BLADEMASTER_FSM::_1_STD_LEGSLASH);
		}
		SetBladeMaster_STATE(BLADEMASTER_STATE::FENCE_STATE);
	}
	if (KEY_TAB(KEY_TYPE::KEY_2))
	{
		if (GetBladeMaster_STATE() == BLADEMASTER_STATE::FENCE_STATE || GetBladeMaster_STATE() == BLADEMASTER_STATE::NON_COMBAT)
		{
			if (m_eDistance <= DISTANCE::MIDDLE)
			{
				SetBladeMaster_FSM(BLADEMASTER_FSM::_2_STD_RUSHING);
				SetBladeMaster_STATE(BLADEMASTER_STATE::FENCE_STATE);
			}			
		}
		else if (GetBladeMaster_STATE() == BLADEMASTER_STATE::BATTO_STATE)
		{
			if (m_eDistance <= DISTANCE::MIDDLE)
			{
				SetBladeMaster_FSM(BLADEMASTER_FSM::_2_HID_RUSHING);
				SetBladeMaster_STATE(BLADEMASTER_STATE::BATTO_STATE);
			}			
		}
	}
	if (KEY_TAB(KEY_TYPE::KEY_3))
	{
		if ((GetBladeMaster_STATE() == BLADEMASTER_STATE::FENCE_STATE || GetBladeMaster_STATE() == BLADEMASTER_STATE::NON_COMBAT) && GetBladeMaster_STATE() != BLADEMASTER_STATE::_CC_DOWN)
		{
			SetBladeMaster_FSM(BLADEMASTER_FSM::_3_STD_LOWSLASH_CAST);
			SetBladeMaster_STATE(BLADEMASTER_STATE::FENCE_STATE);
		}
		else if (GetBladeMaster_STATE() == BLADEMASTER_STATE::BATTO_STATE)
		{
			if (m_eDistance <= DISTANCE::MIDDLE)
			{
				_Ready_resist = true;
				SetBladeMaster_FSM(BLADEMASTER_FSM::_3_HID_FIVE_COMBO_SLASH_1);
				SetBladeMaster_STATE(BLADEMASTER_STATE::BATTO_STATE);
			}			
		}
		else if (GetBladeMaster_STATE() == BLADEMASTER_STATE::_CC_DOWN)
		{
			SetBladeMaster_FSM(BLADEMASTER_FSM::_3_STD_RETURN_READY);
		}
	}
	if (KEY_TAB(KEY_TYPE::KEY_4))
	{
		if ((GetBladeMaster_STATE() == BLADEMASTER_STATE::FENCE_STATE || GetBladeMaster_STATE() == BLADEMASTER_STATE::NON_COMBAT) && GetBladeMaster_STATE() != BLADEMASTER_STATE::_CC_DOWN)
		{
			SetBladeMaster_FSM(BLADEMASTER_FSM::_4_STD_MACH_SLASH);
			SetBladeMaster_STATE(BLADEMASTER_STATE::BATTO_STATE);
		}
		else if (GetBladeMaster_STATE() == BLADEMASTER_STATE::BATTO_STATE)
		{
			SetBladeMaster_FSM(BLADEMASTER_FSM::_4_STD_MACH_SLASH);
			SetBladeMaster_STATE(BLADEMASTER_STATE::BATTO_STATE);
		}
		else if (GetBladeMaster_STATE() == BLADEMASTER_STATE::_CC_DOWN)
		{
			SetBladeMaster_FSM(BLADEMASTER_FSM::_4_STD_LYING_RETURN_READY);
			SetBladeMaster_STATE(BLADEMASTER_STATE::FENCE_STATE);
		}
	}
	// Z, X, C, V
	if (KEY_TAB(KEY_TYPE::KEY_Z))
	{
		if ((GetBladeMaster_STATE() == BLADEMASTER_STATE::FENCE_STATE || GetBladeMaster_STATE() == BLADEMASTER_STATE::NON_COMBAT) && !m_bThird_State)
		{
			if (m_eDistance != DISTANCE::END)
			{
				SetBladeMaster_FSM(BLADEMASTER_FSM::_Z_STD_FLYING_SWALLOW_SWORD_EXEC);
				SetBladeMaster_STATE(BLADEMASTER_STATE::BATTO_STATE);
			}			
		}
		else if ((GetBladeMaster_STATE() == BLADEMASTER_STATE::FENCE_STATE || GetBladeMaster_STATE() == BLADEMASTER_STATE::NON_COMBAT) && m_bThird_State)
		{
			if (m_eDistance != DISTANCE::END)
			{
				SetBladeMaster_FSM(BLADEMASTER_FSM::_Z_STD_SOULBLAST);
				SetBladeMaster_STATE(BLADEMASTER_STATE::FENCE_STATE);
				m_bThird_State = false;
			}			
		}
		else if ((GetBladeMaster_STATE() == BLADEMASTER_STATE::BATTO_STATE) && !m_bThird_State)
		{
			SetBladeMaster_FSM(BLADEMASTER_FSM::_Z_HID_LAND_CUTTING);
			SetBladeMaster_STATE(BLADEMASTER_STATE::BATTO_STATE);
		}
		else if ((GetBladeMaster_STATE() == BLADEMASTER_STATE::BATTO_STATE) && m_bThird_State)
		{
			if (m_eDistance != DISTANCE::END)
			{
				SetBladeMaster_FSM(BLADEMASTER_FSM::_Z_HID_SOULBLAST);
				SetBladeMaster_STATE(BLADEMASTER_STATE::BATTO_STATE);
				m_bThird_State = false;
			}
		}
	}
	if (KEY_TAB(KEY_TYPE::KEY_X))
	{
		if (!m_bThird_State)
		{
			if (m_eDistance == DISTANCE::NEARBY)
			{
				SetBladeMaster_FSM(BLADEMASTER_FSM::_X_STD_KICKKNOCKBACK_CAST);
				SetBladeMaster_STATE(BLADEMASTER_STATE::FENCE_STATE);
			}
		}
		else if ((GetBladeMaster_STATE() == BLADEMASTER_STATE::FENCE_STATE || GetBladeMaster_STATE() == BLADEMASTER_STATE::NON_COMBAT) && m_bThird_State)
		{
			if (m_eDistance != DISTANCE::END)
			{
				SetBladeMaster_FSM(BLADEMASTER_FSM::_X_STD_GUARDSOUL_BLADE);
				SetBladeMaster_STATE(BLADEMASTER_STATE::FENCE_STATE);
			}			
		}
		else if (GetBladeMaster_STATE() == BLADEMASTER_STATE::BATTO_STATE && m_bThird_State) 
		{
			if (m_eDistance != DISTANCE::END)
			{
				SetBladeMaster_FSM(BLADEMASTER_FSM::_X_HID_THIRDBLADE_SLASH);
				SetBladeMaster_STATE(BLADEMASTER_STATE::BATTO_STATE);
			}			
		}
	}
	if (KEY_TAB(KEY_TYPE::KEY_C))
	{
		if ((GetBladeMaster_STATE() == BLADEMASTER_STATE::FENCE_STATE || GetBladeMaster_STATE() == BLADEMASTER_STATE::NON_COMBAT) && !m_bThird_State)
		{
			SetBladeMaster_FSM(BLADEMASTER_FSM::_C_STD_SHOULDERCHARGE);
			SetBladeMaster_STATE(BLADEMASTER_STATE::FENCE_STATE);
		}
		else if ((GetBladeMaster_STATE() == BLADEMASTER_STATE::FENCE_STATE || GetBladeMaster_STATE() == BLADEMASTER_STATE::NON_COMBAT) && m_bThird_State)
		{
			if (m_eDistance != DISTANCE::END)
			{
				SetBladeMaster_FSM(BLADEMASTER_FSM::_C_STD_SOULSTUNLIGHT);
				SetBladeMaster_STATE(BLADEMASTER_STATE::FENCE_STATE);
			}			
		}
		else if (GetBladeMaster_STATE() == BLADEMASTER_STATE::BATTO_STATE && !m_bThird_State)
		{
			SetBladeMaster_FSM(BLADEMASTER_FSM::_C_HID_WINDYMOONSLASH_01);
			SetBladeMaster_STATE(BLADEMASTER_STATE::BATTO_STATE);
		}
		else if (GetBladeMaster_STATE() == BLADEMASTER_STATE::BATTO_STATE && m_bThird_State)
		{
			if (m_eDistance != DISTANCE::END)
			{
				SetBladeMaster_FSM(BLADEMASTER_FSM::_C_HID_SOULSTUNLIGHT);
				SetBladeMaster_STATE(BLADEMASTER_STATE::BATTO_STATE);
			}			
		}
	}
	if (KEY_TAB(KEY_TYPE::KEY_V))
	{
		if ((GetBladeMaster_STATE() == BLADEMASTER_STATE::FENCE_STATE || GetBladeMaster_STATE() == BLADEMASTER_STATE::NON_COMBAT) && !m_bThird_State)
		{
			SetBladeMaster_FSM(BLADEMASTER_FSM::_V_STD_DIFFUSIONSOUL_1);
			SetBladeMaster_STATE(BLADEMASTER_STATE::FENCE_STATE);
			m_bThird_State = true;
		}
		else if ((GetBladeMaster_STATE() == BLADEMASTER_STATE::FENCE_STATE || GetBladeMaster_STATE() == BLADEMASTER_STATE::NON_COMBAT) && m_bThird_State)
		{
			SetBladeMaster_FSM(BLADEMASTER_FSM::_V_STD_ROTATESOULBLADE);
			SetBladeMaster_STATE(BLADEMASTER_STATE::FENCE_STATE);
		}
		else if (GetBladeMaster_STATE() == BLADEMASTER_STATE::BATTO_STATE && !m_bThird_State && GetBladeMaster_FSM() != BLADEMASTER_FSM::_V_HID_PIERCE1 && GetBladeMaster_FSM() != BLADEMASTER_FSM::_V_HID_PIERCE2 && GetBladeMaster_FSM() != BLADEMASTER_FSM::_V_HID_PIERCE3)
		{
			SetBladeMaster_FSM(BLADEMASTER_FSM::_V_HID_PIERCE1);
			SetBladeMaster_STATE(BLADEMASTER_STATE::BATTO_STATE);
		}
		else if (GetBladeMaster_STATE() == BLADEMASTER_STATE::BATTO_STATE && !m_bThird_State && GetBladeMaster_FSM() == BLADEMASTER_FSM::_V_HID_PIERCE1)
		{
			int Count = Object()->Animator3D()->m_iFrameIdx;
			if (20 < Count && Count < 37)
			{
				SetBladeMaster_FSM(BLADEMASTER_FSM::_V_HID_PIERCE2);
			}
			SetBladeMaster_STATE(BLADEMASTER_STATE::BATTO_STATE);
		}
		else if (GetBladeMaster_STATE() == BLADEMASTER_STATE::BATTO_STATE && !m_bThird_State && GetBladeMaster_FSM() == BLADEMASTER_FSM::_V_HID_PIERCE2)
		{
			int Count = Object()->Animator3D()->m_iFrameIdx;
			if (20 < Count && Count < 37)
			{
				SetBladeMaster_FSM(BLADEMASTER_FSM::_V_HID_PIERCE3);
			}
			SetBladeMaster_STATE(BLADEMASTER_STATE::BATTO_STATE);
		}
		else if (GetBladeMaster_STATE() == BLADEMASTER_STATE::BATTO_STATE && m_bThird_State)
		{
			SetBladeMaster_FSM(BLADEMASTER_FSM::_V_HID_ROTATESOULBLADE);
			SetBladeMaster_STATE(BLADEMASTER_STATE::BATTO_STATE);
		}
	}

	//tab
	if (KEY_TAB(KEY_TYPE::KEY_LTAB))
	{
		if (GetBladeMaster_STATE() == BLADEMASTER_STATE::FENCE_STATE || GetBladeMaster_STATE() == BLADEMASTER_STATE::NON_COMBAT || GetBladeMaster_STATE() == BLADEMASTER_STATE::BATTO_STATE)
		{
			SetBladeMaster_FSM(BLADEMASTER_FSM::_T_FULL_MOON_SLASH);
			SetBladeMaster_STATE(BLADEMASTER_STATE::BATTO_STATE);
		}
		else if (GetBladeMaster_STATE() == BLADEMASTER_STATE::_CC_DOWN || GetBladeMaster_STATE() == BLADEMASTER_STATE::_CC_GROGGY || GetBladeMaster_STATE() == BLADEMASTER_STATE::_CC_STUN)
		{
			SetBladeMaster_FSM(BLADEMASTER_FSM::_T_ESCAPE);
			SetBladeMaster_STATE(BLADEMASTER_STATE::FENCE_STATE);
		}
	}

	//F
	if (KEY_TAB(KEY_TYPE::KEY_F)) // 미완성
	{
		// Target이 기절 or 그로기 일 때 
		if (GetBladeMaster_STATE() == BLADEMASTER_STATE::FENCE_STATE || GetBladeMaster_STATE() == BLADEMASTER_STATE::NON_COMBAT || GetBladeMaster_STATE() == BLADEMASTER_STATE::BATTO_STATE)
		{
			SetBladeMaster_FSM(BLADEMASTER_FSM::_F_STD_PHANTOMSLASH3_SWING);
			SetBladeMaster_STATE(BLADEMASTER_STATE::FENCE_STATE);
		}
		if (GetBladeMaster_STATE() == BLADEMASTER_STATE::_CC_DOWN || GetBladeMaster_STATE() == BLADEMASTER_STATE::_CC_GROGGY)
		{
			SetBladeMaster_FSM(BLADEMASTER_FSM::_F_BACKWARDROLL);
			SetBladeMaster_STATE(BLADEMASTER_STATE::FENCE_STATE);
		}
	}

	//Mouse
	if (KEY_TAB(KEY_TYPE::KEY_LBTN)) // 미완성
	{
		if ((GetBladeMaster_STATE() == BLADEMASTER_STATE::FENCE_STATE || GetBladeMaster_STATE() == BLADEMASTER_STATE::NON_COMBAT) && !GetDash() && !GetGlide())	// Target이 없거나 타겟이 있는데 경공 중이 아닐 때
		{
			SetBladeMaster_FSM(BLADEMASTER_FSM::_L_STD_HACK);
		//	SetBladeMaster_FSM(BLADEMASTER_FSM::_L_STD_RUSHING_WIND_SPHERE);
			SetBladeMaster_STATE(BLADEMASTER_STATE::FENCE_STATE);
		}
		else if ((GetBladeMaster_STATE() == BLADEMASTER_STATE::FENCE_STATE || GetBladeMaster_STATE() == BLADEMASTER_STATE::NON_COMBAT) && !GetDash() && GetGlide())	// Target이 있고 경공 (AIR)  (신검합일)
		{
			SetBladeMaster_FSM(BLADEMASTER_FSM::_L_STD_BODY_BLADE_UNITY_FIRE);
			SetBladeMaster_STATE(BLADEMASTER_STATE::FENCE_STATE);
		}
		else if((GetBladeMaster_STATE() == BLADEMASTER_STATE::FENCE_STATE || GetBladeMaster_STATE() == BLADEMASTER_STATE::NON_COMBAT) && GetDash() && !GetGlide())	// Target이 있고 경공 (Grounds)  (신검합일)
		{
			SetBladeMaster_FSM(BLADEMASTER_FSM::_L_STD_BODY_BLADE_SPRINT_FIRE);
			SetBladeMaster_STATE(BLADEMASTER_STATE::FENCE_STATE);
		}
		else if ((GetBladeMaster_STATE() == BLADEMASTER_STATE::FENCE_STATE && GetBladeMaster_STATE() != BLADEMASTER_STATE::NON_COMBAT) && RotHackActive)	// 회전베기
		{
			SetBladeMaster_FSM(BLADEMASTER_FSM::_L_STD_HACK);
			SetBladeMaster_STATE(BLADEMASTER_STATE::FENCE_STATE);

			RotHackActive = false;
		}
		else if ((GetBladeMaster_STATE() == BLADEMASTER_STATE::FENCE_STATE && GetBladeMaster_STATE() != BLADEMASTER_STATE::NON_COMBAT) && RotHackActive)	// 타겟이 공중상태 일때 승천 여기가 미완성
		{
			SetBladeMaster_FSM(BLADEMASTER_FSM::_L_STD_ASCENSION);
			SetBladeMaster_STATE(BLADEMASTER_STATE::FENCE_STATE);

			RotHackActive = false;
		}
		else if (GetBladeMaster_STATE() == BLADEMASTER_STATE::BATTO_STATE)	// 발도
		{
			SetBladeMaster_FSM(BLADEMASTER_FSM::_L_HID_BATTO);
			SetBladeMaster_STATE(BLADEMASTER_STATE::BATTO_STATE);
		}
	}
	if (KEY_TAB(KEY_TYPE::KEY_RBTN))	// 미완성
	{
		if (GetBladeMaster_STATE() == BLADEMASTER_STATE::FENCE_STATE || GetBladeMaster_STATE() == BLADEMASTER_STATE::NON_COMBAT && !GetDash() && !GetGlide())	// 찌르기
		{
		//	SetBladeMaster_FSM(BLADEMASTER_FSM::_R_STD_RUSH_STAB_EXEC);
			SetBladeMaster_FSM(BLADEMASTER_FSM::_R_STD_CRIPPLING_CUT_FIRE);
			SetBladeMaster_STATE(BLADEMASTER_STATE::FENCE_STATE);
		}
		else if (GetBladeMaster_STATE() == BLADEMASTER_STATE::BATTO_STATE)					// 급소베기
		{
			SetBladeMaster_FSM(BLADEMASTER_FSM::_R_HID_VITALSLASH_EXEC);
			SetBladeMaster_STATE(BLADEMASTER_STATE::BATTO_STATE);
		}
		else if (GetBladeMaster_STATE() == BLADEMASTER_STATE::NON_COMBAT && !GetDash() && GetGlide())				// 번개찍기
		{
			SetBladeMaster_FSM(BLADEMASTER_FSM::_R_STD_LIGHTNINGSMASH_FIRE);
			SetBladeMaster_STATE(BLADEMASTER_STATE::BATTO_STATE);
		}
		else if (GetBladeMaster_STATE() == BLADEMASTER_STATE::FENCE_STATE)	// 미완성
		{
			SetBladeMaster_FSM(BLADEMASTER_FSM::_R_STD_MIDAIR_EXEC_01);
			SetBladeMaster_STATE(BLADEMASTER_STATE::FENCE_STATE);
		}
		else if (GetBladeMaster_STATE() == BLADEMASTER_STATE::NON_COMBAT && !GetDash() && GetGlide())	// 미완성
		{
			SetBladeMaster_FSM(BLADEMASTER_FSM::_R_STD_CRIPPLING_CUT_FIRE);
			SetBladeMaster_STATE(BLADEMASTER_STATE::FENCE_STATE);
		}
	}

	if (KEY_TAB(KEY_TYPE::KEY_Q))
	{
		if (m_eDistance == DISTANCE::NEARBY)
		{
			SetBladeMaster_FSM(BLADEMASTER_FSM::_Q_COUNTERATTACK_Q);
			SetBladeMaster_STATE(BLADEMASTER_STATE::BATTO_STATE);
		}
	}
	if (KEY_TAB(KEY_TYPE::KEY_E))
	{
		if (m_eDistance == DISTANCE::NEARBY)
		{
			SetBladeMaster_FSM(BLADEMASTER_FSM::_E_COUNTERATTACK_E);
			SetBladeMaster_STATE(BLADEMASTER_STATE::FENCE_STATE);
		}
	}

	if (KEY_TAB(KEY_TYPE::KEY_N))
	{
		SetBladeMaster_STATE(BLADEMASTER_STATE::NON_COMBAT);
	}
}

void CPlayerScript::BasicAttackEndNotify()
{
	m_bAttack = false;
}

void CPlayerScript::FenceIdle_To_ReturnNotify()
{
}

void CPlayerScript::ReturnToIdleNotify()
{
	SetSocket_nonBehind();
}

void CPlayerScript::SetFrameAnimationFunc(int _SetFrame, void(CPlayerScript::*functiontype)())
{
	int Count = Object()->Animator3D()->m_iFrameIdx;

	if (Count == _SetFrame)
	{
		(this->*functiontype)();
	}
}

void CPlayerScript::DefinePlayer()
{
	if (PlayerObject == nullptr)
	{
		PlayerObject = Object();
	}
	GetAIScript();
	_Distance_calculating();
	_Rotating_calculating();
}

const wstring& CPlayerScript::GetCurPlayingAnimationName()
{
	return (Object()->Animator3D()->GetAnimClip()->GetClipName());
}

void CPlayerScript::MovingSettingPosition()
{
	Vec3 vPos = Transform()->GetLocalPos();
	Vec3 vRot = Transform()->GetLocalRot();

	float fSpeed = m_fSpeed;

	Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();

	vRot.y += vDrag.x * DT * 0.5f;
	//예외처리
	if (vRot.x * _RADIAN > -90.f && vRot.x * _RADIAN < 90.f)
	{
		Transform()->SetLocalRot(vRot);
	}




	// 만약 Player Object 가 Default 값이 오른쪽 Right 를 바라보았을 때
	if (KEY_HOLD(KEY_TYPE::KEY_W))
	{
		Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
		vPos += vFront * fSpeed * DT;
		m_bFront = true;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_S))
	{
		Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
		vPos -= vFront * fSpeed * DT;
		m_bBack = true;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_A))
	{
		Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
		vPos += vFront * fSpeed * DT;
		m_bLeft = true;
	}

	if (KEY_HOLD(KEY_TYPE::KEY_D))
	{
		Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
		vPos -= vFront * fSpeed * DT;
		m_bRight = true;
	}


	if (KEY_AWAY(KEY_TYPE::KEY_W))
	{
		m_bFront = false;
	}

	if (KEY_AWAY(KEY_TYPE::KEY_S))
	{
		m_bBack = false;
	}

	if (KEY_AWAY(KEY_TYPE::KEY_A))
	{
		m_bLeft = false;
	}

	if (KEY_AWAY(KEY_TYPE::KEY_D))
	{
		m_bRight = false;
	}

	Transform()->SetLocalPos(vPos);
	Transform()->SetLocalRot(vRot);
}

void CPlayerScript::MovingSetting_FSM()
{
	switch (Player_Position)
	{
	case BLADEMASTER_STATE::NON_COMBAT:
		switch (BM_DIR)
		{
		case BLADEMASTER_DIR::NONE:
			if (GetBladeMaster_FSM() != BLADEMASTER_FSM::_NON_MOV_IDLE)
			{
				SetBladeMaster_FSM(BLADEMASTER_FSM::_NON_MOV_MOVETOIDLE);
			}
			 break;
		case BLADEMASTER_DIR::FRONT:
			Player_FSM = BLADEMASTER_FSM::_NON_MOV_RUNFRONT;
			 break;
		case BLADEMASTER_DIR::FRONT_LEFT:
			Player_FSM = BLADEMASTER_FSM::_NON_MOV_RUNFRONT;
			 break;
		case BLADEMASTER_DIR::LEFT:
			if (GetBladeMaster_FSM() == BLADEMASTER_FSM::_NON_MOV_RIGHTTOLEFT)	// 현재 전환애니메이션이라면 해당 에니메이션이 끝났다면.
			{
				if (Animator3D()->AnimIsFinish())
				{
					Player_FSM = BLADEMASTER_FSM::_NON_MOV_RUNLEFT;
				}
			}
			else			
				Player_FSM = BLADEMASTER_FSM::_NON_MOV_RUNLEFT;
			
			 break;
		case BLADEMASTER_DIR::FRONT_RIGHT:
			Player_FSM = BLADEMASTER_FSM::_NON_MOV_RUNFRONT;
			 break;
		case BLADEMASTER_DIR::RIGHT:
			if (GetBladeMaster_FSM() == BLADEMASTER_FSM::_NON_MOV_LEFTTORIGHT)
			{
				if (Animator3D()->AnimIsFinish())
				{
					Player_FSM = BLADEMASTER_FSM::_NON_MOV_RUNRIGHT;
				}
			}
			else
				Player_FSM = BLADEMASTER_FSM::_NON_MOV_RUNRIGHT;
			 break;
		case BLADEMASTER_DIR::BACK:
			Player_FSM = BLADEMASTER_FSM::_NON_MOV_RUNBACK;
			 break;
		case BLADEMASTER_DIR::BACK_LEFT:
			Player_FSM = BLADEMASTER_FSM::_NON_MOV_RUNLEFTBACK;
			 break;
		case BLADEMASTER_DIR::BACK_RIGHT:
			Player_FSM = BLADEMASTER_FSM::_NON_MOV_RUNRIGHTBACK;
			 break;
		case BLADEMASTER_DIR::RIGHT_TO_LEFT:
			Player_FSM = BLADEMASTER_FSM::_NON_MOV_RIGHTTOLEFT;
			 break;
		case BLADEMASTER_DIR::LEFT_TO_RIGHT:
			Player_FSM = BLADEMASTER_FSM::_NON_MOV_LEFTTORIGHT;
			 break;
		case BLADEMASTER_DIR::END:
			 break;
		default:
			 break;
		}
		 break;
	case BLADEMASTER_STATE::FENCE_STATE:
		switch (BM_DIR)
		{
		case BLADEMASTER_DIR::NONE:
			if (GetBladeMaster_FSM() != BLADEMASTER_FSM::_STD_MOV_IDLE)
			{
				SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_MOVETOIDLE);
			}
			 break;
		case BLADEMASTER_DIR::FRONT:
			Player_FSM = BLADEMASTER_FSM::_STD_MOV_RUNFRONT;
			 break;
		case BLADEMASTER_DIR::FRONT_LEFT:
			Player_FSM = BLADEMASTER_FSM::_STD_MOV_RUNFRONT;
			 break;
		case BLADEMASTER_DIR::LEFT:
			if (GetBladeMaster_FSM() == BLADEMASTER_FSM::_STD_MOV_RIGHTTOLEFT)
			{
				if (Animator3D()->AnimIsFinish())
				{
					Player_FSM = BLADEMASTER_FSM::_STD_MOV_RUNLEFT;
				}
			}
			else
				Player_FSM = BLADEMASTER_FSM::_STD_MOV_RUNLEFT;
			 break;
		case BLADEMASTER_DIR::FRONT_RIGHT:
			Player_FSM = BLADEMASTER_FSM::_STD_MOV_RUNFRONT;
			 break;
		case BLADEMASTER_DIR::RIGHT:
			if (GetBladeMaster_FSM() == BLADEMASTER_FSM::_STD_MOV_LEFTTORIGHT)
			{
				if (Animator3D()->AnimIsFinish())
				{
					Player_FSM = BLADEMASTER_FSM::_STD_MOV_RUNRIGHT;
				}
			}
			else
				Player_FSM = BLADEMASTER_FSM::_STD_MOV_RUNRIGHT;
			 break;
		case BLADEMASTER_DIR::BACK:
			Player_FSM = BLADEMASTER_FSM::_STD_MOV_RUNBACK;
			 break;
		case BLADEMASTER_DIR::BACK_LEFT:
			Player_FSM = BLADEMASTER_FSM::_STD_MOV_RUNLEFTBACK;
			 break;
		case BLADEMASTER_DIR::BACK_RIGHT:
			Player_FSM = BLADEMASTER_FSM::_STD_MOV_RUNRIGHTBACK;
			 break;
		case BLADEMASTER_DIR::RIGHT_TO_LEFT:
			Player_FSM = BLADEMASTER_FSM::_STD_MOV_RIGHTTOLEFT;
			 break;
		case BLADEMASTER_DIR::LEFT_TO_RIGHT:
			Player_FSM = BLADEMASTER_FSM::_STD_MOV_LEFTTORIGHT;
			 break;
		case BLADEMASTER_DIR::END:
			 break;
		default:
			 break;
		}
		 break;
	case BLADEMASTER_STATE::BATTO_STATE:
		switch (BM_DIR)
		{
		case BLADEMASTER_DIR::NONE:
			if (GetBladeMaster_FSM() != BLADEMASTER_FSM::_HID_SKL_IDLE)
			{
				SetBladeMaster_FSM(BLADEMASTER_FSM::_HID_MOV_MOVETOIDLE);
			}
			 break;
		case BLADEMASTER_DIR::FRONT:
			Player_FSM = BLADEMASTER_FSM::_HID_MOV_RUNFRONT;
			 break;
		case BLADEMASTER_DIR::FRONT_LEFT:
			Player_FSM = BLADEMASTER_FSM::_HID_MOV_RUNFRONT;
			 break;
		case BLADEMASTER_DIR::LEFT:
			if (GetBladeMaster_FSM() == BLADEMASTER_FSM::_HID_MOV_RIGHTTOLEFT)
			{
				if (Animator3D()->AnimIsFinish())
				{
					Player_FSM = BLADEMASTER_FSM::_HID_MOV_RUNLEFT;
				}
			}
			else
				Player_FSM = BLADEMASTER_FSM::_HID_MOV_RUNLEFT;
			 break;
		case BLADEMASTER_DIR::FRONT_RIGHT:
			Player_FSM = BLADEMASTER_FSM::_HID_MOV_RUNFRONT;
			 break;
		case BLADEMASTER_DIR::RIGHT:
			if (GetBladeMaster_FSM() == BLADEMASTER_FSM::_HID_MOV_LEFTTORIGHT)
			{
				if (Animator3D()->AnimIsFinish())
				{
					Player_FSM = BLADEMASTER_FSM::_HID_MOV_RUNRIGHT;
				}
			}
			else
				Player_FSM = BLADEMASTER_FSM::_HID_MOV_RUNRIGHT;
			 break;
		case BLADEMASTER_DIR::BACK:
			Player_FSM = BLADEMASTER_FSM::_HID_MOV_RUNBACK;
			 break;
		case BLADEMASTER_DIR::BACK_LEFT:
			Player_FSM = BLADEMASTER_FSM::_HID_MOV_RUNLEFTBACK;
			 break;
		case BLADEMASTER_DIR::BACK_RIGHT:
			Player_FSM = BLADEMASTER_FSM::_HID_MOV_RUNRIGHTBACK;
			 break;
		case BLADEMASTER_DIR::RIGHT_TO_LEFT:
			Player_FSM = BLADEMASTER_FSM::_HID_MOV_RIGHTTOLEFT;
			 break;
		case BLADEMASTER_DIR::LEFT_TO_RIGHT:
			Player_FSM = BLADEMASTER_FSM::_HID_MOV_LEFTTORIGHT;
			 break;
		case BLADEMASTER_DIR::END:
			 break;
		default:
			 break;
		}
		 break;
	case BLADEMASTER_STATE::END:
		 break;
	default:
		 break;
	}
}

void CPlayerScript::SkillCancelling_FSM()
{
	switch (Player_Position)
	{
	case BLADEMASTER_STATE::NON_COMBAT:
		switch (BM_DIR)
		{
		case BLADEMASTER_DIR::NONE:
			break;
		case BLADEMASTER_DIR::FRONT:
			Player_FSM = BLADEMASTER_FSM::_NON_MOV_RUNFRONT;
			break;
		case BLADEMASTER_DIR::FRONT_LEFT:
			Player_FSM = BLADEMASTER_FSM::_NON_MOV_RUNFRONT;
			break;
		case BLADEMASTER_DIR::LEFT:
			if (GetBladeMaster_FSM() == BLADEMASTER_FSM::_NON_MOV_RIGHTTOLEFT)	// 현재 전환애니메이션이라면 해당 에니메이션이 끝났다면.
			{
				if (Animator3D()->AnimIsFinish())
				{
					Player_FSM = BLADEMASTER_FSM::_NON_MOV_RUNLEFT;
				}
			}
			else if (GetBladeMaster_FSM() == BLADEMASTER_FSM::_1_STD_BLOCKING || GetBladeMaster_FSM() == BLADEMASTER_FSM::_1_STD_BLOCK1 || GetBladeMaster_FSM() == BLADEMASTER_FSM::_1_STD_BLOCK2 || GetBladeMaster_FSM() == BLADEMASTER_FSM::_1_STD_BLOCK3 || GetBladeMaster_FSM() == BLADEMASTER_FSM::_F_HID_BATTO_BLOCKING || GetBladeMaster_FSM() == BLADEMASTER_FSM::_F_HID_BATTO_BLOCKING_START)
				Player_FSM = BLADEMASTER_FSM::_NON_MOV_RIGHTTOLEFT;
			else
				Player_FSM = BLADEMASTER_FSM::_NON_MOV_RUNLEFT;
			break;
		case BLADEMASTER_DIR::FRONT_RIGHT:
			Player_FSM = BLADEMASTER_FSM::_NON_MOV_RUNFRONT;
			break;
		case BLADEMASTER_DIR::RIGHT:
			if (GetBladeMaster_FSM() == BLADEMASTER_FSM::_NON_MOV_LEFTTORIGHT)
			{
				if (Animator3D()->AnimIsFinish())
				{
					Player_FSM = BLADEMASTER_FSM::_NON_MOV_RUNRIGHT;
				}
			}
			else if (GetBladeMaster_FSM() == BLADEMASTER_FSM::_1_STD_BLOCKING || GetBladeMaster_FSM() == BLADEMASTER_FSM::_1_STD_BLOCK1 || GetBladeMaster_FSM() == BLADEMASTER_FSM::_1_STD_BLOCK2 || GetBladeMaster_FSM() == BLADEMASTER_FSM::_1_STD_BLOCK3 || GetBladeMaster_FSM() == BLADEMASTER_FSM::_F_HID_BATTO_BLOCKING || GetBladeMaster_FSM() == BLADEMASTER_FSM::_F_HID_BATTO_BLOCKING_START)
				Player_FSM = BLADEMASTER_FSM::_NON_MOV_LEFTTORIGHT;
			else
				Player_FSM = BLADEMASTER_FSM::_NON_MOV_RUNRIGHT;
			break;
		case BLADEMASTER_DIR::BACK:
			Player_FSM = BLADEMASTER_FSM::_NON_MOV_RUNBACK;
			break;
		case BLADEMASTER_DIR::BACK_LEFT:
			Player_FSM = BLADEMASTER_FSM::_NON_MOV_RUNLEFTBACK;
			break;
		case BLADEMASTER_DIR::BACK_RIGHT:
			Player_FSM = BLADEMASTER_FSM::_NON_MOV_RUNRIGHTBACK;
			break;
		case BLADEMASTER_DIR::RIGHT_TO_LEFT:
			Player_FSM = BLADEMASTER_FSM::_NON_MOV_RIGHTTOLEFT;
			break;
		case BLADEMASTER_DIR::LEFT_TO_RIGHT:
			Player_FSM = BLADEMASTER_FSM::_NON_MOV_LEFTTORIGHT;
			break;
		case BLADEMASTER_DIR::END:
			break;
		default:
			break;
		}
		break;
	case BLADEMASTER_STATE::FENCE_STATE:
		switch (BM_DIR)
		{
		case BLADEMASTER_DIR::NONE:
			break;
		case BLADEMASTER_DIR::FRONT:
			Player_FSM = BLADEMASTER_FSM::_STD_MOV_RUNFRONT;
			break;
		case BLADEMASTER_DIR::FRONT_LEFT:
			Player_FSM = BLADEMASTER_FSM::_STD_MOV_RUNFRONT;
			break;
		case BLADEMASTER_DIR::LEFT:
			if (GetBladeMaster_FSM() == BLADEMASTER_FSM::_STD_MOV_RIGHTTOLEFT)
			{
				if (Animator3D()->AnimIsFinish())
				{
					Player_FSM = BLADEMASTER_FSM::_STD_MOV_RUNLEFT;
				}
			}
			else if (GetBladeMaster_FSM() == BLADEMASTER_FSM::_1_STD_BLOCKING || GetBladeMaster_FSM() == BLADEMASTER_FSM::_1_STD_BLOCK1 || GetBladeMaster_FSM() == BLADEMASTER_FSM::_1_STD_BLOCK2 || GetBladeMaster_FSM() == BLADEMASTER_FSM::_1_STD_BLOCK3 || GetBladeMaster_FSM() == BLADEMASTER_FSM::_F_HID_BATTO_BLOCKING || GetBladeMaster_FSM() == BLADEMASTER_FSM::_F_HID_BATTO_BLOCKING_START)
				Player_FSM = BLADEMASTER_FSM::_STD_MOV_RIGHTTOLEFT;
			else
				Player_FSM = BLADEMASTER_FSM::_STD_MOV_RUNLEFT;
			break;
		case BLADEMASTER_DIR::FRONT_RIGHT:
			Player_FSM = BLADEMASTER_FSM::_STD_MOV_RUNFRONT;
			break;
		case BLADEMASTER_DIR::RIGHT:
			if (GetBladeMaster_FSM() == BLADEMASTER_FSM::_STD_MOV_LEFTTORIGHT)
			{
				if (Animator3D()->AnimIsFinish())
				{
					Player_FSM = BLADEMASTER_FSM::_STD_MOV_RUNRIGHT;
				}
			}
			else if (GetBladeMaster_FSM() == BLADEMASTER_FSM::_1_STD_BLOCKING || GetBladeMaster_FSM() == BLADEMASTER_FSM::_1_STD_BLOCK1 || GetBladeMaster_FSM() == BLADEMASTER_FSM::_1_STD_BLOCK2 || GetBladeMaster_FSM() == BLADEMASTER_FSM::_1_STD_BLOCK3 || GetBladeMaster_FSM() == BLADEMASTER_FSM::_F_HID_BATTO_BLOCKING || GetBladeMaster_FSM() == BLADEMASTER_FSM::_F_HID_BATTO_BLOCKING_START)
				Player_FSM = BLADEMASTER_FSM::_STD_MOV_LEFTTORIGHT;
			else
				Player_FSM = BLADEMASTER_FSM::_STD_MOV_RUNRIGHT;
			break;
		case BLADEMASTER_DIR::BACK:
			Player_FSM = BLADEMASTER_FSM::_STD_MOV_RUNBACK;
			break;
		case BLADEMASTER_DIR::BACK_LEFT:
			Player_FSM = BLADEMASTER_FSM::_STD_MOV_RUNLEFTBACK;
			break;
		case BLADEMASTER_DIR::BACK_RIGHT:
			Player_FSM = BLADEMASTER_FSM::_STD_MOV_RUNRIGHTBACK;
			break;
		case BLADEMASTER_DIR::RIGHT_TO_LEFT:
			Player_FSM = BLADEMASTER_FSM::_STD_MOV_RIGHTTOLEFT;
			break;
		case BLADEMASTER_DIR::LEFT_TO_RIGHT:
			Player_FSM = BLADEMASTER_FSM::_STD_MOV_LEFTTORIGHT;
			break;
		case BLADEMASTER_DIR::END:
			break;
		default:
			break;
		}
		break;
	case BLADEMASTER_STATE::BATTO_STATE:
		switch (BM_DIR)
		{
		case BLADEMASTER_DIR::NONE:
			break;
		case BLADEMASTER_DIR::FRONT:
			Player_FSM = BLADEMASTER_FSM::_HID_MOV_RUNFRONT;
			break;
		case BLADEMASTER_DIR::FRONT_LEFT:
			Player_FSM = BLADEMASTER_FSM::_HID_MOV_RUNFRONT;
			break;
		case BLADEMASTER_DIR::LEFT:
			if (GetBladeMaster_FSM() == BLADEMASTER_FSM::_HID_MOV_RIGHTTOLEFT)
			{
				if (Animator3D()->AnimIsFinish())
				{
					Player_FSM = BLADEMASTER_FSM::_HID_MOV_RUNLEFT;
				}
			}
			else if (GetBladeMaster_FSM() == BLADEMASTER_FSM::_1_STD_BLOCKING || GetBladeMaster_FSM() == BLADEMASTER_FSM::_1_STD_BLOCK1 || GetBladeMaster_FSM() == BLADEMASTER_FSM::_1_STD_BLOCK2 || GetBladeMaster_FSM() == BLADEMASTER_FSM::_1_STD_BLOCK3 || GetBladeMaster_FSM() == BLADEMASTER_FSM::_F_HID_BATTO_BLOCKING || GetBladeMaster_FSM() == BLADEMASTER_FSM::_F_HID_BATTO_BLOCKING_START)
				Player_FSM = BLADEMASTER_FSM::_STD_MOV_RIGHTTOLEFT;
			else
				Player_FSM = BLADEMASTER_FSM::_STD_MOV_RUNLEFT;
			break;
		case BLADEMASTER_DIR::FRONT_RIGHT:
			Player_FSM = BLADEMASTER_FSM::_HID_MOV_RUNFRONT;
			break;
		case BLADEMASTER_DIR::RIGHT:
			if (GetBladeMaster_FSM() == BLADEMASTER_FSM::_HID_MOV_LEFTTORIGHT)
			{
				if (Animator3D()->AnimIsFinish())
				{
					Player_FSM = BLADEMASTER_FSM::_HID_MOV_RUNRIGHT;
				}
			}
			else if (GetBladeMaster_FSM() == BLADEMASTER_FSM::_1_STD_BLOCKING || GetBladeMaster_FSM() == BLADEMASTER_FSM::_1_STD_BLOCK1 || GetBladeMaster_FSM() == BLADEMASTER_FSM::_1_STD_BLOCK2 || GetBladeMaster_FSM() == BLADEMASTER_FSM::_1_STD_BLOCK3 || GetBladeMaster_FSM() == BLADEMASTER_FSM::_F_HID_BATTO_BLOCKING || GetBladeMaster_FSM() == BLADEMASTER_FSM::_F_HID_BATTO_BLOCKING_START)
				Player_FSM = BLADEMASTER_FSM::_STD_MOV_LEFTTORIGHT;
			else
				Player_FSM = BLADEMASTER_FSM::_STD_MOV_RUNRIGHT;
			break;
		case BLADEMASTER_DIR::BACK:
			Player_FSM = BLADEMASTER_FSM::_HID_MOV_RUNBACK;
			break;
		case BLADEMASTER_DIR::BACK_LEFT:
			Player_FSM = BLADEMASTER_FSM::_HID_MOV_RUNLEFTBACK;
			break;
		case BLADEMASTER_DIR::BACK_RIGHT:
			Player_FSM = BLADEMASTER_FSM::_HID_MOV_RUNRIGHTBACK;
			break;
		case BLADEMASTER_DIR::RIGHT_TO_LEFT:
			Player_FSM = BLADEMASTER_FSM::_HID_MOV_RIGHTTOLEFT;
			break;
		case BLADEMASTER_DIR::LEFT_TO_RIGHT:
			Player_FSM = BLADEMASTER_FSM::_HID_MOV_LEFTTORIGHT;
			break;
		case BLADEMASTER_DIR::END:
			break;
		default:
			break;
		}
		break;
	case BLADEMASTER_STATE::END:
		break;
	default:
		break;
	}
}

void CPlayerScript::SkillEndSetting_FSM()				// 끝나는 곳에 함수 호출  
{
	if (Animator3D()->AnimIsFinish())
	{
		switch (Player_FSM)
		{
		case BLADEMASTER_FSM::_1_STD_BLOCKING:
			break;
		case BLADEMASTER_FSM::_1_STD_BLOCK1:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_1_STD_BLOCKING);
			break;
		case BLADEMASTER_FSM::_1_STD_BLOCK2:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_1_STD_BLOCKING);
			break;
		case BLADEMASTER_FSM::_1_STD_BLOCK3:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_1_STD_BLOCKING);
			break;
		case BLADEMASTER_FSM::_1_STD_LEGSLASH:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_2_STD_RUSHING:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_2_STD_RUSHED);
			break;
		case BLADEMASTER_FSM::_2_STD_RUSHED:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_2_HID_RUSHING:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_2_HID_RUSHED);
			break;
		case BLADEMASTER_FSM::_2_HID_RUSHED:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_HID_SKL_IDLE);
			break;		
		case BLADEMASTER_FSM::_3_HID_FIVE_COMBO_SLASH_1:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_3_HID_FIVE_COMBO_SLASH_2);
			break;
		case BLADEMASTER_FSM::_3_HID_FIVE_COMBO_SLASH_2:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_3_HID_FIVE_COMBO_SLASH_3);
			break;
		case BLADEMASTER_FSM::_3_HID_FIVE_COMBO_SLASH_3:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_3_HID_FIVE_COMBO_SLASH_4);
			break;
		case BLADEMASTER_FSM::_3_HID_FIVE_COMBO_SLASH_4:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_3_HID_FIVE_COMBO_SLASH_5);
			break;
		case BLADEMASTER_FSM::_3_HID_FIVE_COMBO_SLASH_5:
			break;
		case BLADEMASTER_FSM::_3_STD_RETURN_READY:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_3_STD_RETURN_STANDBY);
			break;
		case BLADEMASTER_FSM::_3_STD_RETURN_STANDBY:
			break;
		case BLADEMASTER_FSM::_3_STD_RETURN_EXEC:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_3_STD_RETURN_FINISH:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_3_STD_LOWSLASH_CAST:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_3_STD_LOWSLASH_EXEC);
	//		_SetOpponentScriptFSM(KUNGFUMASTER_FSM::_DOWN_LOW_B_R_START, 2.f);
			break;
		case BLADEMASTER_FSM::_3_STD_LOWSLASH_EXEC:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_4_STD_LYING_RETURN_READY:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_4_STD_LYING_RETURN_STANDBY);
			break;
		case BLADEMASTER_FSM::_4_STD_LYING_RETURN_STANDBY:
			break;
		case BLADEMASTER_FSM::_4_STD_LYING_RETURN_FINISH:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_4_STD_MACH_SLASH:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_HID_SKL_IDLE);
			break;
		case BLADEMASTER_FSM::_4_HID_MACH_SLASH:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_HID_SKL_IDLE);
			break;
		case BLADEMASTER_FSM::_Z_STD_FLYING_SWALLOW_SWORD_EXEC:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_BATTO_SKL_END);
			break;
		case BLADEMASTER_FSM::_Z_STD_FLYING_SWALLOW_SWORD_FINISH:
		//	SetBladeMaster_FSM(BLADEMASTER_FSM::_Z_STD_FLYING_SWALLOW_SWORD_END);
			break;
		case BLADEMASTER_FSM::_Z_STD_FLYING_SWALLOW_SWORD_END:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_HID_SKL_IDLE);
			break;
		case BLADEMASTER_FSM::_Z_STD_FLYING_SWALLOW_SWORD_ENDTOIDLE:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_HID_SKL_IDLE);
			break;
		case BLADEMASTER_FSM::_Z_HID_LAND_CUTTING:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_Z_HID_LAND_CUT_END);
			break;
		case BLADEMASTER_FSM::_Z_HID_LAND_CUT_END:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_HID_SKL_IDLE);
			break;
		case BLADEMASTER_FSM::_X_STD_KICKKNOCKBACK_CAST:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_X_STD_KICKKNOCKBACK_EXEC);
			break;
		case BLADEMASTER_FSM::_X_STD_KICKKNOCKBACK_EXEC:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_X_STD_KICKKNOCKBACK_END);
			break;
		case BLADEMASTER_FSM::_X_STD_KICKKNOCKBACK_END:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_S_BACKSTEP:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_HID_SKL_IDLE);
			break;
		case BLADEMASTER_FSM::_C_STD_SHOULDERCHARGE:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_C_HID_WINDYMOONSLASH_01:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_C_HID_WINDYMOONSLASH_02);
			break;
		case BLADEMASTER_FSM::_C_HID_WINDYMOONSLASH_02:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_C_HID_WINDYMOONSLASH_FINISH);
			break;
		case BLADEMASTER_FSM::_C_HID_WINDYMOONSLASH_FINISH:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_HID_SKL_IDLE);
			break;
		case BLADEMASTER_FSM::_V_STD_DIFFUSIONSOUL_1:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_V_STD_DIFFUSIONSOUL_2);
			break;
		case BLADEMASTER_FSM::_V_STD_DIFFUSIONSOUL_2:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_V_HID_PIERCE1:
		case BLADEMASTER_FSM::_V_HID_PIERCE2:
		case BLADEMASTER_FSM::_V_HID_PIERCE3:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_HID_SKL_IDLE);
			break;
		case BLADEMASTER_FSM::_Q_COUNTERATTACK_Q:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_HID_SKL_IDLE);
			break;
		case BLADEMASTER_FSM::_E_COUNTERATTACK_E:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_T_FULL_MOON_SLASH:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_HID_SKL_IDLE);
			break;
		case BLADEMASTER_FSM::_T_ESCAPE:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_T_ESCAPE_END);
			break;
		case BLADEMASTER_FSM::_T_ESCAPE_END:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_L_STD_HACK:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_L_STD_HACK_END);
			break;
		case BLADEMASTER_FSM::_L_STD_HACK_END:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_L_STD_RUSHING_WIND_SPHERE:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_L_STD_BODY_BLADE_UNITY_FIRE:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_L_STD_BODY_BLADE_UNITY_EXEC);
			break;
		case BLADEMASTER_FSM::_L_STD_BODY_BLADE_UNITY_EXEC:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_L_STD_BODY_BLADE_SPRINT_FIRE:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_L_STD_BODY_BLADE_SPRINT_EXEC);
			break;
		case BLADEMASTER_FSM::_L_STD_BODY_BLADE_SPRINT_EXEC:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_L_STD_ASCENSION:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_L_HID_BATTO:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_HID_SKL_IDLE);
			break;
		case BLADEMASTER_FSM::_R_STD_RUSH_STAB_EXEC:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_R_STD_MIDAIR_EXEC_01:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_R_STD_MIDAIR_EXEC_02);
			break;
		case BLADEMASTER_FSM::_R_STD_MIDAIR_EXEC_02:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_R_STD_MIDAIR_EXEC_03);
			break;
		case BLADEMASTER_FSM::_R_STD_MIDAIR_EXEC_03:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_R_STD_MIDAIR_EXEC_04);
			break;
		case BLADEMASTER_FSM::_R_STD_MIDAIR_EXEC_04:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_R_STD_MIDAIR_EXEC_05);
			break;
		case BLADEMASTER_FSM::_R_STD_MIDAIR_EXEC_05:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_R_STD_CRIPPLING_CUT_FIRE:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_R_STD_LIGHTNINGSMASH_FIRE:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_R_STD_LIGHTNINGSMASH_EXEC);
			break;
		case BLADEMASTER_FSM::_R_STD_LIGHTNINGSMASH_EXEC:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_BATTO_SKL_END);
			break;
		case BLADEMASTER_FSM::_R_HID_VITALSLASH_EXEC:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_HID_SKL_IDLE);
			break;
		case BLADEMASTER_FSM::_F_STD_PHANTOMSLASH3_SWING:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_F_STD_PHANTOMSLASH3_EXEC);
			break;
		case BLADEMASTER_FSM::_F_STD_PHANTOMSLASH3_EXEC:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_F_BACKWARDROLL:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_F_HID_BATTO_BLOCKING_START:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_F_HID_BATTO_BLOCKING);
			break;
		case BLADEMASTER_FSM::_F_HID_BATTO_BLOCKING:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_HID_SKL_IDLE);
			break;
		case BLADEMASTER_FSM::_Z_STD_SOULBLAST:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_Z_HID_SOULBLAST:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_HID_SKL_IDLE);
			break;
		case BLADEMASTER_FSM::_X_STD_GUARDSOUL_BLADE:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_X_HID_THIRDBLADE_SLASH:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_HID_SKL_IDLE);
			break;
		case BLADEMASTER_FSM::_C_STD_SOULSTUNLIGHT:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_C_HID_SOULSTUNLIGHT:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_HID_SKL_IDLE);
			break;
		case BLADEMASTER_FSM::_V_STD_ROTATESOULBLADE:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_V_HID_ROTATESOULBLADE:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_HID_SKL_IDLE);
			break;
		case BLADEMASTER_FSM::_NON_MOV_IDLETOJUMP_FRONT:
			break;
		case BLADEMASTER_FSM::_NON_MOV_IDLETOJUMP_LEFT:
			break;
		case BLADEMASTER_FSM::_NON_MOV_IDLETOJUMP_RIGHT:
			break;
		case BLADEMASTER_FSM::_NON_MOV_JUMPFRONT:
			break;
		case BLADEMASTER_FSM::_NON_MOV_JUMPLEFT:
			break;
		case BLADEMASTER_FSM::_NON_MOV_JUMPRIGHT:
			break;
		case BLADEMASTER_FSM::_NON_MOV_JUMPTOIDLE:
			break;
		case BLADEMASTER_FSM::_NON_MOV_JUMPTOMOVE_BACK:
			break;
		case BLADEMASTER_FSM::_NON_MOV_JUMPTOMOVE_FRONT:
			break;
		case BLADEMASTER_FSM::_NON_MOV_JUMPTOMOVE_LEFT:
			break;
		case BLADEMASTER_FSM::_NON_MOV_JUMPTOMOVE_RIGHT:
			break;
		case BLADEMASTER_FSM::_NON_MOV_LEFTTORIGHT:
			break;
		case BLADEMASTER_FSM::_NON_MOV_MOVETOIDLE:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_NON_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_NON_MOV_RIGHTTOLEFT:
			break;
		case BLADEMASTER_FSM::_NON_MOV_RUNBACK:
			break;
		case BLADEMASTER_FSM::_NON_MOV_RUNFRONT:
			break;
		case BLADEMASTER_FSM::_NON_MOV_RUNLEFT:
			break;
		case BLADEMASTER_FSM::_NON_MOV_RUNLEFTBACK:
			break;
		case BLADEMASTER_FSM::_NON_MOV_RUNRIGHT:
			break;
		case BLADEMASTER_FSM::_NON_MOV_RUNRIGHTBACK:
			break;
		case BLADEMASTER_FSM::_STD_MOV_IDLETOJUMP_FRONT:
			break;
		case BLADEMASTER_FSM::_STD_MOV_IDLETOJUMP_LEFT:
			break;
		case BLADEMASTER_FSM::_STD_MOV_IDLETOJUMP_RIGHT:
			break;
		case BLADEMASTER_FSM::_STD_MOV_JUMPFRONT:
			break;
		case BLADEMASTER_FSM::_STD_MOV_JUMPLEFT:
			break;
		case BLADEMASTER_FSM::_STD_MOV_JUMPRIGHT:
			break;
		case BLADEMASTER_FSM::_STD_MOV_JUMPTOIDLE:
			break;
		case BLADEMASTER_FSM::_STD_MOV_JUMPTOMOVE_BACK:
			break;
		case BLADEMASTER_FSM::_STD_MOV_JUMPTOMOVE_FRONT:
			break;
		case BLADEMASTER_FSM::_STD_MOV_JUMPTOMOVE_LEFT:
			break;
		case BLADEMASTER_FSM::_STD_MOV_JUMPTOMOVE_RIGHT:
			break;
		case BLADEMASTER_FSM::_STD_MOV_LEFTTORIGHT:
			break;
		case BLADEMASTER_FSM::_STD_MOV_MOVETOIDLE:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_STD_MOV_RIGHTTOLEFT:
			break;
		case BLADEMASTER_FSM::_STD_MOV_RUNBACK:
			break;
		case BLADEMASTER_FSM::_STD_MOV_RUNFRONT:
			break;
		case BLADEMASTER_FSM::_STD_MOV_RUNLEFT:
			break;
		case BLADEMASTER_FSM::_STD_MOV_RUNLEFTBACK:
			break;
		case BLADEMASTER_FSM::_STD_MOV_RUNRIGHT:
			break;
		case BLADEMASTER_FSM::_STD_MOV_RUNRIGHTBACK:
			break;
		case BLADEMASTER_FSM::_HID_MOV_IDLETOJUMP_FRONT:
			break;
		case BLADEMASTER_FSM::_HID_MOV_IDLETOJUMP_LEFT:
			break;
		case BLADEMASTER_FSM::_HID_MOV_IDLETOJUMP_RIGHT:
			break;
		case BLADEMASTER_FSM::_HID_MOV_JUMPFRONT:
			break;
		case BLADEMASTER_FSM::_HID_MOV_JUMPLEFT:
			break;
		case BLADEMASTER_FSM::_HID_MOV_JUMPRIGHT:
			break;
		case BLADEMASTER_FSM::_HID_MOV_JUMPTOIDLE:
			break;
		case BLADEMASTER_FSM::_HID_MOV_JUMPTOMOVE_BACK:
			break;
		case BLADEMASTER_FSM::_HID_MOV_JUMPTOMOVE_FRONT:
			break;
		case BLADEMASTER_FSM::_HID_MOV_JUMPTOMOVE_LEFT:
			break;
		case BLADEMASTER_FSM::_HID_MOV_JUMPTOMOVE_RIGHT:
			break;
		case BLADEMASTER_FSM::_HID_MOV_LEFTTORIGHT:
			break;
		case BLADEMASTER_FSM::_HID_MOV_MOVETOIDLE:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_HID_SKL_IDLE);
			break;
		case BLADEMASTER_FSM::_HID_MOV_RIGHTTOLEFT:
			break;
		case BLADEMASTER_FSM::_HID_MOV_RUNBACK:
			break;
		case BLADEMASTER_FSM::_HID_MOV_RUNFRONT:
			break;
		case BLADEMASTER_FSM::_HID_MOV_RUNLEFT:
			break;
		case BLADEMASTER_FSM::_HID_MOV_RUNLEFTBACK:
			break;
		case BLADEMASTER_FSM::_HID_MOV_RUNRIGHT:
			break;
		case BLADEMASTER_FSM::_HID_MOV_RUNRIGHTBACK:
			break;
		case BLADEMASTER_FSM::_NON_MOV_IDLE:
			break;
		case BLADEMASTER_FSM::_STD_MOV_IDLE:
			break;
		case BLADEMASTER_FSM::_HID_SKL_IDLE:
			break;
		case BLADEMASTER_FSM::_NON_EQUIPX:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_GLIDE_BOOST_FRONT:
			break;
		case BLADEMASTER_FSM::_GLIDE_BOOST_FRONT_CANCLE:
			break;
		case BLADEMASTER_FSM::_GLIDE_BOOST_FRONT_START:
			break;
		case BLADEMASTER_FSM::_GLIDE_DIVE_FRONT:
			break;
		case BLADEMASTER_FSM::_GLIDE_DIVE_FRONT_CANCLE:
			break;
		case BLADEMASTER_FSM::_GLIDE_IDLETOJUMP_FRONT_01:
			break;
		case BLADEMASTER_FSM::_GLIDE_JUMPFRONT:
			break;
		case BLADEMASTER_FSM::_GLIDE_JUMPLEFT:
			break;
		case BLADEMASTER_FSM::_GLIDE_JUMPRIGHT:
			break;
		case BLADEMASTER_FSM::_SPRINT_FRONT:
			break;
		case BLADEMASTER_FSM::_SPRINT_FRONT_FALLSTART:
			break;
		case BLADEMASTER_FSM::_SPRINT_FRONT_ROCKETSTART:
			break;
		case BLADEMASTER_FSM::_SPRINT_IDLETOJUMP_FRONT_01:
			break;
		case BLADEMASTER_FSM::_SPRINT_IDLETOJUMP_FRONT_02:
			break;
		case BLADEMASTER_FSM::_SPRINT_IDLETOJUMP_FRONT_03:
			break;
		case BLADEMASTER_FSM::_SPRINT_JUMPFRONT:
			break;
		case BLADEMASTER_FSM::_SPRINT_JUMPTOIDLE:
			break;
		case BLADEMASTER_FSM::_SPRINT_JUMPTOMOVE_FRONT:
			break;
		case BLADEMASTER_FSM::_SPRINT_MOVETOIDLE:
			break;
		case BLADEMASTER_FSM::_CAPTURE_EXEC2_FIRE:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_CAPTURE_EXEC2_END);
			break;
		case BLADEMASTER_FSM::_CAPTURE_EXEC2_END:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_KNOCKBACK_LONG_B_START:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_DOWN_FRONT_END);
			break;
		case BLADEMASTER_FSM::_DOWN_FRONT_END:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_STUN_START:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STUN_LOOPING);
			break;
		case BLADEMASTER_FSM::_STUN_LOOPING:
			break;
		case BLADEMASTER_FSM::_GROGGY_F_START:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_GROGGY_LOOPING);
			break;
		case BLADEMASTER_FSM::_GROGGY_LOOPING:
			break;
		case BLADEMASTER_FSM::_GROGGY_END:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_DOWN_MID_B_START:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_DOWN_LOOPING);
			break;
		case BLADEMASTER_FSM::_DOWN_LOW_B_R_START:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_DOWN_LOOPING);
			break;
		case BLADEMASTER_FSM::_DOWN_LOOPING:
			break;
		case BLADEMASTER_FSM::_DOWN_END:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_MIDAIR1_UPPER:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_DOWN_LOOPING);
			break;
		case BLADEMASTER_FSM::_UPPER_SLASH:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_DOWN_LOOPING);
			break;
		case BLADEMASTER_FSM::_MIDAIR2_UPPER:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_DOWN_LOOPING);
			break;
		case BLADEMASTER_FSM::_MIDAIR1_LOWER:
			break;
		case BLADEMASTER_FSM::_SKYKICK:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_DOWN_LOOPING);
			break;
		case BLADEMASTER_FSM::_ASCENSION_ED:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_DOWN_LOOPING);
			break;
		case BLADEMASTER_FSM::_FLYTAKEDOWN:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_MOUNTX_BAT_IDLE);
			break;
		case BLADEMASTER_FSM::_MOUNTX_BAT_IDLE:
			break;
		case BLADEMASTER_FSM::_MOUNTX_BAT_END:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_MOUNTX_SKL_ARMBAR:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			break;
		case BLADEMASTER_FSM::_MOUNTX_SKL_POUNDING_FIRST:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_DOWN_END);
			break;
		case BLADEMASTER_FSM::_MOUNTX_SKL_POUNDING:
			break;
		case BLADEMASTER_FSM::_DAMAGEBACK:
			if (GetBladeMaster_STATE() == BLADEMASTER_STATE::FENCE_STATE)
				SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			else if (GetBladeMaster_STATE() == BLADEMASTER_STATE::BATTO_STATE)
				SetBladeMaster_FSM(BLADEMASTER_FSM::_HID_SKL_IDLE);
			break;
		case BLADEMASTER_FSM::_DAMAGEFRONT:
			if (GetBladeMaster_STATE() == BLADEMASTER_STATE::FENCE_STATE)
				SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			else if (GetBladeMaster_STATE() == BLADEMASTER_STATE::BATTO_STATE)
				SetBladeMaster_FSM(BLADEMASTER_FSM::_HID_SKL_IDLE);
			break;
		case BLADEMASTER_FSM::_BE_DOWNCOUNTER:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_DOWN_LOW_B_R_START);
			break;
		case BLADEMASTER_FSM::_VIOLENCEPUNCH:
			if (GetBladeMaster_STATE() == BLADEMASTER_STATE::FENCE_STATE)
				SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			else if (GetBladeMaster_STATE() == BLADEMASTER_STATE::BATTO_STATE)
				SetBladeMaster_FSM(BLADEMASTER_FSM::_HID_SKL_IDLE);
			break;
		case BLADEMASTER_FSM::_TRIPLEKICK:
			if (GetBladeMaster_STATE() == BLADEMASTER_STATE::FENCE_STATE)
				SetBladeMaster_FSM(BLADEMASTER_FSM::_STD_MOV_IDLE);
			else if (GetBladeMaster_STATE() == BLADEMASTER_STATE::BATTO_STATE)
				SetBladeMaster_FSM(BLADEMASTER_FSM::_HID_SKL_IDLE);
			break;
		case BLADEMASTER_FSM::_BATTO_SKL_END:
			SetBladeMaster_FSM(BLADEMASTER_FSM::_HID_SKL_IDLE);
			break;
		case BLADEMASTER_FSM::END:
			break;
		default:
			break;
		}
	}
}

void CPlayerScript::SocketState_Confirm()
{
	if (Player_FSM == BLADEMASTER_FSM::_NON_MOV_IDLE || Player_FSM == BLADEMASTER_FSM::_STD_MOV_IDLE || Player_FSM == BLADEMASTER_FSM::_HID_SKL_IDLE || Player_FSM >= BLADEMASTER_FSM::_NON_MOV_IDLETOJUMP_FRONT)
	{
		switch (Player_Position)
		{
		case BLADEMASTER_STATE::_CC_DOWN:
		case BLADEMASTER_STATE::_CC_GROGGY:
		case BLADEMASTER_STATE::_CC_STUN:
		case BLADEMASTER_STATE::_CC_AIR:
		case BLADEMASTER_STATE::_CC_MOUNT:
		case BLADEMASTER_STATE::FENCE_STATE:
			SetSocket_stdStance();
			break;
		case BLADEMASTER_STATE::NON_COMBAT:
			SetSocket_nonBehind();
			break;
		case BLADEMASTER_STATE::BATTO_STATE:
			SetSocket_hidStance();
			break;
		case BLADEMASTER_STATE::END:
			break;
		default:
			break;
		}
	}
}

void CPlayerScript::_AnimateFrame_Event()	// 진행중인 곳에 함수 호출 
{
	switch (Player_FSM)
	{
	case BLADEMASTER_FSM::_1_STD_BLOCKING:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_1_STD_BLOCK1:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_1_STD_BLOCK2:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_1_STD_BLOCK3:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_1_STD_LEGSLASH:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		SetFrameAnimationFunc(25, &CPlayerScript::SetAI_Damage);
		break;
	case BLADEMASTER_FSM::_2_STD_RUSHING:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		SetFrameAnimationFunc(20, &CPlayerScript::SetAI_StunRush);
		break;
	case BLADEMASTER_FSM::_2_STD_RUSHED:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_2_HID_RUSHING:
		SetSocket_hidStance();
		SetFrameAnimationFunc(20, &CPlayerScript::SetAI_Damage);
		break;
	case BLADEMASTER_FSM::_2_HID_RUSHED:
		SetSocket_hidStance();
		break;
	case BLADEMASTER_FSM::_3_HID_FIVE_COMBO_SLASH_1:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		SetFrameAnimationFunc(10, &CPlayerScript::SetAI_Damage);
	//	SetFrameAnimationFunc(7, &CPlayerScript::SetBladeMaster_BattoEnd);
		break;
	case BLADEMASTER_FSM::_3_HID_FIVE_COMBO_SLASH_2:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		SetFrameAnimationFunc(8, &CPlayerScript::SetAI_Damage);
		break;
	case BLADEMASTER_FSM::_3_HID_FIVE_COMBO_SLASH_3:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		SetFrameAnimationFunc(8, &CPlayerScript::SetAI_Damage);
		break;
	case BLADEMASTER_FSM::_3_HID_FIVE_COMBO_SLASH_4:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		SetFrameAnimationFunc(8, &CPlayerScript::SetAI_Damage);
		break;
	case BLADEMASTER_FSM::_3_HID_FIVE_COMBO_SLASH_5:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		SetFrameAnimationFunc(8, &CPlayerScript::SetAI_Damage);
		SetFrameAnimationFunc(34, &CPlayerScript::SetReadyResist_False);
		SetFrameAnimationFunc(34, &CPlayerScript::SetBladeMaster_BattoEnd);
		break;
	case BLADEMASTER_FSM::_3_STD_RETURN_READY:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_3_STD_RETURN_STANDBY:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_3_STD_RETURN_EXEC:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		SetFrameAnimationFunc(10, &CPlayerScript::SetAI_DownLowSlash);
		break;
	case BLADEMASTER_FSM::_3_STD_RETURN_FINISH:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_3_STD_LOWSLASH_CAST:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_3_STD_LOWSLASH_EXEC:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		SetFrameAnimationFunc(0, &CPlayerScript::SetAI_DownLowSlash);
		break;
	case BLADEMASTER_FSM::_4_STD_LYING_RETURN_READY:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_4_STD_LYING_RETURN_STANDBY:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_4_STD_LYING_RETURN_FINISH:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_4_STD_MACH_SLASH:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		SetFrameAnimationFunc(25, &CPlayerScript::SetSocket_hidStance);
		break;
	case BLADEMASTER_FSM::_4_HID_MACH_SLASH:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		SetFrameAnimationFunc(25, &CPlayerScript::SetSocket_hidStance);
		break;
	case BLADEMASTER_FSM::_Z_STD_FLYING_SWALLOW_SWORD_EXEC:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		SetFrameAnimationFunc(34, &CPlayerScript::SetAI_CaptureFire);
		break;
	case BLADEMASTER_FSM::_Z_STD_FLYING_SWALLOW_SWORD_FINISH:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_Z_STD_FLYING_SWALLOW_SWORD_END:
	//	SetFrameAnimationFunc(9, &CPlayerScript::SetBladeMaster_flyingSwordEndToIdle);
		break;
	case BLADEMASTER_FSM::_Z_STD_FLYING_SWALLOW_SWORD_ENDTOIDLE:
	//	SetFrameAnimationFunc(2, &CPlayerScript::SetSocket_hidStance);
	//	SetFrameAnimationFunc(7, &CPlayerScript::SetBladeMaster_hidStance);
		break;
	case BLADEMASTER_FSM::_Z_HID_LAND_CUTTING:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		SetFrameAnimationFunc(16, &CPlayerScript::SetBladeMaster_LandCut);
		break;
	case BLADEMASTER_FSM::_Z_HID_LAND_CUT_END:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_hidEndHand);
		SetFrameAnimationFunc(37, &CPlayerScript::SetSocket_hidStance);
		break;
	case BLADEMASTER_FSM::_X_STD_KICKKNOCKBACK_CAST:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_X_STD_KICKKNOCKBACK_EXEC:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		SetFrameAnimationFunc(1, &CPlayerScript::SetAI_DownFrontKick);

		break;
	case BLADEMASTER_FSM::_S_BACKSTEP:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_hidStance);
		break;
	case BLADEMASTER_FSM::_C_STD_SHOULDERCHARGE:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		SetFrameAnimationFunc(15, &CPlayerScript::SetAI_Damage);
		break;
	case BLADEMASTER_FSM::_C_HID_WINDYMOONSLASH_01:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		SetFrameAnimationFunc(3, &CPlayerScript::SetAI_Damage);
		break;
	case BLADEMASTER_FSM::_C_HID_WINDYMOONSLASH_02:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		SetFrameAnimationFunc(13, &CPlayerScript::SetAI_Damage);
		break;
	case BLADEMASTER_FSM::_C_HID_WINDYMOONSLASH_FINISH:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		SetFrameAnimationFunc(33, &CPlayerScript::SetSocket_hidEndHand);
		SetFrameAnimationFunc(73, &CPlayerScript::SetSocket_hidStance);
		break;
	case BLADEMASTER_FSM::_V_STD_DIFFUSIONSOUL_1:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_IdlePos);
		SetFrameAnimationFunc(2, &CPlayerScript::SetAI_KnockBack);
		break;
	case BLADEMASTER_FSM::_V_STD_DIFFUSIONSOUL_2:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_V_HID_PIERCE1:		
	case BLADEMASTER_FSM::_V_HID_PIERCE2:
	case BLADEMASTER_FSM::_V_HID_PIERCE3:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		SetFrameAnimationFunc(45, &CPlayerScript::SetSocket_hidEndHand);
		SetFrameAnimationFunc(80, &CPlayerScript::SetSocket_hidStance);
		SetFrameAnimationFunc(8, &CPlayerScript::SetAI_Damage);
		break;
	case BLADEMASTER_FSM::_Q_COUNTERATTACK_Q:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_hidStance);
		break;
	case BLADEMASTER_FSM::_E_COUNTERATTACK_E:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_T_FULL_MOON_SLASH:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		SetFrameAnimationFunc(24, &CPlayerScript::SetSocket_hidStance);
		SetFrameAnimationFunc(6, &CPlayerScript::SetAI_Damage);
		break;
	case BLADEMASTER_FSM::_T_ESCAPE:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		SetFrameAnimationFunc(6, &CPlayerScript::SetAI_GroggyNear);
		break;
	case BLADEMASTER_FSM::_T_ESCAPE_END:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_L_STD_HACK:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		SetFrameAnimationFunc(6, &CPlayerScript::SetAI_Damage);
		SetFrameAnimationFunc(10, &CPlayerScript::SetAI_Damage);
		SetFrameAnimationFunc(21, &CPlayerScript::SetAI_Damage);
		SetFrameAnimationFunc(27, &CPlayerScript::SetAI_Damage);
		break;
	case BLADEMASTER_FSM::_L_STD_HACK_END:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_L_STD_RUSHING_WIND_SPHERE:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		SetFrameAnimationFunc(9, &CPlayerScript::SetAI_Damage);
		break;
	case BLADEMASTER_FSM::_L_STD_BODY_BLADE_UNITY_FIRE:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_L_STD_BODY_BLADE_UNITY_EXEC:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		SetFrameAnimationFunc(9, &CPlayerScript::SetAI_Stun);
		break;
	case BLADEMASTER_FSM::_L_STD_BODY_BLADE_SPRINT_FIRE:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_L_STD_BODY_BLADE_SPRINT_EXEC:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		SetFrameAnimationFunc(9, &CPlayerScript::SetAI_Stun);
		break;
	case BLADEMASTER_FSM::_L_STD_ASCENSION:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_L_HID_BATTO:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		SetFrameAnimationFunc(9, &CPlayerScript::SetSocket_hidStance);
		SetFrameAnimationFunc(3, &CPlayerScript::SetAI_Damage);
		break;
	case BLADEMASTER_FSM::_R_STD_RUSH_STAB_EXEC:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		SetFrameAnimationFunc(0, &CPlayerScript::SetAI_Damage);
		break;
	case BLADEMASTER_FSM::_R_STD_MIDAIR_EXEC_01:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_R_STD_MIDAIR_EXEC_02:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_R_STD_MIDAIR_EXEC_03:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_R_STD_MIDAIR_EXEC_04:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_R_STD_MIDAIR_EXEC_05:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_R_STD_CRIPPLING_CUT_FIRE:
		SetFrameAnimationFunc(3, &CPlayerScript::SetAI_R_CRASH);
		break;
	case BLADEMASTER_FSM::_R_STD_LIGHTNINGSMASH_FIRE:
		break;
	case BLADEMASTER_FSM::_R_STD_LIGHTNINGSMASH_EXEC:
		break;
	case BLADEMASTER_FSM::_R_HID_VITALSLASH_EXEC:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_vitStance);
		SetFrameAnimationFunc(31, &CPlayerScript::SetSocket_hidEndHand);
		SetFrameAnimationFunc(68, &CPlayerScript::SetSocket_hidStance);

	//	//
	//	SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
	//	SetFrameAnimationFunc(45, &CPlayerScript::SetSocket_hidEndHand);
	//	SetFrameAnimationFunc(80, &CPlayerScript::SetSocket_hidStance);

		//
		break;
	case BLADEMASTER_FSM::_F_STD_PHANTOMSLASH3_SWING:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_F_STD_PHANTOMSLASH3_EXEC:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_F_BACKWARDROLL:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_F_HID_BATTO_BLOCKING_START:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_F_HID_BATTO_BLOCKING:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_Z_STD_SOULBLAST:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_Z_HID_SOULBLAST:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_hidStance);
		break;
	case BLADEMASTER_FSM::_X_STD_GUARDSOUL_BLADE:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_X_HID_THIRDBLADE_SLASH:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_hidStance);
		break;
	case BLADEMASTER_FSM::_C_STD_SOULSTUNLIGHT:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_C_HID_SOULSTUNLIGHT:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_hidStance);
		break;
	case BLADEMASTER_FSM::_V_STD_ROTATESOULBLADE:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_V_HID_ROTATESOULBLADE:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_hidStance);
		break;
	case BLADEMASTER_FSM::_NON_MOV_IDLE:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_nonBehind);
		break;
	case BLADEMASTER_FSM::_STD_MOV_IDLE:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_HID_SKL_IDLE:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_hidStance);
		break;
	case BLADEMASTER_FSM::_NON_EQUIPX:
		SetFrameAnimationFunc(20, &CPlayerScript::SetSocket_nonBehind);
		SetBladeMaster_STATE(BLADEMASTER_STATE::NON_COMBAT);
		break;
	case BLADEMASTER_FSM::_CAPTURE_EXEC2_FIRE:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_CAPTURE_EXEC2_END:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_KNOCKBACK_LONG_B_START:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_DOWN_FRONT_END:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_STUN_START:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_STUN_LOOPING:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_GROGGY_F_START:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_GROGGY_LOOPING:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_GROGGY_END:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_DOWN_MID_B_START:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_DOWN_LOW_B_R_START:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_DOWN_LOOPING:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_DOWN_END:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_MIDAIR1_UPPER:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_UPPER_SLASH:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_MIDAIR2_UPPER:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_MIDAIR1_LOWER:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_SKYKICK:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_ASCENSION_ED:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_FLYTAKEDOWN:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_MOUNTX_BAT_IDLE:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_MOUNTX_BAT_END:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_MOUNTX_SKL_ARMBAR:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_MOUNTX_SKL_POUNDING_FIRST:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_MOUNTX_SKL_POUNDING:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		break;
	case BLADEMASTER_FSM::_DAMAGEBACK:
		if (Player_Position == BLADEMASTER_STATE::BATTO_STATE)
		{
			SetSocket_hidStance();
		}
		else if (Player_Position != BLADEMASTER_STATE::NON_COMBAT)
		{
			SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		}
		break;
	case BLADEMASTER_FSM::_DAMAGEFRONT:
		if (Player_Position == BLADEMASTER_STATE::BATTO_STATE)
		{
			SetSocket_hidStance();
		}
		else if (Player_Position != BLADEMASTER_STATE::NON_COMBAT)
		{
			SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		}
		break;
	case BLADEMASTER_FSM::_BE_DOWNCOUNTER:
		if (Player_Position == BLADEMASTER_STATE::BATTO_STATE)
		{
			SetSocket_hidStance();
		}
		else if (Player_Position != BLADEMASTER_STATE::NON_COMBAT)
		{
			SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		}
		break;
	case BLADEMASTER_FSM::_VIOLENCEPUNCH:
		if (Player_Position == BLADEMASTER_STATE::BATTO_STATE)
		{
			SetSocket_hidStance();
		}
		else if (Player_Position != BLADEMASTER_STATE::NON_COMBAT)
		{
			SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		}
		break;
	case BLADEMASTER_FSM::_TRIPLEKICK:
		if (Player_Position == BLADEMASTER_STATE::BATTO_STATE)
		{
			SetSocket_hidStance();
		}
		else if (Player_Position != BLADEMASTER_STATE::NON_COMBAT)
		{
			SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_stdStance);
		}
		break;
	case BLADEMASTER_FSM::_BATTO_SKL_END:
		SetFrameAnimationFunc(0, &CPlayerScript::SetSocket_hidEndHand);
		SetFrameAnimationFunc(47, &CPlayerScript::SetSocket_hidStance);
		break;
	case BLADEMASTER_FSM::_NON_MOV_IDLETOJUMP_FRONT:
		break;
	case BLADEMASTER_FSM::_NON_MOV_IDLETOJUMP_LEFT:
		break;
	case BLADEMASTER_FSM::_NON_MOV_IDLETOJUMP_RIGHT:
		break;
	case BLADEMASTER_FSM::_NON_MOV_JUMPFRONT:
		break;
	case BLADEMASTER_FSM::_NON_MOV_JUMPLEFT:
		break;
	case BLADEMASTER_FSM::_NON_MOV_JUMPRIGHT:
		break;
	case BLADEMASTER_FSM::_NON_MOV_JUMPTOIDLE:
		break;
	case BLADEMASTER_FSM::_NON_MOV_JUMPTOMOVE_BACK:
		break;
	case BLADEMASTER_FSM::_NON_MOV_JUMPTOMOVE_FRONT:
		break;
	case BLADEMASTER_FSM::_NON_MOV_JUMPTOMOVE_LEFT:
		break;
	case BLADEMASTER_FSM::_NON_MOV_JUMPTOMOVE_RIGHT:
		break;
	case BLADEMASTER_FSM::_NON_MOV_LEFTTORIGHT:
		break;
	case BLADEMASTER_FSM::_NON_MOV_MOVETOIDLE:
		break;
	case BLADEMASTER_FSM::_NON_MOV_RIGHTTOLEFT:
		break;
	case BLADEMASTER_FSM::_NON_MOV_RUNBACK:
		break;
	case BLADEMASTER_FSM::_NON_MOV_RUNFRONT:
		break;
	case BLADEMASTER_FSM::_NON_MOV_RUNLEFT:
		break;
	case BLADEMASTER_FSM::_NON_MOV_RUNLEFTBACK:
		break;
	case BLADEMASTER_FSM::_NON_MOV_RUNRIGHT:
		break;
	case BLADEMASTER_FSM::_NON_MOV_RUNRIGHTBACK:
		break;
	case BLADEMASTER_FSM::_STD_MOV_IDLETOJUMP_FRONT:
		break;
	case BLADEMASTER_FSM::_STD_MOV_IDLETOJUMP_LEFT:
		break;
	case BLADEMASTER_FSM::_STD_MOV_IDLETOJUMP_RIGHT:
		break;
	case BLADEMASTER_FSM::_STD_MOV_JUMPFRONT:
		break;
	case BLADEMASTER_FSM::_STD_MOV_JUMPLEFT:
		break;
	case BLADEMASTER_FSM::_STD_MOV_JUMPRIGHT:
		break;
	case BLADEMASTER_FSM::_STD_MOV_JUMPTOIDLE:
		break;
	case BLADEMASTER_FSM::_STD_MOV_JUMPTOMOVE_BACK:
		break;
	case BLADEMASTER_FSM::_STD_MOV_JUMPTOMOVE_FRONT:
		break;
	case BLADEMASTER_FSM::_STD_MOV_JUMPTOMOVE_LEFT:
		break;
	case BLADEMASTER_FSM::_STD_MOV_JUMPTOMOVE_RIGHT:
		break;
	case BLADEMASTER_FSM::_STD_MOV_LEFTTORIGHT:
		break;
	case BLADEMASTER_FSM::_STD_MOV_MOVETOIDLE:
		break;
	case BLADEMASTER_FSM::_STD_MOV_RIGHTTOLEFT:
		break;
	case BLADEMASTER_FSM::_STD_MOV_RUNBACK:
		break;
	case BLADEMASTER_FSM::_STD_MOV_RUNFRONT:
		break;
	case BLADEMASTER_FSM::_STD_MOV_RUNLEFT:
		break;
	case BLADEMASTER_FSM::_STD_MOV_RUNLEFTBACK:
		break;
	case BLADEMASTER_FSM::_STD_MOV_RUNRIGHT:
		break;
	case BLADEMASTER_FSM::_STD_MOV_RUNRIGHTBACK:
		break;
	case BLADEMASTER_FSM::_HID_MOV_IDLETOJUMP_FRONT:
		break;
	case BLADEMASTER_FSM::_HID_MOV_IDLETOJUMP_LEFT:
		break;
	case BLADEMASTER_FSM::_HID_MOV_IDLETOJUMP_RIGHT:
		break;
	case BLADEMASTER_FSM::_HID_MOV_JUMPFRONT:
		break;
	case BLADEMASTER_FSM::_HID_MOV_JUMPLEFT:
		break;
	case BLADEMASTER_FSM::_HID_MOV_JUMPRIGHT:
		break;
	case BLADEMASTER_FSM::_HID_MOV_JUMPTOIDLE:
		break;
	case BLADEMASTER_FSM::_HID_MOV_JUMPTOMOVE_BACK:
		break;
	case BLADEMASTER_FSM::_HID_MOV_JUMPTOMOVE_FRONT:
		break;
	case BLADEMASTER_FSM::_HID_MOV_JUMPTOMOVE_LEFT:
		break;
	case BLADEMASTER_FSM::_HID_MOV_JUMPTOMOVE_RIGHT:
		break;
	case BLADEMASTER_FSM::_HID_MOV_LEFTTORIGHT:
		break;
	case BLADEMASTER_FSM::_HID_MOV_MOVETOIDLE:
		break;
	case BLADEMASTER_FSM::_HID_MOV_RIGHTTOLEFT:
		break;
	case BLADEMASTER_FSM::_HID_MOV_RUNBACK:
		break;
	case BLADEMASTER_FSM::_HID_MOV_RUNFRONT:
		break;
	case BLADEMASTER_FSM::_HID_MOV_RUNLEFT:
		break;
	case BLADEMASTER_FSM::_HID_MOV_RUNLEFTBACK:
		break;
	case BLADEMASTER_FSM::_HID_MOV_RUNRIGHT:
		break;
	case BLADEMASTER_FSM::_HID_MOV_RUNRIGHTBACK:
		break;
	case BLADEMASTER_FSM::_GLIDE_BOOST_FRONT:
		break;
	case BLADEMASTER_FSM::_GLIDE_BOOST_FRONT_CANCLE:
		break;
	case BLADEMASTER_FSM::_GLIDE_BOOST_FRONT_START:
		break;
	case BLADEMASTER_FSM::_GLIDE_DIVE_FRONT:
		break;
	case BLADEMASTER_FSM::_GLIDE_DIVE_FRONT_CANCLE:
		break;
	case BLADEMASTER_FSM::_GLIDE_IDLETOJUMP_FRONT_01:
		break;
	case BLADEMASTER_FSM::_GLIDE_JUMPFRONT:
		break;
	case BLADEMASTER_FSM::_GLIDE_JUMPLEFT:
		break;
	case BLADEMASTER_FSM::_GLIDE_JUMPRIGHT:
		break;
	case BLADEMASTER_FSM::_SPRINT_FRONT:
		break;
	case BLADEMASTER_FSM::_SPRINT_FRONT_FALLSTART:
		break;
	case BLADEMASTER_FSM::_SPRINT_FRONT_ROCKETSTART:
		break;
	case BLADEMASTER_FSM::_SPRINT_IDLETOJUMP_FRONT_01:
		break;
	case BLADEMASTER_FSM::_SPRINT_IDLETOJUMP_FRONT_02:
		break;
	case BLADEMASTER_FSM::_SPRINT_IDLETOJUMP_FRONT_03:
		break;
	case BLADEMASTER_FSM::_SPRINT_JUMPFRONT:
		break;
	case BLADEMASTER_FSM::_SPRINT_JUMPTOIDLE:
		break;
	case BLADEMASTER_FSM::_SPRINT_JUMPTOMOVE_FRONT:
		break;
	case BLADEMASTER_FSM::_SPRINT_MOVETOIDLE:
		break;
	case BLADEMASTER_FSM::END:
		break;
	default:
		break;
	}
}

void CPlayerScript::SetAI_CaptureFire()
{
	if (_AIScript && _AIScript->_Ready_resist == false)
	{
		_AIScript->SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_FB_CAPTURE_EXEC2_FIRE);
		SetBladeMaster_FSM(BLADEMASTER_FSM::_Z_STD_FLYING_SWALLOW_SWORD_FINISH);
	}
}

void CPlayerScript::SetAI_DownFrontKick()
{
	if (_AIScript && _AIScript->_Ready_resist == false)
	{
		if (m_eDistance == DISTANCE::NEARBY)
		{
			if (_AIScript->_Counterattacking)
			{
				_SetOpponentScriptFSM(KUNGFUMASTER_FSM::_STUN_START, 2.f);
			}
			else
			{
				_SetOpponentScriptFSM(KUNGFUMASTER_FSM::_DOWN_MID_B_START, 3.f);
			}
		}
	}
}

void CPlayerScript::SetAI_DownLowSlash() // 하단베기
{
	if (_AIScript && _AIScript->_Ready_resist == false)
	{
		if (m_eDistance == DISTANCE::NEARBY)
		{
			if (_AIScript->_Counterattacking == false)
			{
				_AIScript->SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_DOWN_LOW_B_R_START);	// 반격 상태가 아니라면 다운.
			}
			else
			{
				_AIScript->SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_1_COUNTERENEMY_EXEC); // 반격 상태일 때 반격성공 모션.
			}
		}
	}
}

void CPlayerScript::SetAI_StunRush()
{
	if (_AIScript && _AIScript->_Ready_resist == false)
	{
		if (_AIScript->_Counterattacking == false)
		{
			_SetOpponentScriptFSM(KUNGFUMASTER_FSM::_STUN_START, 2.f);
		}
		else
		{
			_AIScript->SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_1_COUNTERENEMY_EXEC);
		}
	}
}

void CPlayerScript::SetAI_GroggyNear()
{
	if (_AIScript && _AIScript->_Ready_resist == false)
	{
		if (m_eDistance == DISTANCE::NEARBY)
		{
			_AIScript->SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_GROGGY_KNEE_F_START);
		}
	}
}

void CPlayerScript::SetAI_GroggyFar()
{
	if (_AIScript && _AIScript->_Ready_resist == false)
	{
		if (m_eDistance <= DISTANCE::MIDDLE)
		{
			_AIScript->SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_GROGGY_KNEE_F_START);
		}
	}
}

void CPlayerScript::SetAI_Stun()
{
	if (_AIScript && _AIScript->_Ready_resist == false)
	{
		_SetOpponentScriptFSM(KUNGFUMASTER_FSM::_STUN_START, 2.f);
	}
}

void CPlayerScript::SetAI_UpperSlash()
{
	if (_AIScript && _AIScript->_Ready_resist == false)
	{
		_AIScript->SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_AIR_UPPER_SLASH);
	}
}

void CPlayerScript::SetAI_AirComboL()
{
	if (_AIScript && _AIScript->_Ready_resist == false)
	{
		_AIScript->SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_AIR_MIDAIR2_UPPER);
	}
}

void CPlayerScript::SetAI_AirComboR()
{
	if (_AIScript && _AIScript->_Ready_resist == false)
	{
		_AIScript->SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_AIR_MIDAIR1_UPPER);
	}
}

void CPlayerScript::SetAI_R_CRASH()
{
	if (_AIScript && _AIScript->_Ready_resist == false)
	{
		_AIScript->SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_AIR_BIGHITX_SKL_SKYKICK);
	}
}

void CPlayerScript::SetAI_L_ascension()
{
	if (_AIScript && _AIScript->_Ready_resist == false)
	{
		_AIScript->SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_FB_ASCENSION);
	}
}

void CPlayerScript::SetAI_KnockBack()
{
	if (_AIScript && _AIScript->_Ready_resist == false)
	{
		_AIScript->SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_FB_KNOCKBACK_LONG_B_START);
	}
}

void CPlayerScript::SetAI_Damage()
{
	if (_Rotating_calculating())
	{
		SetAI_DamageFront();
	}
	else
	{
		SetAI_DamageBack();
	}
}

void CPlayerScript::SetAI_DamageFront()
{
	if (_AIScript && _AIScript->_Ready_resist == false)
	{
		KUNGFUMASTER_FSM _AI_FSM = _AIScript->GetKungfuMaster_FSM();

		if (_AI_FSM == KUNGFUMASTER_FSM::_BAT_MOV_IDLE || _AI_FSM == KUNGFUMASTER_FSM::_NON_MOV_IDLE || _AI_FSM == KUNGFUMASTER_FSM::_RUNBACK || _AI_FSM == KUNGFUMASTER_FSM::_RUNFRONT || _AI_FSM == KUNGFUMASTER_FSM::_RUNLEFT || _AI_FSM == KUNGFUMASTER_FSM::_RUNRIGHT || _AI_FSM == KUNGFUMASTER_FSM::_RUNLEFTBACK || _AI_FSM == KUNGFUMASTER_FSM::_RUNRIGHTBACK)
		{
			_AIScript->SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_DAMAGEFRONT);
		}
	}
}

void CPlayerScript::SetAI_DamageBack()
{
	if (_AIScript && _AIScript->_Ready_resist == false)
	{
		KUNGFUMASTER_FSM _AI_FSM = _AIScript->GetKungfuMaster_FSM();

		if (_AI_FSM == KUNGFUMASTER_FSM::_BAT_MOV_IDLE || _AI_FSM == KUNGFUMASTER_FSM::_NON_MOV_IDLE || _AI_FSM == KUNGFUMASTER_FSM::_RUNBACK || _AI_FSM == KUNGFUMASTER_FSM::_RUNFRONT || _AI_FSM == KUNGFUMASTER_FSM::_RUNLEFT || _AI_FSM == KUNGFUMASTER_FSM::_RUNRIGHT || _AI_FSM == KUNGFUMASTER_FSM::_RUNLEFTBACK || _AI_FSM == KUNGFUMASTER_FSM::_RUNRIGHTBACK)
		{
			_AIScript->SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_DAMAGEBACK);
		}
	}
}

void CPlayerScript::SetAI_CC_extension()
{
	if (_AIScript && _AIScript->_Ready_resist == false)
	{
		// 미완성
	}
}


void CPlayerScript::_Distance_calculating()
{
	if (CAIScript::AIObject && _AIScript)
	{
		Vec3 PlayerPos = Transform()->GetLocalPos();
		Vec3 AIPos = _AIScript->Transform()->GetLocalPos();

		m_fDistance = sqrt(pow(fabs(PlayerPos.x - AIPos.x), 2) + pow(fabs(PlayerPos.z - AIPos.z), 2));
		m_fDistance *= INGAME_DISTANCE;

		if (0.f <= m_fDistance && m_fDistance < 4.f)
		{
			m_eDistance = DISTANCE::NEARBY;
		}
		else if (4.f <= m_fDistance && m_fDistance < 14.f)
		{
			m_eDistance = DISTANCE::MIDDLE;
		}
		else if (14.f <= m_fDistance && m_fDistance < 17.f)
		{
			m_eDistance = DISTANCE::AFAR;
		}
		else		
			m_eDistance = DISTANCE::END;
		
	}
	else
		m_fDistance = 0.f;
}

bool CPlayerScript::_Rotating_calculating()
{
	if (CAIScript::AIObject && _AIScript)
	{
		Vec3 PlayerPos = Transform()->GetLocalPos();
		Vec3 AIPos = _AIScript->Transform()->GetLocalPos();

		Vec3 PlayerRot = Transform()->GetLocalRot();
		Vec3 AIRot = _AIScript->Transform()->GetLocalRot();
	

		float PlayerRotation = (360.0f / 65536) * ((int)((PlayerRot.y * _RADIAN)*(65536 / 360.0f)) & 65535);
		float AIRotation = (360.0f / 65536) * ((int)((AIRot.y * _RADIAN)*(65536 / 360.0f)) & 65535);

		//	(360.0f / 65536) * ((int)((각도)*(65536 / 360.0f)) & 65535);

		// X축을 기준으로 나눴을 경우
		if (PlayerPos.x - AIPos.x < 0)
		{
			if ((0.f <= PlayerRotation && PlayerRotation <= 90.f) || (270.f <= PlayerRotation && PlayerRotation <= 360.f))
			{
				if ((90.f <= AIRotation && AIRotation <= 270.f))
				{
					return true;
				}
			}
		}
		else
		{
			if ((0.f <= AIRotation && AIRotation <= 90.f) || (270.f <= AIRotation && AIRotation <= 360.f))
			{
				if ((90.f <= PlayerRotation && PlayerRotation <= 270.f))
				{
					return true;
				}
			}
		}
	}

	return false;
}

void CPlayerScript::Animation_Setting_FSM()
{
	switch (Player_FSM)
	{
	case BLADEMASTER_FSM::_1_STD_BLOCKING:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Standard_Skl_blocking_Looping_Move");
		break;
	case BLADEMASTER_FSM::_1_STD_BLOCK1:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Bat_Parry_F");
		break;
	case BLADEMASTER_FSM::_1_STD_BLOCK2:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Bat_Parry_L");
		break;
	case BLADEMASTER_FSM::_1_STD_BLOCK3:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Bat_Parry_R");
		break;
	case BLADEMASTER_FSM::_1_STD_LEGSLASH:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Standard_Skl_legslash");
		break;
	case BLADEMASTER_FSM::_2_STD_RUSHING:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Standard_Skl_Rush_Fire");
		break;
	case BLADEMASTER_FSM::_2_STD_RUSHED:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Standard_Skl_Rush_Exec");
		break;
	case BLADEMASTER_FSM::_2_HID_RUSHING:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_hideblade_Skl_rush_Fire");
		break;
	case BLADEMASTER_FSM::_2_HID_RUSHED:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_hideblade_Skl_rush_Exec");
		break;
	case BLADEMASTER_FSM::_3_HID_FIVE_COMBO_SLASH_1:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_hideblade_Skl_Six_Combo_Slash_Exec_01");
		break;
	case BLADEMASTER_FSM::_3_HID_FIVE_COMBO_SLASH_2:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_hideblade_Skl_Six_Combo_Slash_Exec_02");
		break;
	case BLADEMASTER_FSM::_3_HID_FIVE_COMBO_SLASH_3:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_hideblade_Skl_Six_Combo_Slash_Exec_03");
		break;
	case BLADEMASTER_FSM::_3_HID_FIVE_COMBO_SLASH_4:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_hideblade_Skl_Six_Combo_Slash_Exec_04");
		break;
	case BLADEMASTER_FSM::_3_HID_FIVE_COMBO_SLASH_5:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_hideblade_Skl_Six_Combo_Slash_Exec_05");
		break;
	case BLADEMASTER_FSM::_3_STD_RETURN_READY:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Standard_Skl_Return_ready");
		break;
	case BLADEMASTER_FSM::_3_STD_RETURN_STANDBY:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Standard_Skl_Return_standby");
		break;
	case BLADEMASTER_FSM::_3_STD_RETURN_EXEC:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Standard_Skl_Return_Exec");
		break;
	case BLADEMASTER_FSM::_3_STD_RETURN_FINISH:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Standard_Skl_Return_finish");
		break;
	case BLADEMASTER_FSM::_3_STD_LOWSLASH_CAST:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Standard_SKL_turning_strike_Cast");
		break;
	case BLADEMASTER_FSM::_3_STD_LOWSLASH_EXEC:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Standard_SKL_turning_strike_Exec");
		break;
	case BLADEMASTER_FSM::_4_STD_LYING_RETURN_READY:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Standard_Skl_Return_ready");
		break;
	case BLADEMASTER_FSM::_4_STD_LYING_RETURN_STANDBY:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Standard_Skl_Return_standby");
		break;
	case BLADEMASTER_FSM::_4_STD_LYING_RETURN_FINISH:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Standard_Skl_Return_finish");
		break;
	case BLADEMASTER_FSM::_4_STD_MACH_SLASH:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_hideblade_Skl_Mach_Slash");
		break;
	case BLADEMASTER_FSM::_4_HID_MACH_SLASH:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_hideblade_Skl_Mach_Slash2");
		break;
	case BLADEMASTER_FSM::_Z_STD_FLYING_SWALLOW_SWORD_EXEC:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Standard_Skl_flying_swallow_sword_String");
		break;
	case BLADEMASTER_FSM::_Z_STD_FLYING_SWALLOW_SWORD_FINISH:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Standard_Skl_flying_swallow_sword_StringPull");
		break;
	case BLADEMASTER_FSM::_Z_STD_FLYING_SWALLOW_SWORD_END:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Phantom_G3_Skl_PhantomSlash1_Swing");
		break;
	case BLADEMASTER_FSM::_Z_STD_FLYING_SWALLOW_SWORD_ENDTOIDLE:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Phantom_G3_Skl_PhantomSlash2_Swing");
		break;
	case BLADEMASTER_FSM::_Z_HID_LAND_CUTTING:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Phantom_G3_Skl_PhantomSlash3_Exec");
		break;
	case BLADEMASTER_FSM::_Z_HID_LAND_CUT_END:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_hideblade_G1_Skl_FreezingSword_End");
		break;
	case BLADEMASTER_FSM::_X_STD_KICKKNOCKBACK_CAST:
		Animator3D()->SetCurrentAnimClip(L"BM\\TA_A_Protection_SKL_KickKnockback_Cast");
		break;
	case BLADEMASTER_FSM::_X_STD_KICKKNOCKBACK_EXEC:
		Animator3D()->SetCurrentAnimClip(L"BM\\TA_A_Protection_SKL_KickKnockback_Exec");
		break;
	case BLADEMASTER_FSM::_X_STD_KICKKNOCKBACK_END:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Phantom_G3_Skl_Six_End");
		break;
	case BLADEMASTER_FSM::_S_BACKSTEP:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_hideblade_SKL_backstep");
		break;
	case BLADEMASTER_FSM::_C_STD_SHOULDERCHARGE:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Standard_SKL_shoulderCharge");
		break;
	case BLADEMASTER_FSM::_C_HID_WINDYMOONSLASH_01:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_hideblade_Skl_WindyMoonSlash_01");
		break;
	case BLADEMASTER_FSM::_C_HID_WINDYMOONSLASH_02:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_hideblade_Skl_WindyMoonSlash_02");
		break;
	case BLADEMASTER_FSM::_C_HID_WINDYMOONSLASH_FINISH:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_hideblade_Skl_WindyMoonSlash_Finish");
		break;
	case BLADEMASTER_FSM::_V_STD_DIFFUSIONSOUL_1:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_Phantom_Skl_Diffusionsoul");
		break;
	case BLADEMASTER_FSM::_V_STD_DIFFUSIONSOUL_2:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Phantom_G3_Skl_Six_End");
		break;
	case BLADEMASTER_FSM::_V_HID_PIERCE1:		
	case BLADEMASTER_FSM::_V_HID_PIERCE2:		
	case BLADEMASTER_FSM::_V_HID_PIERCE3:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_hideblade_Skl_Pierce");
		break;
	case BLADEMASTER_FSM::_Q_COUNTERATTACK_Q:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_hideblade_SKL_counterattack_L");
		break;
	case BLADEMASTER_FSM::_E_COUNTERATTACK_E:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Standard_SKL_counterattack_R");
		break;
	case BLADEMASTER_FSM::_T_FULL_MOON_SLASH:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_hideblade_Skl_full_moon_slash_Exec");
		break;
	case BLADEMASTER_FSM::_T_ESCAPE:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Standard_SKL_escape");
		break;
	case BLADEMASTER_FSM::_T_ESCAPE_END:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Standard_SKL_escape_END");
		break;
	case BLADEMASTER_FSM::_L_STD_HACK:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Standard_SKL_Hack");
		break;
	case BLADEMASTER_FSM::_L_STD_HACK_END:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Standard_SKL_Hack_END");
		break;
	case BLADEMASTER_FSM::_L_STD_RUSHING_WIND_SPHERE:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Standard_SKL_rushing_wind_sphere");
		break;
	case BLADEMASTER_FSM::_L_STD_BODY_BLADE_UNITY_FIRE:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Standard_SKL_Body_Blade_Unity_Fire");
		break;
	case BLADEMASTER_FSM::_L_STD_BODY_BLADE_UNITY_EXEC:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Standard_SKL_Body_Blade_Unity_Exec");
		break;
	case BLADEMASTER_FSM::_L_STD_BODY_BLADE_SPRINT_FIRE:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Standard_SKL_Body_Blade_Unity_Sprint_Fire");
		break;
	case BLADEMASTER_FSM::_L_STD_BODY_BLADE_SPRINT_EXEC:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Standard_SKL_Body_Blade_Unity_Sprint_Exec");
		break;
	case BLADEMASTER_FSM::_L_STD_ASCENSION:
		Animator3D()->SetCurrentAnimClip(L"BM\\Skill_Ascension");
		break;
	case BLADEMASTER_FSM::_L_HID_BATTO:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_hideblade_SKL_Batto");
		break;
	case BLADEMASTER_FSM::_R_STD_RUSH_STAB_EXEC:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Standard_Skl_Rush_Stab_Exec");
		break;
	case BLADEMASTER_FSM::_R_STD_MIDAIR_EXEC_01:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_hideblade_SKL_Five_combo_slash_midair_Exec_01");
		break;
	case BLADEMASTER_FSM::_R_STD_MIDAIR_EXEC_02:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_hideblade_SKL_Five_combo_slash_midair_Exec_02");
		break;
	case BLADEMASTER_FSM::_R_STD_MIDAIR_EXEC_03:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_hideblade_SKL_Five_combo_slash_midair_Exec_03");
		break;
	case BLADEMASTER_FSM::_R_STD_MIDAIR_EXEC_04:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_hideblade_SKL_Five_combo_slash_midair_Exec_04");
		break;
	case BLADEMASTER_FSM::_R_STD_MIDAIR_EXEC_05:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_hideblade_SKL_Five_combo_slash_midair_Exec_05");
		break;
	case BLADEMASTER_FSM::_R_STD_CRIPPLING_CUT_FIRE:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Standard_Skl_Escape_Link");
		break;
	case BLADEMASTER_FSM::_R_STD_LIGHTNINGSMASH_FIRE:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Standard_SKL_LightningSmash_Fire");
		break;
	case BLADEMASTER_FSM::_R_STD_LIGHTNINGSMASH_EXEC:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Standard_SKL_LightningSmash_Exec");
		break;
	case BLADEMASTER_FSM::_R_HID_VITALSLASH_EXEC:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_hideblade_Skl_VitalSlash01_Exec");
		break;
	case BLADEMASTER_FSM::_F_STD_PHANTOMSLASH3_SWING:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Phantom_G3_Skl_PhantomSlash3_Swing");
		break;
	case BLADEMASTER_FSM::_F_STD_PHANTOMSLASH3_EXEC:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Phantom_G3_Skl_PhantomSlash3_Exec");
		break;
	case BLADEMASTER_FSM::_F_BACKWARDROLL:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Standard_Skl_BackwardRoll");
		break;
	case BLADEMASTER_FSM::_F_HID_BATTO_BLOCKING_START:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_hideblade_Skl_Batto_Blocking_Start");
		break;
	case BLADEMASTER_FSM::_F_HID_BATTO_BLOCKING:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_hideblade_Skl_Batto_Blocking");
		break;
	case BLADEMASTER_FSM::_Z_STD_SOULBLAST:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Phantom_Skl_RotateSoulBlade_Caster02");
		break;
	case BLADEMASTER_FSM::_Z_HID_SOULBLAST:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_hideblade_Skl_SoulBlast");
		break;
	case BLADEMASTER_FSM::_X_STD_GUARDSOUL_BLADE:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Phantom_Skl_GuardSoulBlade");
		break;
	case BLADEMASTER_FSM::_X_HID_THIRDBLADE_SLASH:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_hideblade_Skl_SoulStunLight");
		break;
	case BLADEMASTER_FSM::_C_STD_SOULSTUNLIGHT:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Phantom_Skl_GuardSoulBlade");
		break;
	case BLADEMASTER_FSM::_C_HID_SOULSTUNLIGHT:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_hideblade_Skl_SoulStunLight");
		break;
	case BLADEMASTER_FSM::_V_STD_ROTATESOULBLADE:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_Phantom_Skl_RotateSoulBlade_Caster");
		break;
	case BLADEMASTER_FSM::_V_HID_ROTATESOULBLADE:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_hideblade_Skl_RotateSoulBlade");
		break;
	case BLADEMASTER_FSM::_NON_MOV_IDLETOJUMP_FRONT:
		Animator3D()->SetCurrentAnimClip(L"BM\\BK_P_Standard_Mov_IdleToJump_Front");
		break;
	case BLADEMASTER_FSM::_NON_MOV_IDLETOJUMP_LEFT:
		Animator3D()->SetCurrentAnimClip(L"BM\\BK_P_Standard_Mov_IdleToJump_Left");
		break;
	case BLADEMASTER_FSM::_NON_MOV_IDLETOJUMP_RIGHT:
		Animator3D()->SetCurrentAnimClip(L"BM\\BK_P_Standard_Mov_IdleToJump_Right");
		break;
	case BLADEMASTER_FSM::_NON_MOV_JUMPFRONT:
		Animator3D()->SetCurrentAnimClip(L"BM\\BK_P_Standard_Mov_JumpFront");
		break;
	case BLADEMASTER_FSM::_NON_MOV_JUMPLEFT:
		Animator3D()->SetCurrentAnimClip(L"BM\\BK_P_Standard_Mov_JumpLeft");
		break;
	case BLADEMASTER_FSM::_NON_MOV_JUMPRIGHT:
		Animator3D()->SetCurrentAnimClip(L"BM\\BK_P_Standard_Mov_JumpRight");
		break;
	case BLADEMASTER_FSM::_NON_MOV_JUMPTOIDLE:
		Animator3D()->SetCurrentAnimClip(L"BM\\BK_P_Standard_Mov_JumpToIdle");
		break;
	case BLADEMASTER_FSM::_NON_MOV_JUMPTOMOVE_BACK:
		Animator3D()->SetCurrentAnimClip(L"BM\\BK_P_Standard_Mov_JumpToMove_Back");
		break;
	case BLADEMASTER_FSM::_NON_MOV_JUMPTOMOVE_FRONT:
		Animator3D()->SetCurrentAnimClip(L"BM\\BK_P_Standard_Mov_JumpToMove_Front");
		break;
	case BLADEMASTER_FSM::_NON_MOV_JUMPTOMOVE_LEFT:
		Animator3D()->SetCurrentAnimClip(L"BM\\BK_P_Standard_Mov_JumpToMove_Left");
		break;
	case BLADEMASTER_FSM::_NON_MOV_JUMPTOMOVE_RIGHT:
		Animator3D()->SetCurrentAnimClip(L"BM\\BK_P_Standard_Mov_JumpToMove_Right");
		break;
	case BLADEMASTER_FSM::_NON_MOV_LEFTTORIGHT:
		Animator3D()->SetCurrentAnimClip(L"BM\\BK_P_Standard_Mov_LeftToRight");
		break;
	case BLADEMASTER_FSM::_NON_MOV_MOVETOIDLE:
		Animator3D()->SetCurrentAnimClip(L"BM\\BK_P_Standard_Mov_MoveToIdle");
		break;
	case BLADEMASTER_FSM::_NON_MOV_RIGHTTOLEFT:
		Animator3D()->SetCurrentAnimClip(L"BM\\BK_P_Standard_Mov_RightToLeft");
		break;
	case BLADEMASTER_FSM::_NON_MOV_RUNBACK:
		Animator3D()->SetCurrentAnimClip(L"BM\\BK_P_Standard_Mov_RunBack");
		break;
	case BLADEMASTER_FSM::_NON_MOV_RUNFRONT:
		Animator3D()->SetCurrentAnimClip(L"BM\\BK_P_Standard_Mov_RunFront");
		break;
	case BLADEMASTER_FSM::_NON_MOV_RUNLEFT:
		Animator3D()->SetCurrentAnimClip(L"BM\\BK_P_Standard_Mov_RunLeft");
		break;
	case BLADEMASTER_FSM::_NON_MOV_RUNLEFTBACK:
		Animator3D()->SetCurrentAnimClip(L"BM\\BK_P_Standard_Mov_RunLeftBack");
		break;
	case BLADEMASTER_FSM::_NON_MOV_RUNRIGHT:
		Animator3D()->SetCurrentAnimClip(L"BM\\BK_P_Standard_Mov_RunRight");
		break;
	case BLADEMASTER_FSM::_NON_MOV_RUNRIGHTBACK:
		Animator3D()->SetCurrentAnimClip(L"BM\\BK_P_Standard_Mov_RunRightBack");
		break;
	case BLADEMASTER_FSM::_STD_MOV_IDLETOJUMP_FRONT:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_Standard_Mov_IdleToJump_Front");
		break;
	case BLADEMASTER_FSM::_STD_MOV_IDLETOJUMP_LEFT:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_Standard_Mov_IdleToJump_Left");
		break;
	case BLADEMASTER_FSM::_STD_MOV_IDLETOJUMP_RIGHT:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_Standard_Mov_IdleToJump_Right");
		break;
	case BLADEMASTER_FSM::_STD_MOV_JUMPFRONT:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_Standard_Mov_JumpFront");
		break;
	case BLADEMASTER_FSM::_STD_MOV_JUMPLEFT:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_Standard_Mov_JumpLeft");
		break;
	case BLADEMASTER_FSM::_STD_MOV_JUMPRIGHT:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_Standard_Mov_JumpRight");
		break;
	case BLADEMASTER_FSM::_STD_MOV_JUMPTOIDLE:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_Standard_Mov_JumpToIdle");
		break;
	case BLADEMASTER_FSM::_STD_MOV_JUMPTOMOVE_BACK:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_Standard_Mov_JumpToMove_Back");
		break;
	case BLADEMASTER_FSM::_STD_MOV_JUMPTOMOVE_FRONT:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_Standard_Mov_JumpToMove_Front");
		break;
	case BLADEMASTER_FSM::_STD_MOV_JUMPTOMOVE_LEFT:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_Standard_Mov_JumpToMove_Left");
		break;
	case BLADEMASTER_FSM::_STD_MOV_JUMPTOMOVE_RIGHT:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_Standard_Mov_JumpToMove_Right");
		break;
	case BLADEMASTER_FSM::_STD_MOV_LEFTTORIGHT:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_Standard_Mov_LeftToRight");
		break;
	case BLADEMASTER_FSM::_STD_MOV_MOVETOIDLE:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_Standard_Mov_MoveToIdle");
		break;
	case BLADEMASTER_FSM::_STD_MOV_RIGHTTOLEFT:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_Standard_Mov_RightToLeft");
		break;
	case BLADEMASTER_FSM::_STD_MOV_RUNBACK:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_Standard_Mov_RunBack");
		break;
	case BLADEMASTER_FSM::_STD_MOV_RUNFRONT:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_Standard_Mov_RunFront");
		break;
	case BLADEMASTER_FSM::_STD_MOV_RUNLEFT:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_Standard_Mov_RunLeft");
		break;
	case BLADEMASTER_FSM::_STD_MOV_RUNLEFTBACK:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_Standard_Mov_RunLeftBack");
		break;
	case BLADEMASTER_FSM::_STD_MOV_RUNRIGHT:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_Standard_Mov_RunRight");
		break;
	case BLADEMASTER_FSM::_STD_MOV_RUNRIGHTBACK:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_Standard_Mov_RunRightBack");
		break;
	case BLADEMASTER_FSM::_HID_MOV_IDLETOJUMP_FRONT:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_hideblade_Mov_IdleToJump_Front");
		break;
	case BLADEMASTER_FSM::_HID_MOV_IDLETOJUMP_LEFT:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_hideblade_Mov_IdleToJump_Left");
		break;
	case BLADEMASTER_FSM::_HID_MOV_IDLETOJUMP_RIGHT:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_hideblade_Mov_IdleToJump_Right");
		break;
	case BLADEMASTER_FSM::_HID_MOV_JUMPFRONT:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_hideblade_Mov_JumpFront");
		break;
	case BLADEMASTER_FSM::_HID_MOV_JUMPLEFT:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_hideblade_Mov_JumpLeft");
		break;
	case BLADEMASTER_FSM::_HID_MOV_JUMPRIGHT:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_hideblade_Mov_JumpRight");
		break;
	case BLADEMASTER_FSM::_HID_MOV_JUMPTOIDLE:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_hideblade_Mov_JumpToIdle");
		break;
	case BLADEMASTER_FSM::_HID_MOV_JUMPTOMOVE_BACK:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_hideblade_Mov_JumpToMove_Back");
		break;
	case BLADEMASTER_FSM::_HID_MOV_JUMPTOMOVE_FRONT:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_hideblade_Mov_JumpToMove_Front");
		break;
	case BLADEMASTER_FSM::_HID_MOV_JUMPTOMOVE_LEFT:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_hideblade_Mov_JumpToMove_Left");
		break;
	case BLADEMASTER_FSM::_HID_MOV_JUMPTOMOVE_RIGHT:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_hideblade_Mov_JumpToMove_Right");
		break;
	case BLADEMASTER_FSM::_HID_MOV_LEFTTORIGHT:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_hideblade_Mov_LeftToRight");
		break;
	case BLADEMASTER_FSM::_HID_MOV_MOVETOIDLE:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_hideblade_Mov_MoveToIdle");
		break;
	case BLADEMASTER_FSM::_HID_MOV_RIGHTTOLEFT:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_hideblade_Mov_RightToLeft");
		break;
	case BLADEMASTER_FSM::_HID_MOV_RUNBACK:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_hideblade_Mov_RunBack");
		break;
	case BLADEMASTER_FSM::_HID_MOV_RUNFRONT:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_hideblade_Mov_RunFront");
		break;
	case BLADEMASTER_FSM::_HID_MOV_RUNLEFT:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_hideblade_Mov_RunLeft");
		break;
	case BLADEMASTER_FSM::_HID_MOV_RUNLEFTBACK:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_hideblade_Mov_RunLeftBack");
		break;
	case BLADEMASTER_FSM::_HID_MOV_RUNRIGHT:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_hideblade_Mov_RunRight");
		break;
	case BLADEMASTER_FSM::_HID_MOV_RUNRIGHTBACK:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_hideblade_Mov_RunRightBack");
		break;
	case BLADEMASTER_FSM::_NON_MOV_IDLE:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_P_Standard_Mov_Idle");
		break;
	case BLADEMASTER_FSM::_STD_MOV_IDLE:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_Standard_Mov_Idle");
		break;
	case BLADEMASTER_FSM::_HID_SKL_IDLE:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_hideblade_Skl_Idle");
		break;
	case BLADEMASTER_FSM::_NON_EQUIPX:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_None_Bat_EquipX");
		break;
	case BLADEMASTER_FSM::_GLIDE_BOOST_FRONT:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Mov_Glide_Boost_Front");
		break;
	case BLADEMASTER_FSM::_GLIDE_BOOST_FRONT_CANCLE:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Mov_Glide_Boost_Front_Cancle");
		break;
	case BLADEMASTER_FSM::_GLIDE_BOOST_FRONT_START:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Mov_Glide_Boost_Front_Start");
		break;
	case BLADEMASTER_FSM::_GLIDE_DIVE_FRONT:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Mov_Glide_Dive_Front");
		break;
	case BLADEMASTER_FSM::_GLIDE_DIVE_FRONT_CANCLE:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Mov_Glide_Dive_Front_Cancle");
		break;
	case BLADEMASTER_FSM::_GLIDE_IDLETOJUMP_FRONT_01:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Mov_Glide_IdleToJump_Front_01");
		break;
	case BLADEMASTER_FSM::_GLIDE_JUMPFRONT:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Mov_Glide_JumpFront");
		break;
	case BLADEMASTER_FSM::_GLIDE_JUMPLEFT:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Mov_Glide_JumpLeft");
		break;
	case BLADEMASTER_FSM::_GLIDE_JUMPRIGHT:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Mov_Glide_JumpRight");
		break;
	case BLADEMASTER_FSM::_SPRINT_FRONT:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Mov_Sprint_Front");
		break;
	case BLADEMASTER_FSM::_SPRINT_FRONT_FALLSTART:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Mov_Sprint_Front_FallStart");
		break;
	case BLADEMASTER_FSM::_SPRINT_FRONT_ROCKETSTART:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Mov_Sprint_Front_RocketStart");
		break;
	case BLADEMASTER_FSM::_SPRINT_IDLETOJUMP_FRONT_01:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Mov_Sprint_IdleToJump_Front_01");
		break;
	case BLADEMASTER_FSM::_SPRINT_IDLETOJUMP_FRONT_02:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Mov_Sprint_IdleToJump_Front_02");
		break;
	case BLADEMASTER_FSM::_SPRINT_IDLETOJUMP_FRONT_03:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Mov_Sprint_IdleToJump_Front_03");
		break;
	case BLADEMASTER_FSM::_SPRINT_JUMPFRONT:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Mov_Sprint_JumpFront");
		break;
	case BLADEMASTER_FSM::_SPRINT_JUMPTOIDLE:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Mov_Sprint_JumpToIdle");
		break;
	case BLADEMASTER_FSM::_SPRINT_JUMPTOMOVE_FRONT:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Mov_Sprint_JumpToMove_Front");
		break;
	case BLADEMASTER_FSM::_SPRINT_MOVETOIDLE:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Mov_Sprint_MoveToIdle");
		break;
	case BLADEMASTER_FSM::_CAPTURE_EXEC2_FIRE:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Skl_Capture_Exec2_fire");
		break;
	case BLADEMASTER_FSM::_CAPTURE_EXEC2_END:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Skl_Capture_Exec2_End");
		break;
	case BLADEMASTER_FSM::_KNOCKBACK_LONG_B_START:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Bat_Knockback_Long_B_Start");
		break;
	case BLADEMASTER_FSM::_DOWN_FRONT_END:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Bat_Down_Front_End");
		break;
	case BLADEMASTER_FSM::_STUN_START:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Bat_Stun_Start");
		break;
	case BLADEMASTER_FSM::_STUN_LOOPING:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Bat_Stun_Looping");
		break;
	case BLADEMASTER_FSM::_GROGGY_F_START:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Bat_Knee_F_Start");
		break;
	case BLADEMASTER_FSM::_GROGGY_LOOPING:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Bat_Knee_Looping");
		break;
	case BLADEMASTER_FSM::_GROGGY_END:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Bat_Knee_End");
		break;
	case BLADEMASTER_FSM::_DOWN_MID_B_START:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Bat_Down_Mid_B_Start");
		break;
	case BLADEMASTER_FSM::_DOWN_LOW_B_R_START:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Bat_Down_Low_B_R_Start");
		break;
	case BLADEMASTER_FSM::_DOWN_LOOPING:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Bat_Down_Looping");
		break;
	case BLADEMASTER_FSM::_DOWN_END:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Bat_Down_End");
		break;
	case BLADEMASTER_FSM::_MIDAIR1_UPPER:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Bat_Midair1_upper");
		break;
	case BLADEMASTER_FSM::_UPPER_SLASH:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_StandardX_SKL_upper_slash");
		break;
	case BLADEMASTER_FSM::_MIDAIR2_UPPER:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Bat_Midair2_upper");
		break;
	case BLADEMASTER_FSM::_MIDAIR1_LOWER:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Bat_Midair1_lower");
		break;
	case BLADEMASTER_FSM::_SKYKICK:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_BighitX_Skl_skykick");
		break;
	case BLADEMASTER_FSM::_ASCENSION_ED:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_StandardX_SKL_ascension");
		break;
	case BLADEMASTER_FSM::_FLYTAKEDOWN:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_BighitX_Skl_Flytakedown");
		break;
	case BLADEMASTER_FSM::_MOUNTX_BAT_IDLE:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_MountX_Bat_idle");
		break;
	case BLADEMASTER_FSM::_MOUNTX_BAT_END:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_MountX_Bat_End");
		break;
	case BLADEMASTER_FSM::_MOUNTX_SKL_ARMBAR:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_MountX_Skl_armbar");
		break;
	case BLADEMASTER_FSM::_MOUNTX_SKL_POUNDING_FIRST:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_MountX_Skl_pounding_First");
		break;
	case BLADEMASTER_FSM::_MOUNTX_SKL_POUNDING:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_MountX_Skl_pounding");
		break;
	case BLADEMASTER_FSM::_DAMAGEBACK:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Bat_Damageback");
		break;
	case BLADEMASTER_FSM::_DAMAGEFRONT:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_None_Bat_Damagefront");
		break;
	case BLADEMASTER_FSM::_BE_DOWNCOUNTER:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_BighitX_Skl_Downcounter");
		break;
	case BLADEMASTER_FSM::_VIOLENCEPUNCH:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_BighitX_Skl_Violencepunch");
		break;
	case BLADEMASTER_FSM::_TRIPLEKICK:
		Animator3D()->SetCurrentAnimClip(L"BM\\AA_A_BighitX_Skl_triplekick");
		break;
	case BLADEMASTER_FSM::_BATTO_SKL_END:
		Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_hideblade_G1_Skl_FireDragonSpin_End");
		break;
	case BLADEMASTER_FSM::END:
		break;
	default:
		break;
	}
}

void CPlayerScript::SkillActive_Confirm()
{
	
}

void CPlayerScript::GetAIScript()
{
	if (CAIScript::AIObject && _AIScript == nullptr)
	{
		int ScriptSize = CAIScript::AIObject->GetScripts().size();

		for (size_t i = 0; i < ScriptSize; ++i)
		{
			if ((SCRIPT_TYPE)(CAIScript::AIObject->GetScripts()[i]->GetScriptType()) == SCRIPT_TYPE::AISCRIPT)
			{
				_AIScript = (CAIScript*)(CAIScript::AIObject->GetScripts()[i]);
			}
		}
	}
}

void CPlayerScript::awake()
{
	m_pOriginMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = m_pOriginMtrl->Clone();

	// 06/19  
// Transition 설정

	// 이동
	// 각 방향 IDLE -> 걷기
	// 각 방향 걷기 -> 걷기 중지
	// 각 방향 걷기 중지 -> IDLE

	// 스킬
	// 비전투 OR 검술세 IDLE ->  검술세 스킬
	// 검술세 스킬 -> 검술세 IDLE

	// 검술세 스킬 -> 발도세 IDLE
	// 발도세 스킬 -> 발도세 IDLE


//	// Idle - Front
//	Object()->Animator3D()->AddTransition<CPlayerScript>(L"IdleToFront", L"BM\\SW_P_Standard_Mov_Idle", this, &CPlayerScript::IdleToFront,
//		L"BM\\BK_P_Standard_Mov_RunFront");
//	
//	// Idle - Back
//	Object()->Animator3D()->AddTransition<CPlayerScript>(L"IdleToBack", L"BM\\SW_P_Standard_Mov_Idle", this, &CPlayerScript::IdleToBack,
//		L"BM\\BK_P_Standard_Mov_RunBack");
//	
//	// Run - Idle
//	Object()->Animator3D()->AddTransition<CPlayerScript>(L"RunToIdle", L"BM\\BK_P_Standard_Mov_RunFront", this, &CPlayerScript::RunToIdle,
//		L"BM\\SW_P_Standard_Mov_Idle");
//	Object()->Animator3D()->AddTransition<CPlayerScript>(L"RunToIdle", L"BM\\BK_P_Standard_Mov_RunBack", this, &CPlayerScript::RunToIdle,
//		L"BM\\SW_P_Standard_Mov_Idle");
//	
//	
//	// Idle - Attack
//	Object()->Animator3D()->AddTransition<CPlayerScript>(L"IdleToBasicAttack", L"BM\\SW_P_Standard_Mov_Idle", this, &CPlayerScript::IdleToAttack,
//		L"BM\\SW_A_Standard_SKL_rushing_wind_sphere");
//	
//	// BasicAttack - Attack_Idle
//	Object()->Animator3D()->AutoTransition(L"BasicAttack_To_FenceIdle", L"BM\\SW_A_Standard_SKL_rushing_wind_sphere", L"BM\\SW_B_Standard_Mov_Idle");
//	// Attack_Idle - Return
//	Object()->Animator3D()->AutoTransition(L"AttackIdleToReturn", L"BM\\SW_B_Standard_Mov_Idle", L"BM\\SW_A_None_Bat_EquipX");
//	// Return - Idle
//	Object()->Animator3D()->AutoTransition(L"ReturnToIdle", L"BM\\SW_A_None_Bat_EquipX", L"BM\\SW_P_Standard_Mov_Idle");
//	
//	//// 돌아올때 Notify
//	Object()->Animator3D()->AddTransitionNotify<CPlayerScript>(L"BM\\SW_A_Standard_SKL_rushing_wind_sphere", L"BasicAttack_To_FenceIdle", this, &CPlayerScript::BasicAttackEndNotify);
//	Object()->Animator3D()->AddTransitionNotify<CPlayerScript>(L"BM\\SW_B_Standard_Mov_Idle", L"AttackIdleToReturn", this, &CPlayerScript::FenceIdle_To_ReturnNotify);
//	Object()->Animator3D()->AddTransitionNotify<CPlayerScript>(L"BM\\SW_A_None_Bat_EquipX", L"ReturnToIdle", this, &CPlayerScript::ReturnToIdleNotify);
}


void CPlayerScript::update()
{
	CAMERA_MOD _Mod = CSceneMgr::GetInst()->GetCurScene()->_SceneCameraMOD;
	DefinePlayer();

	if (_Mod == CAMERA_MOD::CLIENT_MOD)
	{
		_AnimateFrame_Event();	// 현재(이전프레임(실행 적용된) Animation Event처리

		Player_SkillSetting();	// 현재 키 입력시 Event 처리

		SkillEndSetting_FSM();	// 현재 Animation 끝난 후 FSM 처리

		Player_MovingSetting();	// 현재 Dir 방향과 Skill 상호작용 후 FSM 처리

		SocketState_Confirm();	// Socket 무기 확인 Animation

		if (Prev_FSM != Player_FSM)
		{
			Animation_Setting_FSM();
			Changed_Prev_FSM = Changed_FSM;
			Changed_FSM = Prev_FSM;
		}
	}
	Prev_FSM = Player_FSM;
	
	Prev_Left = m_bLeft;
	Prev_Right = m_bRight;

	bool m_bAnimTest = true;
	if (m_bAnimTest)
	{
		if (KEY_TAB(KEY_TYPE::KEY_NUM7))
		{
			std::vector<struct CAnimClip*>::iterator iter = GetObj()->Animator3D()->GetVecAnimClip()->begin();
			std::vector<struct CAnimClip*>::iterator iterEnd = GetObj()->Animator3D()->GetVecAnimClip()->end();


			std::vector<struct CAnimClip*>* temp = GetObj()->Animator3D()->GetVecAnimClip();

			for (; iter != iterEnd; ++iter)
			{
				if ((*iter)->GetClipName() != GetObj()->Animator3D()->GetAnimClip()->GetClipName())
				{
					continue;
				}
				else
				{
					wstring CurAnimName = (*iter)->GetClipName();

					++iter;
					if (iter != iterEnd)
					{
						wstring NextAnimName = (*iter)->GetClipName();
						GetObj()->Animator3D()->SetCurrentAnimClip((*iter)->GetClipName().c_str());
					}
					else
					{
						break;
					}
				}
			}
		}

		if (KEY_TAB(KEY_TYPE::KEY_NUM8))
		{
			std::vector<struct CAnimClip*>::iterator iter = GetObj()->Animator3D()->GetVecAnimClip()->begin();
			std::vector<struct CAnimClip*>::iterator iterEnd = GetObj()->Animator3D()->GetVecAnimClip()->end();


			std::vector<struct CAnimClip*>* temp = GetObj()->Animator3D()->GetVecAnimClip();

			for (; iter != iterEnd; --iterEnd)
			{
				if (iterEnd == GetObj()->Animator3D()->GetVecAnimClip()->end())
				{
					--iterEnd;
				}
				if ((*iterEnd)->GetClipName() != GetObj()->Animator3D()->GetAnimClip()->GetClipName())
				{
					continue;
				}
				else
				{
					wstring CurAnimName = (*iterEnd)->GetClipName();

					--iterEnd;

					GetObj()->Animator3D()->SetCurrentAnimClip((*iterEnd)->GetClipName().c_str());
					if (iter == iterEnd)
					{
						break;
					}
				}
			}
		}

		if (KEY_TAB(KEY_TYPE::KEY_NUM9))
		{
			std::vector<struct CAnimClip*>::iterator iter = GetObj()->Animator3D()->GetVecAnimClip()->begin();
			std::vector<struct CAnimClip*>::iterator iterEnd = GetObj()->Animator3D()->GetVecAnimClip()->end();


			std::vector<struct CAnimClip*>* temp = GetObj()->Animator3D()->GetVecAnimClip();

			wstring CurAnimName = (*iter)->GetClipName();

			if (iter != iterEnd)
			{
				wstring NextAnimName = (*iter)->GetClipName();
				GetObj()->Animator3D()->SetCurrentAnimClip((*iter)->GetClipName().c_str());
			}
		}


		// TEST : 무기 바꾸기
		if (KEY_TAB(KEY_TYPE::KEY_NUM1))
		{
			SetSocket_hidEndHand();
			Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_hideblade_G1_Skl_FireDragonSpin_End");
		}
		if (KEY_TAB(KEY_TYPE::KEY_NUM2))
		{
			SetSocket_stdStance();
		}
		if (KEY_TAB(KEY_TYPE::KEY_NUM3))
		{
			SetSocket_hidStance();
			Animator3D()->SetCurrentAnimClip(L"BM\\SW_B_hideblade_Skl_Idle");
		}
	}

}
