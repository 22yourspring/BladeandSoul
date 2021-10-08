// CGameView.cpp: 구현 파일
//

#include "pch.h"
#include "StudioYCS.h"
#include "GameView.h"

#include "MainFrm.h"
//#include "InfoView.h"
//#include "HierachyView.h"
#include "ClassView.h"

#include <Engine/SceneMgr.h>
#include <Engine/GameObject.h>

#include <Engine/Core.h>
#include <Engine/Transform.h>
#include <Engine/Camera.h>
#include <Engine/Device.h>
#include <Engine/Shader.h>
#include <Engine/Material.h>
#include <Engine/MRT.h>
#include "ToolCamScript.h"
#include "GridScript.h"

#include <Engine/Camera.h>

#include <Engine/Terrain.h>

#include "Engine/SSLRManager.h"
#include "Engine/MRT.h"
#include "Engine/RenderMgr.h"
#include "Engine/RenderTarget24.h"
#include "Engine/Texture.h"
#include "Engine/SSAOManager.h"
#include "Engine/ResMgr.h"
#include "Engine/PostFX.h"
#include "Engine/ConstBuffer.h"

bool CGameView::ViewFocus = false;

IMPLEMENT_DYNCREATE(CGameView, CStudioYCSView)
CGameView::CGameView()
	: m_pToolCam(nullptr)
	, TessLevel(0.f, 0.f, 0.f)
	, PrevLevel(0.f, 0.f, 0.f)
{

}

CGameView::~CGameView()
{
	SAFE_DELETE(m_pToolCam);
	SAFE_DELETE(m_pGridObject);

	CMaterial* pMtrl = m_pGridMtrl.GetPointer();
	SAFE_DELETE(pMtrl);

	CShader* pShader = m_pGridShader.GetPointer();
	SAFE_DELETE(pShader);

	Safe_Delete_Vector(m_vecTargetUI);
}

BEGIN_MESSAGE_MAP(CGameView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CGameView 그리기

void CGameView::OnDraw(CDC* pDC)
{
	//	CDocument* pDoc = GetDocument();
		// TODO: 여기에 그리기 코드를 추가합니다.
	//	Rectangle(pDC->m_hDC, 0, 0, 10, 10);
}


// CGameView 진단

#ifdef _DEBUG
void CGameView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CGameView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG

void CGameView::init()
{
	// Tool Resource
	m_pGridShader = new CShader;

	m_pGridShader->CreateVertexShader(L"Shader\\tool.fx", "VS_Grid", "vs_5_0");
	m_pGridShader->CreatePixelShader(L"Shader\\tool.fx", "PS_Grid", "ps_5_0");
	m_pGridShader->SetBlendState(CRenderMgr::GetInst()->GetBlendState(BLEND_TYPE::ALPHABLEND));
	m_pGridShader->SetRSType(RS_TYPE::CULL_NONE);
	m_pGridShader->SetDepthStencilState(CRenderMgr::GetInst()->GetDepthStencilState(DEPTH_STENCIL_STATE::NO_TEST_NO_WIRTE));
	m_pGridShader->SetName(L"GridShader");

	m_pGridMtrl = new CMaterial;
	m_pGridMtrl->SetName(L"Material\\GridMtrl.mtrl");
	m_pGridMtrl->DisableFileSave();
	m_pGridMtrl->SetShader(m_pGridShader);
	Ptr<CTexture> pPositionTargetTex = CResMgr::GetInst()->FindRes<CTexture>(L"PositionTargetTex");
	m_pGridMtrl->SetData(SHADER_PARAM::TEX_0, &pPositionTargetTex);

	// Tool Camera 만들기
	m_pToolCam = new CGameObject;

	m_pToolCam->AddComponent(new CTransform);
	m_pToolCam->AddComponent(new CCamera);
	m_pToolCam->AddComponent(new CToolCamScript);

	m_pToolCam->Camera()->SetFar(500000.f);
	m_pToolCam->Camera()->SetLayerAllCheck();
	m_pToolCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	CRenderMgr::GetInst()->RegisterToolCamera(m_pToolCam->Camera());

	m_pDirMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"DirMtrl");
	m_pDirMesh = CResMgr::GetInst()->FindRes<CMesh>(L"DirMesh");

	// Grid Object
	m_pGridObject = new CGameObject;

	m_pGridObject->AddComponent(new CTransform);
	m_pGridObject->AddComponent(new CMeshRender);
	m_pGridObject->AddComponent(new CGridScript);
	m_pGridObject->GetScript<CGridScript>()->SetToolCamera(m_pToolCam);

	m_pGridObject->Transform()->SetLocalScale(Vec3(1000000.f, 1000000.f, 1.f));
	m_pGridObject->Transform()->SetLocalRot(Vec3(XM_PI / 2.f, 0.f, 0.f));
	m_pGridObject->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	m_pGridObject->MeshRender()->SetMaterial(m_pGridMtrl);

	// Target UI Object 만들기
	CGameObject* pTargetUI = nullptr;
	float fSize = 150.f;

	for (UINT i = 0; i < 6; ++i)
	{
		pTargetUI = new CGameObject;
		pTargetUI->AddComponent(new CTransform);
		pTargetUI->AddComponent(new CMeshRender);

		pTargetUI->Transform()->SetLocalPos(Vec3(-(float)(GAMEVIEW_X * 0.5f) + (fSize * 0.5f) + (float)i * fSize
			, (float)(GAMEVIEW_Y * 0.5f) - (fSize * 0.5f)
			, 100.f));

		pTargetUI->Transform()->SetLocalScale(Vec3(fSize, fSize, 1.f));
		pTargetUI->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));

		pTargetUI->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"2DUIMtrl"));
		pTargetUI->MeshRender()->GetCloneMaterial();
		pTargetUI->MeshRender()->GetSharedMaterial()->DisableFileSave();

		m_vecTargetUI.push_back(pTargetUI);
	}

	Ptr<CTexture> pTex = nullptr;

	m_vecTargetUI[0]->SetName(L"DiffuseTargetUI");
	m_vecTargetUI[1]->SetName(L"NormalTargetUI");
	m_vecTargetUI[2]->SetName(L"PositionTargetUI");
	m_vecTargetUI[3]->SetName(L"LightTargetUI");
	m_vecTargetUI[4]->SetName(L"SpecularTargetUI");
	m_vecTargetUI[5]->SetName(L"DepthBufferTargetUI");

	pTex = CResMgr::GetInst()->FindRes<CTexture>(L"DiffuseTargetTex");
	m_vecTargetUI[0]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, &pTex);

	pTex = CResMgr::GetInst()->FindRes<CTexture>(L"NormalTargetTex");
	m_vecTargetUI[1]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, &pTex);

	pTex = CResMgr::GetInst()->FindRes<CTexture>(L"PositionTargetTex");
	m_vecTargetUI[2]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, &pTex);

	pTex = CResMgr::GetInst()->FindRes<CTexture>(L"LightTargetTex");
	m_vecTargetUI[3]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, &pTex);

	pTex = CResMgr::GetInst()->FindRes<CTexture>(L"SpecularTargetTex");
	m_vecTargetUI[4]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, &pTex);

	pTex = CResMgr::GetInst()->FindRes<CTexture>(L"DepthBufferTargetTex");
	m_vecTargetUI[5]->MeshRender()->GetSharedMaterial()->SetData(SHADER_PARAM::TEX_0, &pTex);

	// HDR 텍스쳐 문제 생김 고쳐야댐
	//CResMgr::GetInst()->CreateTexture(L"HDR", GAMEVIEW_X, GAMEVIEW_Y, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, DXGI_FORMAT_R16G16B16A16_SINT);


	D3D11_TEXTURE2D_DESC dtd = {
		787, //UINT Width;
		633, //UINT Height;
		1, //UINT MipLevels;
		1, //UINT ArraySize;
		DXGI_FORMAT_R16G16B16A16_TYPELESS, //DXGI_FORMAT Format;
		1, //DXGI_SAMPLE_DESC SampleDesc;
		0,
		D3D11_USAGE_DEFAULT,//D3D11_USAGE Usage;
		D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE,//UINT BindFlags;
		0,//UINT CPUAccessFlags;
		0//UINT MiscFlags;    
	};
	DEVICE->CreateTexture2D(&dtd, NULL, &m_pHDRTexture);

	D3D11_RENDER_TARGET_VIEW_DESC rtsvd =
	{
		DXGI_FORMAT_R16G16B16A16_FLOAT,
		D3D11_RTV_DIMENSION_TEXTURE2D
	};
	DEVICE->CreateRenderTargetView(m_pHDRTexture, &rtsvd, &m_HDRRTV);

	D3D11_SHADER_RESOURCE_VIEW_DESC dsrvd =
	{
		DXGI_FORMAT_R16G16B16A16_FLOAT,
		D3D11_SRV_DIMENSION_TEXTURE2D,
		0,
		0
	};
	dsrvd.Texture2D.MipLevels = 1;
	DEVICE->CreateShaderResourceView(m_pHDRTexture, &dsrvd, &m_HDRSRV);

}

void CGameView::update()
{
	if (this == nullptr)
	{
		return;
	}

	if (m_pToolCam == nullptr || m_pMainCam == nullptr)
	{
		return;
	}

	float ClearColor[4] = { 0.3f, 0.32, 0.7, 0.0f };
	CONTEXT->ClearRenderTargetView(m_HDRRTV, ClearColor);

	if (CCore::GetInst()->GetSceneMod() == SCENE_MOD::SCENE_PLAY)
	{
		m_pMainCam->update();
		m_pMainCam->lateupdate();
		CCamera::SetRegistCam(false);
		m_pMainCam->finalupdate();
		CCamera::SetRegistCam(true);
	}
	else
	{
		m_pToolCam->update();
		m_pToolCam->lateupdate();

		CCamera::SetRegistCam(false);
		m_pToolCam->finalupdate();
		CCamera::SetRegistCam(true);

		m_pGridObject->update();
		m_pGridObject->lateupdate();
		m_pGridObject->finalupdate();
	}

	for (size_t i = 0; i < m_vecTargetUI.size(); ++i)
	{
		m_vecTargetUI[i]->finalupdate();
	}

	Ptr<CMaterial> pTerrainMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TerrainMtrl");

	Vec3 vCamPos;
	if (CCore::GetInst()->GetSceneMod() == SCENE_MOD::SCENE_PLAY)
	{
		vCamPos = m_pMainCam->Transform()->GetWorldPos();
	}
	else
	{
		vCamPos = m_pToolCam->Transform()->GetLocalPos();
	}

	if (CPropertiesWnd::StaticTarget != nullptr && CPropertiesWnd::StaticTarget->Terrain())
	{
		TessLevel = CPropertiesWnd::StaticTarget->Terrain()->GetTessLevel();
	}
	else if (TessLevel != PrevLevel)
	{
		TessLevel = { 20000.f, 80000.f, 4.f };
	}
	PrevLevel = TessLevel;

	pTerrainMtrl->SetData(SHADER_PARAM::VEC4_0, &vCamPos);
	pTerrainMtrl->SetData(SHADER_PARAM::VEC4_1, &TessLevel);

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
	else
	{
		// Render
		m_pToolCam->Camera()->SortGameObject();
		// Deferred Render
		m_pToolCam->Camera()->render_deferred();
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
			CRenderMgr::GetInst()->GetCamera(i)->renderClient_ADDVer();	//skybox 같은 녀석들 추가 렌더링
		}
	}
	else
	{
		m_pToolCam->Camera()->render_forward();
	}

	// Light Merge

	//CRenderMgr::GetInst()->merge_light();

	ID3D11ShaderResourceView *arrSRV[1] = { NULL };
	CONTEXT->PSSetShaderResources(5, 1, arrSRV);

	

	// Clear the depth stencil

	auto Depth = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->m_pDepthStencilTex;
	auto srv = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->m_pDepthStencilTex->GetSRV();
	auto srv2 = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::DEFERRED)->m_arrRT[4]->GetRTTex()->GetSRV();

	CSSAOManager::GetInst()->Compute(CONTEXT, CRenderMgr::GetInst()->GetMRT(MRT_TYPE::DEFERRED)->m_arrRT[4]->GetRTTex()->GetSRV(), CRenderMgr::GetInst()->GetMRT(MRT_TYPE::DEFERRED)->m_arrRT[1]->GetRTTex()->GetSRV());
	arrSRV[0] = CSSAOManager::GetInst()->GetSSAOSRV();
	CONTEXT->PSSetShaderResources(5, 1, arrSRV);

	CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"Billboard")->GetObjects();
	Vec3 vDir(1.5f, -1.f, 1.f);
	Vec3 vColor(1.f, 1.f, 1.f);

	/*if (CRenderMgr::GetInst()->GetMainCamera()->m_pMoon != nullptr)
	{
		auto sunPos = CRenderMgr::GetInst()->GetMainCamera()->m_pMoon->Transform()->GetWorldPos();
		auto camPos = CRenderMgr::GetInst()->GetMainCamera()->Transform()->GetWorldPos();

		vDir = camPos - sunPos;
		vDir.Normalize();
	}*/

	DirectX::SimpleMath::Matrix mSunRotation = DirectX::SimpleMath::Matrix::CreateFromAxisAngle(vDir,XM_PI / 3);
	vDir = *((Vec3*)&mSunRotation._31);

	CSSLRManager::GetInst()->Render(CONTEXT, m_HDRRTV, CSSAOManager::GetInst()->GetSSAOSRV(), vDir.Normalize(), vColor);

	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->OMSet();
	CRenderMgr::GetInst()->merge_light();

	//CONTEXT->OMSetRenderTargets(1, &m_HDRRTV, CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->m_pDepthStencilTex->GetDSV());
	//CRenderMgr::GetInst()->merge_light();
	/*

	auto cbUnPack = CDevice::GetInst()->FindConstBuffer(L"UNPACK");

	D3D11_MAPPED_SUBRESOURCE MappedResource;
	CONTEXT->Map(cbUnPack->GetBuffer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);

	tUnPack* pGBufferUnpackCB = (tUnPack*)MappedResource.pData;
	
	auto pProj = CRenderMgr::GetInst()->GetMainCam()->GetProjMat();
	pGBufferUnpackCB->PerspectiveValues.x = 1.0f / pProj.m[0][0];
	pGBufferUnpackCB->PerspectiveValues.y = 1.0f / pProj.m[1][1];
	pGBufferUnpackCB->PerspectiveValues.z = pProj.m[3][2];
	pGBufferUnpackCB->PerspectiveValues.w = -pProj.m[2][2];

	CRenderMgr::GetInst()->GetMainCam()->Transform()->GetWorldMat().Transpose(pGBufferUnpackCB->ViewInv);

	CONTEXT->Unmap(cbUnPack->GetBuffer(), 0);

	CONTEXT->PSSetConstantBuffers(0, 1, cbUnPack->GetBufferPointer());*/

	//g_GBuffer.PrepareForUnpack(CONTEXT);
	//g_LightManager.DoLighting(CONTEXT);
	

	//auto hdrTexture = CResMgr::GetInst()->FindRes<CTexture>(L"HDR");

	

	//CSSLRManager::GetInst()->Render(CONTEXT, m_HDRRTV, CSSAOManager::GetInst()->GetSSAOSRV(), vDir, vColor);


	//CONTEXT->PSSetShaderResources(_iRegisterNum, 1, &CSSLRManager::GetInst()->m_pLightRaysSRV);
	//CPostFX::GetInst()->PostProcessing(CONTEXT, m_HDRSRV, CRenderMgr::GetInst()->GetMRT(MRT_TYPE::DEFERRED)->m_arrRT[4]->GetRTTex()->GetSRV(), CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->m_arrRT[0]->GetRTTex()->GetRTV());

	/*auto* pTemp = CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->m_arrRT[0]->GetRTTex()->GetRTV();
	CONTEXT->OMSetRenderTargets(1, &pTemp, CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SWAPCHAIN)->m_pDepthStencilTex->GetDSV());*/

	//g_PostFX.PostProcessing(pd3dImmediateContext, m_HDRSRV, g_GBuffer.GetDepthView(), DXUTGetD3D11RenderTargetView());
	//pd3dImmediateContext->OMSetRenderTargets(1, &pRTV, g_GBuffer.GetDepthDSV());

	// Decal Render
	if (CCore::GetInst()->GetSceneMod() == SCENE_MOD::SCENE_PLAY)
	{
		m_pMainCam->Camera()->render_Decal();
	}
	else
	{
		m_pToolCam->Camera()->render_Decal();
	}

	// GridRender
	if (CCore::GetInst()->GetSceneMod() != SCENE_MOD::SCENE_PLAY)
	{
		// Tool Object 추가 렌더
		// GridRender
		m_pToolCam->Camera()->ColliderRender();
		m_pGridObject->MeshRender()->render();
	}

	// posteffect render
	if (CCore::GetInst()->GetSceneMod() == SCENE_MOD::SCENE_PLAY)
	{
		m_pMainCam->Camera()->render_posteffect();
	}
	else
	{
		m_pToolCam->Camera()->render_posteffect();
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
	else
	{
		m_pToolCam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	}

	{
		for (size_t i = 0; i < m_vecTargetUI.size(); ++i)
		{
			m_vecTargetUI[i]->MeshRender()->render();
		}
	}


	if (CCore::GetInst()->GetSceneMod() == SCENE_MOD::SCENE_PLAY)
	{
		m_pMainCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	}
	else
	{
		m_pToolCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
	}

	//	CTimeMgr::GetInst()->render();
	CRenderMgr::GetInst()->Present();
}

void CGameView::SetMainCam(CGameObject * _MainCam)
{
	m_pMainCam = _MainCam;
}


BOOL CGameView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;

	return CView::PreCreateWindow(cs);
}

void CGameView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!m_pToolCam)
	{
		return;
	}
	vector<CGameObject*> vecOut;
	CSceneMgr::GetInst()->FindGameObjectByPoint(point, vecOut, m_pToolCam->Camera());

	if (!vecOut.empty())
	{
		//	((CInfoView*)((CMainFrame*)AfxGetMainWnd())->GetInfoView())->SetTarget(vecOut[0]);
		//	((CClassView*)((CMainFrame*)AfxGetMainWnd())->GetObjectView())->m_wndClassView.SelectObject(vecOut[0]); // 이승훈 레이충돌 기즈모
	}
	else
	{
		//	((CInfoView*)((CMainFrame*)AfxGetMainWnd())->GetInfoView())->SetTarget(nullptr);
	}

	CView::OnLButtonDown(nFlags, point);
}


void CGameView::OnKillFocus(CWnd* pNewWnd)
{
	CStudioYCSView::OnKillFocus(pNewWnd);

	ViewFocus = false;
	//	ShowCursor(true);

		// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CGameView::OnSetFocus(CWnd* pOldWnd)
{
	CStudioYCSView::OnSetFocus(pOldWnd);
	ViewFocus = true;
	//	ShowCursor(false);

		// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CGameView::OnSize(UINT nType, int cx, int cy)
{
	CStudioYCSView::OnSize(nType, cx, cy);

	if (m_pToolCam)
	{
		((CCamera*)(m_pToolCam->GetComponent(COMPONENT_TYPE::CAMERA)))->SetChangeRes(true);

		tResolution ModifRes;
		ModifRes.fWidth = cx;
		ModifRes.fHeight = cy;

		((CCamera*)(m_pToolCam->GetComponent(COMPONENT_TYPE::CAMERA)))->SetResolution(ModifRes);
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
