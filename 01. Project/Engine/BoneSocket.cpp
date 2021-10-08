#include "stdafx.h"
#include "BoneSocket.h"

#include "Component.h"
#include "GameObject.h"
#include "Transform.h"

CBoneSocket::CBoneSocket()
{
}

CBoneSocket::~CBoneSocket()
{
	// SAFE_RELEASE_NORMAL_VECLIST(m_ComponentList);
}

int CBoneSocket::GetBoneIndex() const
{
	return m_iBoneIndex;
}

void CBoneSocket::SetInfo(const wstring & strName, const wstring & strBoneName, int iBoneIndex)
{
	m_strName = strName;
	m_strBoneName = strBoneName;
	m_iBoneIndex = iBoneIndex;
}

void CBoneSocket::SetOffset(const Vec3 & vOffset)
{
	m_vOffset = vOffset;
}

void CBoneSocket::SetRotation(const Vec3 & vRotation)
{
	m_vRotation = vRotation;
}

void CBoneSocket::AddComponent(CComponent * pCom)
{
	m_ComponentList.push_back(pCom);
}

void CBoneSocket::AddObject(CGameObject * pObj)
{
	CComponent* tempComp = nullptr;

	tempComp = pObj->GetComponent(COMPONENT_TYPE::MESHRENDER);
	AddComponent(tempComp);
}

void CBoneSocket::DeleteObject(CGameObject * pObj)
{
	CComponent* pCom = pObj->GetComponent(COMPONENT_TYPE::MESHRENDER);
	auto	iter = m_ComponentList.begin();
	auto	iterEnd = m_ComponentList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (*iter == pCom)
		{
			m_ComponentList.erase(iter);
			break;
		}
	}
}

void CBoneSocket::DeleteObject(CComponent * pCom)
{
	auto	iter = m_ComponentList.begin();
	auto	iterEnd = m_ComponentList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (*iter == pCom)
		{
			m_ComponentList.erase(iter);
			break;
		}
	}
}

void CBoneSocket::DeleteObject()
{

}

CGameObject * CBoneSocket::TakeOutObject(CGameObject * pObj)
{
	CComponent* pCom = pObj->GetComponent(COMPONENT_TYPE::MESHRENDER);
	auto	iter = m_ComponentList.begin();
	auto	iterEnd = m_ComponentList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (*iter == pCom)
		{
			CGameObject* returnObject = pCom->GetObj();
			m_ComponentList.erase(iter);
			return returnObject;
		}
	}

	// �� ���Ͽ� ã������ ���ӿ�����Ʈ�� ����
	// assert(false);
	return nullptr;
}

bool CBoneSocket::Init()
{
	return true;
}

void CBoneSocket::Update(float fTime, const Matrix & matBone)
{
	auto	iter = m_ComponentList.begin();
	auto	iterEnd = m_ComponentList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = m_ComponentList.erase(iter);
			iterEnd = m_ComponentList.end();
			continue;
		}

		Matrix	matRot, matTrans;
		matTrans.Translation(m_vOffset);
		matRot = Matrix::Rotation(m_vRotation);

		// ���� ���ְ� ���� ��ġ��
		//  ĳ������ ���� bone matrix�� ���Ѵ�.

		// �� ��ġ
		CTransform*	pTransform = (*iter)->Transform();

		// �� ��ġ
		Matrix tempBone = matRot * matTrans * matBone;

		// �� ��ġ BoneMatrix�� �ֱ�
		pTransform->SetBoneMatrix(tempBone);

		++iter;
	}
}
