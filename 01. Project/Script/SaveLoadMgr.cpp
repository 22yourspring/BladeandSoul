#include "stdafx.h"
#include "SaveLoadMgr.h"

#include <Engine/SceneMgr.h>
#include <Engine/Scene.h>
#include <Engine/Layer.h>
#include <Engine/GameObject.h>
#include <Engine/Component.h>
#include <Engine/Script.h>
#include "ScriptMgr.h"

#include <Engine/CollisionMgr.h>
#include <Engine/ResMgr.h>
#include <Engine/Res.h>

#include <Engine/Transform.h>
#include <Engine/MeshRender.h>
#include <Engine/Camera.h>
#include <Engine/Collider2D.h>
#include <Engine/Animator2D.h>
#include <Engine/Light2D.h>




void CSaveLoadMgr::SaveResource(FILE* _pFile)
{
	UINT i = 0;
	for (; i < (UINT)RES_TYPE::END; ++i)
	{
		const map<wstring, CResource*>& mapRes = CResMgr::GetInst()->GetResource((RES_TYPE)i);

		if (i == (UINT)RES_TYPE::SHADER || i == (UINT)RES_TYPE::MESH)
			continue;
		
		// 리소스 타입
		size_t iSize = fwrite(&i, 4, 1, _pFile);

		// 리소스 개수
		int iResCount = 0;
		long rescountpos = ftell(_pFile);
		fwrite(&iResCount, 4, 1, _pFile);

		for (const auto& pair : mapRes)
		{
			if (1 <= pair.second->GetRefCount())
			{
				++iResCount;
				pair.second->SaveToScene(_pFile);
			}
		}

		long endpos = ftell(_pFile);
		fseek(_pFile, -(endpos - rescountpos), SEEK_CUR);	
		fwrite(&iResCount, 4, 1, _pFile);
		fseek(_pFile, 0, SEEK_END);		
	}

	// 리소스 타입
	fwrite(&i, 4, 1, _pFile);
}

void CSaveLoadMgr::SaveGameObject(CGameObject * _pObj, FILE * _pFile)
{
	// 이름 저장
	SaveWString(_pFile, _pObj->GetName());

	// Component 저장
	UINT i = 0;
	for (; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		CComponent* pCom = _pObj->GetComponent((COMPONENT_TYPE)i);
		if (nullptr != pCom)
		{
			pCom->SaveToScene(_pFile);
		}
	}

	// 컴포넌트 END 마감 정수값 저장
	fwrite(&i, 4, 1, _pFile);


	// 스크립트 저장
	const vector<CScript*> vecScript = _pObj->GetScripts();
	UINT iScriptCount = (UINT)vecScript.size();
	fwrite(&iScriptCount, sizeof(UINT), 1, _pFile);

	for (UINT i = 0; i < iScriptCount; ++i)
	{	
		SaveWString(_pFile, CScriptMgr::GetScriptName(vecScript[i]));
		vecScript[i]->SaveToScene(_pFile);
	}

	
	// Child Object 정보 저장
	const vector<CGameObject*>& vecChild = _pObj->GetChild();

	UINT iChildCount = (UINT)vecChild.size();
	fwrite(&iChildCount, 4, 1, _pFile);

	for (UINT i = 0; i < iChildCount; ++i)
	{
		SaveGameObject(vecChild[i], _pFile);
	}
}

void CSaveLoadMgr::SaveLayer(CLayer * _pLayer, FILE * _pFile)
{
	SaveWString(_pFile, _pLayer->GetName());

	const vector<CGameObject*>& vecParentObj = _pLayer->GetParentObj();

	// 오브젝트 개수 저장
	UINT iCount = (UINT)vecParentObj.size();
	fwrite(&iCount, 4, 1, _pFile);

	for (size_t i = 0; i < iCount; ++i)
	{
		SaveGameObject(vecParentObj[i], _pFile);
	}
}


void CSaveLoadMgr::SaveScene(const wstring & _strFilePath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"wb");
	
	// 리소스
	SaveResource(pFile);

	// Collision
	
	// Scene Object		
	CScene* pScene = CSceneMgr::GetInst()->GetCurScene();

	// Layer Save
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		SaveLayer(pScene->GetLayer(i), pFile);
	}

	fclose(pFile);
}

void CSaveLoadMgr::LoadResource(FILE * _pFile)
{
	UINT i = 0;
	int iResCount = 0;

	while (true)
	{		
		fread(&i, 4, 1, _pFile);

		if ((UINT)RES_TYPE::END == i)
			break;

		fread(&iResCount, 4, 1, _pFile);

		for (int iRes = 0; iRes < iResCount; ++iRes)
		{			
			switch ((RES_TYPE)i)
			{
			case RES_TYPE::MATERIAL:
			{
				Ptr<CMaterial> pRes = new CMaterial;
				if (!pRes->LoadFromScene<CMaterial>(_pFile))
				{
					delete pRes.GetPointer();					
				}
				else
					CResMgr::GetInst()->AddRes<CMaterial>(pRes->GetName(), pRes);
			}
				break;			

			case RES_TYPE::TEXTURE:
			{
				Ptr<CTexture> pRes = new CTexture;
				if (!pRes->LoadFromScene<CTexture>(_pFile))
				{
					delete pRes.GetPointer();					
				}
				else
					CResMgr::GetInst()->AddRes<CTexture>(pRes->GetName(), pRes);
			}

				break;
			case RES_TYPE::SOUND:
			{
				Ptr<CSound> pRes = new CSound;
				if (!pRes->LoadFromScene<CSound>(_pFile))
				{
					delete pRes.GetPointer();					
				}
				else
					CResMgr::GetInst()->AddRes<CSound>(pRes->GetName(), pRes);
			}
				break;
			}			
		}
	}
}


void CSaveLoadMgr::LoadGameObject(CGameObject * _pObj, FILE * _pFile)
{
	// 이름 
	wstring strName = LoadWString(_pFile);
	_pObj->SetName(strName);

	// Component 
	COMPONENT_TYPE iComType = (COMPONENT_TYPE)0;
	fread(&iComType, 4, 1, _pFile);

	while (iComType != COMPONENT_TYPE::END)
	{
		CComponent* pCom = nullptr;
		switch (iComType)
		{
		case COMPONENT_TYPE::TRANSFORM:
			pCom = new CTransform;
			break;
		case COMPONENT_TYPE::MESHRENDER:
			pCom = new CMeshRender;
			break;
		case COMPONENT_TYPE::CAMERA:
			pCom = new CCamera;
			break;
		case COMPONENT_TYPE::COLLIDER2D:
			pCom = new CCollider2D;
			break;
		case COMPONENT_TYPE::COLLIDER3D:
			pCom = new CCollider2D;
			break;
		case COMPONENT_TYPE::ANIMATOR2D:
			pCom = new CAnimator2D;
			break;
		case COMPONENT_TYPE::ANIMATOR3D:
			break;
		case COMPONENT_TYPE::LIGHT2D:
			pCom = new CLight2D;
			break;
		case COMPONENT_TYPE::LIGHT3D:
			break;		
		}

		pCom->LoadFromScene(_pFile);
		_pObj->AddComponent(pCom);

		fread(&iComType, 4, 1, _pFile);
	}
	   
	// 스크립트
	UINT iScriptCount = 0;	
	fread(&iScriptCount, sizeof(UINT), 1, _pFile);

	for (UINT i = 0; i < iScriptCount; ++i)
	{
		wstring strScriptName = LoadWString(_pFile); 
		CScript* pScript = CScriptMgr::GetScript(strScriptName);
		pScript->LoadFromScene(_pFile);
		_pObj->AddComponent(pScript);
	}

	// Child Object 정보	
	UINT iChildCount = 0;
	fread(&iChildCount, 4, 1, _pFile);

	for (UINT i = 0; i < iChildCount; ++i)
	{
		CGameObject* pChild = new CGameObject;
		LoadGameObject(pChild, _pFile);
		_pObj->AddChild(pChild);
	}
}


void CSaveLoadMgr::LoadLayer(CLayer * _pLayer, FILE * _pFile)
{
	wstring strName;
	strName = LoadWString(_pFile);
	_pLayer->SetName(strName);

	const vector<CGameObject*>& vecParentObj = _pLayer->GetParentObj();

	// 오브젝트 개수 저장
	UINT iCount = 0;
	fread(&iCount, 4, 1, _pFile);

	for (size_t i = 0; i < iCount; ++i)
	{
		CGameObject* pObj = new CGameObject;
		LoadGameObject(pObj, _pFile);
		_pLayer->AddGameObject(pObj);
	}
}


void CSaveLoadMgr::LoadScene(const wstring & _strFilePath)
{
	// 리소스
	// ref 1 미만 날리기
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

	LoadResource(pFile);

	// collion

	// scene object
	CScene* pCurScene = new CScene;
	
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		LoadLayer(pCurScene->GetLayer(i), pFile);		
	}

	CSceneMgr::GetInst()->ChangeScene(pCurScene);

	fclose(pFile);
}



