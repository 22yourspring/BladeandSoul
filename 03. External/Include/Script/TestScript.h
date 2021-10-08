#pragma once
#include <Engine/Script.h>

class CTestScript :
	public CScript
{
private:
	int		m_iLifeCount;

public:	
	virtual void awake();
	virtual void update();
	virtual void OnCollisionEnter(CCollider2D* _pOther);
	virtual void OnCollisionExit(CCollider2D* _pOther);
public:
	CLONE(CTestScript);

public:
	CTestScript();
	virtual ~CTestScript();
};
