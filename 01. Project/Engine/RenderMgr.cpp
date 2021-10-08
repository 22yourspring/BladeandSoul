#include "stdafx.h"
#include "RenderMgr.h"

#include "Device.h"
#include "Core.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "Camera.h"

#include "SceneMgr.h"
#include "ConstBuffer.h"

#include "TimeMgr.h"
#include "EventMgr.h"

#include "Light3D.h"

#include "RenderTarget24.h"
#include "MRT.h"

#include "ResMgr.h"
#include "ConstBuffer.h"
#include "SSLRManager.h"

CRenderMgr::CRenderMgr()
	: m_pSwapChain(nullptr)
	, m_arrRT{}
	, m_arrMRT{}
	, m_tViewportDesc{}
	, m_arrBlendState{}
	, m_arrDepthStencilState{}
	, m_pSampler0(nullptr)
	, m_pSampler1(nullptr)
	, m_iLight2DCount(0)
	, m_pToolCam(nullptr)
	, m_pMainCam(nullptr)
{
}

CRenderMgr::~CRenderMgr()
{
	Safe_Delete_Array(m_arrBlendState);
	Safe_Delete_Array(m_arrDepthStencilState);

	SAFE_RELEASE(m_pSampler0);
	SAFE_RELEASE(m_pSampler1);
	SAFE_RELEASE(m_pSwapChain);

	Safe_Delete_Array(m_arrRT);
	Safe_Delete_Array(m_arrMRT);
}

void CRenderMgr::render()
{
	// 초기화
	float arrColor[4] = { 0.f, 0.f, 0.f, 1.f };
	Clear(arrColor);

	// 전역 상수버퍼 업데이트
	static CConstBuffer* pGlobalBuffer = CDevice::GetInst()->FindConstBuffer(L"GlobalValue");
	pGlobalBuffer->SetData(&g_global);
	pGlobalBuffer->UpdateData(ST_ALL);

	// 광원 정보 업데이트
	UpdateLight2D();
	UpdateLight3D();

	//for (size_t i = 0; i < m_vecCam.size(); ++i)
	//{
	//	m_vecCam[i]->render();	//용도가 궁금함
	//}

	// TimeMgr
//	CTimeMgr::GetInst()->render();
	m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->OMSet();		//용도가 궁금함

	// 출력
//	Present(); //용도가 궁금함
}

void CRenderMgr::render_tool()
{
	// 초기화
	float arrColor[4] = { 0.f, 0.f, 0.f, 1.f };
	Clear(arrColor);

	// 전역 상수버퍼 업데이트
	static CConstBuffer* pGlobalBuffer = CDevice::GetInst()->FindConstBuffer(L"GlobalValue");
	pGlobalBuffer->SetData(&g_global);
	pGlobalBuffer->UpdateData(ST_ALL);

	// 광원 정보 업데이트
	UpdateLight2D();
	UpdateLight3D();

	m_arrMRT[(UINT)MRT_TYPE::DEFERRED]->OMSet();
}

void CRenderMgr::render_shadowmap()
{
	CRenderMgr::GetInst()->GetMRT(MRT_TYPE::SHADOWMAP)->OMSet();

	// 광원 시점으로 깊이를 그림
	for (UINT i = 0; i < m_vecLight3D.size(); ++i)
	{
		if (m_vecLight3D[i]->GetLight3DInfo().iLightType != (UINT)LIGHT_TYPE::DIR)
			continue;

		m_vecLight3D[i]->render_shadowmap();
	}

	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->OMSet(); // 메인카메라 깊이로 되돌리기 위해서
}

void CRenderMgr::render_lights()
{
	m_arrMRT[(UINT)MRT_TYPE::LIGHT]->OMSet();

	// 광원을 그린다.
	CCamera* pMainCam = CRenderMgr::GetInst()->GetMainCam();
	if (nullptr == pMainCam)
		return;

	g_transform.matView = pMainCam->GetViewMat();
	g_transform.matProj = pMainCam->GetProjMat();
	g_transform.matViewInv = pMainCam->GetViewMatInv();

	for (size_t i = 0; i < m_vecLight3D.size(); ++i)
	{
		m_vecLight3D[i]->Light3D()->render();
	}

	m_vecLight3D.clear();

	m_arrMRT[(UINT)MRT_TYPE::SWAPCHAIN]->OMSet();
}

void CRenderMgr::merge_light()
{
	// ========================
	// Merge (Diffuse + Lights)
	// ========================
	static Ptr<CMesh> pRectMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
	static Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"MergeLightMtrl");

	pMtrl->UpdateData();

	CONTEXT->PSSetShaderResources(4, 1, &CSSLRManager::GetInst()->m_pLightRaysSRV);

	pRectMesh->render();
}

void CRenderMgr::UpdateLight2D()
{
	static CConstBuffer* pLight2DBuffer = CDevice::GetInst()->FindConstBuffer(L"Light2D");

	UINT iArr[4] = { m_iLight2DCount, };

	pLight2DBuffer->SetData(m_arrLight2D, sizeof(tLight2D) * 100);
	pLight2DBuffer->SetData(iArr, 16);
	pLight2DBuffer->UpdateData((UINT)SHADER_TYPE::ST_VERTEX | (UINT)SHADER_TYPE::ST_PIXEL);

	m_iLight2DCount = 0;
}

void CRenderMgr::UpdateLight3D()
{
	static CConstBuffer* pLight3DBuffer = CDevice::GetInst()->FindConstBuffer(L"Light3D");
	static struct tLight3DInfo
	{
		tLight3D arrLight3D[100];
		UINT	 iCurCount;
		Vec3     vPadding;
	}tLight3DArray;

	for (size_t i = 0; i < m_vecLight3D.size(); ++i)
	{
		const tLight3D& info = m_vecLight3D[i]->GetLight3DInfo();
		tLight3DArray.arrLight3D[i] = info;
	}
	tLight3DArray.iCurCount = (UINT)m_vecLight3D.size();

	pLight3DBuffer->SetData(&tLight3DArray, sizeof(tLight3DArray));
	pLight3DBuffer->UpdateData((UINT)SHADER_TYPE::ST_VERTEX | (UINT)SHADER_TYPE::ST_PIXEL);
}

CCamera * CRenderMgr::GetMainCam()
{
	if (CCore::GetInst()->GetSceneMod() == SCENE_MOD::SCENE_PLAY)
	{
		if (!m_vecCam.empty())
			return m_vecCam[0];
		return nullptr;
	}

	return m_pToolCam;
}

void CRenderMgr::Clear(float(&_arrFloat)[4])
{
	// MRT Clear
	for (UINT i = 0; i < (UINT)MRT_TYPE::END; ++i)
	{
		if (nullptr != m_arrMRT[i])
			m_arrMRT[i]->Clear();
	}

	// Register Clear
	CTexture::ClearAllData();
	CTexture::ClearAllRWData();
}

void CRenderMgr::UpdateSampler()
{
	CONTEXT->VSSetSamplers(0, 1, &m_pSampler0);
	CONTEXT->HSSetSamplers(0, 1, &m_pSampler0);
	CONTEXT->DSSetSamplers(0, 1, &m_pSampler0);
	CONTEXT->GSSetSamplers(0, 1, &m_pSampler0);
	CONTEXT->CSSetSamplers(0, 1, &m_pSampler0);
	CONTEXT->PSSetSamplers(0, 1, &m_pSampler0);

	CONTEXT->VSSetSamplers(1, 1, &m_pSampler1);
	CONTEXT->HSSetSamplers(1, 1, &m_pSampler1);
	CONTEXT->DSSetSamplers(1, 1, &m_pSampler1);
	CONTEXT->GSSetSamplers(1, 1, &m_pSampler1);
	CONTEXT->CSSetSamplers(1, 1, &m_pSampler1);
	CONTEXT->PSSetSamplers(1, 1, &m_pSampler1);
}

void CRenderMgr::CopySwapToPosteffect()
{
	CONTEXT->CopyResource(m_arrRT[(UINT)RT_TYPE::POSTEFFECT]->GetRTTex()->GetTex2D()
		, m_arrRT[(UINT)RT_TYPE::SWAPCHAIN]->GetRTTex()->GetTex2D());
}