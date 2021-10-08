#include "stdafx.h"
#include "Camera.h"

#include "Transform.h"
#include "Device.h"
#include "SceneMgr.h"
#include "RenderMgr.h"
#include "Scene.h"
#include "Layer.h"

#include "KeyMgr.h"
#include "TimeMgr.h"
#include "ResMgr.h"

#include "Core.h"

#include "MeshRender.h"
#include "Animator3D.h"
#include "Animation2D.h"
#include "ParticleSystem.h"
#include "Collider2D.h"
#include "Collider3D.h"
#include "Shader.h"

#include "InstancingBuffer.h"

bool CCamera::UpdateRegistCam = true;

CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA)
	, m_frustum(this)
	, m_fFar(1000.f)
	, m_fNear(1.f)
	, m_fFOV(XM_PI / 4.f)
	, m_fScale(1.f)
	, m_eProjType(PROJ_TYPE::ORTHOGRAPHIC)
	, m_iLayerCheck(0)
	, m_bModule(false)
	, m_fWidth(1920.f)
	, m_fHeight(1080.f)
{
}

CCamera::~CCamera()
{
}

void CCamera::finalupdate()
{
	// 뷰행렬
	Vec3 vPos = Transform()->GetWorldPos();
	Matrix matViewTrans = XMMatrixTranslation(-vPos.x, -vPos.y, -vPos.z);

	Matrix matViewRot = XMMatrixIdentity();
	Vec3 vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);
	Vec3 vUp = Transform()->GetWorldDir(DIR_TYPE::UP);
	Vec3 vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);

	matViewRot._11 = vRight.x; matViewRot._12 = vUp.x; matViewRot._13 = vFront.x;
	matViewRot._21 = vRight.y; matViewRot._22 = vUp.y; matViewRot._23 = vFront.y;
	matViewRot._31 = vRight.z; matViewRot._32 = vUp.z; matViewRot._33 = vFront.z;

	m_matView = matViewTrans * matViewRot;

	// 투영행렬
	m_matProj = XMMatrixIdentity();

	tResolution res = CRenderMgr::GetInst()->GetResolution();

	if (m_eProjType == PROJ_TYPE::PERSPECTIVE)
	{
		m_matProj = XMMatrixPerspectiveFovLH(m_fFOV, res.fWidth / res.fHeight, m_fNear, m_fFar);
	}
	else
	{
		m_matProj = XMMatrixOrthographicLH(m_fWidth * m_fScale, m_fHeight * m_fScale, m_fNear, m_fFar);
		//m_matProj = XMMatrixOrthographicOffCenterLH(0.f, res.fWidth, res.fHeight, 0.f, m_fNear, m_fFar);		
	}

	m_matViewInv = XMMatrixInverse(nullptr, m_matView);
	m_matProjInv = XMMatrixInverse(nullptr, m_matProj);

	// Frustum 계산
	m_frustum.finalupdate();

	// Ray 계산
	CalRay();

	// 모듈로 사용될때는 RenderMgr 에 등록하지 않는다
	if (!m_bModule && CCamera::UpdateRegistCam)
		CRenderMgr::GetInst()->RegisterCamera(this);
}

void CCamera::SortGameObject()
{
	for (auto& pair : m_mapInstGroup_F)
		pair.second.clear();
	for (auto& pair : m_mapInstGroup_D)
		pair.second.clear();
	for (auto& pair : m_mapInstGroup_P)
		pair.second.clear();

	m_vecParticle.clear();
	m_vecPostEffect.clear();

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (m_iLayerCheck & (1 << i))
		{
			const vector<CGameObject*>& vecObj = pCurScene->GetLayer(i)->GetObjects();

			for (UINT j = 0; j < vecObj.size(); ++j)
			{
				if (!vecObj[j]->GetFrustumCheck() || m_frustum.CheckFrustumSphere(vecObj[j]->Transform()->GetWorldPos(), vecObj[j]->Transform()->GetMaxScale()))
				{
					/*if (vecObj[i]->MeshRender()
						&& vecObj[i]->MeshRender()->GetMesh() != nullptr
						&& vecObj[i]->MeshRender()->GetSharedMaterial() != nullptr
						&& vecObj[i]->MeshRender()->GetSharedMaterial()->GetShader() != nullptr)
					{
						if (SHADER_POV::DEFERRED == vecObj[i]->MeshRender()->GetSharedMaterial()->GetShader()->GetShaderPOV())
							m_vecDeferred.push_back(vecObj[i]);
						else if (SHADER_POV::FORWARD == vecObj[i]->MeshRender()->GetSharedMaterial()->GetShader()->GetShaderPOV())
							m_vecForward.push_back(vecObj[i]);
						else if (SHADER_POV::POSTEFFECT == vecObj[i]->MeshRender()->GetSharedMaterial()->GetShader()->GetShaderPOV())
							m_vecPostEffect.push_back(vecObj[i]);
					}
					else if (vecObj[i]->Particlesystem())
					{
						m_vecParticle.push_back(vecObj[i]);
					}*/

					if (vecObj[j]->MeshRender() && vecObj[j]->MeshRender()->GetMesh() != nullptr)
					{
						int iContainer = vecObj[j]->MeshRender()->GetMaterialContainerCount();

						for (int k = 0; k < iContainer; ++k)
						{
							// 
							UINT iMtrlCount = vecObj[j]->MeshRender()->GetMaterialSubsetCount(k);

							for (UINT iMtrl = 0; iMtrl < iMtrlCount; ++iMtrl)
							{
								if (vecObj[j]->MeshRender()->GetSharedMaterial(iMtrl, k) == nullptr
									|| vecObj[j]->MeshRender()->GetSharedMaterial(iMtrl, k)->GetShader() == nullptr)
								{
									continue;
								}

								Ptr<CMaterial> pMtrl = vecObj[j]->MeshRender()->GetSharedMaterial(iMtrl, k);

								// Material 을 참조하고 있지 않거나, Material 에 아직 Shader 를 연결하지 않은 상태라면 Continue
								if (nullptr == pMtrl || pMtrl->GetShader() == nullptr)
									continue;

								// Shader 가 Deferred 인지 Forward 인지에 따라서
								// 인스턴싱 그룹을 분류한다.
								map<ULONG64, vector<tInstObj>>* pMap = NULL;
								if (pMtrl->GetShader()->GetShaderPOV() == SHADER_POV::DEFERRED)
									pMap = &m_mapInstGroup_D;
								else if (pMtrl->GetShader()->GetShaderPOV() == SHADER_POV::FORWARD)
									pMap = &m_mapInstGroup_F;
								else if (pMtrl->GetShader()->GetShaderPOV() == SHADER_POV::POSTEFFECT)
								{								
									pMap = &m_mapInstGroup_P;
									m_vecPostEffect.push_back(vecObj[j]);
								}
								else
									continue;

								uInstID uID = {};
								uID.llID = vecObj[j]->MeshRender()->GetInstID(iMtrl, k);
								map<ULONG64, vector<tInstObj>>::iterator iter = pMap->find(uID.llID);
								if (iter == pMap->end())
								{
									pMap->insert(make_pair(uID.llID, vector<tInstObj>{tInstObj{ vecObj[j], 10 * k + iMtrl }}));
								}
								else
								{
									iter->second.push_back(tInstObj{ vecObj[j], 10 * k + iMtrl });
								}
							}
						}
					}
					else if (vecObj[j]->Particlesystem())
					{
						m_vecParticle.push_back(vecObj[j]);
					}
				}
			}
		}
	}
}

void CCamera::SortShadowObject()
{
	m_vecShadow.clear();

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* pLayer = nullptr;

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		pLayer = pCurScene->GetLayer(i);
		if (nullptr == pLayer || !(m_iLayerCheck & (1 << i)))
			continue;

		const vector<CGameObject*>& vecObj = pLayer->GetObjects();

		for (int j = 0; j < vecObj.size(); ++j)
		{
			if (!vecObj[j]->GetFrustumCheck()
				|| m_frustum.CheckFrustumSphere(vecObj[j]->Transform()->GetWorldPos(), vecObj[j]->Transform()->GetMaxScale()))
			{
				if (vecObj[j]->MeshRender()
					&& vecObj[j]->MeshRender()->GetMesh() != nullptr
					&& vecObj[j]->MeshRender()->GetSharedMaterial() != nullptr
					&& vecObj[j]->MeshRender()->GetSharedMaterial()->GetShader() != nullptr
					&& vecObj[j]->MeshRender()->IsDynamicShadow())
				{
					m_vecShadow.push_back(vecObj[j]);
				}
			}
		}
	}
}

void CCamera::render_deferred()
{
	g_transform.matView = GetViewMat();
	g_transform.matProj = GetProjMat();
	g_transform.matViewInv = m_matViewInv;
	g_transform.matProjInv = m_matProjInv;

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	/*for (int i = 0; i < m_vecDeferred.size(); ++i)
	{
		m_vecDeferred[i]->MeshRender()->render();
	}*/

	for (auto& pair : m_mapSingleObj)
	{
		pair.second.clear();
	}

	for (auto& pair : m_mapDecalObj)
	{
		pair.second.clear();
	}

	tInstancingData tInstData = {};



	for (auto& pair : m_mapInstGroup_D)
	{
		// 06/02  
		// 서브셋 적용
		int iContainer;
		int iSubset;

		if (!pair.second.empty())
		{
			iContainer = pair.second[0].pObj->MeshRender()->GetMaterialContainerCount();
			iSubset = pair.second[0].pObj->MeshRender()->GetMaterialSubsetCount();
		}
		else
		{
			continue;
		}

		// 그룹 오브젝트가 없거나, 쉐이더가 없는 경우
		if (pair.second.empty())
		{
			continue;
		}
		else if (pair.second.size() <= 1 || pair.second[0].pObj->MeshRender()->GetSharedMaterial(pair.second[0].iMtrlIdx)->GetShader()->GetInstVS() == nullptr) // instancing 개수 조건
		{
			for (UINT i = 0; i < pair.second.size(); ++i)
			{
				map<INT_PTR, vector<tInstObj>>::iterator iter
					= m_mapSingleObj.find((INT_PTR)pair.second[i].pObj);

				if (iter != m_mapSingleObj.end())
					iter->second.push_back(pair.second[i]);
				else
				{
					m_mapSingleObj.insert(make_pair((INT_PTR)pair.second[i].pObj, vector<tInstObj>{pair.second[i]}));
				}
			}

			for (UINT i = 0; i < pair.second.size(); ++i)
			{
				map<INT_PTR, vector<tInstObj>>::iterator iter
					= m_mapDecalObj.find((INT_PTR)pair.second[i].pObj);

				if (iter != m_mapDecalObj.end())
					iter->second.push_back(pair.second[i]);
				else
				{
					if (pair.second[i].pObj->GetLayerName() == L"Decal")
					{
						m_mapDecalObj.insert(make_pair((INT_PTR)pair.second[i].pObj, vector<tInstObj>{pair.second[i]}));
					}
				}
			}

			continue;
		}

		CGameObject* pObj = pair.second[0].pObj;
		Ptr<CMesh> pMesh = pObj->MeshRender()->GetMesh();

		//  
		for (int k = 0; k < iContainer; ++k)
		{


			vector<vector<Ptr<CMaterial>>> vecMtrl;

			vecMtrl.resize(iContainer);
			vecMtrl[k].resize(iSubset);

			for (int j = 0; j < iSubset; ++j)
			{
				vecMtrl[k][j] = pObj->MeshRender()->GetSharedMaterial(j, k);

				Ptr<CMaterial> pMtrl = vecMtrl[k][j];

				if (nullptr == pMtrl->GetShader())
					continue;

				CInstancingBuffer::GetInst()->Clear();
				int iRowIdx = 0;
				for (UINT i = 0; i < pair.second.size(); ++i)
				{
					if (pair.second[i].pObj->Animator2D())
					{
						map<INT_PTR, vector<tInstObj>>::iterator iter
							= m_mapSingleObj.find((INT_PTR)pair.second.at(0).pObj);

						if (iter != m_mapSingleObj.end())
						{
							iter->second.push_back(pair.second[i]);
						}
						else
						{
							m_mapSingleObj.insert(make_pair((INT_PTR)pair.second[0].pObj, vector<tInstObj>{pair.second[i]}));
						}
						continue;
					}

					tInstData.matWorld = pair.second[i].pObj->Transform()->GetWorldMat();
					tInstData.matWV = tInstData.matWorld * m_matView;
					tInstData.matWVP = tInstData.matWV * m_matProj;

					if (pair.second[i].pObj->Animator3D())
					{
						tInstData.iRowIdx = iRowIdx++;
						CInstancingBuffer::GetInst()->AddInstancingBoneMat(pair.second[i].pObj->Animator3D()->GetFinalBoneMat());
					}
					else
					{
						tInstData.iRowIdx = -1;
					}

					CInstancingBuffer::GetInst()->AddInstancingData(tInstData);
				}

				// 인스턴싱에 필요한 데이터를 세팅(SysMem -> GPU Mem)
				CInstancingBuffer::GetInst()->SetData();

				pMtrl->UpdateData(1);
				pMesh->render_instancing(j, k);
			}
		}

		// Ptr<CMaterial> pMtrl = pObj->MeshRender()->GetSharedMaterial(pair.second[0].iMtrlIdx);

		//if (nullptr == pMtrl->GetShader())
		//	continue;

		//CInstancingBuffer::GetInst()->Clear();
		//int iRowIdx = 0;
		//for (UINT i = 0; i < pair.second.size(); ++i)
		//{
		//	if (pair.second[i].pObj->Animator2D())
		//	{
		//		map<INT_PTR, vector<tInstObj>>::iterator iter
		//			= m_mapSingleObj.find((INT_PTR)pair.second.at(0).pObj);

		//		if (iter != m_mapSingleObj.end())
		//			iter->second.push_back(pair.second[i]);
		//		else
		//		{
		//			m_mapSingleObj.insert(make_pair((INT_PTR)pair.second[0].pObj, vector<tInstObj>{pair.second[i]}));
		//		}
		//		continue;
		//	}

		//	tInstData.matWorld = pair.second[i].pObj->Transform()->GetWorldMat();
		//	tInstData.matWV = tInstData.matWorld * m_matView;
		//	tInstData.matWVP = tInstData.matWV * m_matProj;

		//	if (pair.second[i].pObj->Animator3D())
		//	{
		//		tInstData.iRowIdx = iRowIdx++;
		//		CInstancingBuffer::GetInst()->AddInstancingBoneMat(pair.second[i].pObj->Animator3D()->GetFinalBoneMat());
		//	}
		//	else
		//		tInstData.iRowIdx = -1;

		//	CInstancingBuffer::GetInst()->AddInstancingData(tInstData);
		//}

		//인스턴싱에 필요한 데이터를 세팅(SysMem->GPU Mem)
		//	CInstancingBuffer::GetInst()->SetData();

		//pMtrl->UpdateData(1);
		//pMesh->render_instancing(pair.second[0].iMtrlIdx);
	}

	// 개별 랜더링
	for (auto& pair : m_mapSingleObj)
	{
		if (pair.second.empty())
			continue;

		if (pair.second[0].pObj->GetLayerName() != L"Decal")
			pair.second[0].pObj->MeshRender()->render();

	//	if (CCore::GetInst()->GetSceneMod() != SCENE_MOD::SCENE_PLAY)
	//	{
	//		if (pair.second[0].pObj->Collider3D())
	//		{
	//		//	pair.second[0].pObj->Collider3D()->render();		// 3D 콜라이더 렌더 아마 std3D.fx가 아닌 std.fx에서 쉐이더를 지정해서 아마 렌더링이 안되는게 아닐까 하는 생각
	//		}
	//	}
	}

	// Decal Render 해결하기 전까진 임시 봉인
	for (auto& pair : m_mapDecalObj)
	{
		if (pair.second.empty())
			continue;
	
		pair.second[0].pObj->MeshRender()->render();
	}
}

void CCamera::render_forward()
{
	g_transform.matView = GetViewMat();
	g_transform.matProj = GetProjMat();
	g_transform.matViewInv = m_matViewInv;
	g_transform.matProjInv = m_matProjInv;

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	for (int i = 0; i < m_vecForward.size(); ++i)
	{
		m_vecForward[i]->MeshRender()->render();

		if (m_vecForward[i]->Collider2D())
			m_vecForward[i]->Collider2D()->render();
	}

	/*for (int i = 0; i < m_vecParticle.size(); ++i)
	{
		m_vecParticle[i]->Particlesystem()->render();
	}*/

	for (int i = 0; i < m_vecDeferred.size(); ++i)
	{
		if (m_vecDeferred[i]->Collider2D())
			m_vecDeferred[i]->Collider2D()->render();
	}
}

void CCamera::render_posteffect()
{
	g_transform.matView = GetViewMat();
	g_transform.matProj = GetProjMat();
	g_transform.matViewInv = m_matViewInv;
	g_transform.matProjInv = m_matProjInv;

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	
	// Render 순서 변경
	for (int i = 0; i < m_vecParticle.size(); ++i)
	{
		m_vecParticle[i]->Particlesystem()->render();
	}

	for (int i = 0; i < m_vecPostEffect.size(); ++i)
	{
		CRenderMgr::GetInst()->CopySwapToPosteffect();
		m_vecPostEffect[i]->MeshRender()->render();
	}
}

void CCamera::render_shadowmap()
{
	// 뷰행렬과 투영행렬을 광원시점 카메라의 것으로 대체해둠
	g_transform.matView = m_matView;
	g_transform.matProj = m_matProj;
	g_transform.matViewInv = m_matViewInv;

	for (UINT i = 0; i < m_vecShadow.size(); ++i)
	{
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"ShadowMapMtrl");

		if (m_vecShadow[i]->Animator3D())
		{
			// m_vecShadow[i]->Animator3D()->UpdateData();			//이승훈 질문
		}

		m_vecShadow[i]->MeshRender()->render_shadowmap();
	}
}

void CCamera::render()
{
	g_transform.matView = GetViewMat();
	g_transform.matProj = GetProjMat();

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (m_iLayerCheck & (1 << i))
		{
			const vector<CGameObject*>& vecObj = pCurScene->GetLayer(i)->GetObjects();

			for (UINT i = 0; i < vecObj.size(); ++i)
			{
				if (!vecObj[i]->GetFrustumCheck()
					|| m_frustum.CheckFrustumSphere(vecObj[i]->Transform()->GetWorldPos(), vecObj[i]->Transform()->GetMaxScale()))
				{
					if (vecObj[i]->MeshRender())
					{
						vecObj[i]->MeshRender()->render();
					}

					if (vecObj[i]->Collider2D())
					{
						vecObj[i]->Collider2D()->render();
					}
				}
			}
		}
	}
}

void CCamera::renderClient_ADDVer()
{
	g_transform.matView = GetViewMat();
	g_transform.matProj = GetProjMat();

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (m_iLayerCheck & (1 << i))
		{
			const vector<CGameObject*>& vecObj = pCurScene->GetLayer(i)->GetObjects();

			for (UINT i = 0; i < vecObj.size(); ++i)
			{
				if ((!vecObj[i]->GetFrustumCheck()
					|| m_frustum.CheckFrustumSphere(vecObj[i]->Transform()->GetWorldPos(), vecObj[i]->Transform()->GetMaxScale())) && vecObj[i]->GetADD_Render() == RENDER_ADD::RENDER_CLIENT)
				{
					if (vecObj[i]->MeshRender())
					{
						vecObj[i]->MeshRender()->render();
					}

					if (vecObj[i]->Collider2D())
					{
						vecObj[i]->Collider2D()->render();
					}
				}
			}
		}
	}
}

void CCamera::renderEngine_ADDVer()
{
	g_transform.matView = GetViewMat();
	g_transform.matProj = GetProjMat();

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (m_iLayerCheck & (1 << i))
		{
			const vector<CGameObject*>& vecObj = pCurScene->GetLayer(i)->GetObjects();

			for (UINT i = 0; i < vecObj.size(); ++i)
			{
				if ((!vecObj[i]->GetFrustumCheck()
					|| m_frustum.CheckFrustumSphere(vecObj[i]->Transform()->GetWorldPos(), vecObj[i]->Transform()->GetMaxScale())) && vecObj[i]->GetADD_Render() == RENDER_ADD::RENDER_ENGINE)
				{
					if (vecObj[i]->MeshRender())
					{
						vecObj[i]->MeshRender()->render();
					}

					if (vecObj[i]->Collider2D())
					{
						vecObj[i]->Collider2D()->render();
					}
				}
			}
		}
	}
}

void CCamera::ColliderRender()
{
	// 개별 랜더링
	for (auto& pair : m_mapSingleObj)
	{
		if (pair.second.empty())
			continue;

	//	pair.second[0].pObj->MeshRender()->render();

		if (CCore::GetInst()->GetSceneMod() != SCENE_MOD::SCENE_PLAY)
		{
			if (pair.second[0].pObj->Collider3D())
			{
				pair.second[0].pObj->Collider3D()->render();		// 3D 콜라이더 렌더 아마 std3D.fx가 아닌 std.fx에서 쉐이더를 지정해서 아마 렌더링이 안되는게 아닐까 하는 생각
			}
		}
	}
}

void CCamera::SetProjType(PROJ_TYPE _eType)
{
	m_eProjType = _eType;
	Changed();

	m_matProj = XMMatrixIdentity();
	tResolution res = CRenderMgr::GetInst()->GetResolution();

	if (m_eProjType == PROJ_TYPE::PERSPECTIVE)
	{
		m_matProj = XMMatrixPerspectiveFovLH(m_fFOV, res.fWidth / res.fHeight, m_fNear, m_fFar);
	}
	else
	{
		m_matProj = XMMatrixOrthographicLH(res.fWidth * m_fScale, res.fHeight * m_fScale, m_fNear, m_fFar);
	}

	g_transform.matProj = m_matProj;
}


void CCamera::SetChangeRes(bool _SetRes)
{
	m_bChangeRes = _SetRes;
}

void CCamera::SetResolution(tResolution _m_tViewRect)
{
	m_tViewRect = _m_tViewRect;
}

void CCamera::render_Decal()
{
	for (auto& pair : m_vecDecalObj)
	{
		pair->MeshRender()->render();
	}
}

void CCamera::DecalPushBack(CGameObject* _DecalObj)
{
	m_vecDecalObj.push_back(_DecalObj);
}


void CCamera::CalRay()
{
	POINT ptMousePos = CKeyMgr::GetInst()->GetMousePos();

	D3D11_VIEWPORT tVP = {};
	UINT iSize = 1;
	CONTEXT->RSGetViewports(&iSize, &tVP);

	if (iSize == 0)
	{
		tResolution res = CRenderMgr::GetInst()->GetResolution();
		tVP.Width = res.fWidth;
		tVP.Height = res.fHeight;
		tVP.MinDepth = 1.f;
		tVP.MaxDepth = 1.f;
		tVP.TopLeftX = 0.f;
		tVP.TopLeftY = 0.f;
	}
	if (!m_bChangeRes)
	{
		m_tRay.vStart = Transform()->GetWorldPos();
		m_tRay.vDir.x = ((((ptMousePos.x - tVP.TopLeftX) * 2.f / tVP.Width) - 1.f) - m_matProj._31) / m_matProj._11;
		m_tRay.vDir.y = (-(((ptMousePos.y - tVP.TopLeftY) * 2.f / tVP.Height) - 1.f) - m_matProj._32) / m_matProj._22;
		m_tRay.vDir.z = 1.f;

		m_tRay.vDir = XMVector3TransformNormal(m_tRay.vDir, m_matViewInv);
		m_tRay.vDir.Normalize();
	}
	else
	{
		float WidthRatio = tVP.Width / m_tViewRect.fWidth;
		float HeightRatio = tVP.Height / m_tViewRect.fHeight;
		m_tRay.vStart = Transform()->GetWorldPos();
		m_tRay.vDir.x = ((((ptMousePos.x - tVP.TopLeftX) * 2.f / tVP.Width * WidthRatio) - 1.f) - m_matProj._31) / m_matProj._11;
		m_tRay.vDir.y = (-(((ptMousePos.y - tVP.TopLeftY) * 2.f / tVP.Height * HeightRatio) - 1.f) - m_matProj._32) / m_matProj._22;
		m_tRay.vDir.z = 1.f;

		m_tRay.vDir = XMVector3TransformNormal(m_tRay.vDir, m_matViewInv);
		m_tRay.vDir.Normalize();
	}
}

void CCamera::SaveToScene(FILE * _pFile)
{
	UINT iType = (UINT)GetComponentType();
	fwrite(&iType, sizeof(UINT), 1, _pFile);

	fwrite(&m_fNear, sizeof(float), 1, _pFile);
	fwrite(&m_fFar, sizeof(float), 1, _pFile);

	fwrite(&m_fFOV, sizeof(float), 1, _pFile);
	fwrite(&m_fScale, sizeof(float), 1, _pFile);

	fwrite(&m_eProjType, sizeof(UINT), 1, _pFile);
	fwrite(&m_iLayerCheck, 4, 1, _pFile);
}

void CCamera::LoadFromScene(FILE * _pFile)
{
	fread(&m_fNear, sizeof(float), 1, _pFile);
	fread(&m_fFar, sizeof(float), 1, _pFile);

	fread(&m_fFOV, sizeof(float), 1, _pFile);
	fread(&m_fScale, sizeof(float), 1, _pFile);

	fread(&m_eProjType, sizeof(UINT), 1, _pFile);
	fread(&m_iLayerCheck, 4, 1, _pFile);
}
