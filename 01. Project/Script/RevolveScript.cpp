#include "stdafx.h"
#include "RevolveScript.h"
#include <Engine/Light3D.h>

CRevolveScript::CRevolveScript()
	: CScript((UINT)SCRIPT_TYPE::REVOLVESCRIPT)
	, RevolveTarget(nullptr)
	, pCurScene(nullptr)
{
}

CRevolveScript::~CRevolveScript()
{
}

void CRevolveScript::awake()
{

}

void CRevolveScript::update()
{
	Revolve(Vec3(0.302f + ObjectX, 168.8f + ObjectY, -0.258f + ObjectZ));
}

void CRevolveScript::OnCollisionEnter(CCollider2D * _pOther)
{
	// DeleteObject(GetObj());	
	// AddChild(_pOther->GetObj());
	// ClearParent();
}

void CRevolveScript::OnCollisionExit(CCollider2D * _pOther)
{
}

void CRevolveScript::Revolve(CGameObject * _RevolveTarget)
{
}

void CRevolveScript::Revolve(Vec3 _TargetPos)
{
	if (pCurScene == nullptr)
	{
		pCurScene = CSceneMgr::GetInst()->GetCurScene();
	}
	Vec3 vLightPos = (Object()->Transform()->GetWorldPos());
	Vec3 vLightDir = _TargetPos - vLightPos;

	Vec3 Value = vLightDir.Normalize();

	pCurScene->GetLightObject()->Light3D()->SetLightPos(vLightPos);

	if (KEY_TAB(KEY_TYPE::KEY_5))
	{
		pCurScene->GetLightObject()->Light3D()->SetLightDir(Vec3(1.f, -1.f, 1.f));
	}

	if (KEY_TAB(KEY_TYPE::KEY_6))
	{
		pCurScene->GetLightObject()->Light3D()->SetLightDir(vLightDir.Normalize());
	}
//	pCurScene->GetLightObject()->Light3D()->SetLightDir(Vec3(1.f, -1.f, 1.f));
//	pCurScene->GetLightObject()->Light3D()->SetLightDir(vLightDir.Normalize());
}
