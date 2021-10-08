#include "stdafx.h"
#include "BrushScript.h"

#include <Engine/Terrain.h>

#include <Engine/SceneMgr.h>

CBrushScript::CBrushScript()
	: CScript((UINT)SCRIPT_TYPE::BRUSHSCRIPT)
	, m_pOriginMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
	, m_vecTerrainLayer(nullptr)
	, m_pTerrain(nullptr)
	, SelectingState(false)
{
}

CBrushScript::~CBrushScript()
{
}


void CBrushScript::awake()
{
//	m_pOriginMtrl = MeshRender()->GetSharedMaterial();
//	m_pCloneMtrl = m_pOriginMtrl->Clone();

	m_vecTerrainLayer = &((CSceneMgr::GetInst())->GetCurScene()->FindLayer(L"Terrain"))->GetObjects();
}

void CBrushScript::update()
{
	FindTerrain();
	AutoEnable();
	if (SelectingState)
	{
		SetFromScale();
		SetFromPos();
		SetFromType();
	}
	else
	{
		MemoryPoolSetting();
	}

	if (Prev_vScale != m_vScale)
	{
		Transform()->SetLocalScale(m_vScale);
	}

	if (Prev_vPos != m_vPos)
	{
		Transform()->SetLocalPos(m_vPos);
	}

	Prev_vPos = m_vPos;
	Prev_vScale = m_vScale;
}

void CBrushScript::FindTerrain()
{
	if (m_pTerrain == nullptr)
	{
		int TerrainLayer_Size = m_vecTerrainLayer->size();
		
		for (size_t i = 0; i < TerrainLayer_Size; ++i)
		{
			if ((*m_vecTerrainLayer)[i]->GetName() == L"_Base Terrain")
			{
				m_pTerrain = (*m_vecTerrainLayer)[i];
			}
		}
		
	}
}

void CBrushScript::SetFromScale()
{
	if (m_pTerrain != nullptr)
	{
		float BrushScale = ((CTerrain*)(m_pTerrain->GetComponent(COMPONENT_TYPE::TERRAIN)))->GetBrushScale();
		m_vScale.x = BrushScale * 450000;
		m_vScale.y = BrushScale * 4500000;
		m_vScale.z = BrushScale * 450000;
	}
}

void CBrushScript::SetFromPos()
{
	if (m_pTerrain != nullptr)
	{
		Vec2 BrushPos = ((CTerrain*)(m_pTerrain->GetComponent(COMPONENT_TYPE::TERRAIN)))->GetCollisionPos();
		
		
		m_vPos.x = (BrushPos.x - 0.5f) * 400000.f;
		m_vPos.z = (BrushPos.y - 0.5f) * -400000.f;
	}
}

void CBrushScript::SetFromType()
{
	if (m_pTerrain != nullptr)
	{
		BRUSH_TYPE BrushType = ((CTerrain*)(m_pTerrain->GetComponent(COMPONENT_TYPE::TERRAIN)))->GetBrushType();

		if (m_BrushType != BRUSH_TYPE::CIRCLE && BrushType == BRUSH_TYPE::CIRCLE)
		{
			Object()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"_BrushCircleMtrl"));
		}
		else if (m_BrushType != BRUSH_TYPE::INK && BrushType == BRUSH_TYPE::INK)
		{
			Object()->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"_BrushInkMtrl"));
		}

		m_BrushType = BrushType;
	}
}

void CBrushScript::AutoEnable()
{
	if (m_pTerrain != nullptr)
	{
		SelectingState = ((CTerrain*)(m_pTerrain->GetComponent(COMPONENT_TYPE::TERRAIN)))->GetTargetSelectTerrain();

		m_BrushState = ((CTerrain*)(m_pTerrain->GetComponent(COMPONENT_TYPE::TERRAIN)))->GetSplatting();

		if (m_BrushState == BRUSH_STATE::DISABLE)
		{
			SelectingState = false;
		}
	}
}

void CBrushScript::MemoryPoolSetting()
{
	m_vPos = { 0.f, 0.f, 0.f };
	m_vScale = { 0.f, 0.f, 0.f };
}
