#include "stdafx.h"
#include "MeshData.h"

#include "PathMgr.h"
#include "Mesh.h"
#include "ResMgr.h"
#include "Material.h"
#include "Transform.h"
#include "MeshRender.h"
#include "Collider3D.h"
#include "Animator3D.h"
#include "GameObject.h"

#include "FileDirectory.h"

CMeshData::CMeshData()
	: CResource(RES_TYPE::MESHDATA)
{
}

CMeshData::~CMeshData()
{
}

CMeshData * CMeshData::LoadFromFBX(const wstring& _strPath)
{
	wstring strFullPath = CPathMgr::GetResPath();
	strFullPath += _strPath;

	CFBXLoader loader;
	loader.init();
	loader.LoadFbx(strFullPath);

	// 메쉬 가져오기
	CMesh* pMesh = CMesh::CreateFromContainer(loader);

	// ResMgr 에 메쉬 등록
	wstring strMeshName = L"Mesh\\";
	strMeshName += CPathMgr::GetFileName(strFullPath.c_str());
	strMeshName += L".mesh";

	pMesh->SetName(strMeshName);
	pMesh->SetPath(strMeshName);

	CResMgr::GetInst()->AddRes<CMesh>(pMesh->GetName(), pMesh);

	vector<vector<Ptr<CMaterial>>> vecMtrlSet;

	// 메테리얼 가져오기
	// 06/01  
	// 서브셋
	int iContainer = loader.GetAllContainer()->size();
	vecMtrlSet.resize(iContainer);

	for (int k = 0; k < iContainer; ++k)
	{
		int iSubset = loader.GetContainer(k).vecMtrl.size();
		vecMtrlSet[k].resize(iSubset);

		for (int i = 0; i < iSubset; ++i)
		{
			// 예외처리 (material 이름이 입력 안되어있을 수도 있다.)
			Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(loader.GetContainer(k).vecMtrl[i].strMtrlName);
			vecMtrlSet[k][i] = pMtrl;
		}
	}


	CMeshData* pMeshData = new CMeshData;
	pMeshData->m_pMesh = pMesh;

	iContainer = loader.GetAllContainer()->size();
	pMeshData->m_vecMtrlSet.resize(iContainer);

	for (int k = 0; k < iContainer; ++k)
	{
		int iSubset = loader.GetContainer(k).vecMtrl.size();
		pMeshData->m_vecMtrlSet[k].resize(iSubset);

		for (int i = 0; i < iSubset; ++i)
		{
			pMeshData->m_vecMtrlSet[k][i] = vecMtrlSet[k][i];
		}
	}

	pMeshData->SetName(strMeshName);

	pMeshData->Save(strMeshName);

	return pMeshData;
}


void CMeshData::Load(const wstring & _strFilePath)
{
	// Mesh Data를 저장
	CFileDirectory newDir;

	newDir.MoveParent();
	newDir.MoveParent();
	newDir.Move(L"02. File");
	newDir.Move(L"bin");
	newDir.Move(L"content");
	newDir.Move(L"MeshData");

	wstring strFileName = newDir.GetPath();

	wstring filePath = _strFilePath;

	// 앞에 폴더가 포함되어 있다면 폴더 이름을 지운다.
	int LastFolderIndex = filePath.find_last_of(L"\\");
	filePath = filePath.replace(0, LastFolderIndex, L"");

	// full path에 파일 이름과 확장자를 붙인다.
	wstring strKey = filePath;
	LastFolderIndex = strKey.find_last_of(L".");
	strKey = strKey.replace(LastFolderIndex, strKey.size() + 1, L"");
	strKey.append(L".mdat");

	strFileName.append(strKey);

	// mdat 파일 열기
	FILE* pmDatFile = NULL;
	_wfopen_s(&pmDatFile, strFileName.c_str(), L"rb");

	// mesh 파일 열기
	FILE* pFile = NULL;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

	// Mesh Load
	wstring strMeshKey, strMeshPath;
	strMeshKey = LoadWString(pmDatFile);
	strMeshPath = LoadWString(pmDatFile);
	m_pMesh = CResMgr::GetInst()->Load<CMesh>(strMeshKey, strMeshPath);

	int iContainer = 0;
	fread(&iContainer, sizeof(int), 1, pmDatFile);
	m_vecMtrlSet.resize(iContainer);

	// m_vecMtrlSet.resize()
	for (int k = 0; k < iContainer; ++k)
	{
		// material 정보 읽기
		int iMtrlCount = 0;
		fread(&iMtrlCount, sizeof(int), 1, pmDatFile);

		m_vecMtrlSet[k].resize(iMtrlCount);

		for (int i = 0; i < iMtrlCount; ++i)
		{
			//int idx = -1;
			//// fread(&idx, sizeof(int), 1, pFile);
			//if (idx == -1)
			//	break;

			wstring strKey = LoadWString(pmDatFile);
			wstring strPath = LoadWString(pmDatFile);

			Ptr<CMaterial> pMtrl = CResMgr::GetInst()->Load<CMaterial>(strKey, strPath);
			m_vecMtrlSet[k][i] = pMtrl;
		}

	}

	fclose(pFile);
	fclose(pmDatFile);
}

void CMeshData::Save(const wstring& _strFilePath)
{
	wstring strFileName = CPathMgr::GetResPath();
	strFileName += _strFilePath;
	SetPath(_strFilePath);

	FILE* pFile = nullptr;
	errno_t err = _wfopen_s(&pFile, strFileName.c_str(), L"wb");

	// Mesh 를 .mesh 파일로 저장
	m_pMesh->Save(pFile, m_pMesh->GetPath());

	fclose(pFile);

	// Mesh Data를 저장
	CFileDirectory newDir;

	newDir.MoveParent();
	newDir.MoveParent();
	newDir.Move(L"02. File");
	newDir.Move(L"bin");
	newDir.Move(L"content");
	newDir.Move(L"MeshData");

	strFileName = newDir.GetPath();

	// 앞에 폴더가 포함되어 있다면 폴더 이름을 지운다.
	wstring tempString = m_pMesh->GetName();
	int LastFolderIndex = tempString.find_last_of(L"\\");
	tempString = tempString.replace(0, LastFolderIndex, L"");

	// full path에 파일 이름과 확장자를 붙인다.
	wstring strKey = strFileName.append(tempString);
	LastFolderIndex = strKey.find_last_of(L".");

	strKey = strKey.replace(LastFolderIndex, strKey.size() + 1, L"");

	strKey.append(L".mdat");


	pFile = nullptr;
	err = _wfopen_s(&pFile, strKey.c_str(), L"wb");


	// Mesh Key 저장	
	// Mesh Data 저장
	SaveWString(pFile, m_pMesh->GetName().c_str());
	SaveWString(pFile, m_pMesh->GetPath().c_str());

	// material 정보 저장
	int iContainer = m_pMesh->GetContainerCount();
	fwrite(&iContainer, sizeof(int), 1, pFile);

	for (int k = 0; k < iContainer; ++k)
	{
		int iMtrlCount = (int)m_pMesh->GetSubsetCount(k);
		fwrite(&iMtrlCount, sizeof(int), 1, pFile);

		int i = 0;
		for (; i < iMtrlCount; ++i)
		{
			if (nullptr == m_vecMtrlSet[k][i])
				continue;

			// Material 을 파일로 저장
			m_vecMtrlSet[k][i]->Save(m_vecMtrlSet[k][i]->GetPath());

			// Material 인덱스, Key, Path 저장
			// fwrite(&i, sizeof(int), 1, pFile);
			SaveWString(pFile, m_vecMtrlSet[k][i]->GetName().c_str());
			SaveWString(pFile, m_vecMtrlSet[k][i]->GetPath().c_str());
		}


	}

	int a = -1; // 마감 값
	fwrite(&a, sizeof(int), 1, pFile);

	fclose(pFile);
}

CGameObject * CMeshData::Instantiate()
{
	// Mesh
	// Material

	CGameObject* pNewObj = new CGameObject;
	pNewObj->AddComponent(new CTransform);
	pNewObj->AddComponent(new CMeshRender);
	pNewObj->AddComponent(new CCollider3D);
	pNewObj->MeshRender()->SetMesh(m_pMesh);
	pNewObj->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pNewObj->Collider3D()->SetOffsetScale(Vec3(50.f, 50.f, 50.f));

	int iContainer = (int)m_vecMtrlSet.size();

	for (int k = 0; k < iContainer; ++k)
	{
		for (int i = 0; i < m_vecMtrlSet[k].size(); ++i)
		{
			pNewObj->MeshRender()->SetMaterial(m_vecMtrlSet[k][i], i, k);
		}
	}



	if (false == m_pMesh->IsAnimMesh())
		return pNewObj;

	CAnimator3D* pAnimator = new CAnimator3D;
	pNewObj->AddComponent(pAnimator);

	pAnimator->SetBones(m_pMesh->GetBones());
	pAnimator->SetAnimClip(m_pMesh->GetAnimClip());
	//pAnimator->SetBoneTex(m_pMesh->GetBoneTex());

	return pNewObj;
}

