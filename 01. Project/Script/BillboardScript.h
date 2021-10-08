#pragma once
#include <Engine/Script.h>

class CBillboardScript : 
	public CScript
{
private:
	Ptr<CMaterial>		m_pOriginMtrl;
	Ptr<CMaterial>		m_pCloneMtrl;

public:
	virtual void awake();
	virtual void update();

private:
	CGameObject*	m_pMainCam;

public:
	CLONE(CBillboardScript);

public:
	CBillboardScript();
	virtual ~CBillboardScript();
};