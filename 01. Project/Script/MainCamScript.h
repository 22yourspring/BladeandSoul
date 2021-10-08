#pragma once

#include <Engine/Script.h>

enum class MainCAMERAMOVEDIR
{
	KEY_W,
	KEY_A,
	KEY_S,
	KEY_D,
	END,
};

class CMainCamScript :
	public CScript
{
private:
	static 	float	m_fSpeed;
	float		m_fScaleSpeed;

public:
	bool	wKeyCheck;
	bool	aKeyCheck;
	bool	sKeyCheck;
	bool	dKeyCheck;

	bool	CameraFaster;

	MainCAMERAMOVEDIR CamDIr;
	Vec3 vPos;
	Vec3 vRot;

	virtual void update();

public:
	CLONE(CMainCamScript);

	static 	float GetCamSpeed() { return m_fSpeed; };
public:
	CMainCamScript();
	virtual ~CMainCamScript();
};

