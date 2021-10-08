#include "stdafx.h"
#include "MeshRender.h"

#include "GameObject.h"
#include "Transform.h"
#include "Animator2D.h"
#include "Animator3D.h"
#include "ResMgr.h"


CMeshRender::CMeshRender()
	: CComponent(COMPONENT_TYPE::MESHRENDER)
	, m_bShadow(false)
{
	m_vecMtrlSet.resize(1);
	m_vecMtrlSet[0].resize(1);
}

CMeshRender::~CMeshRender()
{
	/*for (int i = 0; i < m_vecMtrlSet.size(); ++i)
	{
		for (int j = 0; j < m_vecMtrlSet[i].size(); ++j)
		{
			SAFE_RELEASE(m_vecMtrlSet[i][j]);
		}
	}*/
}

Ptr<CMaterial> CMeshRender::GetSharedMaterial(UINT _iSubset, UINT _iContainer)
{
	if (m_vecMtrlSet.size() > _iContainer && m_vecMtrlSet[_iContainer].size() > _iSubset)
	{
		return m_vecMtrlSet[_iContainer][_iSubset];
	}
	else
	{
		return NULL;
	}
}

void CMeshRender::render()
{
	if (IsActive() == false || nullptr == m_pMesh)
		return;

	Transform()->UpdateData();

	int iContainer = (int)m_vecMtrlSet.size();
	int iSubset;

	if (Animator3D())
	{
		Animator3D()->UpdateData();
		for (int i = 0; i < iContainer; ++i)
		{
			iSubset = m_vecMtrlSet[i].size();

			for (int j = 0; j < iSubset; ++j)
			{
				if (nullptr == m_vecMtrlSet[i][j])
					continue;

				int a = 1;
				m_vecMtrlSet[i][j]->SetData(SHADER_PARAM::INT_0, &a); // Animation Mesh 알리기
			}


		}
	}
	else if (Animator2D())
	{
		Animator2D()->UpdateData();
	}

	for (int i = 0; i < iContainer; ++i)
	{
		iSubset = m_vecMtrlSet[i].size();

		for (int j = 0; j < iSubset; ++j)
		{
			if (nullptr == m_vecMtrlSet[i][j] || nullptr == m_vecMtrlSet[i][j]->GetShader())
				continue;

			m_vecMtrlSet[i][j]->UpdateData();
			m_pMesh->render((UINT)j, (UINT)i);
		}
	}

	// 정리
	CAnimator2D::ClearData();
	if (Animator3D())
		Animator3D()->ClearData();
}

void CMeshRender::render_shadowmap()
{
	int a = 1;
	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"ShadowMapMtrl");

	Transform()->UpdateData();
	if (Animator3D())
	{
		Animator3D()->UpdateData();
		pMtrl->SetData(SHADER_PARAM::INT_0, &a); // Animation Mesh 알리기
	}

	// 컨테이너 개수만큼 돌린다.
	int iContainerCount = m_pMesh->GetContainerCount();

	for (int k = 0; k < iContainerCount; ++k)
	{
		for (UINT i = 0; i < m_pMesh->GetSubsetCount(k); ++i)
		{
			pMtrl->UpdateData();
			m_pMesh->render(i, k);
		}
	}



	// 정리
	if (Animator3D())
	{
		a = 0;
		Animator3D()->ClearData();
		pMtrl->SetData(SHADER_PARAM::INT_0, &a);
	}
}

void CMeshRender::SetMaterial(Ptr<CMaterial> _pMtrl, UINT _iSubset, UINT _iContainer)
{
	if (m_vecMtrlSet.size() <= (int)_iContainer)
		m_vecMtrlSet.resize(_iContainer + 1);

	if (m_vecMtrlSet[_iContainer].size() <= (int)_iSubset)
		m_vecMtrlSet[_iContainer].resize(_iSubset + 1);

	m_vecMtrlSet[_iContainer][_iSubset] = _pMtrl;
}

Ptr<CMaterial> CMeshRender::GetCloneMaterial(UINT _iSubSet, UINT _iContainer)
{
	if (nullptr == m_vecMtrlSet[_iContainer][_iSubSet])
		return nullptr;

	m_vecMtrlSet[_iContainer][_iSubSet] = m_vecMtrlSet[_iContainer][_iSubSet]->Clone();
	return m_vecMtrlSet[_iContainer][_iSubSet];
}

ULONG64 CMeshRender::GetInstID(UINT _iMtrlIdx, UINT _iContainer)
{
	if (m_pMesh == NULL || m_vecMtrlSet[_iContainer][_iMtrlIdx] == NULL)
		return 0;

	uInstID id{ m_pMesh->GetID(), (WORD)m_vecMtrlSet[_iContainer][_iMtrlIdx]->GetID(), (WORD)(10 * _iContainer + _iMtrlIdx) };
	return id.llID;
}

void CMeshRender::SaveToScene(FILE * _pFile)
{
	//UINT iType = (UINT)GetComponentType();
	//fwrite(&iType, sizeof(UINT), 1, _pFile);

	//// 존재여부 저장(nullptr 인지 아닌지)
	//fwrite(&m_pMesh, sizeof(void*), 1, _pFile);
	//
	//if (nullptr != m_pMesh)
	//{
	//	SaveWString(_pFile, m_pMesh->GetName());
	//	SaveWString(_pFile, m_pMesh->GetPath());
	//}

	//fwrite(&m_pMtrl, sizeof(void*), 1, _pFile);

	//if (nullptr != m_pMtrl)
	//{
	//	SaveWString(_pFile, m_pMtrl->GetName());
	//	SaveWString(_pFile, m_pMtrl->GetPath());
	//}	
}

void CMeshRender::LoadFromScene(FILE * _pFile)
{
	/*void* pData = nullptr;
	fread(&pData, sizeof(void*), 1, _pFile);

	if (pData)
	{
		wstring strMesh = LoadWString(_pFile);
		wstring strPath = LoadWString(_pFile);
		m_pMesh = CResMgr::GetInst()->FindRes<CMesh>(strMesh);

		if (nullptr == m_pMesh)
		{
			m_pMesh = CResMgr::GetInst()->Load<CMesh>(strMesh, strPath);
		}
	}

	fread(&pData, sizeof(void*), 1, _pFile);

	if (pData)
	{
		wstring strMtrl = LoadWString(_pFile);
		wstring strPath = LoadWString(_pFile);
		m_pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(strMtrl);

		if (nullptr == m_pMtrl)
		{
			m_pMtrl = CResMgr::GetInst()->Load<CMaterial>(strMtrl, strPath);
		}
	}*/
}