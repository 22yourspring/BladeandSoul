
// StudioYCS.cpp: 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "StudioYCS.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "StudioYCSDoc.h"
#include "StudioYCSView.h"
#include "GameView.h"

#include <Engine/Core.h>
#include <Engine/Script.h>

#include <Engine/ExcelMgr.h>
#include <Engine/Animator3D.h>
#include <Engine/Anim3DMgr.h>
#include <Engine/BoneSocket.h>

#include <Engine/KeyMgr.h>
#include <Engine/SceneMgr.h>

#include <Engine/Scene.h>
#include <Engine/gameobject.h>
#include <Engine/Collider2D.h>
#include <Engine/Collider3D.h>
#include <Engine/ResMgr.h>
#include <Engine/Material.h>
#include <Engine/MeshRender.h>
#include <Engine/Transform.h>
#include <Engine/ParticleSystem.h>

#include "ClassView.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CStudioYCSApp

BEGIN_MESSAGE_MAP(CStudioYCSApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CStudioYCSApp::OnAppAbout)
	ON_COMMAND(ID_FILE_PRINT, &CStudioYCSApp::OnAppPause)
	// 표준 파일을 기초로 하는 문서 명령입니다.
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// 표준 인쇄 설정 명령입니다.
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CStudioYCSApp 생성

CStudioYCSApp::CStudioYCSApp() noexcept
{
	m_bHiColorIcons = TRUE;

	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 응용 프로그램을 공용 언어 런타임 지원을 사용하여 빌드한 경우(/clr):
	//     1) 이 추가 설정은 다시 시작 관리자 지원이 제대로 작동하는 데 필요합니다.
	//     2) 프로젝트에서 빌드하려면 System.Windows.Forms에 대한 참조를 추가해야 합니다.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 아래 응용 프로그램 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("StudioYCS.AppID.NoVersion"));

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


#include <Engine/Camera.h>
#include <Engine/Light2D.h>
#include <Engine/Light3D.h>
#include <Engine/CollisionMgr.h>
#include <Engine/SceneMgr.h>
#include <Engine/Scene.h>
#include <Script/TestScript.h>
#include <Script/MainCamScript.h>
#include <Engine/Sound.h>
#include <Engine/terrain.h>

#include <Script/PlayerScript.h>
#include <Script/AIScript.h>
#include <Script/BrushScript.h>
#include <Script/BillboardScript.h>
#include <Script/WeatherScript.h>
#include <Script/RevolveScript.h>


void CStudioYCSApp::TestCode()
{
	static CGameObject* pTarget = nullptr;
}

void CStudioYCSApp::CreateTestScene()
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
	pCurScene->SetLayerName(8, L"Billboard");
	pCurScene->SetLayerName(9, L"Particle");
	pCurScene->SetLayerName(10, L"Post Effect");

	CGameObject* pObject = nullptr;
	CGameObject* pCameraObject = nullptr;

	// Camera Object 만들기
	pCameraObject = new CGameObject;
	pCameraObject->SetName(L"_Main Camera");
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
		pObject->SetName(L"_Light");
		pObject->AddComponent(new CTransform());
		pObject->AddComponent(new CMeshRender());
		pObject->AddComponent(new CLight3D);

		pObject->Light3D()->SetLightType(LIGHT_TYPE::DIR);
		pObject->Light3D()->SetDiffuseColor(Vec3(1.f, 1.f, 1.f));
		pObject->Light3D()->SetSpecular(Vec3(1.f, 1.f, 1.f));
		pObject->Light3D()->SetAmbient(Vec3(0.15f, 0.15f, 0.15f));
		pObject->Light3D()->SetLightDir(Vec3(1.f, -1.f, 1.f));
		pObject->Light3D()->SetLightRange(300.f);

		pObject->Transform()->SetLocalPos(Vec3(ObjectX, 168.8f + ObjectY + 200.f, ObjectZ));
		pCurScene->SetLightObject(pObject);
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
		pCurScene->SetLightObject(pObject);
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
	pObject->Transform()->SetLocalPos(Vec3(-2000.f, 150000.f, -400000.f));
	pObject->Transform()->SetLocalScale(Vec3(200000.f, 200000.f, 200000.f));

	pObject->AddComponent(new CMeshRender());

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObject->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"MoonMtrl"));

	pTexture = CResMgr::GetInst()->FindRes<CTexture>(L"Texture\\Moon_Test.png");

	pObject->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, &pTexture);
	pObject->AddComponent(new CBillboardScript);
	pObject->AddComponent(new CRevolveScript);

	pCurScene->AddGameObject(L"Billboard", pObject, false);

	CRenderMgr::GetInst()->GetMainCamera()->m_pMoon = pObject;


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
	pObject = new CGameObject;
	pObject->SetName(L"_Particle1");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CParticleSystem);

	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(ObjectX, 168.8f + ObjectY, ObjectZ));

	pCurScene->AddGameObject(L"Particle", pObject, false);

	pObject = new CGameObject;
	pObject->SetName(L"_Particle2");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CParticleSystem);

	pObject->FrustumCheck(false);
	pObject->Transform()->SetLocalPos(Vec3(4200.f, 3700.f, 3900.f));

	pCurScene->AddGameObject(L"Particle", pObject, false);

	// Weather
	for (size_t i = 1; i < 26; ++i)
	{
		pObject = new CGameObject;
		wstring ParticleName = L"_Weather Particle";
		wstring Number10 = L"0";
		wstring ParticleNumber = std::to_wstring(i);
		if ((int)(i / 10) < 1)
			pObject->SetName(ParticleName + Number10 + ParticleNumber);
		else
			pObject->SetName(ParticleName + ParticleNumber);
	
		pObject->AddComponent(new CTransform);
		pObject->AddComponent(new CParticleSystem);
		pObject->AddComponent(new CWeatherScript);
		pObject->FrustumCheck(false);
		pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
		pObject->Particlesystem()->SetMaxLifeTime(50.f);
		pCurScene->AddGameObject(L"Particle", pObject, false);
	}

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
	
	// ===================
	// Post Effect Object
	// ===================

	pObject = new CGameObject;
	pObject->SetName(L"_Distortion Effect");

	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CMeshRender);

	pObject->MeshRender()->CheckDynamicShadow(false);

	// Material 값 셋팅
	Ptr<CMaterial> pMtrl = new CMaterial;
	pMtrl->DisableFileSave();
	pMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"DistortionShader"));
	Ptr<CTexture> pTex = CResMgr::GetInst()->FindRes<CTexture>(L"PosteffectTargetTex");	
	pMtrl->SetData(SHADER_PARAM::TEX_0, &pTex);
	
	CResMgr::GetInst()->AddRes<CMaterial>(L"DistortionMtrl", pMtrl);

	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
//	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"Mesh\\GonryunSword.mesh", L"Mesh\\GonryunSword.mesh"));
	pObject->MeshRender()->SetMaterial(pMtrl, 0);

	pObject->Transform()->SetLocalScale(Vec3(1500.f, 1500.f, 1500.f));
//	pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 0.f));
	pObject->Transform()->SetLocalPos(Vec3(ObjectX, 168.8f + ObjectY, ObjectZ));

	pCurScene->AddGameObject(L"Post Effect", pObject, false);

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

	pObject->MeshRender()->ShadowCheck(false);

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

// 유일한 CStudioYCSApp 개체입니다.

CStudioYCSApp theApp;
// 이 식별자는 응용 프로그램에서 통계적으로 고유한 값을 가지도록 생성되었습니다.
// 특정 식별자를 선호할 경우 변경할 수 있습니다.

// {538b13a9-27d3-42c7-a9d1-97fd272149d9}
static const CLSID clsid =
{ 0x538b13a9,0x27d3,0x42c7,{0xa9,0xd1,0x97,0xfd,0x27,0x21,0x49,0xd9} };

const GUID CDECL _tlid = { 0x98aefc9e,0x3d56,0x420d,{0x96,0x0b,0xa2,0x32,0xb5,0xb2,0x33,0x88} };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


// CStudioYCSApp 초기화
#include "SplashScreen.h"
BOOL CStudioYCSApp::InitInstance()
{
	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// RichEdit 컨트롤을 사용하려면 AfxInitRichEdit2()가 있어야 합니다.
	// AfxInitRichEdit2();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));
	LoadStdProfileSettings(4);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 응용 프로그램의 문서 템플릿을 등록합니다.  문서 템플릿은
	//  문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_SceneTYPE,
		RUNTIME_CLASS(CStudioYCSDoc),
		RUNTIME_CLASS(CChildFrame), // 사용자 지정 MDI 자식 프레임입니다.
		RUNTIME_CLASS(CGameView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// COleTemplateServer를 문서 템플릿에 연결합니다.
	//  COleTemplateServer는 OLE 컨테이너를 요청하는 대신 문서 템플릿에
	//  지정된 정보를 사용하여 새 문서를
	//  만듭니다.
	m_server.ConnectTemplate(clsid, pDocTemplate, FALSE);
	// 모든 OLE 서버 팩터리를 실행 중으로 등록합니다.  이렇게 하면
	//  OLE 라이브러리가 다른 응용 프로그램에서 개체를 만들 수 있습니다.
	COleTemplateServer::RegisterAll();
	// 참고: MDI 응용 프로그램은 명령줄의 /Embedding 또는 /Automation에
	//  상관 없이 모든 서버 개체를 등록합니다.

// 주 MDI 프레임 창을 만듭니다.
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// 접미사가 있을 경우에만 DragAcceptFiles를 호출합니다.
	//  MDI 응용 프로그램에서는 m_pMainWnd를 설정한 후 바로 이러한 호출이 발생해야 합니다.
	// 끌어서 놓기에 대한 열기를 활성화합니다.
	m_pMainWnd->DragAcceptFiles();

	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// DDE Execute 열기를 활성화합니다.
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// 응용 프로그램이 /Embedding 또는 /Automation 스위치로 시작되었습니다.
	// 응용 프로그램을 자동화 서버로 실행합니다.
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// 주 창을 표시하지 않습니다.
		return TRUE;
	}
	// 응용 프로그램이 /Unregserver 또는 /Unregister 스위치로 시작되었습니다. 
	// typelibrary를 등록 취소합니다.  다른 등록 취소는 ProcessShellCommand()에서 발생합니다.
	else if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister)
	{
		UnregisterShellFileTypes();
		m_server.UpdateRegistry(OAT_DISPATCH_OBJECT, nullptr, nullptr, FALSE);
		AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor);
	}
	// 응용 프로그램이 독립 실행형으로 시작되었거나 다른 스위치로 시작되었습니다(예: /Register
	// 또는 /Regserver).  typelibrary를 포함하여 레지스트리 항목을 업데이트합니다.
	else
	{
		m_server.UpdateRegistry(OAT_DISPATCH_OBJECT);
		COleObjectFactory::UpdateRegistryAll();
		AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid);
	}

	// 명령줄에 지정된 명령을 디스패치합니다.
	// 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// 주 창이 초기화되었으므로 이를 표시하고 업데이트합니다.
	pMainFrame->UpdateWindow();

	// Splash Animation

	ULONG_PTR gdiplus_token;
	GdiplusStartupInput gdiplus_startup_input;

	if (Gdiplus::Status::Ok != ::GdiplusStartup(&gdiplus_token, &gdiplus_startup_input, NULL)) {
		AfxMessageBox(_T("ERROR : Failed to initialize GDI+ library"));
		return FALSE;
	}

	pSplash = new CSplashScreen;
	pSplash->DoModal();
	delete pSplash;
	//	pSplash->Create(IDD_SplashScreen, pSplash);
	//	pSplash->ShowWindow(SW_SHOW);


	CRect rt;
	::GetWindowRect((((CMainFrame *)AfxGetMainWnd())->GetActiveFrame())->GetActiveView()->m_hWnd, &rt);
	CCore::GetInst()->init((((CMainFrame *)AfxGetMainWnd())->GetActiveFrame())->GetActiveView()->m_hWnd, tResolution { (float)rt.Width(), (float)rt.Height() }, true);

	CCore::GetInst()->SetSceneMod(SCENE_MOD::SCENE_STOP);

	((CMainFrame*)m_pMainWnd)->init();

	CreateTestScene();
	((CMainFrame*)m_pMainWnd)->ShowWindowEx(SW_SHOW);

	((CClassView*)((CMainFrame*)m_pMainWnd)->GetObjectView())->Renew();
	((CFileView*)((CMainFrame*)m_pMainWnd)->GetResView())->Renew();


	return TRUE;
}

int CStudioYCSApp::ExitInstance()
{
	//TODO: 추가한 추가 리소스를 처리합니다.
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CStudioYCSApp 메시지 처리기


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg() noexcept;

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() noexcept : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 대화 상자를 실행하기 위한 응용 프로그램 명령입니다.
void CStudioYCSApp::OnAppAbout()
{
	if (CCore::GetInst()->GetSceneMod() == SCENE_MOD::SCENE_PLAY)
	{
		CCore::GetInst()->SetSceneMod(SCENE_MOD::SCENE_STOP);

		if (CSceneMgr::GetInst()->GetCurScene()->_GetUpdate() == true)
		{
			CSceneMgr::GetInst()->GetCurScene()->_SetUpdate(false);
		}
		CSceneMgr::GetInst()->GetCurScene()->_SceneCameraMOD = CAMERA_MOD::ENGINE_MOD;
	}
	else
	{
		CCore::GetInst()->SetSceneMod(SCENE_MOD::SCENE_PLAY);

		if (CSceneMgr::GetInst()->GetCurScene()->_GetUpdate() == false)
		{
			CSceneMgr::GetInst()->GetCurScene()->_SetUpdate(true);
		}
		CSceneMgr::GetInst()->GetCurScene()->_SceneCameraMOD = CAMERA_MOD::CLIENT_MOD;
	}

	//	CAboutDlg aboutDlg;
	//	aboutDlg.DoModal();
}
void CStudioYCSApp::OnAppPause()
{
	if (CSceneMgr::GetInst()->GetCurScene()->_GetUpdate())
	{
		CSceneMgr::GetInst()->GetCurScene()->_SetUpdate(false);
	}
	else
	{
		CSceneMgr::GetInst()->GetCurScene()->_SetUpdate(true);
	}
}

// CStudioYCSApp 사용자 지정 로드/저장 방법

void CStudioYCSApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CStudioYCSApp::LoadCustomState()
{
}

void CStudioYCSApp::SaveCustomState()
{
}

// CStudioYCSApp 메시지 처리기



BOOL CStudioYCSApp::OnIdle(LONG lCount)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CCore::GetInst()->progress();

	TestCode();

	((CMainFrame*)m_pMainWnd)->update();


	return CWinAppEx::OnIdle(0);
}
