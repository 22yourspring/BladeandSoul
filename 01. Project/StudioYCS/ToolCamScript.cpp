#include "pch.h"
#include "ToolCamScript.h"

#include <Engine/Camera.h>
#include <Engine/RenderMgr.h>
#include <Engine/Core.h>
#include "GameView.h"

#include <Script/PlayerScript.h>

float CToolCamScript::m_fSpeed = 0.f;
CToolCamScript::CToolCamScript()
	: CScript(0)
	, m_fScaleSpeed(1.f)
	, wKeyCheck(false)
	, aKeyCheck(false)
	, sKeyCheck(false)
	, dKeyCheck(false)
	, CamDIr(CAMERAMOVEDIR::END)
	, CameraFaster(false)
{
}

CToolCamScript::~CToolCamScript()
{
}

void CToolCamScript::SlidingCamera()
{
	if (KEY_HOLD(KEY_TYPE::KEY_LSHIFT) && (wKeyCheck || aKeyCheck || sKeyCheck || dKeyCheck))
	{
		//	m_fSpeed *= 3;
		//	CameraFaster = true;
	}
	else
	{
		CameraFaster = false;
	}

	if (KEY_AWAY(KEY_TYPE::KEY_LSHIFT))
	{
		CameraFaster = false;
	}

	if (wKeyCheck || aKeyCheck || sKeyCheck || dKeyCheck)
	{
		if (wKeyCheck && sKeyCheck && !aKeyCheck && !dKeyCheck)
		{
			m_fSpeed = 0.f;
		}
		if (!wKeyCheck && !sKeyCheck && aKeyCheck && dKeyCheck)
		{
			m_fSpeed = 0.f;
		}

		if (wKeyCheck && !sKeyCheck && !aKeyCheck && !dKeyCheck)
		{
			if (CamDIr == CAMERAMOVEDIR::KEY_S)
			{
				m_fSpeed = 0.f;
				CamDIr = CAMERAMOVEDIR::END;
			}
		}
		else if (!wKeyCheck && sKeyCheck && !aKeyCheck && !dKeyCheck)
		{
			if (CamDIr == CAMERAMOVEDIR::KEY_W)
			{
				m_fSpeed = 0.f;
				CamDIr = CAMERAMOVEDIR::END;
			}
		}
		else if (!wKeyCheck && !sKeyCheck && aKeyCheck && !dKeyCheck)
		{
			if (CamDIr == CAMERAMOVEDIR::KEY_A)
			{
				m_fSpeed = 0.f;
				CamDIr = CAMERAMOVEDIR::END;
			}
		}
		else if (!wKeyCheck && !sKeyCheck && !aKeyCheck && dKeyCheck)
		{
			if (CamDIr == CAMERAMOVEDIR::KEY_D)
			{
				m_fSpeed = 0.f;
				CamDIr = CAMERAMOVEDIR::END;
			}
		}

		if (!CameraFaster)
		{
			if (m_fSpeed < 4500.f)
			{
				m_fSpeed += 1500.f * DT;
			}
			else
			{
				m_fSpeed = 4500.f;
			}
		}
		else
		{
			if (m_fSpeed < 4500.f)
			{
				if (m_fSpeed < 1500.f)
				{
					m_fSpeed += 550.f * DT;
				}
				else
				{
					m_fSpeed += 750.f * DT;
				}
			}
			else
			{
				m_fSpeed = 4500.f;
			}
		}
	}

	// away check
	if (KEY_AWAY(KEY_TYPE::KEY_W))
	{
		CamDIr = CAMERAMOVEDIR::KEY_W;
		wKeyCheck = false;
	}
	if (KEY_AWAY(KEY_TYPE::KEY_A))
	{
		CamDIr = CAMERAMOVEDIR::KEY_A;
		aKeyCheck = false;
	}
	if (KEY_AWAY(KEY_TYPE::KEY_S))
	{
		CamDIr = CAMERAMOVEDIR::KEY_S;
		sKeyCheck = false;
	}
	if (KEY_AWAY(KEY_TYPE::KEY_D))
	{
		CamDIr = CAMERAMOVEDIR::KEY_D;
		dKeyCheck = false;
	}


	if (!wKeyCheck && !aKeyCheck && !sKeyCheck && !dKeyCheck)
	{
		if (CameraFaster)
		{
			CameraFaster = false;
			m_fSpeed = 1500.f;
		}

		if (m_fSpeed > 0.f)
		{
			if (m_fSpeed > 4000.f)
			{
				m_fSpeed -= 3000.f * DT;
			}
			else if (m_fSpeed > 3000.f)
			{
				m_fSpeed -= 2700.f * DT;
			}
			else if (m_fSpeed > 2000.f)
			{
				m_fSpeed -= 2200.f * DT;
			}
			else if (m_fSpeed > 1000.f)
			{
				m_fSpeed -= 1700.f * DT;
			}
			else
			{
				m_fSpeed -= 1200.f * DT;
			}

			switch (CamDIr)
			{
			case CAMERAMOVEDIR::KEY_W:
			{
				Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
				vPos += vFront * m_fSpeed * DT;
			}
			break;
			case CAMERAMOVEDIR::KEY_S:
			{
				Vec3 vBack = -Transform()->GetWorldDir(DIR_TYPE::FRONT);
				vPos += vBack * m_fSpeed * DT;
			}
			break;
			case CAMERAMOVEDIR::KEY_A:
			{
				Vec3 vLeft = -Transform()->GetWorldDir(DIR_TYPE::RIGHT);
				vPos += vLeft * m_fSpeed * DT;
			}
			break;
			case CAMERAMOVEDIR::KEY_D:
			{
				Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
				vPos += vRight * m_fSpeed * DT;
			}
			break;
			case CAMERAMOVEDIR::END:
				break;
			default:
				break;
			}
		}
		else
		{
			CamDIr = CAMERAMOVEDIR::END;
			m_fSpeed = 0.f;
		}
	}
}

void CToolCamScript::ExexceptionMouse()
{
	//抗寇贸府
	if (vRot.x * _RADIAN < -90.f)
	{
		vRot.x = -90.f * _DEGREE;
		Transform()->SetLocalRot(vRot);
		return;
	}
	else if (vRot.x * _RADIAN > 90.f)
	{
		vRot.x = 90.f * _DEGREE;
		Transform()->SetLocalRot(vRot);
		return;
	}
}

void CToolCamScript::update()
{
	vPos = Transform()->GetLocalPos();
	float fScale = Camera()->GetScale();

	CAMERA_MOD _Mod = CSceneMgr::GetInst()->GetCurScene()->_SceneCameraMOD;

	if (CGameView::ViewFocus && _Mod == CAMERA_MOD::ENGINE_MOD)
	{
		SlidingCamera();

		if (KEY_HOLD(KEY_TYPE::KEY_W))
		{
			wKeyCheck = true;
			Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
			vPos += vFront * m_fSpeed * DT;
		}

		if (KEY_HOLD(KEY_TYPE::KEY_A))
		{
			aKeyCheck = true;
			Vec3 vLeft = -Transform()->GetWorldDir(DIR_TYPE::RIGHT);
			vPos += vLeft * m_fSpeed * DT;
		}

		if (KEY_HOLD(KEY_TYPE::KEY_S))
		{
			sKeyCheck = true;
			Vec3 vBack = -Transform()->GetWorldDir(DIR_TYPE::FRONT);
			vPos += vBack * m_fSpeed * DT;
		}

		if (KEY_HOLD(KEY_TYPE::KEY_D))
		{
			dKeyCheck = true;
			Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
			vPos += vRight * m_fSpeed * DT;
		}

		//
		if (KEY_HOLD(KEY_TYPE::KEY_1))
		{
			fScale -= m_fScaleSpeed * DT;
			Camera()->SetScale(fScale);
		}

		if (KEY_HOLD(KEY_TYPE::KEY_2))
		{
			fScale += m_fScaleSpeed * DT;
			Camera()->SetScale(fScale);
		}

		if (KEY_HOLD(KEY_TYPE::KEY_RBTN))
		{
			Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
			vRot = Transform()->GetLocalRot();

			ExexceptionMouse();

			vRot.x -= vDrag.y * DT * 0.3f;
			vRot.y += vDrag.x * DT * 0.2f;

			//抗寇贸府
			if (vRot.x * _RADIAN > -90.f && vRot.x * _RADIAN < 90.f)
			{
				Transform()->SetLocalRot(vRot);
			}

			/*POINT MOUSEPT = { CRenderMgr::GetInst()->GetResolution().fWidth * 0.5f, CRenderMgr::GetInst()->GetResolution().fHeight * 0.5f };
			ClientToScreen(CCore::GetInst()->GetHwnd(), &MOUSEPT);
			SetCursorPos(MOUSEPT.x, MOUSEPT.y);*/
		}

		if (KEY_TAB(KEY_TYPE::KEY_ESC))
		{
			if (CPlayerScript::PlayerObject)
			{
				vPos = CPlayerScript::PlayerObject->Transform()->GetLocalPos();
				vPos.y += 90.f;
				vPos.z -= 180.f;
			}
			else
			{
				vPos = { 0.f, 1000.f, 0.f };
			}
			m_fSpeed = 0.f;
			CamDIr = CAMERAMOVEDIR::END;
		}

		Transform()->SetLocalPos(vPos);

		Transform()->_SetOrbitalTargetRot(Transform()->GetLocalRot());
	}
}
