#include "stdafx.h"
#include "GameObject.h"

#include "Component.h"
#include "MeshRender.h"
#include "Collider2D.h"
#include "Script.h"

#include "SceneMgr.h"
#include "Scene.h"
#include "Layer.h"

#include "BoneSocket.h"
#include "Animator3D.h"

CGameObject::CGameObject()
	: m_arrCom{}
	, m_pParentObj(nullptr)
	, m_iLayerIdx(-1)
	, m_bDead(false)
	, m_bActive(true)
	, m_bFrustumCheck(true)
{
	for (size_t i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		m_arrCom[i] = nullptr;
	}
	AddComponent(new CTransform);
}

CGameObject::CGameObject(const CGameObject & _origin)
	: CEntity(_origin)
	, m_arrCom{}
	, m_iLayerIdx(-1)
	, m_bDead(false)
	, m_bActive(true)
	, m_bFrustumCheck(_origin.m_bFrustumCheck)
{
	for (size_t i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		m_arrCom[i] = nullptr;
	}

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != _origin.m_arrCom[i])
		{
			AddComponent(_origin.m_arrCom[i]->Clone());
		}
	}
}

CGameObject::~CGameObject()
{
	Safe_Delete_Array(m_arrCom);
	Safe_Delete_Vector(m_vecChild);
	Safe_Delete_Vector(m_vecScript);
}

void CGameObject::AddChild(CGameObject * _pChildObj)
{
	// ���� 1
	// �ڱ��ڽ��� �ڽ��� �Ǵ� ���
	if (this == _pChildObj)
	{
		return;
		assert(this != _pChildObj);
	}

	// ���� 2.
	// �ڽ����� ������ ������Ʈ��, �θ���� ������Ʈ�� ���
	if (IsAncestor(_pChildObj))
	{
		return;
		assert(!IsAncestor(_pChildObj));

	}

	// ���� 3.
	// �ڽ� ������Ʈ�� ������ �ٸ� �θ� ������Ʈ�� �־��ٸ�
	_pChildObj->ClearParent(this);

	// �ڽ� ������Ʈ�� Layer �� ���ԵǾ����� �ʴ� ���
	// �θ� ������Ʈ�� Layer�� �����Ѵ�.
	if (-1 == _pChildObj->m_iLayerIdx)
	{
		_pChildObj->m_iLayerIdx = m_iLayerIdx;
	}


}

bool CGameObject::IsAncestor(CGameObject * _pObj)
{
	CGameObject* pParent = m_pParentObj;

	while (nullptr != pParent)
	{
		if (pParent == _pObj)
			return true;

		pParent = pParent->m_pParentObj;
	}

	return false;
}

void CGameObject::ClearParent(CGameObject* _pNextParent)
{
	// �θ� ���� ���
	if (nullptr == m_pParentObj)
	{
		// ���� �θ� ������ ���
		if (_pNextParent)
		{
			_pNextParent->m_vecChild.push_back(this);
			m_pParentObj = _pNextParent;
		}
		else // �θ� ����, ���� �θ� ����
			CSceneMgr::GetInst()->GetCurScene()->GetLayer(m_iLayerIdx)->DeleteObj(this);

		// �ֻ��� �θ� ������Ʈ,
		// ���� �θ� ������ (Layer ParentList ���� ����)
		if (-1 != m_iLayerIdx)
		{
			CLayer* pCurLayer = CSceneMgr::GetInst()->GetCurScene()->GetLayer(m_iLayerIdx);
			pCurLayer->CheckParentObj();
		}
	}
	else // �θ� �ִ°��
	{
		// �θ�� ���� �ڽ��� ����
		vector<CGameObject*>::iterator iter = m_pParentObj->m_vecChild.begin();
		for (int i = 0; iter != m_pParentObj->m_vecChild.end(); ++iter, ++i)
		{
			if ((*iter) == m_pParentObj->m_vecChild[i])
			{
				m_pParentObj->m_vecChild.erase(iter);
				break;
			}
		}

		// ���� �θ� ������ ��� �ش� �θ�� ����.
		if (_pNextParent)
		{
			_pNextParent->m_vecChild.push_back(this);
			m_pParentObj = _pNextParent;
		}
		else // �ֻ��� ������Ʈ�� ��
		{
			m_pParentObj = nullptr;

			if (!m_bDead)
			{
				CLayer* pCurLayer = CSceneMgr::GetInst()->GetCurScene()->GetLayer(m_iLayerIdx);
				m_iLayerIdx = -1;
				pCurLayer->AddGameObject(this);
			}
		}
	}
}

CGameObject* CGameObject::GetChild(const wstring & _ChildName)
{
	int size = m_vecChild.size();

	for (size_t i = 0; i < size; ++i)
	{
		if (m_vecChild[i]->GetName() == _ChildName)
		{
			return m_vecChild[i];
		}
	}

	return nullptr;
}

CGameObject * CGameObject::GetChild(int _iIndex)
{
	return m_vecChild[_iIndex];
}

void CGameObject::SetDead()
{
	m_bDead = true;

	for (int i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->SetDead();
	}
}

void CGameObject::AddChild(CGameObject * pChild, const wstring & strSocketName)
{
	AddChild(pChild);

	//  
	// ������ ����...
	// 1. ���� �������� ���´�. ������ Animator3D�� ����

	CComponent* pAnim3D = GetComponent(COMPONENT_TYPE::ANIMATOR3D);

	if (nullptr != pAnim3D)
	{
		// std::list<CBoneSocket*> pSocketList = *(((CAnimator3D*)pAnim3D)->GetSocketList());
		// child �� animator3D�� bone socket�� �־��ش�... ���߿� ����

		CBoneSocket* pSocket = ((CAnimator3D*)pAnim3D)->FindSocket(strSocketName.c_str());

		if (pSocket)
		{
			pSocket->AddObject(pChild);
			pChild->BelongtoSockectName = strSocketName;
		}
	}
}

bool CGameObject::AddSocket(const wstring & strSocketName, const wstring & strBoneName)
{
	CAnimator3D* pAnim3D = (CAnimator3D*)m_arrCom[(int)COMPONENT_TYPE::ANIMATOR3D];

	if (nullptr == pAnim3D)
	{
		assert(false);
		return false;
	}
	else
	{
		pAnim3D->AddSocket(strSocketName.c_str(), strBoneName.c_str());
	}

	return true;
}

void CGameObject::AddComponent(CComponent * _pCom)
{
	COMPONENT_TYPE eType = _pCom->GetComponentType();

	if (COMPONENT_TYPE::SCRIPT == eType)
	{
		m_vecScript.push_back((CScript*)_pCom);
	}
	else
	{
		if (m_arrCom[(UINT)eType] == nullptr)
		{
			m_arrCom[(UINT)eType] = _pCom;
		}
		else
		{
			SAFE_DELETE(m_arrCom[(UINT)eType])
				m_arrCom[(UINT)eType] = _pCom;
		}
	}

	_pCom->SetGameObject(this);
}

void CGameObject::awake()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->awake();
	}

	for (int i = 0; i < m_vecChild.size(); ++i)
	{
		if (m_vecChild[i]->IsActive())
			m_vecChild[i]->awake();
	}

	for (int i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->awake();
	}
}

void CGameObject::start()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->start();
	}

	for (int i = 0; i < m_vecChild.size(); ++i)
	{
		if (m_vecChild[i]->IsActive())
			m_vecChild[i]->start();
	}

	for (int i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->start();
	}
}

void CGameObject::update()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->update();
	}

	for (int i = 0; i < m_vecChild.size(); ++i)
	{
		if (m_vecChild[i]->IsActive())
			m_vecChild[i]->update();
	}

	for (int i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->update();
	}
}

void CGameObject::pause_update()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->update();
	}

	for (int i = 0; i < m_vecChild.size(); ++i)
	{
		if (m_vecChild[i]->IsActive())
			m_vecChild[i]->update();
	}

	if (m_arrCom[(UINT)COMPONENT_TYPE::CAMERA] != nullptr)
	{
		for (int i = 0; i < m_vecScript.size(); ++i)
		{
			m_vecScript[i]->update();
		}
	}
}

void CGameObject::EngineUpdate()
{
	for (int i = 0; i < m_vecScript.size(); ++i)
	{		
		m_vecScript[i]->update();		
	}
}

void CGameObject::lateupdate()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->lateupdate();
	}

	for (int i = 0; i < m_vecChild.size(); ++i)
	{
		if (m_vecChild[i]->IsActive())
			m_vecChild[i]->lateupdate();
	}

	for (int i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->lateupdate();
	}
}

void CGameObject::finalupdate()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
			m_arrCom[i]->finalupdate();
	}

	for (int i = 0; i < m_vecChild.size(); ++i)
	{
		if (m_vecChild[i]->IsActive())
			m_vecChild[i]->finalupdate();
	}
}

void CGameObject::pause_finalupdate()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i] && (i != (UINT)COMPONENT_TYPE::ANIMATOR2D && i != (UINT)COMPONENT_TYPE::ANIMATOR3D))
		{
			m_arrCom[i]->finalupdate();
		}
	}

	for (int i = 0; i < m_vecChild.size(); ++i)
	{
		if (m_vecChild[i]->IsActive())
			m_vecChild[i]->pause_finalupdate();
	}
}

void CGameObject::RegisterToLayer()
{
	// layer �� ���
	assert(-1 != m_iLayerIdx);

	CLayer* pCurLayer = CSceneMgr::GetInst()->GetCurScene()->GetLayer(m_iLayerIdx);
	pCurLayer->RegisterObj(this);

	for (int i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->RegisterToLayer();
	}
}


void CGameObject::enable()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i] && m_arrCom[i]->IsActive())
			m_arrCom[i]->enable();
	}

	for (int i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->enable();
	}

	for (int i = 0; i < m_vecScript.size(); ++i)
	{
		if (m_vecScript[i]->IsActive())
			m_vecScript[i]->enable();
	}
}

void CGameObject::disable()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i] && m_arrCom[i]->IsActive())
			m_arrCom[i]->disable();
	}

	for (int i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->disable();
	}

	for (int i = 0; i < m_vecScript.size(); ++i)
	{
		if (m_vecScript[i]->IsActive())
			m_vecScript[i]->disable();
	}
}

const wstring& CGameObject::GetLayerName()
{
	CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
	CLayer* ObjectLayer = CurScene->GetLayer(m_iLayerIdx);
	const wstring& LayerName = ObjectLayer->GetName();
	return LayerName;
}

void CGameObject::SetActive(bool _bTrue)
{
	if (m_bActive)
	{
		if (!_bTrue)
		{
			// ��Ȱ��ȭ
			tEvent event = {};
			event.eType = EVENT_TYPE::DEACTIVATE_GAMEOBJECT;
			event.wParam = (DWORD_PTR)this;

			CEventMgr::GetInst()->AddEvent(event);
		}
	}
	else
	{
		if (_bTrue)
		{
			// Ȱ��ȭ
			// ��Ȱ��ȭ
			tEvent event = {};
			event.eType = EVENT_TYPE::ACTIVATE_GAMEOBJECT;
			event.wParam = (DWORD_PTR)this;

			CEventMgr::GetInst()->AddEvent(event);
		}
	}
}
