#include "stdafx.h"
#include "Terrain.h"

#include "ResMgr.h"
#include "MeshRender.h"

#include "KeyMgr.h"
#include "Texture.h"

#include "RenderMgr.h"
#include "Core.h"

CTerrain::CTerrain()
	: CComponent(COMPONENT_TYPE::TERRAIN)
	, m_iFaceX(64)
	, m_iFaceZ(64)
	, m_fMaxTess(4)
	, m_vBrushScale(Vec2(0.05f, 0.05f))
	, m_PrevBrushScale(Vec2(0.05f, 0.05f))
	, m_iBrushIdx(0)
	, m_iTileIdx(0)
	, TerrainSelect(false)
	, m_BrushState(BRUSH_STATE::DISABLE)
	, m_BrushType(BRUSH_TYPE::DISABLE)
	, m_Material(LANDSCAPE_MATERIAL::DISABLE)
	, TessLevel(Vec3(20000.f, 80000.f, 4.f))
	, m_TerrainOption(TERRAIN_OPTION::DISABLE)
{
}

CTerrain::~CTerrain()
{
}

void CTerrain::finalupdate()
{
	if (m_vBrushScale.x != m_PrevBrushScale.x)
	{
		ScaleModif();
	}

	if (TerrainSelect)
	{
		ModeCheck();				//이승훈
	}

	if (CCore::GetInst()->GetSceneMod() != SCENE_MOD::SCENE_PLAY && TerrainSelect)
	{
		Vec2 PickingPos = GetCollisionPos();		//이승훈

		MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC2_3, &PickingPos);

		if (KEY_HOLD(KEY_TYPE::KEY_LBTN))
		{
			Vec2 vCollsionPos = GetCollisionPos();

			if (vCollsionPos.x < 0.f || 1.f < vCollsionPos.x
				|| vCollsionPos.y < 0.f || 1.f < vCollsionPos.y)
				return;

			if (m_BrushState == BRUSH_STATE::HEIGHT_MAP)
			{
				m_pComputeMtrl->SetData(SHADER_PARAM::VEC2_0, &vCollsionPos);
				m_pComputeMtrl->Dispatch(1, 1024, 1);
			}

			if (m_BrushState == BRUSH_STATE::WEIGHT_MAP)
			{
				m_pWeightMtrl->SetData(SHADER_PARAM::VEC2_0, &vCollsionPos);
				m_pWeightMtrl->Dispatch(1, 1024, 1);
			}
		}
	}

	m_PrevBrushScale = m_vBrushScale;
}


void CTerrain::SaveToScene(FILE * _pFile)
{
}

void CTerrain::LoadFromScene(FILE * _pFile)
{
}

void CTerrain::ModeCheck()
{
	if (m_BrushType == BRUSH_TYPE::CIRCLE)
	{
		m_iBrushIdx = 0;
		m_pWeightMtrl->SetData(SHADER_PARAM::INT_3, &m_iBrushIdx);// g_int_2 : Brush Index (브러쉬 종류)
		m_pComputeMtrl->SetData(SHADER_PARAM::INT_0, &m_iBrushIdx);
	}
	else if (m_BrushType == BRUSH_TYPE::INK)
	{
		m_iBrushIdx = 1;
		m_pWeightMtrl->SetData(SHADER_PARAM::INT_3, &m_iBrushIdx);// g_int_2 : Brush Index (브러쉬 종류)
		m_pComputeMtrl->SetData(SHADER_PARAM::INT_0, &m_iBrushIdx);
		m_pWeightMtrl->SetData(SHADER_PARAM::INT_2, &m_iTileIdx); // g_int_2 : Tile Index (가중치를 올릴 타일)
	}

	if (m_BrushState == BRUSH_STATE::WEIGHT_MAP)
	{
		if (m_Material == LANDSCAPE_MATERIAL::TILE1)
		{
			m_iTileIdx = 0;
			m_pWeightMtrl->SetData(SHADER_PARAM::INT_2, &m_iTileIdx); // g_int_2 : Tile Index (가중치를 올릴 타일)

		}
		else if (m_Material == LANDSCAPE_MATERIAL::TILE2)
		{
			m_iTileIdx = 1;
			m_pWeightMtrl->SetData(SHADER_PARAM::INT_2, &m_iTileIdx); // g_int_2 : Tile Index (가중치를 올릴 타일)
		}
		else if (m_Material == LANDSCAPE_MATERIAL::TILE3)
		{
			m_iTileIdx = 2;
			m_pWeightMtrl->SetData(SHADER_PARAM::INT_2, &m_iTileIdx); // g_int_2 : Tile Index (가중치를 올릴 타일)
		}
		else if (m_Material == LANDSCAPE_MATERIAL::DISABLE)
		{
			m_iTileIdx = 3;
			m_pWeightMtrl->SetData(SHADER_PARAM::INT_2, &m_iTileIdx); // g_int_2 : Tile Index (가중치를 올릴 타일)
		}
	}
	//	if (KEYTAB(KEY_TYPE::KEY_NUM1))
	//	{
	//		m_eMode = TERRAIN_MODE::HEIGHT;
	//		MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, m_vecBrushTex[m_iCurBrushIdx].GetPointer());
	//	}
	//	else if (KEYTAB(KEY_TYPE::KEY_NUM2))
	//	{
	//		m_eMode = TERRAIN_MODE::SPLAT;
	//		MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, m_vecBrushTex[m_iCurBrushIdx].GetPointer());
	//	}
	//	else if (KEYTAB(KEY_TYPE::KEY_NUM3))
	//	{
	//		m_eMode = TERRAIN_MODE::END;
	//		MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, nullptr);
	//	}
	//	
	//	if (KEYTAB(KEY_TYPE::KEY_NUM9))
	//	{
	//		m_iCurBrushIdx += 1;
	//		if (m_iCurBrushIdx >= m_vecBrushTex.size())
	//			m_iCurBrushIdx = 0;
	//	
	//		if (m_eMode != TERRAIN_MODE::END)
	//			MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, m_vecBrushTex[m_iCurBrushIdx].GetPointer());
	//	
	//		m_pBrushMtrl->SetData(SHADER_PARAM::TEX_0, m_vecBrushTex[m_iCurBrushIdx].GetPointer());
	//	}
	//	
	//	if (KEYTAB(KEY_TYPE::KEY_NUM8))
	//	{
	//		m_iCurWeightIdx += 1;
	//		if (m_iCurWeightIdx >= 4)
	//			m_iCurWeightIdx = 0;
	//	
	//		m_pSplatBrushMtrl->SetData(SHADER_PARAM::INT_0, &m_iCurWeightIdx);
	//	}
}

void CTerrain::SetBrushScale(float _Scale)
{
	m_vBrushScale.x = _Scale;
	m_vBrushScale.y = _Scale;
}

void CTerrain::ScaleModif()
{
	m_pComputeMtrl->SetData(SHADER_PARAM::VEC2_1, &m_vBrushScale);
	MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::VEC2_1, &m_vBrushScale);
	m_pWeightMtrl->SetData(SHADER_PARAM::VEC2_1, &m_vBrushScale); // g_vec2_1 : 브러쉬 크기	
}

void CTerrain::SetTargetSelectTerrain(bool _Select)
{
	TerrainSelect = _Select;
}
