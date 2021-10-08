#include "stdafx.h"
#include "Collider3D.h"

#include "ResMgr.h"

#include "GameObject.h"
#include "Transform.h"
#include "Device.h"
#include "ConstBuffer.h"

#include "Script.h"

UINT CCollider3D::g_iColID = 0;

CCollider3D::CCollider3D()
	: CComponent(COMPONENT_TYPE::COLLIDER3D)
	, m_vOffsetPos(Vec3(0.f, 0.f, 0.f))
	, m_vOffsetScale(Vec3(1.f, 1.f, 1.f))
	, m_pColMesh(nullptr)
	, m_pColMtrl(nullptr)
	, m_iColID(g_iColID++)
	, m_iCollisionCount(0)
{
	m_pColMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl_0");
}

CCollider3D::CCollider3D(const CCollider3D & _other)
	: CComponent(COMPONENT_TYPE::COLLIDER3D)
	, m_vOffsetPos(_other.m_vOffsetPos)
	, m_vOffsetScale(_other.m_vOffsetScale)
	, m_eType(_other.m_eType)
	, m_iCollisionCount(0)
	, m_pColMesh(_other.m_pColMesh)
	, m_pColMtrl(_other.m_pColMtrl)
	, m_matColWorld(_other.m_matColWorld)
	, m_iColID(g_iColID++)
{
}

void CCollider3D::operator=(const CCollider3D & _other)
{
	UINT iColID = m_iColID;
	memcpy(this, &_other, sizeof(CCollider3D));
	m_iColID = iColID;
}

CCollider3D::~CCollider3D()
{
}

void CCollider3D::update()
{
}

void CCollider3D::finalupdate()
{
	if (!IsActive())
		return;

	Vec3 vFinalPos = m_vOffsetPos;
	vFinalPos = vFinalPos / Transform()->GetWorldScale(); // GetWorldScale() ·Î º¯°æ

	Matrix matTranslation = XMMatrixTranslation(vFinalPos.x, vFinalPos.y, vFinalPos.z);
	Matrix matScale = XMMatrixScaling(m_vOffsetScale.x, m_vOffsetScale.y, m_vOffsetScale.z);

	m_matColWorld = matScale * matTranslation;
	m_matColWorld *= Transform()->GetWorldMat();
}

void CCollider3D::render()
{
	if (!IsActive())
		return;

	static CConstBuffer* pCB = CDevice::GetInst()->FindConstBuffer(L"Transform Matrix");

	g_transform.matWorld = m_matColWorld;
	pCB->SetData(&g_transform);
	pCB->UpdateData((UINT)SHADER_TYPE::ST_VERTEX | (UINT)SHADER_TYPE::ST_PIXEL);

	m_pColMtrl->UpdateData();
	if (m_pColMesh != nullptr)
	{
		m_pColMesh->render();
	}

	memset(&m_matColWorld, 0, sizeof(Matrix));
}

void CCollider3D::SetCollider3DType(COLLIDER3D_TYPE _eType)
{
	m_eType = _eType;

	if (COLLIDER3D_TYPE::MESH == m_eType)
	{
		m_pColMesh = ((CMeshRender*)(GetOwner()->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetMesh();
	}
	else if (COLLIDER3D_TYPE::CUBE == m_eType)
	{
		m_pColMesh = CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh");
	}
	else if (COLLIDER3D_TYPE::SPHERE == m_eType)
	{
		m_pColMesh = CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh");
	}
}

void CCollider3D::OnCollisionEnter(CCollider3D * _pOther)
{
	m_iCollisionCount += 1;

	const vector<CScript*>& vecScripts = GetObj()->GetScripts();
	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		vecScripts[i]->OnCollisionEnter(_pOther);
	}
}

void CCollider3D::OnCollision(CCollider3D * _pOther)
{
	if (0 < m_iCollisionCount)
	{
		m_pColMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl_1");
	}

	const vector<CScript*>& vecScripts = GetObj()->GetScripts();
	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		vecScripts[i]->OnCollision(_pOther);
	}
}

void CCollider3D::OnCollisionExit(CCollider3D * _pOther)
{
	m_iCollisionCount -= 1;
	if (m_iCollisionCount == 0)
		m_pColMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"Collider3DMtrl_0");

	const vector<CScript*>& vecScripts = GetObj()->GetScripts();
	for (size_t i = 0; i < vecScripts.size(); ++i)
	{
		vecScripts[i]->OnCollisionExit(_pOther);
	}
}


void CCollider3D::SaveToScene(FILE * _pFile)
{
	UINT iType = (UINT)GetComponentType();
	fwrite(&iType, sizeof(UINT), 1, _pFile);

	fwrite(&m_vOffsetPos, sizeof(Vec3), 1, _pFile);
	fwrite(&m_vOffsetScale, sizeof(Vec3), 1, _pFile);
	fwrite(&m_eType, sizeof(UINT), 1, _pFile);
}

void CCollider3D::LoadFromScene(FILE * _pFile)
{
	fread(&m_vOffsetPos, sizeof(Vec3), 1, _pFile);
	fread(&m_vOffsetScale, sizeof(Vec3), 1, _pFile);
	fread(&m_eType, sizeof(UINT), 1, _pFile);
	SetCollider3DType(m_eType);
}