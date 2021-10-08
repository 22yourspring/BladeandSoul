#include "stdafx.h"
#include "BulletScript.h"


CBulletScript::CBulletScript()	
	: CScript((UINT)SCRIPT_TYPE::BULLETSCRIPT)
{
}

CBulletScript::~CBulletScript()
{
}

void CBulletScript::update()
{
	Vec3 vPos = Transform()->GetLocalPos();
	vPos.y += 400.f * DT;
	Transform()->SetLocalPos(vPos);
}

void CBulletScript::OnCollisionEnter(CCollider2D * _pOther)
{
	if (_pOther->GetObj()->GetLayerIdx() == 2)
	{
		DeleteObject(GetObj());
	}
}
