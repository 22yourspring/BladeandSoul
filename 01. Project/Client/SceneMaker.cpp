#include "stdafx.h"
#include "SceneMaker.h"

#include <Engine/global.h>

#include <Engine/Scene.h>
#include <Engine/SceneMgr.h>
#include <Engine/Ptr.h>
#include <Engine/Texture.h>
#include <Engine/ResMgr.h>
#include <Engine/GameObject.h>

#include <Engine/Transform.h>
#include <Engine/Camera.h>
#include <Engine/RenderMgr.h>
#include <Engine/Light3D.h>
#include <Engine/BoneSocket.h>
#include <Engine/Terrain.h>

#include <Engine/Animator3D.h>
#include <Engine/MeshRender.h>
#include <Engine/EventMgr.h>

#include <Script/MainCamScript.h>
#include <Engine/Collider3D.h>

#include <Engine/Core.h>

#include <Script/PlayerScript.h>
#include <Script/AIScript.h>
#include <Script/BrushScript.h>
#include <Script/BillboardScript.h>

CGameObject* CSceneMaker::m_pMainCam = nullptr;

CSceneMaker::CSceneMaker()
{
}

CSceneMaker::~CSceneMaker()
{
}


void CSceneMaker::LoadResource()
{
	wstring strContent = CPathMgr::GetResPath();

	LoadResource(strContent);
}

void CSceneMaker::LoadResource(wstring & _strFolderPath)
{
	wstring strPath = _strFolderPath + L"\\*.*";
	WIN32_FIND_DATA tData = {};
	HANDLE hFindHandle = FindFirstFile(strPath.c_str(), &tData);
	wstring strFileName;

	while (true)
	{
		strFileName = tData.cFileName;

		if (tData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (strFileName != L"." && strFileName != L"..")
			{
				//	std::wstring name = _strFolderPath.append(tData.cFileName) + L"\\";
				std::wstring name = _strFolderPath + tData.cFileName + L"\\";

				LoadResource(name);
			}
		}
		else
		{
			Load(_strFolderPath + strFileName);
		}


		if (!FindNextFile(hFindHandle, &tData))
			break;
	}

	FindClose(hFindHandle);
}

void CSceneMaker::Load(const wstring & _strFullPath)
{
	wstring strExt = CPathMgr::GetExt(_strFullPath.c_str());
	wstring strRelativePath = CPathMgr::GetRelativePath(_strFullPath.c_str());

	if (strExt == L".png" || strExt == L".tga" || strExt == L".dds" || strExt == L".jpeg" || strExt == L".jpg" || strExt == L".bmp")
	{
		CResMgr::GetInst()->Load<CTexture>(strRelativePath, strRelativePath);
	}
	else if (strExt == L".mtrl")
	{
		CResMgr::GetInst()->Load<CMaterial>(strRelativePath, strRelativePath);
	}
	else if (strExt == L".mp3" || strExt == L".ogg" || strExt == L".wav")
	{
		CResMgr::GetInst()->Load<CSound>(strRelativePath, strRelativePath);
	}
	else if (strExt == L".mesh")
	{
		CResMgr::GetInst()->Load<CMesh>(strRelativePath, strRelativePath);
	}
}




void CSceneMaker::init()
{
	CSceneMaker::SceneInit();
	CSceneMaker::CreateScene();
}

void CSceneMaker::SceneInit()
{
	LoadResource();
}

void CSceneMaker::CreateScene()
{
	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	// 필요한 리소스 로딩
	Ptr<CTexture> pTexture = CResMgr::GetInst()->Load<CTexture>(L"Texture\\ironman.png", L"Texture\\ironman.png");
	Ptr<CTexture> pTexture1 = CResMgr::GetInst()->Load<CTexture>(L"Texture\\1.png", L"Texture\\1.png");
	Ptr<CTexture> pTexture2 = CResMgr::GetInst()->Load<CTexture>(L"Texture\\child.png", L"Texture\\child.png");
	Ptr<CTexture> pAtlas = CResMgr::GetInst()->Load<CTexture>(L"Texture\\Animation\\Player\\link_0.png", L"Texture\\Animation\\Player\\link_0.png");
	Ptr<CTexture> pLightIconTex = CResMgr::GetInst()->Load<CTexture>(L"Texture\\LightIcon.png", L"Texture\\LightIcon.png");
	Ptr<CTexture> pBackgroundTex = CResMgr::GetInst()->Load<CTexture>(L"Texture\\Background.png", L"Texture\\Background.png");

	// Layer 추가
	pCurScene->SetLayerName(0, L"Default");
	pCurScene->SetLayerName(1, L"Player");
	pCurScene->SetLayerName(2, L"Monster");
	pCurScene->SetLayerName(3, L"Camera");
	pCurScene->SetLayerName(4, L"Map Object");
	pCurScene->SetLayerName(5, L"Decal");
	pCurScene->SetLayerName(6, L"Terrain");
	pCurScene->SetLayerName(7, L"SkyBox");

	CGameObject* pObject = nullptr;
	CGameObject* pCameraObject = nullptr;

	// Camera Object 만들기
	pCameraObject = new CGameObject;
	pCameraObject->SetName(L"Main Camera");
	pCameraObject->AddComponent(new CTransform());
	pCameraObject->AddComponent(new CCamera);
	pCameraObject->AddComponent(new CMainCamScript);

	pCameraObject->Camera()->SetLayerAllCheck();
	pCurScene->AddGameObject(L"Camera", pCameraObject, false);
	pCameraObject->Camera()->SetFar(500000.f);



#ifdef CLIENT
	m_pMainCam = pCameraObject;
	pCameraObject->Camera()->SetWidth(3000.f);
	pCameraObject->Camera()->SetHeight(3000.f);
#else
	((CMainFrame*)m_pMainWnd)->GetGameView()->SetMainCam(pCameraObject);
#endif



	CRenderMgr::GetInst()->RegisterMainCamera(pCameraObject->Camera());


	LIGHT_TYPE m_eLightStyle = LIGHT_TYPE::DIR;

	if (m_eLightStyle == LIGHT_TYPE::DIR)
	{
		// 광원 추가하기
		pObject = new CGameObject;
		pObject->SetName(L"_Directional Light");
		pObject->AddComponent(new CTransform());
		pObject->AddComponent(new CMeshRender());
		pObject->AddComponent(new CLight3D);

		pObject->Light3D()->SetLightType(LIGHT_TYPE::DIR);
		pObject->Light3D()->SetDiffuseColor(Vec3(1.f, 1.f, 1.f));
		pObject->Light3D()->SetSpecular(Vec3(1.f, 1.f, 1.f));
		pObject->Light3D()->SetAmbient(Vec3(0.15f, 0.15f, 0.15f));
		pObject->Light3D()->SetLightDir(Vec3(1.f, -1.f, 1.f));
		pObject->Light3D()->SetLightRange(300.f);

		pObject->Transform()->SetLocalPos(Vec3(-1000.f, 1000.f, -1000.f));
		pCurScene->AddGameObject(L"Light", pObject, false);
	}
	else if (m_eLightStyle == LIGHT_TYPE::POINT)
	{
		pObject = new CGameObject;
		pObject->SetName(L"Point Light");
		pObject->AddComponent(new CTransform());
		pObject->AddComponent(new CMeshRender());
		pObject->AddComponent(new CLight3D);

		pObject->Light3D()->SetLightType(LIGHT_TYPE::POINT);
		pObject->Light3D()->SetDiffuseColor(Vec3(1.f, 1.f, 1.f));
		pObject->Light3D()->SetSpecular(Vec3(1.f, 1.f, 1.f));
		pObject->Light3D()->SetAmbient(Vec3(0.15f, 0.15f, 0.15f));
		pObject->Light3D()->SetLightDir(Vec3(1.f, -1.f, 1.f));
		pObject->Light3D()->SetLightRange(300.f);


		pObject->Transform()->SetLocalPos(Vec3(ObjectX, 168.8f + ObjectY + 200.f, ObjectZ));
		pCurScene->AddGameObject(L"Light", pObject, false);
	}


	//pObject = pObject->Clone();
	//pObject->SetName(L"Point Light 2");
	//pObject->Transform()->SetLocalPos(Vec3(100.f, 200.f, 0.f));
	//pObject->Light3D()->SetDiffuseColor(Vec3(1.f, 0.2f, 0.2f));
	//pCurScene->AddGameObject(L"Default", pObject, false);	

	//pObject = pObject->Clone();
	//pObject->Light3D()->SetLightType(LIGHT_TYPE::POINT);
	//pObject->Light3D()->SetLightDir(Vec3(-1.f, -1.f, 0.f));
	//pObject->Light3D()->SetDiffuseColor(Vec3(0.2f, 0.2f, 1.f));
	//pCurScene->AddGameObject(L"Default", pObject, false);

	// SkyBox 추가하기
	pObject = new CGameObject;
	pObject->SetName(L"SkyBox");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform());
	pObject->AddComponent(new CMeshRender());

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"SphereMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"SkyboxMtrl"));
	pTexture = CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Skybox\\Modif_tim-barton-skybox-7-equirectangular.png");
	//	pTexture = CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Skybox\\Modif_tim-barton-hydra-nebula-with-watermark.png");
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, &pTexture);
	pObject->SetADD_Render(RENDER_ADD::RENDER_CLIENT);
	pCurScene->AddGameObject(L"SkyBox", pObject, false);


	// 2D Object 추가하기
	pObject = new CGameObject;
	pObject->SetName(L"_Moon");
	pObject->FrustumCheck(false);
	pObject->AddComponent(new CTransform());
	pObject->Transform()->SetLocalPos(Vec3(-1000.f, 2000.f, 0.f));
	pObject->Transform()->SetLocalScale(Vec3(1000.f, 1000.f, 1000.f));

	pObject->AddComponent(new CMeshRender());

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"MoonMtrl"));

	pTexture = CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Moon_Test.png");

	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, &pTexture);
	pObject->AddComponent(new CBillboardScript);

	pCurScene->AddGameObject(L"Map Object", pObject, false);


	// GameObject 를 TestScene 에 추가하기
	// pObject = new CGameObject;
	// pObject->SetName(L"Plane");
	// 
	// pObject->AddComponent(new CTransform);
	// pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	// pObject->Transform()->SetLocalScale(Vec3(1000.f, 1000.f, 1000.f));
	// pObject->Transform()->SetLocalRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
	// 
	// pObject->AddComponent(new CMeshRender);
	// pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	// pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std3DMtrl"));
	// //pObject->MeshRender()->ShadowCheck(true);
	// 
	// pTexture = CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Tile\\TILE_02.tga");
	// pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, &pTexture);
	// 
	// pTexture = CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Tile\\TILE_02_N.tga");
	// pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_1, &pTexture);
	// 
	// pCurScene->AddGameObject(L"Default", pObject, false);

	// ====================
	// Particle Object 생성
	// ====================
	//pObject = new CGameObject;
	//pObject->SetName(L"Particle1");
	//pObject->AddComponent(new CTransform);	
	//pObject->AddComponent(new CParticleSystem);
	//
	//pObject->FrustumCheck(false);
	//pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));		
	//
	//pCurScene->AddGameObject(L"Default", pObject, false);

	// ==============
	// FBX 파일 로드
	// ==============

	Ptr<CMeshData> pMeshData;
	//	pMeshData = CResMgr::GetInst()->LoadFBX(L"FBX\\BladeMaster_Martial_Anim.fbx");
	//	pMeshData->Save(pMeshData->GetPath());



	// =============
	// MeshData 로드
	// =============

	CGameObject* pBladeMaster = nullptr;
	CGameObject* pKungfuMaster = nullptr;
	bool MapObject = true;
	if (MapObject)
	{
		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Area_Fence_C.mdat", L"MeshData\\Area_Fence_C.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Area_Fence_C");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(0.36781f, 0.2688f, 0.36781f));
		pObject->Transform()->SetLocalPos(Vec3(0.302f + ObjectX, 170.483f + ObjectY, -0.258f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Area_FenceSub_C.mdat", L"MeshData\\Area_FenceSub_C.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Area_FenceSub_C");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(0.36655f, 0.31004f, 0.36655f));
		pObject->Transform()->SetLocalPos(Vec3(0.302f + ObjectX, 171.057f + ObjectY, 0.008f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_Cliff_001_03_01_new4.mdat", L"MeshData\\Arena_Cliff_001_03_01_new4.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_Cliff_001_03_01_new4");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(0.52107f, 0.52107f, 0.52107f));
		pObject->Transform()->SetLocalPos(Vec3(7713.673f + ObjectX, 704.242f + ObjectY + CliffY, 738.16f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_Cliff_001_03_01_new005.mdat", L"MeshData\\Arena_Cliff_001_03_01_new005.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_Cliff_001_03_01_new005");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(0.52107f, 0.52107f, 0.52107f));
		pObject->Transform()->SetLocalPos(Vec3(-63.355f + ObjectX, -333.639f + ObjectY + CliffY, -5322.071f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_etc_Slate.mdat", L"MeshData\\Arena_etc_Slate.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_etc_Slate");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(0.79599f, 0.79599f, 0.79599f));
		pObject->Transform()->SetLocalPos(Vec3(4500.0f + ObjectX, -59.244f + ObjectY, 1300.998f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 90.f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_etc_001_floor_L.mdat", L"MeshData\\Arena_etc_001_floor_L.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_etc_001_floor_L");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(0.79467f, 0.63811f, 0.79467f));
		pObject->Transform()->SetLocalPos(Vec3(2701.262f + ObjectX, 565.116f + ObjectY, -289.308f + ObjectZ));
		//	pObject->Transform()->SetLocalRot(Vec3(0.f, 90.f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_etc_001_floor_R.mdat", L"MeshData\\Arena_etc_001_floor_R.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_etc_001_floor_R");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(0.79467f, 0.63811f, 0.79467f));
		pObject->Transform()->SetLocalPos(Vec3(-2055.854f + ObjectX, 565.116f + ObjectY, 57.625f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 90.f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_etc_Altar_L.mdat", L"MeshData\\Arena_etc_Altar_L.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_etc_Altar_L");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(0.41854f, 0.41854f, 0.41854f));
		pObject->Transform()->SetLocalPos(Vec3(2479.861f + ObjectX, 576.621f + ObjectY, 0.0f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 90.000f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_etc_Altar_R.mdat", L"MeshData\\Arena_etc_Altar_R.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_etc_Altar_R");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(0.41854f, 0.41854f, 0.41854f));
		pObject->Transform()->SetLocalPos(Vec3(-2492.687f + ObjectX, 576.621f + ObjectY, -1.642f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 270.000f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_etc_Bimu_B1_C.mdat", L"MeshData\\Arena_etc_Bimu_B1_C.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_etc_Bimu_B1_C");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(1.27004f, 1.02902f, 1.27004f));
		pObject->Transform()->SetLocalPos(Vec3(-3.42f + ObjectX, 98.014f + ObjectY, -0.258f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_etc_Bimu_B1_L.mdat", L"MeshData\\Arena_etc_Bimu_B1_L.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_etc_Bimu_B1_L");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(1.23422f, 1.f, 1.23422f));
		pObject->Transform()->SetLocalPos(Vec3(2333.627f + ObjectX, 500.188f + ObjectY, -0.258f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_etc_Bimu_B1_R.mdat", L"MeshData\\Arena_etc_Bimu_B1_R.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_etc_Bimu_B1_R");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(1.23422f, 1.000f, 1.23422f));
		pObject->Transform()->SetLocalPos(Vec3(-2346.453f + ObjectX, 500.188f + ObjectY, -1.385f + ObjectZ));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		//
		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_etc_Bimu_B1_L.mdat", L"MeshData\\Arena_etc_Bimu_B1_L.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_etc_Bimu_B1_2_L");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(1.15000f, 0.7f, 1.2000f));
		pObject->Transform()->SetLocalPos(Vec3(2333.627f + ObjectX, 500.188f + ObjectY, -0.258f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(180.f * _DEGREE, 0.f, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_etc_Bimu_B1_R.mdat", L"MeshData\\Arena_etc_Bimu_B1_R.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_etc_Bimu_B1_2_R");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(1.15000f, 0.7f, 1.2000f));
		pObject->Transform()->SetLocalPos(Vec3(-2346.453f + ObjectX, 500.188f + ObjectY, -1.385f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(180.f * _DEGREE, 0.f, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);
		//

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_etc_Bimu_B2_C.mdat", L"MeshData\\Arena_etc_Bimu_B2_C.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_etc_Bimu_B2_C");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(1.26000f, 1.02902f, 1.26000f));
		pObject->Transform()->SetLocalPos(Vec3(-3.42f + ObjectX, 60.839f + ObjectY, -0.258f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(180.f * _DEGREE, 0.f, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_etc_Bimu_B2_L.mdat", L"MeshData\\Arena_etc_Bimu_B2_L.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_etc_Bimu_B2_L");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(1.1055f, 0.90381f, 1.1055f));
		pObject->Transform()->SetLocalPos(Vec3(2333.627f + ObjectX, 398.741f + ObjectY, -0.258f + ObjectZ));
		//	pObject->Transform()->SetLocalRot(Vec3(0.f, 180.f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_etc_Bimu_B2_R.mdat", L"MeshData\\Arena_etc_Bimu_B2_R.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_etc_Bimu_B2_R");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(1.1055f, 0.90381f, 1.1055f));
		pObject->Transform()->SetLocalPos(Vec3(-2346.453f + ObjectX, 398.741f + ObjectY, -1.385f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_etc_Bimu_B3_C.mdat", L"MeshData\\Arena_etc_Bimu_B3_C.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_etc_Bimu_B3_C");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(1.08999f, 0.88314f, 1.08999f));
		pObject->Transform()->SetLocalPos(Vec3(-3.42f + ObjectX, -47.898f + ObjectY, -0.258f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(180.f * _DEGREE, 0.f, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_etc_Bimu_B3_L.mdat", L"MeshData\\Arena_etc_Bimu_B3_L.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_etc_Bimu_B3_L");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(1.1155f, 0.90381f, 1.1155f));
		pObject->Transform()->SetLocalPos(Vec3(2333.627f + ObjectX, 367.505f + ObjectY, -0.258f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(180.f * _DEGREE, 0.f, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_etc_Bimu_B3_R.mdat", L"MeshData\\Arena_etc_Bimu_B3_R.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_etc_Bimu_B3_R");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(111.55f * 0.01f, 90.381f * 0.01f, 111.55f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(-2346.453f + ObjectX, 367.505f + ObjectY, -1.385f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(180.f * _DEGREE, 0.f, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_etc_Bimu_B4_C.mdat", L"MeshData\\Arena_etc_Bimu_B4_C.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_etc_Bimu_B4_C");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(108.899f * 0.01f, 88.314f * 0.01f, 108.899f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(-1.569f + ObjectX, -119.985f + ObjectY, -0.258f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(180.f * _DEGREE, 0.f, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_etc_Bimu_B4_L.mdat", L"MeshData\\Arena_etc_Bimu_B4_L.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_etc_Bimu_B4_L");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(98.143f * 0.01f, 79.599f * 0.01f, 98.143f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(2333.627f + ObjectX, 296.979f + ObjectY, -0.258f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(180.f * _DEGREE, 0.f, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_etc_Bimu_B4_R.mdat", L"MeshData\\Arena_etc_Bimu_B4_R.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_etc_Bimu_B4_R");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(98.143f * 0.01f, 79.599f * 0.01f, 98.143f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(-2346.453f + ObjectX, 296.979f + ObjectY, -1.385f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(180.f * _DEGREE, 0.f, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_etc_Cross_floor_C.mdat", L"MeshData\\Arena_etc_Cross_floor_C.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_etc_Cross_floor_C");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(75.122f * 0.01f, 73.67f * 0.01f, 87.064f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(58.027f + ObjectX, 159.1f + ObjectY, -284.557f + ObjectZ));
		//	pObject->Transform()->SetLocalRot(Vec3(0.f, 90.f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_etc_Floor_C.mdat", L"MeshData\\Arena_etc_Floor_C.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_etc_Floor_C");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(12.413f * 0.01f, 3.813f * 0.01f, 12.413f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(0.297f + ObjectX, 168.061f + ObjectY, -0.258f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_far01.mdat", L"MeshData\\Arena_far01.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_far01");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(16954.809f * 0.0008f, 16954.809f * 0.0008f, 16954.809f * 0.0008f));
		pObject->Transform()->SetLocalPos(Vec3(0.0f + ObjectX, -3000.f + ObjectY, 0.0f + ObjectZ));
		//	pObject->Transform()->SetLocalRot(Vec3(0.f, 90.f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_Floor_2F_L.mdat", L"MeshData\\Arena_Floor_2F_L.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_Floor_2F_L");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(55.169f * 0.01f, 44.3f * 0.01f, 55.169f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(2333.627f + ObjectX, 569.557f + ObjectY, -0.258f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_Floor_2F_R.mdat", L"MeshData\\Arena_Floor_2F_R.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_Floor_2F_R");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(55.169f * 0.01f, 44.3f * 0.01f, 55.169f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(-2346.453f + ObjectX, 569.557f + ObjectY, -1.385f + ObjectZ));
		//	pObject->Transform()->SetLocalRot(Vec3(0.f, -180.f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_Statue_003_04.mdat", L"MeshData\\Arena_Statue_003_04.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_Statue_003_04");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(522.335f * 0.01f, 522.335f * 0.01f, 522.335f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(4500.0f + ObjectX, 103.049f + ObjectY, 1370.000000f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 90.f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_Statue_003_005.mdat", L"MeshData\\Arena_Statue_003_005.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_Statue_003_005");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(522.335f * 0.01f, 522.335f * 0.01f, 522.335f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(4500.0f + ObjectX, 103.049f + ObjectY, -1110.000000f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 90.f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_Statue_003_006.mdat", L"MeshData\\Arena_Statue_003_006.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_Statue_003_006");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(522.335f * 0.01f, 522.335f * 0.01f, 522.335f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(-4500.0f + ObjectX, 103.049f + ObjectY, 1115.000000f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, -90.f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_Statue_003_007.mdat", L"MeshData\\Arena_Statue_003_007.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_Statue_003_007");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(522.335f * 0.01f, 522.335f * 0.01f, 522.335f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(-4500.0f + ObjectX, 103.049f + ObjectY, -1380.000000f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, -90.f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_Statue001_God_L.mdat", L"MeshData\\Arena_Statue001_God_L.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_Statue001_God_L");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(30.547f * 0.01f, 30.547f * 0.01f, 30.547f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(2523.777f + ObjectX, 605.363f + ObjectY, 0.0f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 90.f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_Statue001_God_R.mdat", L"MeshData\\Arena_Statue001_God_R.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_Statue001_God_R");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(30.547f * 0.01f, 30.547f * 0.01f, 30.547f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(-2536.604f + ObjectX, 605.363f + ObjectY, -1.642f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, -90.f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_Wait_FloorL.mdat", L"MeshData\\Arena_Wait_FloorL.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_Wait_FloorL");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(32.312f * 0.01f, 25.946f * 0.01f, 32.312f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(2318.747f + ObjectX, 603.836f + ObjectY, -0.258f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Arena_Wait_FloorR.mdat", L"MeshData\\Arena_Wait_FloorR.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Arena_Wait_FloorR");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(32.312f * 0.01f, 25.946f * 0.01f, 32.312f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(-2331.573f + ObjectX, 603.836f + ObjectY, -1.385f + ObjectZ));
		//	pObject->Transform()->SetLocalRot(Vec3(0.f, -180.f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\chain.mdat", L"MeshData\\chain.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"chain");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(16.735f * 0.01f, 16.735f * 0.01f, 16.735f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(1505.779f + ObjectX, 254.395f + ObjectY, 184.491f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(-20.f * _DEGREE, 90.f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Cliff_001_02.mdat", L"MeshData\\Cliff_001_02.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Cliff_001_02");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(100.0f * 0.01f, 100.0f * 0.01f, 100.0f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(6367.125f + ObjectX, 547.095f + ObjectY + CliffY, -2800.598f + ObjectZ));
		//	pObject->Transform()->SetLocalRot(Vec3(0.f, -170.f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Cliff_001_02_new.mdat", L"MeshData\\Cliff_001_02_new.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Cliff_001_02_new");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(73.118f * 0.01f, 73.118f * 0.01f, 73.118f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(5729.613f + ObjectX, -40.705f + ObjectY + CliffY, -3881.743f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Cliff_001_02_T.mdat", L"MeshData\\Cliff_001_02_T.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Cliff_001_02_T");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(100.f * 0.01f, 100.f * 0.01f, 100.f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(5810.717f + ObjectX, 799.982f + ObjectY + CliffY, 4047.907f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Cliff_001_03.mdat", L"MeshData\\Cliff_001_03.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Cliff_001_03");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(100.f * 0.01f, 100.f * 0.01f, 100.f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(6511.182f + ObjectX, -952.541f + ObjectY + CliffY, 2250.756f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Cliff_001_03_01.mdat", L"MeshData\\Cliff_001_03_01.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Cliff_001_03_01");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(100.f * 0.01f, 100.f * 0.01f, 100.f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(3312.708f + ObjectX, 245.585f + ObjectY + CliffY, 4105.398f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, -100.f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Cliff_001_03_01_new3.mdat", L"MeshData\\Cliff_001_03_01_new3.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Cliff_001_03_01_new3");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(100.f * 0.01f, 100.f * 0.01f, 100.f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(1515.387f + ObjectX, -786.484f + ObjectY + CliffY, -2399.71f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Cliff_001_03_01_new004.mdat", L"MeshData\\Cliff_001_03_01_new004.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Cliff_001_03_01_new004");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(100.f * 0.01f, 100.f * 0.01f, 100.f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(1937.627f + ObjectX, -786.484f + ObjectY + CliffY, 2763.508f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Cliff_001_03_002.mdat", L"MeshData\\Cliff_001_03_002.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Cliff_001_03_002");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(100.f * 0.01f, 100.f * 0.01f, 100.f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(3479.341f + ObjectX, 245.585f + ObjectY + CliffY, 4166.034f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 100.f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Cliff_001_03_003.mdat", L"MeshData\\Cliff_001_03_003.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Cliff_001_03_003");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(100.f * 0.01f, 100.f * 0.01f, 100.f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(5910.154f + ObjectX, -1001.144f + ObjectY + CliffY, 2290.506f + ObjectZ));
		//	pObject->Transform()->SetLocalRot(Vec3(0.f, 160.f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Cliff_001_03_new.mdat", L"MeshData\\Cliff_001_03_new.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Cliff_001_03_new");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(100.f * 0.01f, 100.f * 0.01f, 100.f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(2296.002f + ObjectX, -1400.728f + ObjectY + CliffY, 4682.545f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 40.f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Cliff_001_03_new2.mdat", L"MeshData\\Cliff_001_03_new2.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Cliff_001_03_new2");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(100.f * 0.01f, 100.f * 0.01f, 100.f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(-472.036f + ObjectX, -1880.446f + ObjectY + CliffY, 5171.965f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 180.f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Cliff_001_03_Opt.mdat", L"MeshData\\Cliff_001_03_Opt.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Cliff_001_03_Opt");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(100.f * 0.01f, 100.f * 0.01f, 100.f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(-3301.472f + ObjectX, -1319.835f + ObjectY + CliffY, 4309.955f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, -20.f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Cliff_001_03_T.mdat", L"MeshData\\Cliff_001_03_T.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Cliff_001_03_T");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(100.f * 0.01f, 100.f * 0.01f, 100.f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(3436.231f + ObjectX, -1623.561f + ObjectY + CliffY, -4170.944f + ObjectZ));
		//	pObject->Transform()->SetLocalRot(Vec3(0.f, 60.f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Cliff_001_04.mdat", L"MeshData\\Cliff_001_04.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Cliff_001_04");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(100.f * 0.01f, 100.f * 0.01f, 100.f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(773.32f + ObjectX, -2263.174f + ObjectY + CliffY, -4148.177f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Cliff_001_04_new.mdat", L"MeshData\\Cliff_001_04_new.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Cliff_001_04_new");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(100.f * 0.01f, 100.f * 0.01f, 100.f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(-1652.34f + ObjectX, -914.289f + ObjectY + CliffY, -3703.172f + ObjectZ));
		//	pObject->Transform()->SetLocalRot(Vec3(0.f, -180.f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Cliff_001_05.mdat", L"MeshData\\Cliff_001_05.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Cliff_001_05");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(100.f * 0.01f, 100.f * 0.01f, 100.f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(-5373.397f + ObjectX, -1858.976f + ObjectY + CliffY, 3028.102f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Cliff_001_05_new.mdat", L"MeshData\\Cliff_001_05_new.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Cliff_001_05_new");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(100.f * 0.01f, 100.f * 0.01f, 100.f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(-3864.17f + ObjectX, -2322.65f + ObjectY + CliffY, -3268.942f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Cliff_001_06.mdat", L"MeshData\\Cliff_001_06.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Cliff_001_06");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(100.f * 0.01f, 100.f * 0.01f, 100.f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(-7308.122f + ObjectX, -1316.896f + ObjectY + CliffY, 786.039f + ObjectZ));
		//	pObject->Transform()->SetLocalRot(Vec3(0.f, -120.f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Cliff_001_010.mdat", L"MeshData\\Cliff_001_010.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Cliff_001_010");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(100.f * 0.01f, 100.f * 0.01f, 100.f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(7034.46f + ObjectX, -747.381f + ObjectY + CliffY, -792.681f + ObjectZ));
		//	pObject->Transform()->SetLocalRot(Vec3(0.f, -90.f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Cliff_004_005.mdat", L"MeshData\\Cliff_004_005.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Cliff_004_005");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(100.f * 0.01f, 100.f * 0.01f, 100.f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(-6618.249f + ObjectX, -1386.477f + ObjectY + CliffY, -1174.787f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Haetae.mdat", L"MeshData\\Haetae.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Haetae_C_L");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(16.f, 16.f, 16.f));
		pObject->Transform()->SetLocalPos(Vec3(400.f + ObjectX, -1350.0f + ObjectY, -0.258f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Haetae.mdat", L"MeshData\\Haetae.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Haetae_C_R");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(16.f, 16.f, 16.f));
		pObject->Transform()->SetLocalPos(Vec3(-410.f + ObjectX, -1350.0f + ObjectY, -0.258f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 180.f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Haetae.mdat", L"MeshData\\Haetae.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Haetae_L");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(16.f, 16.f, 16.f));
		pObject->Transform()->SetLocalPos(Vec3(1920.f + ObjectX, -900.f + ObjectY, -0.258f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 180.f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Haetae.mdat", L"MeshData\\Haetae.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"Haetae_R");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(16.f, 16.f, 16.f));
		pObject->Transform()->SetLocalPos(Vec3(-1930.f + ObjectX, -900.f + ObjectY, -0.258f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);




		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\ID_Bossroom_Column_L.mdat", L"MeshData\\ID_Bossroom_Column_L.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"ID_Bossroom_Column_L");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(7.482f * 0.01f, 7.482f * 0.01f, 7.482f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(2582.981f + ObjectX, 694.462f + ObjectY, 149.292f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 270.0f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\ID_Bossroom_Column_R.mdat", L"MeshData\\ID_Bossroom_Column_R.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"ID_Bossroom_Column_R");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(7.482f * 0.01f, 7.482f * 0.01f, 7.482f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(-2595.807f + ObjectX, 694.462f + ObjectY, 115.218f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 90.f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\ID_Bossroom_DecoRoof_L_L.mdat", L"MeshData\\ID_Bossroom_DecoRoof_L_L.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"ID_Bossroom_DecoRoof_L_L");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(44.483f * 0.01f, 44.483f * 0.01f, 44.483f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(2574.525f + ObjectX, 803.473f + ObjectY, 0.0f + ObjectZ));
		//	pObject->Transform()->SetLocalRot(Vec3(0.f, 90.f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\ID_Bossroom_DecoRoof_L_R.mdat", L"MeshData\\ID_Bossroom_DecoRoof_L_R.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"ID_Bossroom_DecoRoof_L_R");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(44.483f * 0.01f, 44.483f * 0.01f, 44.483f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(2591.989f + ObjectX, 803.473f + ObjectY, 0.0f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 180.f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\ID_Bossroom_DecoRoof_R_L.mdat", L"MeshData\\ID_Bossroom_DecoRoof_R_L.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"ID_Bossroom_DecoRoof_R_L");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(44.483f * 0.01f, 44.483f * 0.01f, 44.483f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(-2604.815f + ObjectX, 803.473f + ObjectY, -1.642f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\ID_Bossroom_DecoRoof_R_R.mdat", L"MeshData\\ID_Bossroom_DecoRoof_R_R.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"ID_Bossroom_DecoRoof_R_R");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(44.483f * 0.01f, 44.483f * 0.01f, 44.483f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(-2587.351f + ObjectX, 803.473f + ObjectY, -1.642f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 180.000f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\NPC_ArenaBoard_L.mdat", L"MeshData\\NPC_ArenaBoard_L.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"NPC_ArenaBoard_L");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(50.873f * 0.01f, 50.873f * 0.01f, 50.873f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(2448.946f + ObjectX, 571.153f + ObjectY, -233.244f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 0.f, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);

		pMeshData = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\NPC_ArenaBoard_R.mdat", L"MeshData\\NPC_ArenaBoard_R.mdat");
		pObject = pMeshData->Instantiate();
		pObject->SetName(L"NPC_ArenaBoard_R");
		pObject->FrustumCheck(true);
		pObject->Transform()->SetLocalScale(Vec3(50.873f * 0.01f, 50.873f * 0.01f, 50.873f * 0.01f));
		pObject->Transform()->SetLocalPos(Vec3(-2461.772f + ObjectX, 571.153f + ObjectY, 234.887f + ObjectZ));
		pObject->Transform()->SetLocalRot(Vec3(0.f, 180.f * _DEGREE, 0.f));
		pObject->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Map Object", pObject, false);
	}

	Ptr<CMeshData> pMeshData2;
	//	pMeshData2 = CResMgr::GetInst()->LoadFBX(L"FBX\\Move_BladeMaster_JinM_Animset.fbx");		pMeshData2->Save(pMeshData2->GetPath());
	//	pMeshData2 = CResMgr::GetInst()->Load<CMeshData>(L"MeshData\\Move_BladeMaster_JinM_Animset.mdat", L"MeshData\\Move_BladeMaster_JinM_Animset.mdat");

	bool m_bUnit = true;

	if (m_bUnit)
	{
		Ptr<CMeshData> pMeshDataPlayer = CResMgr::GetInst()->Load<CMeshData>(L"Mesh\\Move_BladeMaster_JinM_Animset.mesh", L"Mesh\\Move_BladeMaster_JinM_Animset.mesh");
		pBladeMaster = pMeshDataPlayer->Instantiate();
		pBladeMaster->SetName(L"_BladeMaster Player");
		pBladeMaster->FrustumCheck(false);
		pBladeMaster->Transform()->SetLocalScale(Vec3(0.35f, 0.35f, 0.35f));
		pBladeMaster->Transform()->SetLocalPos(Vec3(ObjectX, 168.8f + ObjectY, ObjectZ));
		pBladeMaster->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Player", pBladeMaster, false);

		CBoneSocket* pSocketHand;

		pBladeMaster->AddSocket(L"RightHand", L"Bip01RHand");
		pSocketHand = pBladeMaster->Animator3D()->FindSocket(L"RightHand");
		pSocketHand->SetOffset(Vec3(0, 61, -27));
		pSocketHand->SetRotation(Vec3(180, 180, 90));

		pBladeMaster->AddSocket(L"LeftHand", L"Bip01LHand");
		pSocketHand = pBladeMaster->Animator3D()->FindSocket(L"LeftHand");
		pSocketHand->SetOffset(Vec3(0, 61, -27));
		pSocketHand->SetRotation(Vec3(180, 180, 90));

		pBladeMaster->AddSocket(L"WeaponBack", L"Bip01Spine2");
		pSocketHand = pBladeMaster->Animator3D()->FindSocket(L"WeaponBack");
		pSocketHand->SetOffset(Vec3(-5, 80, -10));
		pSocketHand->SetRotation(Vec3(90, -50, 80));

		pBladeMaster->AddSocket(L"Idle", L"Bip01RFoot");
		pSocketHand = pBladeMaster->Animator3D()->FindSocket(L"Idle");
		pSocketHand->SetOffset(Vec3(0, -100, 0));
		pSocketHand->SetRotation(Vec3(0, 0, 0));

		pBladeMaster->AddSocket(L"Face_Root", L"Face_Root");
		pSocketHand = pBladeMaster->Animator3D()->FindSocket(L"Face_Root");
		pSocketHand->SetOffset(Vec3(0, 0, 0));
		pSocketHand->SetRotation(Vec3(0, 0, 0));

		pBladeMaster->AddComponent(new CPlayerScript);

		pBladeMaster->Collider3D()->SetOffsetScale(Vec3(5.f, 60.f, 5.f));
		pBladeMaster->Collider3D()->SetOffsetPos(Vec3(0.f, 10.f, 0.f));

		// 이 둘이 세트
//		CExcelMgr::GetInst()->LoadPSA(L"Move_BladeMaster_JinM_Animset.psa");
//		CExcelMgr::GetInst()->SavePSAToAnimClipFile(L"BM", L"BM_Feedback_Animset.psa", pBladeMaster->MeshRender()->GetMesh());
//		CExcelMgr::GetInst()->SavePSAToAnimClipFile(L"Move_BladeMaster_JinM_Animset.psa", pBladeMaster->MeshRender()->GetMesh());

		bool BladeMaster = true;
		if (BladeMaster)
		{
			// Skill_JinF
			if (BladeMaster)
			{
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\Skill_Ascension");
			}

			// Standard_BladeMaster_JinF_Animset.psa
			if (BladeMaster)
			{
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Bat_Dead");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Bat_DeadStay");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Bat_Parry_F");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Bat_Parry_L");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Bat_Parry_R");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_None_Bat_EquipX");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_Standard_Mov_Idle");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_P_Standard_Mov_Idle");
			}

			// Skill_Standard_LynF_Animset.psa
			if (BladeMaster)
			{
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Standard_Skl_BackwardRoll");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Standard_SKL_counterattack_R");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Standard_SKL_escape");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Standard_SKL_escape_END");
				//	pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Standard_Skl_flying_swallow_sword_Exec");
				//	pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Standard_Skl_flying_swallow_sword_Finish");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Standard_SKL_Hack");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Standard_SKL_Hack_END");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Standard_Skl_legslash");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Standard_Skl_Return_Exec");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Standard_Skl_Return_finish");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Standard_Skl_Return_ready");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Standard_Skl_Return_standby");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Standard_Skl_Rush_Exec");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Standard_Skl_Rush_Fire");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Standard_SKL_turning_strike_Cast");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Standard_SKL_turning_strike_Exec");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Standard_SKL_Body_Blade_Unity_Fire");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Standard_SKL_Body_Blade_Unity_Exec");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Standard_SKL_Body_Blade_Unity_Sprint_Fire");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Standard_SKL_Body_Blade_Unity_Sprint_Exec");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Standard_Skl_Escape_Link");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Standard_SKL_LightningSmash_Fire");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Standard_SKL_storm_wind");
			}

			// Skill_Standard_JinF_Animset
			if (BladeMaster)
			{
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Standard_Skl_blocking_Looping_Move");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Standard_SKL_LightningSmash_Exec");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Standard_Skl_Rush_Stab_Exec");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Standard_SKL_rushing_wind_sphere");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Standard_SKL_shoulderCharge");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Standard_Skl_flying_swallow_sword_String");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Standard_Skl_flying_swallow_sword_StringPull");
			}

			// Skill_Protection_F_Animset
			if (BladeMaster)
			{
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\TA_A_Protection_SKL_KickKnockback_Cast");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\TA_A_Protection_SKL_KickKnockback_Exec");
			}

			// Skill_Phantom_LynF_Animset
			if (BladeMaster)
			{
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Phantom_Skl_GuardSoulBlade");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Phantom_Skl_RotateSoulBlade_Caster");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Phantom_Skl_RotateSoulBlade_Caster02");
			}

			// Skill_Phantom_JinF_Animset
			if (BladeMaster)
			{
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Phantom_G3_Skl_PhantomSlash3_Swing");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Phantom_G3_Skl_PhantomSlash3_Exec");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Phantom_G3_Skl_PhantomSlash1_Swing");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Phantom_G3_Skl_PhantomSlash2_Swing");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_Phantom_Skl_Diffusionsoul");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_Phantom_G3_Skl_Six_End");
			}

			// Skill_Hideblade_LynF_Animset
			if (BladeMaster)
			{
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_hideblade_Skl_Batto_Blocking_Start");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_hideblade_Skl_Batto_Blocking");
			}

			// Skill_Hideblade_JinM_Animset
			if (BladeMaster)
			{
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_hideblade_SKL_backstep");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_hideblade_SKL_Batto");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_hideblade_SKL_counterattack_L");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_hideblade_SKL_Five_combo_slash_midair_Exec_01");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_hideblade_SKL_Five_combo_slash_midair_Exec_02");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_hideblade_SKL_Five_combo_slash_midair_Exec_03");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_hideblade_SKL_Five_combo_slash_midair_Exec_04");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_hideblade_SKL_Five_combo_slash_midair_Exec_05");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_hideblade_Skl_Pierce");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_hideblade_Skl_rush_Exec");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_hideblade_Skl_rush_Fire");
				//	pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_hideblade_Skl_Six_Combo_Slash_Cast");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_hideblade_Skl_Six_Combo_Slash_Exec_01");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_hideblade_Skl_Six_Combo_Slash_Exec_02");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_hideblade_Skl_Six_Combo_Slash_Exec_03");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_hideblade_Skl_Six_Combo_Slash_Exec_04");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_hideblade_Skl_Six_Combo_Slash_Exec_05");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_hideblade_Skl_WindyMoonSlash_01");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_hideblade_Skl_WindyMoonSlash_02");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_hideblade_Skl_WindyMoonSlash_Finish");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_hideblade_Skl_Idle");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_hideblade_Skl_full_moon_slash_Exec");
			}

			// Skill_Hideblade_JinF_Animset
			if (BladeMaster)
			{
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_hideblade_G1_Skl_FireDragonSpin_End");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_hideblade_G1_Skl_FreezingSword_End");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_hideblade_Skl_RotateSoulBlade");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_hideblade_Skl_SoulBlast");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_hideblade_Skl_SoulStunLight");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_hideblade_Skl_VitalSlash01_Exec");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_hideblade_Skl_Mach_Slash");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_A_hideblade_Skl_Mach_Slash2");
			}

			// MoveSpecial_JinF_Animset
			if (BladeMaster)
			{
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Mov_Glide_Boost_Front");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Mov_Glide_Boost_Front_Cancle");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Mov_Glide_Boost_Front_Start");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Mov_Glide_Dive_Front");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Mov_Glide_Dive_Front_Cancle");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Mov_Glide_IdleToJump_Front_01");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Mov_Glide_JumpFront");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Mov_Glide_JumpLeft");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Mov_Glide_JumpRight");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Mov_Sprint_Front");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Mov_Sprint_Front_FallStart");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Mov_Sprint_Front_RocketStart");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Mov_Sprint_IdleToJump_Front_01");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Mov_Sprint_IdleToJump_Front_02");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Mov_Sprint_IdleToJump_Front_03");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Mov_Sprint_JumpFront");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Mov_Sprint_JumpToIdle");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Mov_Sprint_JumpToMove_Front");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Mov_Sprint_MoveToIdle");
			}

			// Move_BladeMaster_JinF_Animset
			if (BladeMaster)
			{
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\BK_B_Standard_Mov_JumpToIdle");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\BK_B_Standard_Mov_MoveToIdle");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\BK_P_Standard_Mov_IdleToJump_Front");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\BK_P_Standard_Mov_IdleToJump_Left");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\BK_P_Standard_Mov_IdleToJump_Right");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\BK_P_Standard_Mov_JumpFront");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\BK_P_Standard_Mov_JumpLeft");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\BK_P_Standard_Mov_JumpRight");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\BK_P_Standard_Mov_JumpToIdle");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\BK_P_Standard_Mov_JumpToMove_Back");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\BK_P_Standard_Mov_JumpToMove_Front");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\BK_P_Standard_Mov_JumpToMove_Left");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\BK_P_Standard_Mov_JumpToMove_Right");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\BK_P_Standard_Mov_LeftToRight");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\BK_P_Standard_Mov_MoveToIdle");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\BK_P_Standard_Mov_RightToLeft");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\BK_P_Standard_Mov_RunBack");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\BK_P_Standard_Mov_RunFront");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\BK_P_Standard_Mov_RunLeft");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\BK_P_Standard_Mov_RunLeftBack");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\BK_P_Standard_Mov_RunRight");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\BK_P_Standard_Mov_RunRightBack");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_hideblade_Mov_IdleToJump_Front");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_hideblade_Mov_IdleToJump_Left");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_hideblade_Mov_IdleToJump_Right");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_hideblade_Mov_JumpFront");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_hideblade_Mov_JumpLeft");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_hideblade_Mov_JumpRight");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_hideblade_Mov_JumpToIdle");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_hideblade_Mov_JumpToMove_Back");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_hideblade_Mov_JumpToMove_Front");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_hideblade_Mov_JumpToMove_Left");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_hideblade_Mov_JumpToMove_Right");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_hideblade_Mov_LeftToRight");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_hideblade_Mov_MoveToIdle");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_hideblade_Mov_RightToLeft");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_hideblade_Mov_RunBack");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_hideblade_Mov_RunFront");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_hideblade_Mov_RunLeft");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_hideblade_Mov_RunLeftBack");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_hideblade_Mov_RunRight");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_hideblade_Mov_RunRightBack");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_Phantom_Mov_JumpToIdle");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_Phantom_Mov_MoveToIdle");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_Standard_Mov_IdleToJump_Front");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_Standard_Mov_IdleToJump_Left");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_Standard_Mov_IdleToJump_Right");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_Standard_Mov_JumpFront");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_Standard_Mov_JumpLeft");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_Standard_Mov_JumpRight");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_Standard_Mov_JumpToIdle");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_Standard_Mov_JumpToMove_Back");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_Standard_Mov_JumpToMove_Front");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_Standard_Mov_JumpToMove_Left");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_Standard_Mov_JumpToMove_Right");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_Standard_Mov_LeftToRight");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_Standard_Mov_MoveToIdle");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_Standard_Mov_RightToLeft");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_Standard_Mov_RunBack");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_Standard_Mov_RunFront");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_Standard_Mov_RunLeft");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_Standard_Mov_RunLeftBack");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_Standard_Mov_RunRight");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\SW_B_Standard_Mov_RunRightBack");
			}

			// Feedback_Animset
			if (BladeMaster)
			{
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Skl_Capture_Exec2_fire");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Skl_Capture_Exec2_End");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Bat_Knockback_Mid_B_Start");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Bat_Stun_Start");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Bat_Stun_Looping");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Bat_Knee_F_Start");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Bat_Knee_Looping");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Bat_Knee_End");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Bat_Down_Mid_B_Start");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Bat_Down_Low_B_R_Start");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Bat_Down_Looping");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Bat_Down_End");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Bat_Midair1_upper");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Bat_Midair2_upper");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Bat_Midair1_lower");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_BighitX_Skl_skykick");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_BighitX_Skl_Flytakedown");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_MountX_Bat_idle");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_MountX_Bat_End");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_MountX_Skl_armbar");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_MountX_Skl_pounding_First");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_MountX_Skl_pounding");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Bat_Damageback");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_None_Bat_Damagefront");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_BighitX_Skl_Downcounter");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_BighitX_Skl_Violencepunch");
				pBladeMaster->Animator3D()->AddAnimClip(L"BM\\AA_A_BighitX_Skl_triplekick");
			}

			pBladeMaster->Animator3D()->SetCurrentAnimClip(L"BM\\SW_A_hideblade_SKL_backstep");
		}
	}

	if (m_bUnit)
	{
		Ptr<CMeshData> pMeshDataPlayer = CResMgr::GetInst()->Load<CMeshData>(L"Mesh\\Skill_Bighit_M_Animset.mesh", L"Mesh\\Skill_Bighit_M_Animset.mesh");
		pKungfuMaster = pMeshDataPlayer->Instantiate();
		pKungfuMaster->SetName(L"_KungfuMaster AI");
		pKungfuMaster->FrustumCheck(false);
		pKungfuMaster->Transform()->SetLocalScale(Vec3(0.35f, 0.35f, 0.35f));
		pKungfuMaster->Transform()->SetLocalPos(Vec3(ObjectX + 100.f, 168.8f + ObjectY, ObjectZ));
		pKungfuMaster->MeshRender()->ShadowCheck(true);
		pCurScene->AddGameObject(L"Monster", pKungfuMaster, false);

		pKungfuMaster->AddSocket(L"RightHand", L"Bip01RHand");	//WeaponR

		CBoneSocket* pSocketHand = pKungfuMaster->Animator3D()->FindSocket(L"RightHand");
		pSocketHand->SetOffset(Vec3(0, 61, -27));
		pSocketHand->SetRotation(Vec3(180, 180, 90));

		pKungfuMaster->AddSocket(L"LeftHand", L"Bip01LHand");	//WeaponL

		pSocketHand = pKungfuMaster->Animator3D()->FindSocket(L"LeftHand");
		pSocketHand->SetOffset(Vec3(-5, 80, -10));
		pSocketHand->SetRotation(Vec3(90, -50, 80));

		pKungfuMaster->AddSocket(L"Face_Root", L"Face_Root");

		pSocketHand = pKungfuMaster->Animator3D()->FindSocket(L"Face_Root");
		pSocketHand->SetOffset(Vec3(0, 0, 0));
		pSocketHand->SetRotation(Vec3(0, 0, 0));

		pKungfuMaster->AddComponent(new CAIScript);

		// 이 둘이 세트
	//	CExcelMgr::GetInst()->LoadPSA(L"Skill_Bighit_M_Animset.psa");
	//	CExcelMgr::GetInst()->SavePSAToAnimClipFile(L"Skill_Mount_M_Animset.psa", pKungfuMaster->MeshRender()->GetMesh());
	//	CExcelMgr::GetInst()->SavePSAToAnimClipFile(L"KFM", L"Skill_Bighit_M_Animset.psa", pKungfuMaster->MeshRender()->GetMesh());

		bool KungfuMaster = true;
		if (KungfuMaster)
		{
			//Standard_KungfuFighter_GonF_Animset	비전투 대기
			if (KungfuMaster)
			{
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Bat_Dead");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Bat_DeadStay");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_None_Bat_EquipX");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_P_None_Mov_Idle");
			}

			//Skill_Protection_F_Animset.psa	뒤구르기
			if (KungfuMaster)
			{
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\TA_A_Protection_Skl_BackwardRoll");
			}

			//Skill_Mount_M_Animset
			if (KungfuMaster)
			{
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Mount_Bat_End");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Mount_Bat_idle");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Mount_Skl_armbar");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Mount_Skl_pounding_First");
			}

			//Skill_Combo_M_Animset
			if (KungfuMaster)
			{
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Combo_Skl_crushground_Swing");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Combo_Skl_Finalattack");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\Bk_A_Combo_Skl_LeftElbow");
			}

			//Skill_Bighit_M_Animset
			if (KungfuMaster)
			{
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_G1_Skl_1000hand_Exec1");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_G1_Skl_1000hand_Exec2");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_G1_Skl_1000hand_Finish1");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_G1_Skl_1000hand_Finish2");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_G1_Skl_1000hand_Swing1");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_G1_Skl_1000hand_Swing2");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_G1_Skl_wind_kick1_End");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_G1_Skl_wind_kick1_Exec");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_G1_Skl_wind_kick1_Swing");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_G1_Skl_wind_mode_End");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_G1_Skl_wind_mode_Exec");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_G1_Skl_wind_mode_Swing");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_G2_Skl_fire_punch1_End");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_G2_Skl_fire_punch1_Swing");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_G2_Skl_fire_punch2_End");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_G2_Skl_fire_punch2_Swing");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_G2_Skl_fire_punch3_End");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_G2_Skl_fire_punch3_Swing");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_G2_Skl_fire_punch4_End");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_G2_Skl_fire_punch4_Swing");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_bighit_Skl_4hitkick");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_bighit_Skl_AbdomenAttack_Exec");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_bighit_Skl_AbdomenAttack_Fire");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\Bk_A_Bighit_Skl_Backstep");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_bighit_Skl_BurstAbdomen");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_Command_Left_Side_Exec");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_Command_Left_Side_Fire");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_Command_Right_Side_Exec");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_Command_Right_Side_Fire");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_Counterenemy_Exec");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_counterenemy_finish");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_counterenemy_ready");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_counterenemy_standby");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_bighit_Skl_DashUpper_Exec");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_bighit_Skl_DashUpper_Fire");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_Downcounter_Exec");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_Downcounter_finish");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_Downcounter_ready");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_Downcounter_standby");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\Bk_A_Bighit_Skl_Dragon_Fly_Exec");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_DragonKick_Exec");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_DragonKick_Fire");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_escape");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_Flytakedown");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_Heavypunch_Swing");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_Heavypunch_Swing2");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_bighit_Skl_IceBlast_Kick");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_Jumppunch_Exec");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_Jumppunch_Fire");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_lowkick");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_Martialdance1_Exec");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_Martialdance1_Fire");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_Martialdance2_Exec");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_Martialdance2_Fire");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_Martialdance3_Exec");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_Martialdance3_Fire");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_Martialdance4_Exec");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_Martialdance4_Fire");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_skykick");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_triplekick");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_uppercut");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_uppercut2");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_Violencepunch_Cast");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_Violencepunch_Exec");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_A_Bighit_Skl_Windmill");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_B_Bighit_Mov_idle");
			}


			//MoveSpecial_KungfuFighter_Animset
			if (KungfuMaster)
			{
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Mov_Glide_Boost_Front");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Mov_Glide_Boost_Front_Cancle");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Mov_Glide_Boost_Front_Start");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Mov_Glide_Dive_Front");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Mov_Glide_Dive_Front_Cancle");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Mov_Glide_IdleToJump_Front_01");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Mov_Glide_JumpFront");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Mov_Glide_JumpLeft");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Mov_Glide_JumpRight");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Mov_Sprint_Front");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Mov_Sprint_Front_FallStart");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Mov_Sprint_Front_RocketStart");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Mov_Sprint_IdleToJump_Front_01");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Mov_Sprint_IdleToJump_Front_02");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Mov_Sprint_IdleToJump_Front_03");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Mov_Sprint_JumpFront");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Mov_Sprint_JumpToIdle");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Mov_Sprint_JumpToMove_Front");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Mov_Sprint_MoveToIdle");
			}

			//Move_KungfuFighter_GonF_Animset
			if (KungfuMaster)
			{
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_B_Bighit_Mov_IdleToJump_Front");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\Bk_B_Bighit_Mov_IdleToJump_Left");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\Bk_B_Bighit_Mov_IdleToJump_Right");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_B_Bighit_Mov_JumpFront");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\Bk_B_Bighit_Mov_JumpLeft");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\Bk_B_Bighit_Mov_JumpRight");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_B_Bighit_Mov_JumpToIdle");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\Bk_B_Bighit_Mov_JumpToMove_Back");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\Bk_B_Bighit_Mov_JumpToMove_Front");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\Bk_B_Bighit_Mov_JumpToMove_Left");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\Bk_B_Bighit_Mov_JumpToMove_Right");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\Bk_B_Bighit_Mov_LeftToRight");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\Bk_B_Bighit_Mov_MoveToIdle");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\Bk_B_Bighit_Mov_RightToLeft");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_B_Bighit_Mov_RunBack");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_B_Bighit_Mov_RunFront");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_B_Bighit_Mov_RunLeft");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\Bk_B_Bighit_Mov_RunLeftBack");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_B_Bighit_Mov_RunRight");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\Bk_B_Bighit_Mov_RunRightBack");
				//pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\Bk_B_Combo_Mov_IdleToJump_Front");
				//pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\Bk_B_Combo_Mov_IdleToJump_Left");
				//pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\Bk_B_Combo_Mov_IdleToJump_Right");
				//pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\Bk_B_Combo_Mov_JumpFront");
				//pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\Bk_B_Combo_Mov_JumpLeft");
				//pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\Bk_B_Combo_Mov_JumpRight");
				//pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\Bk_B_Combo_Mov_JumpToIdle");
				//pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\Bk_B_Combo_Mov_JumpToMove_Front");
				//pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\Bk_B_Combo_Mov_JumpToMove_Left");
				//pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\Bk_B_Combo_Mov_JumpToMove_Right");
				//pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\Bk_B_Combo_Mov_LeftToRight");
				//pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\Bk_B_Combo_Mov_MoveToIdle");
				//pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\Bk_B_Combo_Mov_RightToLeft");
				//pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_B_Combo_Mov_RunFront");
				//pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\Bk_B_Combo_Mov_RunLeft");
				//pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\Bk_B_Combo_Mov_RunRight");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\Bk_P_Bighit_Mov_LeftToRight");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\Bk_P_Bighit_Mov_RightToLeft");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_P_None_Mov_IdleToJump_Front");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_P_None_Mov_JumpToIdle");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\BK_P_None_Mov_MoveToIdle");
			}

			//KFM_Feedback_Animset
			if (KungfuMaster)
			{
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_BighitX_Skl_skykick");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Bat_Damageback");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Bat_Damagefront");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Bat_Down_End");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Bat_Down_Front_End");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Bat_Down_Front_Looping");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Bat_Down_Looping");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Bat_Down_Low_B_R_Start");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Bat_Down_Mid_B_Start");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Bat_Knee_End");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Bat_Knee_F_Start");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Bat_Knee_Looping");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Bat_Knockback_Long_B_Start");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Bat_Midair1_lower");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Bat_Midair1_upper");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Bat_Midair2_upper");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Bat_Stun_Looping");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Bat_Stun_Start");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Skl_Capture_Exec2_End");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_None_Skl_Capture_Exec2_fire");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_StandardX_SKL_ascension");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_StandardX_Skl_crash");
				pKungfuMaster->Animator3D()->AddAnimClip(L"KFM\\AA_A_StandardX_SKL_upper_slash");
			}

			//	pKungfuMaster->Animator3D()->SetCurrentAnimClip(L"KFM\\SW_P_Standard_Mov_Idle");
			pKungfuMaster->Animator3D()->SetCurrentAnimClip(L"KFM\\BK_P_None_Mov_Idle");
		}
	}


	// 무기 및 선글라스
	if (true)
	{
		//곤륜검
		if (true)
		{
			Ptr<CMeshData> pMeshData2 = CResMgr::GetInst()->Load<CMeshData>(L"Mesh\\GonryunBlade.mesh", L"Mesh\\GonryunBlade.mesh");
			CGameObject* pObject2 = pMeshData2->Instantiate();
			pObject2->SetName(L"_GonryunBlade");
			pObject2->FrustumCheck(false);

			//	pObject2->Collider3D()->SetOffsetScale(Vec3(5.f, 60.f, 5.f));
			pObject2->Collider3D()->SetOffsetScale(Vec3(0.f, 0.f, 0.f));
			pObject2->Collider3D()->SetOffsetPos(Vec3(0.f, 7.f, 0.f));

			//	if (pObject2->MeshRender())
			//	{
			//		int MaterialContainerSize = (((CMeshRender*)(pObject2->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetMaterialContainerCount());
			//	
			//		for (size_t i = 0; i < MaterialContainerSize; ++i)
			//		{
			//			int MaterialSubSetSize = (((CMeshRender*)(pObject2->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetMaterialSubsetCount(i));
			//	
			//			for (size_t j = 0; j < MaterialSubSetSize; ++j)
			//			{
			//				(((CMeshRender*)(pObject2->GetComponent(COMPONENT_TYPE::MESHRENDER)))->GetSharedMaterial(j, i))->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"Std3D_AlphaBlendShader"));
			//			}
			//		}
			//	}

			pObject2->Transform()->SetLocalScale(Vec3(1.1f, 1.1f, 1.1f));

			pObject2->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
			pObject2->Transform()->SetLocalRot(Vec3(0.f * _DEGREE, 0.f * _DEGREE, 10.f * _DEGREE));

			pObject2->MeshRender()->ShadowCheck(true);
			pCurScene->AddGameObject(L"Default", pObject2, false);

			pObject2->Animator3D()->SetStatic();
			pBladeMaster->AddChild(pObject2, L"WeaponBack");
		}

		if (true) //Sunglass
		{
			if (true)
			{
				Ptr<CMeshData> pMeshData2 = CResMgr::GetInst()->Load<CMeshData>(L"Mesh\\Sunglass.mesh", L"Mesh\\Sunglass.mesh");
				CGameObject* pObject2 = pMeshData2->Instantiate();
				pObject2->SetName(L"_Sunglass");
				pObject2->FrustumCheck(false);

				pObject2->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
				// pObject2->Transform()->SetLocalPos(Vec3(0.f, 60.f, -27.f));
				// pObject2->Transform()->SetLocalRot(Vec3(180.f, 180.f, 90.f));

				pObject2->MeshRender()->ShadowCheck(true);
				pCurScene->AddGameObject(L"Default", pObject2, false);

				pObject2->Animator3D()->SetStatic();
				pBladeMaster->AddChild(pObject2, L"Face_Root");
			}
			if (true)
			{
				Ptr<CMeshData> pMeshData2 = CResMgr::GetInst()->Load<CMeshData>(L"Mesh\\Sunglass.mesh", L"Mesh\\Sunglass.mesh");
				CGameObject* pObject2 = pMeshData2->Instantiate();
				pObject2->SetName(L"_Sunglass");
				pObject2->FrustumCheck(false);

				pObject2->Transform()->SetLocalScale(Vec3(1.f, 1.f, 1.f));
				// pObject2->Transform()->SetLocalPos(Vec3(0.f, 60.f, -27.f));
				// pObject2->Transform()->SetLocalRot(Vec3(180.f, 180.f, 90.f));

				pObject2->MeshRender()->ShadowCheck(true);
				pCurScene->AddGameObject(L"Default", pObject2, false);

				pObject2->Animator3D()->SetStatic();
				pKungfuMaster->AddChild(pObject2, L"Face_Root");
			}
		}	 // Sunglass
	}


	// ===================
	// Post Effect Object
	// ===================

	//pObject = new CGameObject;
	//pObject->SetName(L"PostEffect");

	//pObject->AddComponent(new CTransform);
	//pObject->AddComponent(new CMeshRender);

	//pObject->MeshRender()->CheckDynamicShadow(false);

	//// Material 값 셋팅
	//Ptr<CMaterial> pMtrl = new CMaterial;
	//pMtrl->DisableFileSave();
	//pMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"DistortionShader"));
	//Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(L"PosteffectTargetTex");	
	//pMtrl->SetData(SHADER_PARAM::TEX_0, &pTex);
	//
	//CResMgr::GetInst()->AddRes<CMaterial>(L"DistortionMtrl", pMtrl);

	//pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pObject->MeshRender()->SetMaterial(pMtrl, 0);

	//pObject->Transform()->SetLocalScale(Vec3(1500.f, 1500.f, 1500.f));
	//pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));

	//pCurScene->AddGameObject(L"Default", pObject, false);

	// ========
	// Terrain
	// ========
	pObject = new CGameObject;
	pObject->SetName(L"_Base Terrain");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);
	pObject->AddComponent(new CCollider3D);

	pObject->AddComponent(new CTerrain);

	pObject->FrustumCheck(false);
	//	pObject->Transform()->SetLocalPos(Vec3(-10000.f, -3450.f + ObjectY, -10000.f));
	//	pObject->Transform()->SetLocalScale(Vec3(500.f, 500.f, 500.f));
	pObject->Transform()->SetLocalPos(Vec3(-200000.000000f, -3453.f + ObjectY, -200000.000000f));
	pObject->Transform()->SetLocalScale(Vec3(6250.000000f, 6250.000000f, 6250.000000f));

	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TerrainMtrl"));
	pObject->Terrain()->init();

	//	((CCollider3D*)(pObject->GetComponent(COMPONENT_TYPE::COLLIDER3D)))->SetCollider3DType(COLLIDER3D_TYPE::MESH);


	pCurScene->AddGameObject(L"Terrain", pObject, false);



	// Decal
	pObject = new CGameObject;
	pObject->SetName(L"_Fist Decal");
	pObject->AddComponent(new CTransform());
	pObject->AddComponent(new CMeshRender());

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"_FistMtrl"));

	pTexture = CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Decal\\_Fist.tga");
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, &pTexture);

	pTexture = CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Decal\\_Fist_N.tga");
	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_2, &pTexture);

	pObject->Transform()->SetLocalPos(Vec3(ObjectX + 100.f, 357.0f + ObjectY - 200.f, ObjectZ + 100.f));
	pObject->Transform()->SetLocalScale(Vec3(70.f, 30.f, 200.f));

	pObject->MeshRender()->ShadowCheck(true);

	pObject->AddComponent(new CCollider3D);
	pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	pObject->Collider3D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
	pObject->Collider3D()->SetOffsetPos(Vec3(1.f, 1.f, 1.f));

	pCurScene->AddGameObject(L"Decal", pObject, false);

#ifdef CLIENT
	CRenderMgr::GetInst()->GetMainCamera()->DecalPushBack(pObject);
#else
	CRenderMgr::GetInst()->GetMainCamera()->DecalPushBack(pObject);
	CRenderMgr::GetInst()->GetToolCamera()->DecalPushBack(pObject);
#endif

	// Brush Decal
	pObject = new CGameObject;
	pObject->SetName(L"Brush Decal");
	pObject->AddComponent(new CTransform());
	pObject->AddComponent(new CMeshRender());

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"CubeMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"_BrushInkMtrl"));

	pObject->Transform()->SetLocalPos(Vec3(-0, 0, 0));
	pObject->Transform()->SetLocalScale(Vec3(500.f, 500.f, 500.f));

	//pObject->AddComponent(new CCollider3D);
	//pObject->Collider3D()->SetCollider3DType(COLLIDER3D_TYPE::CUBE);
	//pObject->Collider3D()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
	//pObject->Collider3D()->SetOffsetPos(Vec3(1.f, 1.f, 1.f));

	pObject->AddComponent(new CBrushScript);

	pCurScene->AddGameObject(L"Decal", pObject, false);


	//	pCameraObject->Camera()->DecalPushBack(pObject);

#ifdef CLIENT
	CRenderMgr::GetInst()->GetMainCamera()->DecalPushBack(pObject);
#else
	CRenderMgr::GetInst()->GetMainCamera()->DecalPushBack(pObject);
	CRenderMgr::GetInst()->GetToolCamera()->DecalPushBack(pObject);
#endif

	pCurScene->awake();
	pCurScene->start();
	CEventMgr::GetInst()->update();
}

void CSceneMaker::update()
{
	if (m_pMainCam == nullptr)
	{
		return;
	}

	if (CCore::GetInst()->GetSceneMod() == SCENE_MOD::SCENE_PLAY)
	{
		m_pMainCam->update();
		m_pMainCam->lateupdate();
		m_pMainCam->finalupdate();
	}

	Ptr<CMaterial> pTerrainMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TerrainMtrl");

	Vec3 vCamPos;
	if (CCore::GetInst()->GetSceneMod() == SCENE_MOD::SCENE_PLAY)
	{
		vCamPos = m_pMainCam->Transform()->GetWorldPos();
	}
	pTerrainMtrl->SetData(SHADER_PARAM::VEC4_0, &vCamPos);

	// ======
	// Render
	// ======
	// 물체 분류
	if (CCore::GetInst()->GetSceneMod() == SCENE_MOD::SCENE_PLAY)
	{
		// Render
		m_pMainCam->Camera()->SortGameObject();
		// Deferred Render
		m_pMainCam->Camera()->render_deferred();
	}

	// ShadowMap Render
	CRenderMgr::GetInst()->render_shadowmap();

	// Lights Render
	CRenderMgr::GetInst()->render_lights();

	// forward render
	if (CCore::GetInst()->GetSceneMod() == SCENE_MOD::SCENE_PLAY)
	{
		m_pMainCam->Camera()->render_forward();

		int CamSize = CRenderMgr::GetInst()->GetCamera()->size();

		for (size_t i = 0; i < CamSize; ++i)
		{
			CRenderMgr::GetInst()->GetCamera(i)->render();	//skybox 같은 녀석들 추가 렌더링
		}
	}

	// Light Merge
	CRenderMgr::GetInst()->merge_light();

	// posteffect render
	if (CCore::GetInst()->GetSceneMod() == SCENE_MOD::SCENE_PLAY)
	{
		m_pMainCam->Camera()->render_posteffect();
	}

	// TargetObject Dir Render
	/*CGameObject* pTargetObj = ((CInfoView*)((CMainFrame*)AfxGetMainWnd())->GetInfoView())->GetTargetObj();
	if (nullptr != pTargetObj)
	{
		pTargetObj->Transform()->UpdateData();
		m_pDirMtrl->UpdateData();
		m_pDirMesh->render();
	}*/

	// TagetUI Render

	if (CCore::GetInst()->GetSceneMod() == SCENE_MOD::SCENE_PLAY)
	{
		m_pMainCam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	}


	if (CCore::GetInst()->GetSceneMod() == SCENE_MOD::SCENE_PLAY)
	{
		m_pMainCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	}

	//	CTimeMgr::GetInst()->render();
	CRenderMgr::GetInst()->Present();
}