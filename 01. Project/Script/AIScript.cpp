#include "stdafx.h"
#include "AIScript.h"

#include "TestScript.h"
#include "PlayerScript.h"

#include <Engine/Animator3D.h>

CGameObject* CAIScript::AIObject = nullptr;

CAIScript::CAIScript()
	: CScript((UINT)SCRIPT_TYPE::AISCRIPT)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
	, m_fSpeed(100.f)
	, m_bDash(false)
	, m_bGlide(false)
	, m_strClipName(nullptr)
	, AI_Position(KUNGFUMASTER_STATE::NON_COMBAT)
	, KFM_DIR(KUNGFUMASTER_DIR::FRONT)
	, Active_skill(false)
	, RedDragon_Fist(false)
	, Changed_FSM(KUNGFUMASTER_FSM::END)
	, Changed_Prev_FSM(KUNGFUMASTER_FSM::END)
	, AI_FSM(KUNGFUMASTER_FSM::_NON_MOV_IDLE)
	, RedDragonFistCount(0)
	, DragonFistCount(0)
	, m_bCounter(false)
	, m_bViolence_Active(false)
	, m_bTargetting(false)
	, m_bBackStep(false)
	, _Has_Resisted(false)
	, _Ready_resist(false)
	, _PlayerScript(nullptr)
	, _CC_Holdingtime(0.f)
	, _Counterattacking(0.f)
{
}

CAIScript::~CAIScript()
{
}

void CAIScript::AI_MovingSetting()
{

}

void CAIScript::AI_SkillSetting()
{
	return;
	// 1, 2, 3, 4
	if (KEY_TAB(KEY_TYPE::KEY_1))
	{
		if ((GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::NON_COMBAT || GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::COMBAT_STATE) && GetKungfuMaster_STATE() != KUNGFUMASTER_STATE::_CC_DOWN)
		{
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_1_COUNTERENEMY_READY);
		}
		else if (GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::_CC_DOWN)
		{
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_1_WINDMILL);
		}
		SetKungfuMaster_STATE(KUNGFUMASTER_STATE::COMBAT_STATE);
	}

	if (KEY_TAB(KEY_TYPE::KEY_2))	// 용권, 적룡권, 누반, 묵사발
	{
		if ((GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::NON_COMBAT || GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::COMBAT_STATE) && GetKungfuMaster_STATE() != KUNGFUMASTER_STATE::_CC_DOWN && GetKungfuMaster_STATE() != KUNGFUMASTER_STATE::_CC_MOUNT)
		{
			if (RedDragon_Fist == false)	// 용권
			{
				++DragonFistCount;
				switch (DragonFistCount)
				{
				case 1:
					SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_2_FIRE_PUNCH1_SWING);
					break;
				case 2:
					SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_2_FIRE_PUNCH2_SWING);
					break;
				case 3:
					SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_2_FIRE_PUNCH3_SWING);
					break;
				case 4:
					SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_2_FIRE_PUNCH4_SWING);
					break;
				default:
					break;
				}
				if (DragonFistCount > 4)
				{
					DragonFistCount = 0;
				}
			}
			else							// 적룡권
			{
				++RedDragonFistCount;
			}
		}
		else if (GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::_CC_DOWN)
		{
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_2_DOWNCOUNTER_READY);
		}
		else if (GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::_CC_MOUNT)
		{
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_2_POUNDING_FIRST);
		}
		SetKungfuMaster_STATE(KUNGFUMASTER_STATE::COMBAT_STATE);
	}

	if (KEY_TAB(KEY_TYPE::KEY_3))
	{
		if ((GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::NON_COMBAT || GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::COMBAT_STATE) && GetKungfuMaster_STATE() != KUNGFUMASTER_STATE::_CC_DOWN)
		{
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_3_LOWKICK);
		}
		SetKungfuMaster_STATE(KUNGFUMASTER_STATE::COMBAT_STATE);
	}

	if (KEY_TAB(KEY_TYPE::KEY_4))
	{
		if ((GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::NON_COMBAT || GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::COMBAT_STATE) && GetKungfuMaster_STATE() != KUNGFUMASTER_STATE::_CC_DOWN && GetKungfuMaster_STATE() != KUNGFUMASTER_STATE::_CC_MOUNT)
		{
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_4_HEAVYPUNCH_SWING);
		}
		else if ((GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::NON_COMBAT || GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::COMBAT_STATE) && GetKungfuMaster_STATE() != KUNGFUMASTER_STATE::_CC_DOWN && GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::_CC_MOUNT)
		{
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_4_ARMBAR);
		}
		SetKungfuMaster_STATE(KUNGFUMASTER_STATE::COMBAT_STATE);
	}
	//// Z, X, C, V
	if (KEY_TAB(KEY_TYPE::KEY_Z))
	{
		if (GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::NON_COMBAT || GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::COMBAT_STATE)
		{
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_Z_ASCENSION_KICK);
			SetKungfuMaster_STATE(KUNGFUMASTER_STATE::COMBAT_STATE);
		}
	}

	if (KEY_TAB(KEY_TYPE::KEY_X))
	{
		if (GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::NON_COMBAT || GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::COMBAT_STATE && m_bViolence_Active)
		{
			if (m_eDistance == DISTANCE::NEARBY)
			{
				SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_X_VIOLENCEPUNCH_CAST);
			}
		}
		else if (GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::NON_COMBAT || GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::COMBAT_STATE && !m_bViolence_Active)
		{
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_X_BURSTABDOMEN);
		}
		SetKungfuMaster_STATE(KUNGFUMASTER_STATE::COMBAT_STATE);
	}

	if (KEY_TAB(KEY_TYPE::KEY_C))
	{
		if (GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::NON_COMBAT || GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::COMBAT_STATE)
		{
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_C_MARTIALDANCE1_FIRE);
			SetKungfuMaster_STATE(KUNGFUMASTER_STATE::COMBAT_STATE);
		}
	}
	if (KEY_TAB(KEY_TYPE::KEY_V))
	{
		if (GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::NON_COMBAT || GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::COMBAT_STATE)
		{
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_V_CRUSHGROUND_SWING);
			SetKungfuMaster_STATE(KUNGFUMASTER_STATE::COMBAT_STATE);
		}
	}

	////tab
	if (KEY_TAB(KEY_TYPE::KEY_LTAB))	// Target 공중제압 해결해야 함
	{
		if (GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::NON_COMBAT || GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::COMBAT_STATE && !m_bTargetting)
		{
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_T_WIND_MODE_SWING);
		}
		else if (GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::NON_COMBAT || GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::COMBAT_STATE && m_bTargetting && _PlayerScript->GetBladeMaster_STATE() != BLADEMASTER_STATE::_CC_AIR)
		{
			if (m_eDistance == DISTANCE::MIDDLE)
			{
				SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_T_JUMPPUNCH_FIRE);
			}
		}
		else if (GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::NON_COMBAT || GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::COMBAT_STATE && m_bTargetting && _PlayerScript->GetBladeMaster_STATE() == BLADEMASTER_STATE::_CC_AIR)
		{
			if (m_eDistance == DISTANCE::NEARBY)
			{
				SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_T__FLYTAKEDOWN);
			}
		}
		else if (GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::_CC_DOWN || GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::_CC_GROGGY || GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::_CC_STUN)
		{
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_T_ESCAPE);
		}
		SetKungfuMaster_STATE(KUNGFUMASTER_STATE::COMBAT_STATE);
	}

	if (KEY_TAB(KEY_TYPE::KEY_CAP))
	{
		if (GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::NON_COMBAT || GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::COMBAT_STATE)
		{
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_3_DRAGON_FLY_EXEC);
		}
		SetKungfuMaster_STATE(KUNGFUMASTER_STATE::COMBAT_STATE);
	}

	//F
	if (KEY_TAB(KEY_TYPE::KEY_F)) // 미완성 강타 해야함
	{
		if (GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::COMBAT_STATE && m_bCounter)
		{
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_F_FINALATTACK);	// 멸산고 or 철산고
		}
		else if (GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::_CC_DOWN || GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::_CC_GROGGY)
		{
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_F_BACKWARDROLL);	// 뒤구르기
		}
		SetKungfuMaster_STATE(KUNGFUMASTER_STATE::COMBAT_STATE);

	}

	//Mouse
	if (KEY_TAB(KEY_TYPE::KEY_LBTN)) // 미완성
	{
		if (GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::NON_COMBAT || GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::COMBAT_STATE && m_bTargetting && !m_bDash)
		{
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_L_4HITKICK);
		}
		else if (GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::NON_COMBAT || GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::COMBAT_STATE && m_bTargetting && m_bDash)
		{
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_L_FLYING_DRAGONKICK_FIRE);
		}
		else if (GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::NON_COMBAT || GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::COMBAT_STATE && m_bTargetting && m_bBackStep)
		{
			m_bBackStep = false;
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_L_DRAGONKICK_FIRE);
		}
		SetKungfuMaster_STATE(KUNGFUMASTER_STATE::COMBAT_STATE);
	}
	if (KEY_TAB(KEY_TYPE::KEY_RBTN))	// 미완성
	{
		if (_PlayerScript->GetBladeMaster_STATE() == BLADEMASTER_STATE::NON_COMBAT || _PlayerScript->GetBladeMaster_STATE() == BLADEMASTER_STATE::BATTO_STATE || _PlayerScript->GetBladeMaster_STATE() == BLADEMASTER_STATE::FENCE_STATE)
		{
			if ((GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::NON_COMBAT || GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::COMBAT_STATE))
			{
				SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_R_DASHUPPER_FIRE);
			}
		}
		else if (_PlayerScript->GetBladeMaster_STATE() == BLADEMASTER_STATE::_CC_GROGGY || _PlayerScript->GetBladeMaster_STATE() == BLADEMASTER_STATE::_CC_STUN)
		{
			if ((GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::NON_COMBAT || GetKungfuMaster_STATE() == KUNGFUMASTER_STATE::COMBAT_STATE))
			{
				if (_Has_Resisted)
				{
					SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_R_TRIPLEKICK);
				}
				else
				{
					SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_R_UPPERCUT);
				}
			}			
		}
		else if (_PlayerScript->GetBladeMaster_STATE() == BLADEMASTER_STATE::_CC_AIR)
		{

		}
	}

	if (KEY_TAB(KEY_TYPE::KEY_Q))
	{
		if (m_eDistance == DISTANCE::NEARBY)
		{
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_Q_LEFT_SIDE_FIRE);
			SetKungfuMaster_STATE(KUNGFUMASTER_STATE::COMBAT_STATE);
		}
	}
	if (KEY_TAB(KEY_TYPE::KEY_E))
	{
		if (m_eDistance == DISTANCE::NEARBY)
		{
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_E_RIGHT_SIDE_FIRE);
			SetKungfuMaster_STATE(KUNGFUMASTER_STATE::COMBAT_STATE);
		}
	}

	if (KEY_TAB(KEY_TYPE::KEY_N))
	{
		SetKungfuMaster_STATE(KUNGFUMASTER_STATE::NON_COMBAT);
	}

	if (KEY_TAB(KEY_TYPE::KEY_L))
	{
		SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_STUN_START);
	}

	if (KEY_TAB(KEY_TYPE::KEY_K))
	{
		SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_FB_KNOCKBACK_LONG_B_START);
	}
}


void CAIScript::_Distance_calculating()
{
	if (CPlayerScript::PlayerObject && _PlayerScript)
	{
		Vec3 AIPos = Transform()->GetLocalPos();
		Vec3 PlayerPos = _PlayerScript->Transform()->GetLocalPos();

		m_fDistance = sqrt(pow(fabs(AIPos.x - PlayerPos.x), 2) + pow(fabs(AIPos.z - PlayerPos.z), 2));
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

void CAIScript::Animation_Setting_FSM()
{
	switch (AI_FSM)
	{
	case KUNGFUMASTER_FSM::_1_COUNTERENEMY_READY:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_counterenemy_ready");
		break;
	case KUNGFUMASTER_FSM::_1_COUNTERENEMY_STANDBY:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_counterenemy_standby");
		break;
	case KUNGFUMASTER_FSM::_1_COUNTERENEMY_FINISH:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_counterenemy_finish");
		break;
	case KUNGFUMASTER_FSM::_1_COUNTERENEMY_EXEC:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_Counterenemy_Exec");
		break;
	case KUNGFUMASTER_FSM::_1_WINDMILL:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_Windmill");
		break;
	case KUNGFUMASTER_FSM::_2_FIRE_PUNCH1_END:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_G2_Skl_fire_punch1_End");
		break;
	case KUNGFUMASTER_FSM::_2_FIRE_PUNCH1_SWING:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_G2_Skl_fire_punch1_Swing");
		break;
	case KUNGFUMASTER_FSM::_2_FIRE_PUNCH2_END:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_G2_Skl_fire_punch2_End");
		break;
	case KUNGFUMASTER_FSM::_2_FIRE_PUNCH2_SWING:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_G2_Skl_fire_punch2_Swing");
		break;
	case KUNGFUMASTER_FSM::_2_FIRE_PUNCH3_END:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_G2_Skl_fire_punch3_End");
		break;
	case KUNGFUMASTER_FSM::_2_FIRE_PUNCH3_SWING:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_G2_Skl_fire_punch3_Swing");
		break;
	case KUNGFUMASTER_FSM::_2_FIRE_PUNCH4_END:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_G2_Skl_fire_punch4_End");
		break;
	case KUNGFUMASTER_FSM::_2_FIRE_PUNCH4_SWING:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_G2_Skl_fire_punch4_Swing");
		break;
	case KUNGFUMASTER_FSM::_2_DOWNCOUNTER_READY:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_Downcounter_ready");
		break;
	case KUNGFUMASTER_FSM::_2_DOWNCOUNTER_STANDBY:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_Downcounter_standby");
		break;
	case KUNGFUMASTER_FSM::_2_DOWNCOUNTER_EXEC:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_Downcounter_Exec");
		break;
	case KUNGFUMASTER_FSM::_2_DOWNCOUNTER_FINISH:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_Downcounter_finish");
		break;
	case KUNGFUMASTER_FSM::_2_1000HAND_SWING1:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_G1_Skl_1000hand_Swing1");
		break;
	case KUNGFUMASTER_FSM::_2_1000HAND_EXEC1:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_G1_Skl_1000hand_Exec1");
		break;
	case KUNGFUMASTER_FSM::_2_1000HAND_FINISH1:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_G1_Skl_1000hand_Finish1");
		break;
	case KUNGFUMASTER_FSM::_2_1000HAND_SWING2:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_G1_Skl_1000hand_Swing2");
		break;
	case KUNGFUMASTER_FSM::_2_1000HAND_EXEC2:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_G1_Skl_1000hand_Exec2");
		break;
	case KUNGFUMASTER_FSM::_2_1000HAND_FINISH2:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_G1_Skl_1000hand_Finish2");
		break;
	case KUNGFUMASTER_FSM::_2_WIND_KICK1_SWING:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_G1_Skl_wind_kick1_Swing");
		break;
	case KUNGFUMASTER_FSM::_2_WIND_KICK1_EXEC:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_G1_Skl_wind_kick1_Exec");
		break;
	case KUNGFUMASTER_FSM::_2_WIND_KICK1_END:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_G1_Skl_wind_kick1_End");
		break;
	case KUNGFUMASTER_FSM::_2_POUNDING_FIRST:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_MountX_Skl_pounding_First");
		break;
	case KUNGFUMASTER_FSM::_3_DRAGON_FLY_EXEC:
		Animator3D()->SetCurrentAnimClip(L"KFM\\Bk_A_Bighit_Skl_Dragon_Fly_Exec");
		break;
	case KUNGFUMASTER_FSM::_3_LOWKICK:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_lowkick");
		break;
	case KUNGFUMASTER_FSM::_4_HEAVYPUNCH_SWING:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_Heavypunch_Swing");
		break;
	case KUNGFUMASTER_FSM::_4_ICEBLAST_KICK:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_bighit_Skl_IceBlast_Kick");
		break;
	case KUNGFUMASTER_FSM::_4_ARMBAR:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_MountX_Skl_armbar");
		break;
	case KUNGFUMASTER_FSM::_Z_ASCENSION_KICK:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_escape");
		break;
	case KUNGFUMASTER_FSM::_X_BURSTABDOMEN:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_bighit_Skl_BurstAbdomen");
		break;
	case KUNGFUMASTER_FSM::_X_VIOLENCEPUNCH_CAST:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_Violencepunch_Cast");
		break;
	case KUNGFUMASTER_FSM::_X_VIOLENCEPUNCH_EXEC:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_Violencepunch_Exec");
		break;
	case KUNGFUMASTER_FSM::_C_MARTIALDANCE1_EXEC:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_Martialdance1_Exec");
		break;
	case KUNGFUMASTER_FSM::_C_MARTIALDANCE1_FIRE:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_Martialdance1_Fire");
		break;
	case KUNGFUMASTER_FSM::_C_MARTIALDANCE2_EXEC:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_Martialdance2_Exec");
		break;
	case KUNGFUMASTER_FSM::_C_MARTIALDANCE2_FIRE:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_Martialdance2_Fire");
		break;
	case KUNGFUMASTER_FSM::_C_MARTIALDANCE3_EXEC:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_Martialdance3_Exec");
		break;
	case KUNGFUMASTER_FSM::_C_MARTIALDANCE3_FIRE:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_Martialdance3_Fire");
		break;
	case KUNGFUMASTER_FSM::_C_MARTIALDANCE4_EXEC:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_Martialdance4_Exec");
		break;
	case KUNGFUMASTER_FSM::_C_MARTIALDANCE4_FIRE:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_Martialdance4_Fire");
		break;
	case KUNGFUMASTER_FSM::_V_CRUSHGROUND_SWING:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Combo_Skl_crushground_Swing");
		break;
	case KUNGFUMASTER_FSM::_Q_LEFT_SIDE_FIRE:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_Command_Left_Side_Fire");
		break;
	case KUNGFUMASTER_FSM::_Q_LEFT_SIDE_EXEC:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_Command_Left_Side_Exec");
		break;
	case KUNGFUMASTER_FSM::_E_RIGHT_SIDE_FIRE:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_Command_Right_Side_Fire");
		break;
	case KUNGFUMASTER_FSM::_E_RIGHT_SIDE_EXEC:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_Command_Right_Side_Exec");
		break;
	case KUNGFUMASTER_FSM::_T_ESCAPE:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_escape");
		break;
	case KUNGFUMASTER_FSM::_T_JUMPPUNCH_FIRE:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_Jumppunch_Fire");
		break;
	case KUNGFUMASTER_FSM::_T_JUMPPUNCH_EXEC:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_Jumppunch_Exec");
		break;
	case KUNGFUMASTER_FSM::_T_WIND_MODE_SWING:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_G1_Skl_wind_mode_Swing");
		break;
	case KUNGFUMASTER_FSM::_T_WIND_MODE_EXEC:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_G1_Skl_wind_mode_Exec");
		break;
	case KUNGFUMASTER_FSM::_T_WIND_MODE_END:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_G1_Skl_wind_mode_End");
		break;
	case KUNGFUMASTER_FSM::_T__FLYTAKEDOWN:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_Flytakedown");
		break;
	case KUNGFUMASTER_FSM::_L_DRAGONKICK_FIRE:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_DragonKick_Fire");
		break;
	case KUNGFUMASTER_FSM::_L_DRAGONKICK_EXEC:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_DragonKick_Exec");
		break;
	case KUNGFUMASTER_FSM::_L_FLYING_DRAGONKICK_FIRE:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_DragonKick_Fire");
		break;
	case KUNGFUMASTER_FSM::_L_FLYING_DRAGONKICK_EXEC:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_DragonKick_Exec");
		break;
	case KUNGFUMASTER_FSM::_L_4HITKICK:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_bighit_Skl_4hitkick");
		break;
	case KUNGFUMASTER_FSM::_L_SKYKICK:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_skykick");
		break;
	case KUNGFUMASTER_FSM::_R_UPPERCUT:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_uppercut");
		break;
	case KUNGFUMASTER_FSM::_R_UPPERCUT2:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_uppercut2");
		break;
	case KUNGFUMASTER_FSM::_R_DASHUPPER_FIRE:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_bighit_Skl_DashUpper_Fire");
		break;
	case KUNGFUMASTER_FSM::_R_DASHUPPER_EXEC:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_bighit_Skl_DashUpper_Exec");
		break;
	case KUNGFUMASTER_FSM::_R_LEFTELBOW:
		Animator3D()->SetCurrentAnimClip(L"KFM\\Bk_A_Combo_Skl_LeftElbow");
		break;
	case KUNGFUMASTER_FSM::_R_TRIPLEKICK:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Bighit_Skl_triplekick");
		break;
	case KUNGFUMASTER_FSM::_F_ABDOMENATTACK_FIRE:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_bighit_Skl_AbdomenAttack_Fire");
		break;
	case KUNGFUMASTER_FSM::_F_ABDOMENATTACK_EXEC:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_bighit_Skl_AbdomenAttack_Exec");
		break;
	case KUNGFUMASTER_FSM::_F_FINALATTACK:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Combo_Skl_Finalattack");
		break;
	case KUNGFUMASTER_FSM::_F_BACKWARDROLL:
		Animator3D()->SetCurrentAnimClip(L"KFM\\TA_A_Protection_Skl_BackwardRoll");
		break;
	case KUNGFUMASTER_FSM::_S_BACKSTEP:
		Animator3D()->SetCurrentAnimClip(L"KFM\\Bk_A_Bighit_Skl_Backstep");
		break;
	case KUNGFUMASTER_FSM::_MOUNT_BAT_IDLE:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Mount_Bat_idle");
		break;
	case KUNGFUMASTER_FSM::_MOUNT_BAT_END:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_A_Mount_Bat_End");
		break;
	case KUNGFUMASTER_FSM::_FB_CAPTURE_EXEC2_FIRE:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Skl_Capture_Exec2_fire");
		break;
	case KUNGFUMASTER_FSM::_FB_CAPTURE_EXEC2_END:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Skl_Capture_Exec2_End");
		break;
	case KUNGFUMASTER_FSM::_FB_KNOCKBACK_LONG_B_START:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Bat_Knockback_Long_B_Start");
		break;
	case KUNGFUMASTER_FSM::_FB_DOWN_FRONT_LOOPING:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Bat_Down_Front_Looping");
		break;
	case KUNGFUMASTER_FSM::_FB_DOWN_FRONT_END:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Bat_Down_Front_End");
		break;
	case KUNGFUMASTER_FSM::_STUN_START:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Bat_Stun_Start");
		break;
	case KUNGFUMASTER_FSM::_STUN_LOOPING:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Bat_Stun_Looping");
		break;
	case KUNGFUMASTER_FSM::_GROGGY_KNEE_F_START:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Bat_Knee_F_Start");
		break;
	case KUNGFUMASTER_FSM::_GROGGY_KNEE_LOOPING:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Bat_Knee_Looping");
		break;
	case KUNGFUMASTER_FSM::_GROGGY_KNEE_END:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Bat_Knee_End");
		break;
	case KUNGFUMASTER_FSM::_DOWN_MID_B_START:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Bat_Down_Mid_B_Start");
		break;
	case KUNGFUMASTER_FSM::_DOWN_LOW_B_R_START:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Bat_Down_Low_B_R_Start");
		break;
	case KUNGFUMASTER_FSM::_DOWN_LOOPING:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Bat_Down_Looping");
		break;
	case KUNGFUMASTER_FSM::_DOWN_END:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Bat_Down_End");
		break;
	case KUNGFUMASTER_FSM::_AIR_MIDAIR1_UPPER:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Bat_Midair1_upper");
		break;
	case KUNGFUMASTER_FSM::_AIR_UPPER_SLASH:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_StandardX_SKL_upper_slash");
		break;
	case KUNGFUMASTER_FSM::_AIR_MIDAIR2_UPPER:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Bat_Midair2_upper");
		break;
	case KUNGFUMASTER_FSM::_AIR_MIDAIR1_LOWER:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Bat_Midair1_lower");
		break;
	case KUNGFUMASTER_FSM::_AIR_BIGHITX_SKL_SKYKICK:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_BighitX_Skl_skykick");
		break;
	case KUNGFUMASTER_FSM::_AIR_STANDARDX_SKL_CRASH:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_StandardX_Skl_crash");
		break;
	case KUNGFUMASTER_FSM::_FB_ASCENSION:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_StandardX_SKL_ascension");
		break;
	case KUNGFUMASTER_FSM::_DAMAGEBACK:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Bat_Damageback");
		break;
	case KUNGFUMASTER_FSM::_DAMAGEFRONT:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Bat_Damagefront");
		break;
	case KUNGFUMASTER_FSM::_NON_MOV_IDLE:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_P_None_Mov_Idle");
		break;
	case KUNGFUMASTER_FSM::_BAT_MOV_IDLE:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_B_Bighit_Mov_idle");
		break;
	case KUNGFUMASTER_FSM::_BAT_EQUIPX:
		Animator3D()->SetCurrentAnimClip(L"KFM\\_BAT_EQUIPX");
		break;
	case KUNGFUMASTER_FSM::_COMBAT_JUMPTOIDLE:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_B_Bighit_Mov_JumpToIdle");
		break;
	case KUNGFUMASTER_FSM::_COMBAT_MOVETOIDLE:
		Animator3D()->SetCurrentAnimClip(L"KFM\\Bk_B_Bighit_Mov_MoveToIdle");
		break;
	case KUNGFUMASTER_FSM::_BAT_IDLETOJUMP_FRONT:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_B_Bighit_Mov_IdleToJump_Front");
		break;
	case KUNGFUMASTER_FSM::_NON_IDLETOJUMP_FRONT:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_P_None_Mov_IdleToJump_Front");
		break;
	case KUNGFUMASTER_FSM::_IDLETOJUMP_LEFT:
		Animator3D()->SetCurrentAnimClip(L"KFM\\Bk_B_Bighit_Mov_IdleToJump_Left");
		break;
	case KUNGFUMASTER_FSM::_IDLETOJUMP_RIGHT:
		Animator3D()->SetCurrentAnimClip(L"KFM\\Bk_B_Bighit_Mov_IdleToJump_Right");
		break;
	case KUNGFUMASTER_FSM::_JUMPFRONT:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_B_Bighit_Mov_JumpFront");
		break;
	case KUNGFUMASTER_FSM::_JUMPLEFT:
		Animator3D()->SetCurrentAnimClip(L"KFM\\Bk_B_Bighit_Mov_JumpLeft");
		break;
	case KUNGFUMASTER_FSM::_JUMPRIGHT:
		Animator3D()->SetCurrentAnimClip(L"KFM\\Bk_B_Bighit_Mov_JumpRight");
		break;
	case KUNGFUMASTER_FSM::_NONCOMBAT_JUMPTOIDLE:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_P_None_Mov_JumpToIdle");
		break;
	case KUNGFUMASTER_FSM::_JUMPTOMOVE_BACK:
		Animator3D()->SetCurrentAnimClip(L"KFM\\Bk_B_Bighit_Mov_JumpToMove_Back");
		break;
	case KUNGFUMASTER_FSM::_JUMPTOMOVE_FRONT:
		Animator3D()->SetCurrentAnimClip(L"KFM\\Bk_B_Bighit_Mov_JumpToMove_Front");
		break;
	case KUNGFUMASTER_FSM::_JUMPTOMOVE_LEFT:
		Animator3D()->SetCurrentAnimClip(L"KFM\\Bk_B_Bighit_Mov_JumpToMove_Left");
		break;
	case KUNGFUMASTER_FSM::_JUMPTOMOVE_RIGHT:
		Animator3D()->SetCurrentAnimClip(L"KFM\\Bk_B_Bighit_Mov_JumpToMove_Right");
		break;
	case KUNGFUMASTER_FSM::_LEFTTORIGHT:
		Animator3D()->SetCurrentAnimClip(L"KFM\\Bk_P_Bighit_Mov_LeftToRight");
		break;
	case KUNGFUMASTER_FSM::_NONCOMBAT_MOVETOIDLE:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_P_None_Mov_MoveToIdle");
		break;
	case KUNGFUMASTER_FSM::_RIGHTTOLEFT:
		Animator3D()->SetCurrentAnimClip(L"KFM\\Bk_P_Bighit_Mov_RightToLeft");
		break;
	case KUNGFUMASTER_FSM::_RUNBACK:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_B_Bighit_Mov_RunBack");
		break;
	case KUNGFUMASTER_FSM::_RUNFRONT:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_B_Bighit_Mov_RunFront");
		break;
	case KUNGFUMASTER_FSM::_RUNLEFT:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_B_Bighit_Mov_RunLeft");
		break;
	case KUNGFUMASTER_FSM::_RUNLEFTBACK:
		Animator3D()->SetCurrentAnimClip(L"KFM\\Bk_B_Bighit_Mov_RunLeftBack");
		break;
	case KUNGFUMASTER_FSM::_RUNRIGHT:
		Animator3D()->SetCurrentAnimClip(L"KFM\\BK_B_Bighit_Mov_RunRight");
		break;
	case KUNGFUMASTER_FSM::_RUNRIGHTBACK:
		Animator3D()->SetCurrentAnimClip(L"KFM\\Bk_B_Bighit_Mov_RunRightBack");
		break;
	case KUNGFUMASTER_FSM::_GLIDE_BOOST_FRONT:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Mov_Glide_Boost_Front");
		break;
	case KUNGFUMASTER_FSM::_GLIDE_BOOST_FRONT_CANCLE:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Mov_Glide_Boost_Front_Cancle");
		break;
	case KUNGFUMASTER_FSM::_GLIDE_BOOST_FRONT_START:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Mov_Glide_Boost_Front_Start");
		break;
	case KUNGFUMASTER_FSM::_GLIDE_DIVE_FRONT:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Mov_Glide_Dive_Front");
		break;
	case KUNGFUMASTER_FSM::_GLIDE_DIVE_FRONT_CANCLE:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Mov_Glide_Dive_Front_Cancle");
		break;
	case KUNGFUMASTER_FSM::_GLIDE_IDLETOJUMP_FRONT_01:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Mov_Glide_IdleToJump_Front_01");
		break;
	case KUNGFUMASTER_FSM::_GLIDE_JUMPFRONT:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Mov_Glide_JumpFront");
		break;
	case KUNGFUMASTER_FSM::_GLIDE_JUMPLEFT:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Mov_Glide_JumpLeft");
		break;
	case KUNGFUMASTER_FSM::_GLIDE_JUMPRIGHT:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Mov_Glide_JumpRight");
		break;
	case KUNGFUMASTER_FSM::_SPRINT_FRONT:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Mov_Sprint_Front");
		break;
	case KUNGFUMASTER_FSM::_SPRINT_FRONT_FALLSTART:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Mov_Sprint_Front_FallStart");
		break;
	case KUNGFUMASTER_FSM::_SPRINT_FRONT_ROCKETSTART:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Mov_Sprint_Front_RocketStart");
		break;
	case KUNGFUMASTER_FSM::_SPRINT_IDLETOJUMP_FRONT_01:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Mov_Sprint_IdleToJump_Front_01");
		break;
	case KUNGFUMASTER_FSM::_SPRINT_IDLETOJUMP_FRONT_02:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Mov_Sprint_IdleToJump_Front_02");
		break;
	case KUNGFUMASTER_FSM::_SPRINT_IDLETOJUMP_FRONT_03:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Mov_Sprint_IdleToJump_Front_03");
		break;
	case KUNGFUMASTER_FSM::_SPRINT_JUMPFRONT:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Mov_Sprint_JumpFront");
		break;
	case KUNGFUMASTER_FSM::_SPRINT_JUMPTOIDLE:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Mov_Sprint_JumpToIdle");
		break;
	case KUNGFUMASTER_FSM::_SPRINT_JUMPTOMOVE_FRONT:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Mov_Sprint_JumpToMove_Front");
		break;
	case KUNGFUMASTER_FSM::_SPRINT_MOVETOIDLE:
		Animator3D()->SetCurrentAnimClip(L"KFM\\AA_A_None_Mov_Sprint_MoveToIdle");
		break;
	case KUNGFUMASTER_FSM::END:
		break;
	default:
		break;
	}
}

void CAIScript::GetPlayerScript()
{
	if (CPlayerScript::PlayerObject && _PlayerScript == nullptr)
	{		
		int ScriptSize = CPlayerScript::PlayerObject->GetScripts().size();
		
		for (size_t i = 0; i < ScriptSize; ++i)
		{
			if ((SCRIPT_TYPE)(CPlayerScript::PlayerObject->GetScripts()[i]->GetScriptType()) == SCRIPT_TYPE::PLAYERSCRIPT)
			{
				_PlayerScript = (CPlayerScript*)(CPlayerScript::PlayerObject->GetScripts()[i]);
			}			
		}
	}
}

void CAIScript::awake()
{
}

void CAIScript::update()
{
	DefineAI();

	_AnimateFrame_Event();				 // 현재 Animation Event처리

	AI_SkillSetting();					 // 현재 키 입력시 Event 처리

	SkillEndSetting_FSM();				 // 현재 Animation 끝난 후 FSM 처리

	AI_MovingSetting();					 // 현재 Dir 방향과 Skill 상호작용 후 FSM 처리

	if (Prev_FSM != AI_FSM)				
	{
		Animation_Setting_FSM();
		Changed_Prev_FSM = Changed_FSM;
		Changed_FSM = Prev_FSM;
	}
	Prev_FSM = AI_FSM;

	if (true)
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
	}
}

void CAIScript::DefineAI()
{
	if (AIObject == nullptr)
	{
		AIObject = Object();
	}
	GetPlayerScript();
	_Distance_calculating();
}

const wstring & CAIScript::GetCurPlayingAnimationName()
{
	// TODO: 여기에 반환 구문을 삽입합니다.
	return (Object()->Animator3D()->GetAnimClip()->GetClipName());
}

void CAIScript::SkillEndSetting_FSM()
{
	if (Animator3D()->AnimIsFinish())
	{
		switch (AI_FSM)
		{
		case KUNGFUMASTER_FSM::_1_COUNTERENEMY_READY:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_1_COUNTERENEMY_STANDBY);
			_Counterattacking = true;
			break;
		case KUNGFUMASTER_FSM::_1_COUNTERENEMY_STANDBY:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_1_COUNTERENEMY_FINISH);
			_Counterattacking = true;
			break;
		case KUNGFUMASTER_FSM::_1_COUNTERENEMY_FINISH:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_BAT_MOV_IDLE);
			_Counterattacking = false;
			break;
		case KUNGFUMASTER_FSM::_1_COUNTERENEMY_EXEC:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_BAT_MOV_IDLE);
			_Counterattacking = false;
			m_bCounter = true;
			break;
		case KUNGFUMASTER_FSM::_1_WINDMILL:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_BAT_MOV_IDLE);
			break;
		case KUNGFUMASTER_FSM::_2_FIRE_PUNCH1_END:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_BAT_MOV_IDLE);
			break;
		case KUNGFUMASTER_FSM::_2_FIRE_PUNCH1_SWING:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_2_FIRE_PUNCH1_END);
			break;
		case KUNGFUMASTER_FSM::_2_FIRE_PUNCH2_END:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_BAT_MOV_IDLE);
			break;
		case KUNGFUMASTER_FSM::_2_FIRE_PUNCH2_SWING:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_2_FIRE_PUNCH2_END);
			break;
		case KUNGFUMASTER_FSM::_2_FIRE_PUNCH3_END:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_BAT_MOV_IDLE);
			break;
		case KUNGFUMASTER_FSM::_2_FIRE_PUNCH3_SWING:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_2_FIRE_PUNCH3_END);
			break;
		case KUNGFUMASTER_FSM::_2_FIRE_PUNCH4_END:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_BAT_MOV_IDLE);
			break;
		case KUNGFUMASTER_FSM::_2_FIRE_PUNCH4_SWING:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_2_FIRE_PUNCH4_END);
			break;
		case KUNGFUMASTER_FSM::_2_DOWNCOUNTER_READY:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_2_DOWNCOUNTER_STANDBY);
			break;
		case KUNGFUMASTER_FSM::_2_DOWNCOUNTER_STANDBY:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_2_DOWNCOUNTER_FINISH);
			break;
		case KUNGFUMASTER_FSM::_2_DOWNCOUNTER_EXEC:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_BAT_MOV_IDLE);
			break;
		case KUNGFUMASTER_FSM::_2_DOWNCOUNTER_FINISH:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_DOWN_END);
			break;
		case KUNGFUMASTER_FSM::_2_1000HAND_SWING1:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_2_1000HAND_EXEC1);
			break;
		case KUNGFUMASTER_FSM::_2_1000HAND_EXEC1:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_2_1000HAND_FINISH1);
			break;
		case KUNGFUMASTER_FSM::_2_1000HAND_FINISH1:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_BAT_MOV_IDLE);
			break;
		case KUNGFUMASTER_FSM::_2_1000HAND_SWING2:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_2_1000HAND_EXEC2);
			break;
		case KUNGFUMASTER_FSM::_2_1000HAND_EXEC2:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_2_1000HAND_FINISH2);
			break;
		case KUNGFUMASTER_FSM::_2_1000HAND_FINISH2:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_BAT_MOV_IDLE);
			break;
		case KUNGFUMASTER_FSM::_2_WIND_KICK1_SWING:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_2_WIND_KICK1_EXEC);
			break;
		case KUNGFUMASTER_FSM::_2_WIND_KICK1_EXEC:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_2_WIND_KICK1_END);
			break;
		case KUNGFUMASTER_FSM::_2_WIND_KICK1_END:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_BAT_MOV_IDLE);
			break;
		case KUNGFUMASTER_FSM::_2_POUNDING_FIRST:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_MOUNT_BAT_END);
			break;
		case KUNGFUMASTER_FSM::_3_DRAGON_FLY_EXEC:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_BAT_MOV_IDLE);
			break;
		case KUNGFUMASTER_FSM::_3_LOWKICK:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_BAT_MOV_IDLE);
			break;
		case KUNGFUMASTER_FSM::_4_HEAVYPUNCH_SWING:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_BAT_MOV_IDLE);
			break;
		case KUNGFUMASTER_FSM::_4_ICEBLAST_KICK:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_BAT_MOV_IDLE);
			break;
		case KUNGFUMASTER_FSM::_4_ARMBAR:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_BAT_MOV_IDLE);
			break;
		case KUNGFUMASTER_FSM::_Z_ASCENSION_KICK:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_BAT_MOV_IDLE);
			break;
		case KUNGFUMASTER_FSM::_X_BURSTABDOMEN:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_BAT_MOV_IDLE);
			break;
		case KUNGFUMASTER_FSM::_X_VIOLENCEPUNCH_CAST:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_X_VIOLENCEPUNCH_EXEC);
			break;
		case KUNGFUMASTER_FSM::_X_VIOLENCEPUNCH_EXEC:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_BAT_MOV_IDLE);
			break;
		case KUNGFUMASTER_FSM::_C_MARTIALDANCE1_EXEC:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_C_MARTIALDANCE2_FIRE);
			break;
		case KUNGFUMASTER_FSM::_C_MARTIALDANCE1_FIRE:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_C_MARTIALDANCE1_EXEC);
			break;
		case KUNGFUMASTER_FSM::_C_MARTIALDANCE2_EXEC:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_C_MARTIALDANCE3_FIRE);
			break;
		case KUNGFUMASTER_FSM::_C_MARTIALDANCE2_FIRE:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_C_MARTIALDANCE2_EXEC);
			break;
		case KUNGFUMASTER_FSM::_C_MARTIALDANCE3_EXEC:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_C_MARTIALDANCE4_FIRE);
			break;
		case KUNGFUMASTER_FSM::_C_MARTIALDANCE3_FIRE:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_C_MARTIALDANCE3_EXEC);
			break;
		case KUNGFUMASTER_FSM::_C_MARTIALDANCE4_EXEC:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_BAT_MOV_IDLE);
			break;
		case KUNGFUMASTER_FSM::_C_MARTIALDANCE4_FIRE:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_C_MARTIALDANCE4_EXEC);
			break;
		case KUNGFUMASTER_FSM::_V_CRUSHGROUND_SWING:
			break;
		case KUNGFUMASTER_FSM::_Q_LEFT_SIDE_FIRE:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_Q_LEFT_SIDE_EXEC);
			break;
		case KUNGFUMASTER_FSM::_Q_LEFT_SIDE_EXEC:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_BAT_MOV_IDLE);
			break;
		case KUNGFUMASTER_FSM::_E_RIGHT_SIDE_FIRE:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_E_RIGHT_SIDE_EXEC);
			break;
		case KUNGFUMASTER_FSM::_E_RIGHT_SIDE_EXEC:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_BAT_MOV_IDLE);
			break;
		case KUNGFUMASTER_FSM::_T_ESCAPE:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_BAT_MOV_IDLE);
			break;
		case KUNGFUMASTER_FSM::_T_JUMPPUNCH_FIRE:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_T_JUMPPUNCH_EXEC);
			break;
		case KUNGFUMASTER_FSM::_T_JUMPPUNCH_EXEC:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_BAT_MOV_IDLE);
			break;
		case KUNGFUMASTER_FSM::_T_WIND_MODE_SWING:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_T_WIND_MODE_EXEC);
			break;
		case KUNGFUMASTER_FSM::_T_WIND_MODE_EXEC:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_T_WIND_MODE_END);
			break;
		case KUNGFUMASTER_FSM::_T_WIND_MODE_END:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_BAT_MOV_IDLE);
			break;
		case KUNGFUMASTER_FSM::_T__FLYTAKEDOWN:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_MOUNT_BAT_IDLE);
			break;
		case KUNGFUMASTER_FSM::_L_DRAGONKICK_FIRE:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_L_DRAGONKICK_EXEC);
			break;
		case KUNGFUMASTER_FSM::_L_DRAGONKICK_EXEC:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_MOUNT_BAT_IDLE);
			break;
		case KUNGFUMASTER_FSM::_L_FLYING_DRAGONKICK_FIRE:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_L_FLYING_DRAGONKICK_EXEC);
			break;
		case KUNGFUMASTER_FSM::_L_FLYING_DRAGONKICK_EXEC:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_MOUNT_BAT_IDLE);
			break;
		case KUNGFUMASTER_FSM::_L_4HITKICK:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_MOUNT_BAT_IDLE);
			break;
		case KUNGFUMASTER_FSM::_L_SKYKICK:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_MOUNT_BAT_IDLE);
			break;
		case KUNGFUMASTER_FSM::_R_UPPERCUT:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_MOUNT_BAT_IDLE);
			break;
		case KUNGFUMASTER_FSM::_R_UPPERCUT2:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_MOUNT_BAT_IDLE);
			break;
		case KUNGFUMASTER_FSM::_R_DASHUPPER_FIRE:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_MOUNT_BAT_IDLE);
			break;
		case KUNGFUMASTER_FSM::_R_DASHUPPER_EXEC:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_MOUNT_BAT_IDLE);
			break;
		case KUNGFUMASTER_FSM::_R_LEFTELBOW:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_MOUNT_BAT_IDLE);
			break;
		case KUNGFUMASTER_FSM::_R_TRIPLEKICK:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_MOUNT_BAT_IDLE);
			break;
		case KUNGFUMASTER_FSM::_F_ABDOMENATTACK_FIRE:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_F_ABDOMENATTACK_EXEC);
			break;
		case KUNGFUMASTER_FSM::_F_ABDOMENATTACK_EXEC:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_MOUNT_BAT_IDLE);
			break;
		case KUNGFUMASTER_FSM::_F_FINALATTACK:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_MOUNT_BAT_IDLE);
			m_bCounter = false;
			break;
		case KUNGFUMASTER_FSM::_F_BACKWARDROLL:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_MOUNT_BAT_IDLE);
			break;
		case KUNGFUMASTER_FSM::_S_BACKSTEP:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_MOUNT_BAT_IDLE);
			m_bBackStep = true;
			break;
		case KUNGFUMASTER_FSM::_MOUNT_BAT_IDLE:
			break;
		case KUNGFUMASTER_FSM::_MOUNT_BAT_END:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_MOUNT_BAT_IDLE);
			break;
		case KUNGFUMASTER_FSM::_FB_CAPTURE_EXEC2_FIRE:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_FB_CAPTURE_EXEC2_END);
			break;
		case KUNGFUMASTER_FSM::_FB_CAPTURE_EXEC2_END:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_STUN_START);
			break;
		case KUNGFUMASTER_FSM::_FB_KNOCKBACK_LONG_B_START:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_FB_DOWN_FRONT_LOOPING); //넉백 다운 이벤트 처리 고민
			break;
		case KUNGFUMASTER_FSM::_FB_DOWN_FRONT_LOOPING:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_FB_DOWN_FRONT_END);
			break;
		case KUNGFUMASTER_FSM::_FB_DOWN_FRONT_END:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_BAT_MOV_IDLE);
			break;
		case KUNGFUMASTER_FSM::_STUN_START:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_STUN_LOOPING);
			break;
		case KUNGFUMASTER_FSM::_STUN_LOOPING:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_COMBAT_MOVETOIDLE);
			break;
		case KUNGFUMASTER_FSM::_GROGGY_KNEE_F_START:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_GROGGY_KNEE_LOOPING);
			break;
		case KUNGFUMASTER_FSM::_GROGGY_KNEE_LOOPING:
			break;
		case KUNGFUMASTER_FSM::_GROGGY_KNEE_END:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_BAT_MOV_IDLE);
			break;
		case KUNGFUMASTER_FSM::_DOWN_MID_B_START:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_DOWN_LOOPING);
			break;
		case KUNGFUMASTER_FSM::_DOWN_LOW_B_R_START:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_DOWN_LOOPING);
			break;
		case KUNGFUMASTER_FSM::_DOWN_LOOPING:
			break;
		case KUNGFUMASTER_FSM::_DOWN_END:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_BAT_MOV_IDLE);
			break;
		case KUNGFUMASTER_FSM::_AIR_MIDAIR1_UPPER:
			break;
		case KUNGFUMASTER_FSM::_AIR_UPPER_SLASH:
			break;
		case KUNGFUMASTER_FSM::_AIR_MIDAIR2_UPPER:
			break;
		case KUNGFUMASTER_FSM::_AIR_MIDAIR1_LOWER:
			break;
		case KUNGFUMASTER_FSM::_AIR_BIGHITX_SKL_SKYKICK:
			break;
		case KUNGFUMASTER_FSM::_AIR_STANDARDX_SKL_CRASH:
			break;
		case KUNGFUMASTER_FSM::_FB_ASCENSION:
			break;
		case KUNGFUMASTER_FSM::_DAMAGEBACK:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_BAT_MOV_IDLE);
			break;
		case KUNGFUMASTER_FSM::_DAMAGEFRONT:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_BAT_MOV_IDLE);
			break;
		case KUNGFUMASTER_FSM::_NON_MOV_IDLE:
			break;
		case KUNGFUMASTER_FSM::_BAT_MOV_IDLE:
			break;
		case KUNGFUMASTER_FSM::_BAT_EQUIPX:
			break;
		case KUNGFUMASTER_FSM::_COMBAT_JUMPTOIDLE:
			break;
		case KUNGFUMASTER_FSM::_COMBAT_MOVETOIDLE:
			SetKungfuMaster_FSM(KUNGFUMASTER_FSM::_BAT_MOV_IDLE);
			break;
		case KUNGFUMASTER_FSM::_BAT_IDLETOJUMP_FRONT:
			break;
		case KUNGFUMASTER_FSM::_NON_IDLETOJUMP_FRONT:
			break;
		case KUNGFUMASTER_FSM::_IDLETOJUMP_LEFT:
			break;
		case KUNGFUMASTER_FSM::_IDLETOJUMP_RIGHT:
			break;
		case KUNGFUMASTER_FSM::_JUMPFRONT:
			break;
		case KUNGFUMASTER_FSM::_JUMPLEFT:
			break;
		case KUNGFUMASTER_FSM::_JUMPRIGHT:
			break;
		case KUNGFUMASTER_FSM::_NONCOMBAT_JUMPTOIDLE:
			break;
		case KUNGFUMASTER_FSM::_JUMPTOMOVE_BACK:
			break;
		case KUNGFUMASTER_FSM::_JUMPTOMOVE_FRONT:
			break;
		case KUNGFUMASTER_FSM::_JUMPTOMOVE_LEFT:
			break;
		case KUNGFUMASTER_FSM::_JUMPTOMOVE_RIGHT:
			break;
		case KUNGFUMASTER_FSM::_LEFTTORIGHT:
			break;
		case KUNGFUMASTER_FSM::_NONCOMBAT_MOVETOIDLE:
			break;
		case KUNGFUMASTER_FSM::_RIGHTTOLEFT:
			break;
		case KUNGFUMASTER_FSM::_RUNBACK:
			break;
		case KUNGFUMASTER_FSM::_RUNFRONT:
			break;
		case KUNGFUMASTER_FSM::_RUNLEFT:
			break;
		case KUNGFUMASTER_FSM::_RUNLEFTBACK:
			break;
		case KUNGFUMASTER_FSM::_RUNRIGHT:
			break;
		case KUNGFUMASTER_FSM::_RUNRIGHTBACK:
			break;
		case KUNGFUMASTER_FSM::_GLIDE_BOOST_FRONT:
			break;
		case KUNGFUMASTER_FSM::_GLIDE_BOOST_FRONT_CANCLE:
			break;
		case KUNGFUMASTER_FSM::_GLIDE_BOOST_FRONT_START:
			break;
		case KUNGFUMASTER_FSM::_GLIDE_DIVE_FRONT:
			break;
		case KUNGFUMASTER_FSM::_GLIDE_DIVE_FRONT_CANCLE:
			break;
		case KUNGFUMASTER_FSM::_GLIDE_IDLETOJUMP_FRONT_01:
			break;
		case KUNGFUMASTER_FSM::_GLIDE_JUMPFRONT:
			break;
		case KUNGFUMASTER_FSM::_GLIDE_JUMPLEFT:
			break;
		case KUNGFUMASTER_FSM::_GLIDE_JUMPRIGHT:
			break;
		case KUNGFUMASTER_FSM::_SPRINT_FRONT:
			break;
		case KUNGFUMASTER_FSM::_SPRINT_FRONT_FALLSTART:
			break;
		case KUNGFUMASTER_FSM::_SPRINT_FRONT_ROCKETSTART:
			break;
		case KUNGFUMASTER_FSM::_SPRINT_IDLETOJUMP_FRONT_01:
			break;
		case KUNGFUMASTER_FSM::_SPRINT_IDLETOJUMP_FRONT_02:
			break;
		case KUNGFUMASTER_FSM::_SPRINT_IDLETOJUMP_FRONT_03:
			break;
		case KUNGFUMASTER_FSM::_SPRINT_JUMPFRONT:
			break;
		case KUNGFUMASTER_FSM::_SPRINT_JUMPTOIDLE:
			break;
		case KUNGFUMASTER_FSM::_SPRINT_JUMPTOMOVE_FRONT:
			break;
		case KUNGFUMASTER_FSM::_SPRINT_MOVETOIDLE:
			break;
		case KUNGFUMASTER_FSM::END:
			break;
		default:
			break;
		}
	}
}

void CAIScript::_AnimateFrame_Event()
{
	switch (AI_FSM)
	{
	case KUNGFUMASTER_FSM::_1_COUNTERENEMY_READY:
		_Counterattacking = true;
		break;
	case KUNGFUMASTER_FSM::_1_COUNTERENEMY_STANDBY:
		_Counterattacking = true;
		break;
	case KUNGFUMASTER_FSM::_1_COUNTERENEMY_FINISH:
		_Counterattacking = true;
		break;
	case KUNGFUMASTER_FSM::_1_COUNTERENEMY_EXEC:
		_Counterattacking = false;
		break;
	case KUNGFUMASTER_FSM::_1_WINDMILL:
		break;
	case KUNGFUMASTER_FSM::_2_FIRE_PUNCH1_END:
		break;
	case KUNGFUMASTER_FSM::_2_FIRE_PUNCH1_SWING:
		break;
	case KUNGFUMASTER_FSM::_2_FIRE_PUNCH2_END:
		break;
	case KUNGFUMASTER_FSM::_2_FIRE_PUNCH2_SWING:
		break;
	case KUNGFUMASTER_FSM::_2_FIRE_PUNCH3_END:
		break;
	case KUNGFUMASTER_FSM::_2_FIRE_PUNCH3_SWING:
		break;
	case KUNGFUMASTER_FSM::_2_FIRE_PUNCH4_END:
		break;
	case KUNGFUMASTER_FSM::_2_FIRE_PUNCH4_SWING:
		break;
	case KUNGFUMASTER_FSM::_2_DOWNCOUNTER_READY:
		break;
	case KUNGFUMASTER_FSM::_2_DOWNCOUNTER_STANDBY:
		break;
	case KUNGFUMASTER_FSM::_2_DOWNCOUNTER_EXEC:
		break;
	case KUNGFUMASTER_FSM::_2_DOWNCOUNTER_FINISH:
		break;
	case KUNGFUMASTER_FSM::_2_1000HAND_SWING1:
		break;
	case KUNGFUMASTER_FSM::_2_1000HAND_EXEC1:
		break;
	case KUNGFUMASTER_FSM::_2_1000HAND_FINISH1:
		break;
	case KUNGFUMASTER_FSM::_2_1000HAND_SWING2:
		break;
	case KUNGFUMASTER_FSM::_2_1000HAND_EXEC2:
		break;
	case KUNGFUMASTER_FSM::_2_1000HAND_FINISH2:
		break;
	case KUNGFUMASTER_FSM::_2_WIND_KICK1_SWING:
		break;
	case KUNGFUMASTER_FSM::_2_WIND_KICK1_EXEC:
		break;
	case KUNGFUMASTER_FSM::_2_WIND_KICK1_END:
		break;
	case KUNGFUMASTER_FSM::_2_POUNDING_FIRST:
		break;
	case KUNGFUMASTER_FSM::_3_DRAGON_FLY_EXEC:
		break;
	case KUNGFUMASTER_FSM::_3_LOWKICK:
		break;
	case KUNGFUMASTER_FSM::_4_HEAVYPUNCH_SWING:
		break;
	case KUNGFUMASTER_FSM::_4_ICEBLAST_KICK:
		break;
	case KUNGFUMASTER_FSM::_4_ARMBAR:
		break;
	case KUNGFUMASTER_FSM::_Z_ASCENSION_KICK:
		break;
	case KUNGFUMASTER_FSM::_X_BURSTABDOMEN:
		break;
	case KUNGFUMASTER_FSM::_X_VIOLENCEPUNCH_CAST:
		break;
	case KUNGFUMASTER_FSM::_X_VIOLENCEPUNCH_EXEC:
		break;
	case KUNGFUMASTER_FSM::_C_MARTIALDANCE1_EXEC:
		break;
	case KUNGFUMASTER_FSM::_C_MARTIALDANCE1_FIRE:
		break;
	case KUNGFUMASTER_FSM::_C_MARTIALDANCE2_EXEC:
		break;
	case KUNGFUMASTER_FSM::_C_MARTIALDANCE2_FIRE:
		break;
	case KUNGFUMASTER_FSM::_C_MARTIALDANCE3_EXEC:
		break;
	case KUNGFUMASTER_FSM::_C_MARTIALDANCE3_FIRE:
		break;
	case KUNGFUMASTER_FSM::_C_MARTIALDANCE4_EXEC:
		break;
	case KUNGFUMASTER_FSM::_C_MARTIALDANCE4_FIRE:
		break;
	case KUNGFUMASTER_FSM::_V_CRUSHGROUND_SWING:
		break;
	case KUNGFUMASTER_FSM::_Q_LEFT_SIDE_FIRE:
		break;
	case KUNGFUMASTER_FSM::_Q_LEFT_SIDE_EXEC:
		break;
	case KUNGFUMASTER_FSM::_E_RIGHT_SIDE_FIRE:
		break;
	case KUNGFUMASTER_FSM::_E_RIGHT_SIDE_EXEC:
		break;
	case KUNGFUMASTER_FSM::_T_ESCAPE:
		break;
	case KUNGFUMASTER_FSM::_T_JUMPPUNCH_FIRE:
		break;
	case KUNGFUMASTER_FSM::_T_JUMPPUNCH_EXEC:
		break;
	case KUNGFUMASTER_FSM::_T_WIND_MODE_SWING:
		break;
	case KUNGFUMASTER_FSM::_T_WIND_MODE_EXEC:
		break;
	case KUNGFUMASTER_FSM::_T_WIND_MODE_END:
		break;
	case KUNGFUMASTER_FSM::_T__FLYTAKEDOWN:
		break;
	case KUNGFUMASTER_FSM::_L_DRAGONKICK_FIRE:
		break;
	case KUNGFUMASTER_FSM::_L_DRAGONKICK_EXEC:
		break;
	case KUNGFUMASTER_FSM::_L_FLYING_DRAGONKICK_FIRE:
		break;
	case KUNGFUMASTER_FSM::_L_FLYING_DRAGONKICK_EXEC:
		break;
	case KUNGFUMASTER_FSM::_L_4HITKICK:
		break;
	case KUNGFUMASTER_FSM::_L_SKYKICK:
		break;
	case KUNGFUMASTER_FSM::_R_UPPERCUT:
		break;
	case KUNGFUMASTER_FSM::_R_UPPERCUT2:
		break;
	case KUNGFUMASTER_FSM::_R_DASHUPPER_FIRE:
		break;
	case KUNGFUMASTER_FSM::_R_DASHUPPER_EXEC:
		break;
	case KUNGFUMASTER_FSM::_R_LEFTELBOW:
		break;
	case KUNGFUMASTER_FSM::_R_TRIPLEKICK:
		break;
	case KUNGFUMASTER_FSM::_F_ABDOMENATTACK_FIRE:
		break;
	case KUNGFUMASTER_FSM::_F_ABDOMENATTACK_EXEC:
		break;
	case KUNGFUMASTER_FSM::_F_FINALATTACK:
		break;
	case KUNGFUMASTER_FSM::_F_BACKWARDROLL:
		break;
	case KUNGFUMASTER_FSM::_S_BACKSTEP:
		break;
	case KUNGFUMASTER_FSM::_MOUNT_BAT_IDLE:
		break;
	case KUNGFUMASTER_FSM::_MOUNT_BAT_END:
		break;
	case KUNGFUMASTER_FSM::_FB_CAPTURE_EXEC2_FIRE:
		break;
	case KUNGFUMASTER_FSM::_FB_CAPTURE_EXEC2_END:
		break;
	case KUNGFUMASTER_FSM::_FB_KNOCKBACK_LONG_B_START:
		break;
	case KUNGFUMASTER_FSM::_FB_DOWN_FRONT_LOOPING:
		break;
	case KUNGFUMASTER_FSM::_FB_DOWN_FRONT_END:
		break;
	case KUNGFUMASTER_FSM::_STUN_START:
		break;
	case KUNGFUMASTER_FSM::_STUN_LOOPING:
		break;
	case KUNGFUMASTER_FSM::_GROGGY_KNEE_F_START:
		break;
	case KUNGFUMASTER_FSM::_GROGGY_KNEE_LOOPING:
		break;
	case KUNGFUMASTER_FSM::_GROGGY_KNEE_END:
		break;
	case KUNGFUMASTER_FSM::_DOWN_MID_B_START:
		break;
	case KUNGFUMASTER_FSM::_DOWN_LOW_B_R_START:
		break;
	case KUNGFUMASTER_FSM::_DOWN_LOOPING:
		break;
	case KUNGFUMASTER_FSM::_DOWN_END:
		break;
	case KUNGFUMASTER_FSM::_AIR_MIDAIR1_UPPER:
		break;
	case KUNGFUMASTER_FSM::_AIR_UPPER_SLASH:
		break;
	case KUNGFUMASTER_FSM::_AIR_MIDAIR2_UPPER:
		break;
	case KUNGFUMASTER_FSM::_AIR_MIDAIR1_LOWER:
		break;
	case KUNGFUMASTER_FSM::_AIR_BIGHITX_SKL_SKYKICK:
		break;
	case KUNGFUMASTER_FSM::_AIR_STANDARDX_SKL_CRASH:
		break;
	case KUNGFUMASTER_FSM::_FB_ASCENSION:
		break;
	case KUNGFUMASTER_FSM::_DAMAGEBACK:
		break;
	case KUNGFUMASTER_FSM::_DAMAGEFRONT:
		break;
	case KUNGFUMASTER_FSM::_NON_MOV_IDLE:
		break;
	case KUNGFUMASTER_FSM::_BAT_MOV_IDLE:
		break;
	case KUNGFUMASTER_FSM::_BAT_EQUIPX:
		break;
	case KUNGFUMASTER_FSM::_COMBAT_JUMPTOIDLE:
		break;
	case KUNGFUMASTER_FSM::_COMBAT_MOVETOIDLE:
		break;
	case KUNGFUMASTER_FSM::_BAT_IDLETOJUMP_FRONT:
		break;
	case KUNGFUMASTER_FSM::_NON_IDLETOJUMP_FRONT:
		break;
	case KUNGFUMASTER_FSM::_IDLETOJUMP_LEFT:
		break;
	case KUNGFUMASTER_FSM::_IDLETOJUMP_RIGHT:
		break;
	case KUNGFUMASTER_FSM::_JUMPFRONT:
		break;
	case KUNGFUMASTER_FSM::_JUMPLEFT:
		break;
	case KUNGFUMASTER_FSM::_JUMPRIGHT:
		break;
	case KUNGFUMASTER_FSM::_NONCOMBAT_JUMPTOIDLE:
		break;
	case KUNGFUMASTER_FSM::_JUMPTOMOVE_BACK:
		break;
	case KUNGFUMASTER_FSM::_JUMPTOMOVE_FRONT:
		break;
	case KUNGFUMASTER_FSM::_JUMPTOMOVE_LEFT:
		break;
	case KUNGFUMASTER_FSM::_JUMPTOMOVE_RIGHT:
		break;
	case KUNGFUMASTER_FSM::_LEFTTORIGHT:
		break;
	case KUNGFUMASTER_FSM::_NONCOMBAT_MOVETOIDLE:
		break;
	case KUNGFUMASTER_FSM::_RIGHTTOLEFT:
		break;
	case KUNGFUMASTER_FSM::_RUNBACK:
		break;
	case KUNGFUMASTER_FSM::_RUNFRONT:
		break;
	case KUNGFUMASTER_FSM::_RUNLEFT:
		break;
	case KUNGFUMASTER_FSM::_RUNLEFTBACK:
		break;
	case KUNGFUMASTER_FSM::_RUNRIGHT:
		break;
	case KUNGFUMASTER_FSM::_RUNRIGHTBACK:
		break;
	case KUNGFUMASTER_FSM::_GLIDE_BOOST_FRONT:
		break;
	case KUNGFUMASTER_FSM::_GLIDE_BOOST_FRONT_CANCLE:
		break;
	case KUNGFUMASTER_FSM::_GLIDE_BOOST_FRONT_START:
		break;
	case KUNGFUMASTER_FSM::_GLIDE_DIVE_FRONT:
		break;
	case KUNGFUMASTER_FSM::_GLIDE_DIVE_FRONT_CANCLE:
		break;
	case KUNGFUMASTER_FSM::_GLIDE_IDLETOJUMP_FRONT_01:
		break;
	case KUNGFUMASTER_FSM::_GLIDE_JUMPFRONT:
		break;
	case KUNGFUMASTER_FSM::_GLIDE_JUMPLEFT:
		break;
	case KUNGFUMASTER_FSM::_GLIDE_JUMPRIGHT:
		break;
	case KUNGFUMASTER_FSM::_SPRINT_FRONT:
		break;
	case KUNGFUMASTER_FSM::_SPRINT_FRONT_FALLSTART:
		break;
	case KUNGFUMASTER_FSM::_SPRINT_FRONT_ROCKETSTART:
		break;
	case KUNGFUMASTER_FSM::_SPRINT_IDLETOJUMP_FRONT_01:
		break;
	case KUNGFUMASTER_FSM::_SPRINT_IDLETOJUMP_FRONT_02:
		break;
	case KUNGFUMASTER_FSM::_SPRINT_IDLETOJUMP_FRONT_03:
		break;
	case KUNGFUMASTER_FSM::_SPRINT_JUMPFRONT:
		break;
	case KUNGFUMASTER_FSM::_SPRINT_JUMPTOIDLE:
		break;
	case KUNGFUMASTER_FSM::_SPRINT_JUMPTOMOVE_FRONT:
		break;
	case KUNGFUMASTER_FSM::_SPRINT_MOVETOIDLE:
		break;
	case KUNGFUMASTER_FSM::END:
		break;
	default:
		break;
	}
}
