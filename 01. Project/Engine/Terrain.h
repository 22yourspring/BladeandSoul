#pragma once
#include "Component.h"

#include "Ptr.h"
#include "Mesh.h"
#include "Material.h"
#include "Texture.h"

class CTerrain :
	public CComponent
{
private:
	UINT			m_iFaceX;
	UINT			m_iFaceZ;
	float			m_fMaxTess;

	Ptr<CTexture>	m_pHeightMap;
	Ptr<CMaterial>	m_pComputeMtrl;

	Ptr<CTexture>	m_pWeightTex;
	Ptr<CMaterial>	m_pWeightMtrl;

	Vec2			m_vBrushScale;
	Vec2			m_PrevBrushScale;

	UINT			m_iBrushIdx;
	UINT			m_iTileIdx;

	bool			TerrainSelect;

	BRUSH_STATE		m_BrushState;
	BRUSH_TYPE		m_BrushType;

	LANDSCAPE_MATERIAL	m_Material;
	TERRAIN_OPTION		m_TerrainOption;

	Vec3			TessLevel;

public:
	void init();
	virtual void finalupdate();

private:
	void ChangeFaceCount(UINT _iFaceX, UINT _iFaceZ);
	void CreateTerrainMesh();
	void CreateComputeShader();
public:
	Vec2 GetCollisionPos();

public:
	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

	void ModeCheck();
	void SetBrushScale(float _Scale);

	float GetBrushScale() { return m_vBrushScale.x; };
	void ScaleModif();
	void SetSplatting(BRUSH_STATE _SetSplat_State) { m_BrushState = _SetSplat_State; };
	BRUSH_STATE GetSplatting() { return m_BrushState; };
	void SetBrushType(BRUSH_TYPE _BrushType) { m_BrushType = _BrushType; };
	BRUSH_TYPE GetBrushType() { return m_BrushType; };

	void SetLandscape_Material(LANDSCAPE_MATERIAL _SetMaterial) { m_Material = _SetMaterial; };

	void SetTargetSelectTerrain(bool _Select);
	bool GetTargetSelectTerrain() { return TerrainSelect; };

	void SetTessLevel(Vec3 _SetLevel) { TessLevel = _SetLevel; };
	Vec3 GetTessLevel() { return TessLevel; };

	void SetTerrainOption(TERRAIN_OPTION _SetOption) { m_TerrainOption = _SetOption; };
	TERRAIN_OPTION GetTerrainOption() { return m_TerrainOption; };

public:
	CLONE(CTerrain);

public:
	CTerrain();
	virtual ~CTerrain();
};