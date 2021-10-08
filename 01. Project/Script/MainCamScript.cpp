#include "stdafx.h"
#include "MainCamScript.h"

#include <Engine/Camera.h>
#include <Engine/RenderMgr.h>
#include <Engine/Core.h>

#include "PlayerScript.h"

float CMainCamScript::m_fSpeed = 0.f;
CMainCamScript::CMainCamScript()
	: CScript((UINT)SCRIPT_TYPE::MAINCAMSCRIPT)
	, m_fScaleSpeed(1.f)
	, wKeyCheck(false)
	, aKeyCheck(false)
	, sKeyCheck(false)
	, dKeyCheck(false)
	, CamDIr(MainCAMERAMOVEDIR::END)
	, CameraFaster(false)
{
}

CMainCamScript::~CMainCamScript()
{
}


void CMainCamScript::update()
{
	if (CPlayerScript::PlayerObject && CSceneMgr::GetInst()->GetCurScene()->_GetUpdate())
	{
		Vec3 PlayerPos = CPlayerScript::PlayerObject->Transform()->GetLocalPos();
		Vec3 PlayerRot = CPlayerScript::PlayerObject->Transform()->GetLocalRot();

		vPos = Transform()->GetLocalPos();
		
//		vPos.y = 50.f;
//		vPos.z = -220.f;
		vPos.y = 35.f;
		vPos.z = -120.f;
		

	//	PlayerRot.x += 25.f * _DEGREE;
		PlayerRot.y += 90.f * _DEGREE;

	//	Transform()->SetLocalRot(PlayerRot);
	
		Vec2 vDrag = CKeyMgr::GetInst()->GetDragDir();
		vRot = Transform()->GetLocalRot();

		vRot.x -= vDrag.y * DT * 0.5f;

		//예외처리
		if (vRot.x * _RADIAN < -10.f)
		{
			vRot.x = -10.f * _DEGREE;
		}
		else if (vRot.x * _RADIAN > 90.f)
		{
			vRot.x = 90.f * _DEGREE;
		}
		PlayerRot.x = vRot.x;

		Transform()->SetLocalRot(PlayerRot);
		Transform()->SetMainCamMatrix(PlayerRot, PlayerPos);
		Transform()->SetLocalPos(vPos);

		Transform()->_SetOrbitalTargetRot(CPlayerScript::PlayerObject->Transform()->GetLocalRot());
	}
}
