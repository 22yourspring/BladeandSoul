#pragma once
#include <Engine/Script.h>

class CMonsterScript :
	public CScript
{
private:

public:
	virtual void update() {}

public:
	CLONE(CMonsterScript);

public:
	CMonsterScript();
	virtual ~CMonsterScript();
};

