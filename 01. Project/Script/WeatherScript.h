#pragma once
#include <Engine/Script.h>

class CWeatherScript : public CScript
{
private:
	Vec3 vPos;
	bool m_bStart;

	wstring		Script_strWeatherShader;

	int ClimatePosX;
	int ClimatePosY;
	int ClimatePosZ;

public:
	void AutoPosition();

public:
	virtual void update();
public:
	CLONE(CWeatherScript);

public:
	CWeatherScript();
	virtual ~CWeatherScript();
};

