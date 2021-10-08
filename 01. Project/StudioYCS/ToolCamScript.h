#pragma once

#include <Engine/Script.h>

enum class CAMERAMOVEDIR
{
	KEY_W,
	KEY_A,
	KEY_S,
	KEY_D,
	END,
};

class CToolCamScript :
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

	CAMERAMOVEDIR CamDIr;
	Vec3 vPos;
	Vec3 vRot;

	void SlidingCamera();
	void ExexceptionMouse();

	virtual void update();

public:
	CLONE(CToolCamScript);

	static 	float GetCamSpeed() { return m_fSpeed; };
public:
	CToolCamScript();
	virtual ~CToolCamScript();
};

