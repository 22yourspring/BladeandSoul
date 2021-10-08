#include "stdafx.h"
#include "Light3D.h"

#include "Transform.h"
#include "RenderMgr.h"

#include "ResMgr.h"
#include "Camera.h"

#include "Core.h"


CLight3D::CLight3D()
	: CComponent(COMPONENT_TYPE::LIGHT3D)
	, m_iArrIdx(-1)
	, m_pCamObj(nullptr)
	, RimLight(0)
{
	// ���� �������� ī�޶� ����
	m_pCamObj = new CGameObject;
	m_pCamObj->AddComponent(new CTransform);
	m_pCamObj->AddComponent(new CCamera);

	// ������ �����ϴ� ī�޶�� �����Ŵ����� ��ϵǸ� �ȵȴ�.
	m_pCamObj->Camera()->SetModule(true);
	m_pCamObj->Camera()->SetLayerAllCheck(); // ��� ���̾ ��´�(shadow map)
}

CLight3D::CLight3D(const CLight3D & _light)
	: CComponent(_light)
	, m_tLightInfo(_light.m_tLightInfo)
	, m_pLightMtrl(_light.m_pLightMtrl)
	, m_pVolumeMesh(_light.m_pVolumeMesh)
	, m_iArrIdx(-1)
	, m_pCamObj(nullptr)
	, RimLight(1)
{
	m_pCamObj = _light.m_pCamObj->Clone();
}

CLight3D::~CLight3D()
{
	SAFE_DELETE(m_pCamObj);
}

void CLight3D::SetLightType(LIGHT_TYPE _eType)
{
	m_tLightInfo.iLightType = (int)_eType;

	if (LIGHT_TYPE::DIR == (LIGHT_TYPE)m_tLightInfo.iLightType)
	{
		m_pVolumeMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
		m_pLightMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"DirLightMtrl");

		m_pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
		m_pCamObj->Camera()->SetScale(1.f);
		m_pCamObj->Camera()->SetFar(500000.f);
		m_pCamObj->Camera()->SetWidth(_SHADOWSIZE_X);			//ShadowMap Render SIze.x
		m_pCamObj->Camera()->SetHeight(_SHADOWSIZE_Y);			//ShadowMap Render SIze.y
	}
	else if (LIGHT_TYPE::POINT == (LIGHT_TYPE)m_tLightInfo.iLightType)
	{
		m_pVolumeMesh = CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh");
		m_pLightMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"PointLightMtrl");

		m_pCamObj->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
		m_pCamObj->Camera()->SetScale(1.f);
		m_pCamObj->Camera()->SetFar(500000.f);
		m_pCamObj->Camera()->SetWidth(_SHADOWSIZE_X);
		m_pCamObj->Camera()->SetHeight(_SHADOWSIZE_Y);
	}
	else if (LIGHT_TYPE::SPOT == (LIGHT_TYPE)m_tLightInfo.iLightType)
	{
		m_pVolumeMesh = CResMgr::GetInst()->FindRes<CMesh>(L"ConeMesh");
		m_pLightMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"SpotLightMtrl");
	}
}

void CLight3D::SetLightPos(const Vec3 & _vPos)
{
	m_tLightInfo.vLightPos = _vPos;
	Transform()->SetLocalPos(_vPos);
}

void CLight3D::SetLightDir(const Vec3& _vDir)
{
	m_tLightInfo.vLightDir = _vDir;
	m_tLightInfo.vLightDir.Normalize();
	Transform()->LookAt(Vec3(m_tLightInfo.vLightDir.x, m_tLightInfo.vLightDir.y, m_tLightInfo.vLightDir.z));
}

void CLight3D::SetRimLight(bool _SetLight)
{
	if (_SetLight == true)
	{
		RimLight = 1;
	}
	else
		RimLight = 0;
}

bool CLight3D::GetRimLight()
{
	if (RimLight != 0)
	{
		return true;
	}
	else
		return false;
}


void CLight3D::finalupdate()
{
	m_tLightInfo.vLightPos = Transform()->GetWorldPos();
	Transform()->SetLocalScale(Vec3(m_tLightInfo.fRange, m_tLightInfo.fRange, m_tLightInfo.fRange));

	m_iArrIdx = CRenderMgr::GetInst()->RegisterLight3D(this);

	// ���� ���� ī�޶� ������ ���� Transform ������ ������ �Ѵ�.
	*m_pCamObj->Transform() = *Transform();

	if (CCore::GetInst()->GetSceneMod() == SCENE_MOD::SCENE_PLAY)
	{
		Vec3 TargetPos = CRenderMgr::GetInst()->GetMainCamera()->Transform()->GetCameraTargetPos();

		TargetPos.x -= 2000.f;
		TargetPos.y += 2000.f;
		TargetPos.z -= 2000.f;

		Vec3 Orbital = CRenderMgr::GetInst()->GetMainCamera()->Transform()->_GetOrbitalTargetRot();
		float SinY = sin(Orbital.y);
		float CosX = cos(Orbital.y);

		SinY *= -390.f;
		CosX *= 390.f;
		TargetPos.x += CosX;
		TargetPos.z += SinY;

		m_pCamObj->Transform()->SetLocalPos(TargetPos);
	}
	else
	{
		Vec3 ModifTrans = CRenderMgr::GetInst()->GetMainCam()->Transform()->GetWorldPos();

		ModifTrans.x -= 2000.f;
		ModifTrans.y += 2000.f;
		ModifTrans.z -= 2000.f;

		m_pCamObj->Transform()->SetLocalPos(ModifTrans);
	}
	//	*m_pCamObj->Transform() = *(CRenderMgr::GetInst()->GetMainCam()->Transform());				// �̽��� ī�޶� = ������ �ǹ���.
	m_pCamObj->finalupdate(); // �����Ŵ����� ������� �ʰ� �صξ���.
}

void CLight3D::render()
{
	if (-1 == m_iArrIdx)
		return;

	Transform()->UpdateData();

	// Directional Light �� ���
	if (m_tLightInfo.iLightType == (UINT)LIGHT_TYPE::DIR)
	{
		// ���� ���� ShadowMap �������� �ؽ���
		Ptr<CTexture> pShadowMapTex = CResMgr::GetInst()->FindRes<CTexture>(L"ShadowMapTargetTex");
		m_pLightMtrl->SetData(SHADER_PARAM::TEX_3, &pShadowMapTex);

		// �������� ������Ű�� ���� ���� View, Proj ���
		Matrix matVP = m_pCamObj->Camera()->GetViewMat() * m_pCamObj->Camera()->GetProjMat();
		m_pLightMtrl->SetData(SHADER_PARAM::MATRIX_0, &matVP);
	}

	m_pLightMtrl->SetData(SHADER_PARAM::INT_0, &m_iArrIdx); // ���� �迭 �ε������� ������Ʈ

	m_pLightMtrl->SetData(SHADER_PARAM::INT_1, &RimLight); // ���� �迭 �ε������� ������Ʈ

	m_pLightMtrl->UpdateData();

	m_pVolumeMesh->render();
}

void CLight3D::render_shadowmap()
{
	m_pCamObj->Camera()->SortShadowObject();
	m_pCamObj->Camera()->render_shadowmap();
}

void CLight3D::SaveToScene(FILE * _pFile)
{
}

void CLight3D::LoadFromScene(FILE * _pFile)
{
}