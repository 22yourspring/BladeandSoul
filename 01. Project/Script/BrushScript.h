#pragma once
#include <Engine/Script.h>

class CBrushScript :
	public CScript
{
private:
	Ptr<CMaterial>		m_pOriginMtrl;
	Ptr<CMaterial>		m_pCloneMtrl;

	Vec3	m_vPos;
	Vec3	Prev_vPos;

	Vec3	m_vScale;
	Vec3	Prev_vScale;

	const	vector<CGameObject*>* 		m_vecTerrainLayer;
	CGameObject*				m_pTerrain;
	
	BRUSH_TYPE m_BrushType;
	BRUSH_STATE m_BrushState;

	bool SelectingState;

private:
	float m_fSpeed;

public:
	virtual void awake();
	virtual void update();

public:
	void FindTerrain();
	void SetFromScale();
	void SetFromPos();
	void SetFromType();
	void AutoEnable();
	void MemoryPoolSetting();
public:
	CLONE(CBrushScript);

public:
	CBrushScript();
	virtual ~CBrushScript();

};

