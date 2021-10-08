#include "pch.h"
#include "GridScript.h"

CGridScript::CGridScript()
	: CScript((UINT)COMPONENT_TYPE::SCRIPT)
	, m_pToolCam(nullptr)
	, m_fThickness(0.1f)
	, m_fDistance(0.f)
	, m_iMaxStep(5)
{
}

CGridScript::~CGridScript()
{
}

void CGridScript::update()
{
	m_vToolCamPos = m_pToolCam->Transform()->GetWorldPos();
	Transform()->SetLocalPos(Vec3(m_vToolCamPos.x, 0.f, m_vToolCamPos.z));

	CalculateGrid();
}

void CGridScript::CalculateGrid()
{
	// 카메라 높이에 따라서 Grid 간격을 정한다.
	// 1 단계 : 간격 10, 카메라 높이 0 ~ 100
	// 2 단계 : 간격 100, 카메라 높이 100 ~ 1000
	// 3 단계 : 간격 1000, 카메라 높이 1000 ~ 10000
	// 4 단계 : 간격 10000, 카메라 높이 10000 ~ 
	Ptr<CMaterial> pMtrl = MeshRender()->GetSharedMaterial();

	UINT iHeight = (UINT)abs(m_vToolCamPos.y);
	UINT iStep = 1;
	UINT i = 100;
	for (; iStep <= 4; i *= 10, ++iStep)
	{
		if (iHeight <= i)
			break;
	}

	m_fThickness = ((float)iHeight * 0.01f);
	m_fAlpha = abs(1.f - (float)((double)iHeight / pow(10., (double)iStep + 1.)));

	// Material 에 값을 전달	
	pMtrl->SetData(SHADER_PARAM::INT_0, &iStep);
	pMtrl->SetData(SHADER_PARAM::FLOAT_0, &m_fAlpha);
	pMtrl->SetData(SHADER_PARAM::FLOAT_1, &m_fThickness);
	pMtrl->SetData(SHADER_PARAM::VEC4_0, &m_vToolCamPos);
}
