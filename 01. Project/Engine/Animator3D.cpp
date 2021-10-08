#include "stdafx.h"
#include "Animator3D.h"

#include "TimeMgr.h"
#include "MeshRender.h"
#include "StructuredBuffer.h"
#include "ResMgr.h"
#include "BoneSocket.h"
#include "Transform.h"

#include "AnimClip.h"
#include "Anim3DMgr.h"
#include "Mesh.h"

CAnimator3D::CAnimator3D()
	: m_iCurClip(0)
	, m_dCurTime(0.)
	, m_iFrameCount(30)
	, m_pBoneFinalMat(nullptr)
	, m_bFinalMatUpdate(false)
	, CComponent(COMPONENT_TYPE::ANIMATOR3D)
	, m_pMesh(nullptr)
	, m_bStatic(false)
	, m_pTransition(nullptr)
	, m_fChangeLimitTime(0.f)
{
	m_pBoneMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"Animation3DUpdateMtrl");
	m_pBoneFinalMat = new CStructuredBuffer;
	m_pBoneKeyFrame = new CStructuredBuffer;
	m_pBoneNextKeyFrame = new CStructuredBuffer;
}

CAnimator3D::~CAnimator3D()
{
	SAFE_DELETE(m_pBoneFinalMat);
	SAFE_DELETE(m_pBoneKeyFrame);
	SAFE_DELETE(m_pBoneNextKeyFrame);
	SAFE_DELETE_VECLIST(m_SocketList);

	int size = (int)m_vecNotify.size();
	for (size_t i = 0; i < size; ++i)
	{
		SAFE_DELETE(m_vecNotify[i]);
	}

	m_vecClip.clear();
}


void CAnimator3D::update()
{


}

void CAnimator3D::lateupdate()
{
}

void CAnimator3D::finalupdate()
{
	float deltaTime = DT;
	m_bEnd = false;

	m_fAnimationGlobalTime += deltaTime;

	m_dCurTime = 0.f;
	m_vecClipUpdateTime[m_iCurClip] += deltaTime;

	int iCur = 0;
	int iNext = 0;
	bool bTrans = false;


	//==============
	// 같은 애니메이션
	//==============
	if (nullptr == m_pTransition)
	{

		// m_dCurTime = 0.f;
		// 현재 재생중인 Clip 의 시간을 진행한다.
		// m_vecClipUpdateTime[m_iCurClip] += deltaTime;

		if (true == m_bStatic)
		{
		}
		else
		{
			if (m_pCurrentAnimClip)
			{
				if (m_vecClipUpdateTime[m_iCurClip] >= m_pCurrentAnimClip->m_dTimeLength)
				{
					m_vecClipUpdateTime[m_iCurClip] = 0.f;
					m_bEnd = true;
				}
				m_dCurTime = m_pCurrentAnimClip->m_dStartTime + m_vecClipUpdateTime[m_iCurClip];
			}
		}

		m_iFrameIdx = (int)(m_dCurTime * (float)m_iFrameCount);

		if (true == m_bStatic)
		{
			return;
		}

		// Socket Update Index
		iCur = m_iFrameIdx;

		if (iCur >= m_pCurrentAnimClip->m_iFrameLength - 1)
		{
			iNext = iCur;
		}
		else
		{
			iNext = iCur + 1;
		}

		bTrans = false;

		if (true == m_bEnd)
		{
			for (size_t i = 0; i < m_vecNotify.size(); ++i)
			{
				tAnimNotify*	pNotify = m_vecNotify[i];

				if (pNotify->pAnimClip != m_pCurrentAnimClip)
					continue;

				pNotify->bCall = false;
			}
		}
		else
		{
			m_pCurrentAnimClip->m_iChangeFrame = m_iFrameIdx;
		}
	}
	//==============
	// 다른 애니메이션
	//==============
	else
	{
		m_fChangeTime += deltaTime;

		// 06/22  
		// Transition 완료됐는지 확인
		bool	bChange = false;
		if (m_fChangeTime >= m_fChangeLimitTime)
		{
			m_fChangeTime = m_fChangeLimitTime;
			bChange = true;
		}

		float	fAnimationTime = m_fAnimationGlobalTime + (float)m_pCurrentAnimClip->m_dStartTime;

		/*if (true == m_bStatic)
		{
		}
		else
		{
			if (m_vecClipUpdateTime[m_iCurClip] >= m_pCurrentAnimClip->m_dTimeLength)
			{
				m_vecClipUpdateTime[m_iCurClip] = 0.f;
				m_bEnd = true;
			}
			m_dCurTime = m_pCurrentAnimClip->m_dStartTime + m_vecClipUpdateTime[m_iCurClip];
		}*/

		m_iFrameIdx = (int)(m_dCurTime * (float)m_iFrameCount);

		if (m_pCurrentAnimClip->m_vecKeyFrame.empty())
		{
			return;
		}

		iCur = m_pCurrentAnimClip->m_iChangeFrame;
		iNext = 0;

		bTrans = true;

		// Transition 완료
		if (true == bChange)
		{
			for (size_t i = 0; i < m_vecNotify.size(); ++i)
			{
				tAnimNotify*	pNotify = m_vecNotify[i];

				if (pNotify->pAnimClip != m_pCurrentAnimClip)
					continue;

				pNotify->bCall = false;
			}

			//   0707 
			// 밑줄과 같이 m_pTransition을 바로 m_pCurrentAnimClip에 넣지 말고 
			// m_pCurrentAnimClip = m_pTransition->pAnimClip;

			// 이 함수를 이용해서 현재 애님클립을 설정해야 애니메이션 업데이트 시간이 초기화된다.
			SetCurrentAnimClip(m_pTransition->pAnimClip->GetClipName().c_str());

			m_pBoneKeyFrame->Create(sizeof(tFrameTrans), m_pCurrentAnimClip->m_iFrameSize, (void*)m_pCurrentAnimClip->GetKeyFrame()->data());

			m_pTransition = nullptr;
			m_fAnimationGlobalTime = 0.f;
			m_fChangeTime = 0.f;
			bTrans = false;
			iCur = 0;
			m_iFrameIdx = iCur;
		}


	}

	//==============
	// Notify 돌리기
	//==============
	float fAnimationTime = m_fAnimationGlobalTime + m_pCurrentAnimClip->m_dStartTime;

	for (size_t i = 0; i < m_vecNotify.size(); ++i)
	{
		tAnimNotify*	pNotify = m_vecNotify[i];

		if (pNotify->pAnimClip != m_pCurrentAnimClip)
			continue;

		if (pNotify->fTime <= fAnimationTime && !pNotify->bCall)
		{
			pNotify->bCall = true;

			for (size_t j = 0; j < pNotify->vecFunction.size(); ++j)
			{
				pNotify->vecFunction[j](deltaTime);
			}
		}
	}

	//==================
	// Transition 돌리기
	//==================
	auto	iter = m_pCurrentAnimClip->m_TransitionList.begin();
	auto	iterEnd = m_pCurrentAnimClip->m_TransitionList.end();

	if (nullptr == m_pTransition)
	{
		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->bAutoTransition && m_bEnd)	//Transition Func 돌아가는 곳
			{
				m_pTransition = *iter;
				m_pBoneNextKeyFrame->Create(sizeof(tFrameTrans), m_pTransition->pAnimClip->m_iFrameSize
					, (void*)m_pTransition->pAnimClip->GetKeyFrame()->data());


				auto	iter1 = (*iter)->NotifyList.begin();
				auto	iter1End = (*iter)->NotifyList.end();

				for (; iter1 != iter1End; ++iter1)
				{
					(*iter1)();
				}
				break;
			}
			else
			{
				if ((*iter)->funcTransition != nullptr)
				{
					if ((*iter)->funcTransition())
					{
						m_pTransition = *iter;
						m_pBoneNextKeyFrame->Create(sizeof(tFrameTrans), m_pTransition->pAnimClip->m_iFrameSize
							, (void*)m_pTransition->pAnimClip->GetKeyFrame()->data());

						auto	iter1 = (*iter)->NotifyList.begin();
						auto	iter1End = (*iter)->NotifyList.end();

						for (; iter1 != iter1End; ++iter1)
						{
							(*iter1)();
						}
						break;
					}
				}
			}
		}

		if (nullptr != m_pTransition)
		{

		}
	}






	//================================================

	m_bFinalMatUpdate = false;

	// 키프레임 인덱스 초과 방지
	if (iCur >= m_pCurrentAnimClip->m_iFrameLength - 1)
	{
		iCur = m_pCurrentAnimClip->m_iFrameLength - 1;
	}


	// 06/04  
	// bone matrix 찾기...
	CTransform* trans = Transform();

	// 본 업데이트
	trans->ComputeWorld();

	// 본 소켓 업데이트
	SocketUpdate(deltaTime, trans->GetWorldMat(), iCur, iNext, bTrans);


	return;
}

bool CAnimator3D::AnimIsFinish()
{
	if (m_pCurrentAnimClip->GetEndFrame() == m_iFrameIdx)
	{
		return true;
	}
	
	return false;
}

void CAnimator3D::SetCurrentAnimClip(const wchar_t* _clipName)
{
	m_pCurrentAnimClip = FindAnimClip(_clipName);
	m_iCurClip = FindAnimClipIndex(m_pCurrentAnimClip);

	m_pBoneKeyFrame->Create(sizeof(tFrameTrans), m_pCurrentAnimClip->m_iFrameSize, (void*)m_pCurrentAnimClip->GetKeyFrame()->data());

	m_vecClipUpdateTime[m_iCurClip] = 0.f;
}

void CAnimator3D::SetAnimClip(const vector<tMTAnimClip>* _vecAnimClip)
{
	m_pVecClip = _vecAnimClip;
	m_vecClipUpdateTime.resize(m_pVecClip->size());
}


void CAnimator3D::UpdateData()
{
	if (!m_bFinalMatUpdate)
	{
		// 트랜지션 없을때
		if (nullptr == m_pTransition)
		{
			// Bone Data Update	
			m_pMesh = MeshRender()->GetMesh();
			if (true == m_bStatic)
			{
				m_pMesh->GetBoneFrameDataBuffer()->UpdateData(13);
			}
			else
			{
				m_pBoneKeyFrame->UpdateData(13);
			}
			m_pMesh->GetBoneOffsetBuffer()->UpdateData(14);

			// Buffer 생성
			check_mesh(m_pMesh);

			m_pBoneFinalMat->UpdateRWData(6);


			UINT iBoneCount = (UINT)m_pVecBones->size();
			m_pBoneMtrl->SetData(SHADER_PARAM::INT_0, &iBoneCount);
			m_pBoneMtrl->SetData(SHADER_PARAM::INT_1, &m_iFrameIdx);

			// m_pBoneMtrl->SetData(SHADER_PARAM::INT_1, &m_iFrameIdx);


			UINT iGrounX = (iBoneCount / 256) + 1;
			m_pBoneMtrl->Dispatch(iGrounX, 1, 1);

			CStructuredBuffer::ClearData(13);
			CStructuredBuffer::ClearData(14);
			CStructuredBuffer::ClearRWData(6);

		}
		// 트랜지션 있을때
		else
		{
			// Bone Data Update	
			m_pMesh = MeshRender()->GetMesh();
			if (true == m_bStatic)
			{
				m_pMesh->GetBoneFrameDataBuffer()->UpdateData(13);
			}
			else
			{
				m_pBoneKeyFrame->UpdateData(13);
				m_pBoneNextKeyFrame->UpdateData(15);

			}
			m_pMesh->GetBoneOffsetBuffer()->UpdateData(14);

			// Buffer 생성
			check_mesh(m_pMesh);

			m_pBoneFinalMat->UpdateRWData(6);

			int iNextFrameIdx = 0;

			UINT iBoneCount = (UINT)m_pVecBones->size();
			m_pBoneMtrl->SetData(SHADER_PARAM::INT_0, &iBoneCount);
			m_pBoneMtrl->SetData(SHADER_PARAM::INT_1, &m_iFrameIdx);
			// m_pBoneMtrl->SetData(SHADER_PARAM::INT_2, &iNextFrameIdx);


			UINT iGrounX = (iBoneCount / 256) + 1;
			m_pBoneMtrl->Dispatch(iGrounX, 0, 0);

			CStructuredBuffer::ClearData(13);
			CStructuredBuffer::ClearData(14);
			CStructuredBuffer::ClearData(15);
			CStructuredBuffer::ClearRWData(6);



		}


		m_bFinalMatUpdate = true;
	}

	// t8 레지스터에 최종행렬 데이터(구조버퍼) 바인딩	
	m_pBoneFinalMat->UpdateData(12);
}

CStructuredBuffer * CAnimator3D::GetFinalBoneMat()
{
	return m_pBoneFinalMat;
}

void CAnimator3D::ClearData()
{
	CStructuredBuffer::ClearData(12);

	int iContainer = MeshRender()->GetMaterialContainerCount();

	for (int k = 0; k < iContainer; ++k)
	{
		UINT iMtrlCount = (UINT)MeshRender()->GetMaterialSubsetCount(k);
		Ptr<CMaterial> pMtrl = nullptr;
		for (UINT i = 0; i < iMtrlCount; ++i)
		{
			pMtrl = MeshRender()->GetSharedMaterial(i, k);
			if (nullptr == pMtrl)
				continue;
			int a = 0;
			pMtrl->SetData(SHADER_PARAM::INT_0, &a); // Animation Mesh 알리기
		}
	}
}

std::vector<struct CAnimClip*>* CAnimator3D::GetVecAnimClip()
{
	return &m_vecClip;
}

void CAnimator3D::check_mesh(Ptr<CMesh> _pMesh)
{
	UINT iBoneCount = _pMesh->GetBoneCount();
	if (m_pBoneFinalMat->GetElementCount() < iBoneCount)
	{
		m_pBoneFinalMat->Create(sizeof(Matrix), iBoneCount, nullptr);
	}
}

void CAnimator3D::SaveToScene(FILE * _pFile)
{
}

void CAnimator3D::LoadFromScene(FILE * _pFile)
{
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 본 소켓 작업



void CAnimator3D::MoveWeaponFromSocket(const wchar_t * strSocketNameA, const wchar_t * strSocketNameB, CGameObject * pObj)
{
	int Count = wcsncmp(strSocketNameA, strSocketNameB, 20);	// const wchar_t * 문자 비교

	if (Count != 0)
	{
		CBoneSocket* pSocketA = FindSocket(strSocketNameA);
		CBoneSocket* pSocketB = FindSocket(strSocketNameB);

		CGameObject* pTemp = pSocketA->TakeOutObject(pObj);

		if (nullptr == pTemp)
		{
			return;
		}
		pTemp->BelongtoSockectName = strSocketNameB;
		pSocketB->AddObject(pTemp);
	}
}



bool CAnimator3D::AddSocket(const wchar_t* strSocketName,
	const wchar_t* strBoneName)
{
	int	iBoneIndex = GetBoneIndex(strBoneName);

	if (iBoneIndex == -1)
		return false;

	auto	iter = m_SocketList.begin();
	auto	iterEnd = m_SocketList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->m_strName == strSocketName)
			return false;
	}

	CBoneSocket*	pSocket = new CBoneSocket;

	pSocket->Init();

	pSocket->SetInfo(strSocketName, strBoneName, iBoneIndex);

	m_SocketList.push_back(pSocket);

	return true;
}

bool CAnimator3D::SetSocketObject(const wchar_t* strSocketName, CGameObject * pObj)
{
	auto	iter = m_SocketList.begin();
	auto	iterEnd = m_SocketList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->m_strName == strSocketName)
		{
			(*iter)->AddObject(pObj);
			return true;
		}
	}

	return false;
}


bool CAnimator3D::DeleteSocketObject(const wchar_t* strSocketName,
	CGameObject * pObj)
{
	auto	iter = m_SocketList.begin();
	auto	iterEnd = m_SocketList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->m_strName == strSocketName)
		{
			(*iter)->DeleteObject(pObj);
			return true;
		}
	}

	return false;
}


bool CAnimator3D::DeleteSocketObject(const wchar_t* strSocketName)
{
	auto	iter = m_SocketList.begin();
	auto	iterEnd = m_SocketList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->DeleteObject();
	}

	return true;
}

bool CAnimator3D::SetSocketOffset(const wchar_t* strSocketName, const Vec3 & vOffset)
{
	auto	iter = m_SocketList.begin();
	auto	iterEnd = m_SocketList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->m_strName == strSocketName)
		{
			(*iter)->SetOffset(vOffset);
			return true;
		}
	}

	return false;
}

bool CAnimator3D::SetSocketRotation(const wchar_t* strSocketName, const Vec3 & vRotation)
{
	auto	iter = m_SocketList.begin();
	auto	iterEnd = m_SocketList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->m_strName == strSocketName)
		{
			(*iter)->SetRotation(vRotation);
			return true;
		}
	}

	return false;
}

CBoneSocket * CAnimator3D::FindSocket(const wchar_t* strSocketName)
{
	auto	iter = m_SocketList.begin();
	auto	iterEnd = m_SocketList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->m_strName == strSocketName)
			return *iter;
	}

	return nullptr;
}








///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//  
void CAnimator3D::SocketUpdate(float fTime, const Matrix& matWorld, int _iCur, int _iNext, bool _bTrans)
{

	auto	iter = m_SocketList.begin();
	auto	iterEnd = m_SocketList.end();

	if (nullptr == m_pMesh)
	{
		m_pMesh = MeshRender()->GetMesh();
		m_iBoneCount = m_pMesh->GetBoneCount();
	}

	for (; iter != iterEnd; ++iter)
	{
		int index = (*iter)->GetBoneIndex();

		////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////////////////
		// socket에 해당하는 본만 final bone 행렬 추출.
		if (m_pVecBones->at(index).vecKeyFrame.empty())
		{
			// 키프레임 별 행렬이 없는 본일 경우
			m_vecFinalBoneMat[index] = m_pVecBones->at(index).matBone;
			continue;
		}

		int keyFrameIndex = m_iBoneCount * _iCur + index;
		int keyFrameNextIndex = m_iBoneCount * _iNext + index;

		tFrameTrans tKeyFrame;
		tFrameTrans tNextKeyFrame;

		if (false == _bTrans)
		{
			tKeyFrame = m_pCurrentAnimClip->GetKeyFrame()->at(keyFrameIndex);
			tNextKeyFrame = m_pCurrentAnimClip->GetKeyFrame()->at(keyFrameNextIndex);
		}
		else
		{
			tKeyFrame = m_pCurrentAnimClip->GetKeyFrame()->at(keyFrameIndex);
			tNextKeyFrame = m_pTransition->pAnimClip->GetKeyFrame()->at(keyFrameNextIndex);
		}

		// double	fFrameTime = tKeyFrame.dTime;
		// double	fNextFrameTime = tNextKeyFrame.dTime;

		// 프레임간의 시간에 따른 비율을 구해준다.
		double frameTime = (double)_iCur / (double)30;

		double	fPercent = (m_dCurTime - frameTime) / (1.f / m_iFrameCount);

		XMVECTOR vS1 = tKeyFrame.vScale;
		XMVECTOR vS2 = tNextKeyFrame.vScale;

		XMVECTOR vT1 = tKeyFrame.vTranslate;
		XMVECTOR vT2 = tNextKeyFrame.vTranslate;

		XMVECTOR vQ1 = XMLoadFloat4(&tKeyFrame.qRot);
		XMVECTOR vQ2 = XMLoadFloat4(&tNextKeyFrame.qRot);

		XMVECTOR vS = XMVectorLerp(vS1, vS2, (float)fPercent);
		XMVECTOR vT = XMVectorLerp(vT1, vT2, (float)fPercent);
		XMVECTOR vQ = XMQuaternionSlerp(vQ1, vQ2, (float)fPercent);

		XMVECTOR vQZero = XMVectorSet(0.f, 0.f, 0.f, 1.f);


		// 오프셋 행렬을 곱하여 최종 본행렬을 만들어낸다.				
		// m_vecFinalBoneMat[index] = m_pVecBones->at(index).matOffset * XMMatrixAffineTransformation(vS, vQZero, vQ, vT);
		Matrix matBone = m_pVecBones->at(index).matOffset * XMMatrixAffineTransformation(vS, vQZero, vQ, vT);

		Matrix matLast = matBone * matWorld;

		(*iter)->Update(fTime, matLast);
	}
}





int CAnimator3D::GetBoneCount() const
{
	return m_pVecBones->size();
}

const tMTBone * CAnimator3D::GetBone(int iIndex) const
{
	return &m_pVecBones->at(iIndex);
}

const tMTBone * CAnimator3D::GetBone(const wstring & strName) const
{
	for (int i = 0; i < m_pVecBones->size(); ++i)
	{
		if ((m_pVecBones->at(i)).strBoneName == strName)
			return &m_pVecBones->at(i);
	}

	return nullptr;
}

int CAnimator3D::GetBoneIndex(const wstring & strName) const
{
	for (int i = 0; i < m_pVecBones->size(); ++i)
	{
		if ((m_pVecBones->at(i)).strBoneName == strName)
			return i;
	}

	return -1;
}

CAnimClip * CAnimator3D::GetAnimClip() const
{
	return m_pCurrentAnimClip;
}

void CAnimator3D::SetPlayTime(const wstring & strClipName, float fPlayTime)
{
}

bool CAnimator3D::AddAnimClip(const wstring & strClipName, const wstring & strInfoName)
{
	return false;
}

bool CAnimator3D::AddAnimClip(const wstring & strClipName)
{
	// Anim3DMgr에서 찾기
	CAnimClip* temp = CAnim3DMgr::GetInst()->FindAnimClipByClipName(strClipName.c_str());

	// 없으면 AnimClip폴더에서 로드
	// 폴더에 없으면 assert(false)
	if (nullptr == temp)
	{
		temp = CAnim3DMgr::GetInst()->Load(strClipName.c_str());
	}

	m_vecClip.push_back(temp);

	m_vecClipUpdateTime.resize(m_vecClip.size());

	return true;
}

void CAnimator3D::SetStatic()
{
	m_bStatic = true;
}

//void CAnimator3D::SetCurrentAnimClip(const wstring & strClipName)
//{
//	m_pCurrentAnimClip = FindAnimClip(strClipName.c_str());
//}

CAnimClip * CAnimator3D::FindAnimClip(const wchar_t* strName)
{
	int size = m_vecClip.size();

	for (int i = 0; i < size; ++i)
	{
		if (strName == m_vecClip[i]->GetClipName())
		{
			return m_vecClip[i];
		}
	}

	return nullptr;
}

int CAnimator3D::FindAnimClipIndex(CAnimClip * _clip)
{
	int size = m_vecClip.size();

	for (int i = 0; i < size; ++i)
	{
		if (_clip == m_vecClip[i])
		{
			return i;
		}
	}

	return -1;
}






void CAnimator3D::AddTransition(const wstring & strTransitionName, const wstring & strName, bool(*pFunc)(), const wstring & strNextName)
{
	CAnimClip* pClip = FindAnimClip(strName.c_str());

	if (nullptr == pClip)
	{
		return;

	}

	tAnimationTransition*	pTransition = new tAnimationTransition;

	pTransition->pAnimClip = pClip;
	pTransition->funcTransition = std::bind(pFunc);
	pTransition->strTransitionName = strTransitionName;

	pClip->m_TransitionList.push_back(pTransition);
}

void CAnimator3D::AutoTransition(const wstring & strInfoName, const wstring & strName, const wstring & strNextName)
{
	CAnimClip*	pClip = FindAnimClip(strName.c_str());

	if (nullptr == pClip)
	{
		return;
	}

	CAnimClip*	pNextClip = FindAnimClip(strNextName.c_str());

	if (nullptr == pClip)
	{
		return;
	}


	tAnimationTransition*	pTransition = new tAnimationTransition;

	pTransition->pAnimClip = pNextClip;
	pTransition->strTransitionName = strInfoName;
	pTransition->bAutoTransition = true;

	pClip->m_TransitionList.push_back(pTransition);
}

// InfoName = ClipName
void CAnimator3D::AddTransitionNotify(const wstring & strName, const wstring & strInfoName, void(*pFunc)())
{
	CAnimClip* pClip = FindAnimClip(strInfoName.c_str());

	if (nullptr == pClip)
	{
		return;

	}

	auto	iter = pClip->m_TransitionList.begin();
	auto	iterEnd = pClip->m_TransitionList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->strTransitionName == strName)
		{
			(*iter)->NotifyList.push_back(std::bind(pFunc));
			break;
		}
	}
}

bool CAnimator3D::CreateNotify(const wstring & strSequenceName, const wstring & strNotifyName, float fTime)
{
	CAnimClip*	pClip = FindAnimClip(strSequenceName.c_str());

	if (!pClip)
	{
		return false;
	}

	tAnimNotify*	pNotify = new tAnimNotify;
	pNotify->strName = strNotifyName;
	pNotify->pAnimClip = pClip;
	pNotify->fTime = fTime;

	m_vecNotify.push_back(pNotify);

	return true;
}

bool CAnimator3D::AddNotifyFunction(const wstring & strNotifyName, void(*pFunc)(float))
{
	auto	iter = m_vecNotify.begin();
	auto	iterEnd = m_vecNotify.end();

	tAnimNotify*	pNotify = nullptr;

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->strName == strNotifyName)
		{
			pNotify = *iter;
			break;
		}
	}

	if (nullptr == pNotify)
	{
		return false;
	}

	pNotify->vecFunction.push_back(std::bind(pFunc, std::placeholders::_1));

	return true;
}


