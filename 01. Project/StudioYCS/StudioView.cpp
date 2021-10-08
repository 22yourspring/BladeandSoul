#include "pch.h"
#include "StudioView.h"

#include "StudioYCS.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "PropertiesWnd.h"

#include <Engine/EventMgr.h>
#include <Engine/SceneMgr.h>
#include <Engine/Scene.h>
#include <Engine/layer.h>
#include <Engine/GameObject.h>
#include <Engine/PathMgr.h>

void CStudioView::init()
{
	/*
	CRect rt;
	GetClientRect(rt);
	CViewTree::CTreeCtrl::SetWindowPos(nullptr, 0, 0, rt.Width(), rt.Height(), 0);
	*/

	ModifyStyle(NULL, TVS_SHOWSELALWAYS, 0);
	//UI �׸�
	SetBkColor(RGB(35, 35, 35));
	SetLineColor(RGB(125, 125, 125));
	SetTextColor(RGB(255, 255, 255));
	//	SetBkColor(RGB(245, 245, 255));
	//	SetLineColor(RGB(125, 125, 125));
	//	SetTextColor(RGB(35, 35, 35));

	// Ʈ����Ʈ�� ���� ����
}

void CStudioView::update()
{
	ShowScrollBar(SB_HORZ, FALSE);
	// �������� ������Ʈ�� ������ ����
	for (size_t i = 0; i < m_vecDead.size(); ++i)
	{
		if (m_vecDead[i] == CPropertiesWnd::m_pPrevTarget)	// ���� ������Ʈ�� ���⼭ ���� ���߿� �ʿ�� �迭�� ���� �̽���
		{
			CPropertiesWnd::m_pPrevTarget = nullptr;
		}
		SAFE_DELETE(m_vecDead[i]);
	}
	m_vecDead.clear();


	// �߻��� �̺�Ʈ�� Ȯ���Ѵ�.
	const vector<tEvent>& vecEvent = CEventMgr::GetInst()->GetEventList();

	for (size_t i = 0; i < vecEvent.size(); ++i)
	{
		switch (vecEvent[i].eType)
		{
		case EVENT_TYPE::CREATE_OBJECT:
			AddGameObject((CGameObject*)vecEvent[i].wParam, nullptr);
			break;
		case EVENT_TYPE::DELETE_OBJECT:
		{
			HTREEITEM hFind = FindItemByData((DWORD_PTR)vecEvent[i].wParam);
			assert(hFind);
			DeleteItem(hFind);
		}
		break;
		case EVENT_TYPE::ADD_CHILD:
		{
			// ������, �̵��� ���� ������ �ڵ� ����
			HTREEITEM hDest = FindItemByData((DWORD_PTR)vecEvent[i].wParam);
			HTREEITEM hSrc = FindItemByData((DWORD_PTR)vecEvent[i].lParam);

			// ������ ������Ų��.
			DeleteItem(hSrc);

			// �������� ����(���� �ϱ� ���ؼ�)
			CGameObject* pDestObj = (CGameObject*)CViewTree::CTreeCtrl::GetItemData(hDest);
			HTREEITEM hDestParent = CViewTree::CTreeCtrl::GetParentItem(hDest);
			DeleteItem(hDest);

			// ������ �������� ���� �߰��Ѵ�.
			AddGameObject(pDestObj, hDestParent);
		}
		break;
		case EVENT_TYPE::CLEAR_PARENT:
		{
			HTREEITEM hTarget = FindItemByData((DWORD_PTR)vecEvent[i].wParam);
			MoveItem(hTarget, nullptr);
		}
		break;
		}
	}

	if (CEventMgr::GetInst()->GetClearing())
	{
		Renew();
		CEventMgr::GetInst()->SetClearing(false);
	}
}

void CStudioView::Renew()
{
	CViewTree::CTreeCtrl::DeleteAllItems();

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();

	HTREEITEM hRoot = InsertItem(_T("Scene"), 1, 1);
	SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		const vector<CGameObject*>& vecParentObj = pCurScene->GetLayer(i)->GetParentObj();
		for (UINT j = 0; j < vecParentObj.size(); ++j)
		{
			AddGameObject(vecParentObj[j], hRoot);
		}
	}
	ExpandAll_Item();
	//	Expand(hRoot, TVE_EXPAND);
}

void CStudioView::SelectObject(CGameObject * _pObject)
{
	HTREEITEM hItem = FindItemByData((DWORD_PTR)_pObject);
	assert(hItem);
	CViewTree::CTreeCtrl::SelectItem(hItem);
}

void CStudioView::ExpandAll_Item()
{
	HTREEITEM hItem;

	hItem = CViewTree::CTreeCtrl::GetFirstVisibleItem();

	while (hItem != NULL)
	{
		CViewTree::CTreeCtrl::Expand(hItem, TVE_EXPAND);
		hItem = CViewTree::CTreeCtrl::GetNextItem(hItem, TVGN_NEXTVISIBLE);
	}
}


void CStudioView::AddGameObject(CGameObject * _pGameObject, HTREEITEM _hParent)
{
	HTREEITEM hObj = AddItem(_pGameObject->GetName().c_str(), _hParent, (DWORD_PTR)_pGameObject);

	SetItemImage(hObj, 3, 3);
	if (GetParentItem(hObj) != GetRootItem())
	{
		SetItemImage(hObj, 5, 5);
		SetItemImage(GetParentItem(hObj), 4, 4);
	}
	const vector<CGameObject*>& child = _pGameObject->GetChild();
	for (size_t i = 0; i < child.size(); ++i)
	{
		AddGameObject(child[i], hObj);
	}
}

void CStudioView::DragDrop(HTREEITEM _hSrcItem, HTREEITEM _hDestItem)
{
	if (!_hSrcItem || !_hDestItem)
	{
		return;
	}

	CGameObject* pSrcObj = (CGameObject*)CViewTree::CTreeCtrl::GetItemData(_hSrcItem);
	CGameObject* pCheckDestObj = (CGameObject*)CViewTree::CTreeCtrl::GetItemData(_hDestItem);

	if (nullptr == pCheckDestObj)
	{
		if (!pSrcObj)
		{
			return;
		}
		pSrcObj->ClearParent();
	}
	else
	{
		CGameObject* pDestObj = (CGameObject*)CViewTree::CTreeCtrl::GetItemData(_hDestItem);
		pDestObj->AddChild(pSrcObj);
	}
}

CStudioView::CStudioView()
{
}

CStudioView::~CStudioView()
{
}
BEGIN_MESSAGE_MAP(CStudioView, CViewTree)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CStudioView::OnTvnSelchanged)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, &CStudioView::OnTvnEndlabeledit)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &CStudioView::OnTvnBegindrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_NOTIFY_REFLECT(NM_DBLCLK, &CStudioView::OnNMDblclk)
END_MESSAGE_MAP()


void CStudioView::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CTreeCtrl::SelectItem(pNMTreeView->itemNew.hItem);

	CGameObject* pTargetObject = (CGameObject*)CViewTree::CTreeCtrl::GetItemData(pNMTreeView->itemNew.hItem);

	((CPropertiesWnd*)((CMainFrame*)AfxGetMainWnd())->GetPropertyView())->SetTarget(pTargetObject);


	HTREEITEM hItem = pNMTreeView->itemNew.hItem;

	if (hItem != NULL)
		Select(hItem, TVGN_DROPHILITE);

	*pResult = 0;
}


void CStudioView::OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CEdit* pEdit = CViewTree::CTreeCtrl::GetEditControl();

	if (pEdit)
	{
		CString str;
		pEdit->GetWindowTextW(str);
		CViewTree::CTreeCtrl::SetItemText(pTVDispInfo->item.hItem, str);

		CGameObject* pObject = (CGameObject*)CViewTree::CTreeCtrl::GetItemData(pTVDispInfo->item.hItem);
		if (pObject != nullptr)
		{
			pObject->SetName(str.GetBuffer());
		}
	}

	*pResult = 0;

}


BOOL CStudioView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	if (pMsg->message == WM_KEYDOWN)
	{
		/*if (pMsg->wParam == VK_RETURN)
		{
			((CWnd*)((CMainFrame*)AfxGetMainWnd())->GetGameView())->SetFocus();
		}

		else */

		if (pMsg->wParam == VK_DELETE)
		{
			HTREEITEM hTarget = CViewTree::CTreeCtrl::GetSelectedItem();
			CGameObject* pTargetObject = (CGameObject*)CViewTree::CTreeCtrl::GetItemData(hTarget);

			if (pTargetObject != nullptr)
			{
				pTargetObject->SetDead();
				pTargetObject->ClearParent();
				m_vecDead.push_back(pTargetObject);

				DeleteItem(hTarget);
			}
		}
	}

	return CViewTree::PreTranslateMessage(pMsg);
}


void CStudioView::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	EditLabel(GetSelectedItem());
	*pResult = 0;
}
