#pragma once
#include <Engine/Script.h>

class CRevolveScript :
	public CScript
{
private:
	CGameObject* RevolveTarget;
	CScene*		pCurScene;
public:
	virtual void awake();
	virtual void update();
	virtual void OnCollisionEnter(CCollider2D* _pOther);
	virtual void OnCollisionExit(CCollider2D* _pOther);

	void Revolve(CGameObject* _RevolveTarget);
	void Revolve(Vec3 _TargetPos);

public:
	CLONE(CRevolveScript);

public:
	CRevolveScript();
	virtual ~CRevolveScript();
};
