
#include "pch.h"
#include "framework.h"
#include "ViewTree.h"

#include "MainFrm.h"
#include <Engine/GameObject.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char ViewTree_THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewTree

CViewTree::CViewTree() noexcept
	: m_hDragItem(nullptr)
	, m_hDestItem(nullptr)
	, isSelect(false)
{
}

HTREEITEM CViewTree::AddItem(CString _strItemName, HTREEITEM _hParent, DWORD_PTR _dwData)
{
	TVINSERTSTRUCT st = {};

	st.hParent = _hParent;
	st.hInsertAfter = TVI_SORT;
	st.item.pszText = _strItemName.GetBuffer();
	st.item.mask = TVIF_TEXT;

	HTREEITEM hItem = CTreeCtrl::InsertItem(&st);
	CTreeCtrl::SetItemData(hItem, _dwData);

	CGameObject* pSceneObj = (CGameObject*)CViewTree::CTreeCtrl::GetItemData(hItem);

	if (pSceneObj != nullptr)
	{
		SetItemImage(hItem, 2, 2);
	}


	if (_dwData != 0)
	{
		m_mapHandle.insert(make_pair(_dwData, hItem));
	}

	return hItem;
}

CViewTree::~CViewTree()
{
}

BEGIN_MESSAGE_MAP(CViewTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &CViewTree::OnTvnBegindrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTree 메시지 처리기

void CViewTree::DeleteItem(HTREEITEM _hItem)
{
	list<HTREEITEM> listChild;
	listChild.push_back(_hItem);

	HTREEITEM hTarget = nullptr;
	while (!listChild.empty())
	{
		hTarget = listChild.front();
		listChild.pop_front();

		DeleteItemByData(CTreeCtrl::GetItemData(hTarget));

		if (CTreeCtrl::ItemHasChildren(hTarget))
		{
			HTREEITEM hNextItem = nullptr;
			HTREEITEM hChildItem = CTreeCtrl::GetChildItem(hTarget);

			while (hChildItem != NULL)
			{
				listChild.push_back(hChildItem);
				hChildItem = CTreeCtrl::GetNextSiblingItem(hChildItem);
			}
		}
	}

	CTreeCtrl::DeleteItem(_hItem);
}

void CViewTree::MoveItem(HTREEITEM _hSrcItem, HTREEITEM _hDestItem)
{
	// 자기 자신으로 드롭되는경우, 드래그 아이템이 없는 경우
	if (_hSrcItem == _hDestItem || nullptr == _hSrcItem || _hDestItem == nullptr)
	{
		return;
	}

	wchar_t szName[255] = {};

	TV_ITEM tItem = {};
	tItem.hItem = _hSrcItem;
	tItem.pszText = szName;
	tItem.cchTextMax = 255;
	tItem.mask = TVIF_TEXT | TVIF_HANDLE;
	DWORD_PTR dwData = CTreeCtrl::GetItemData(_hSrcItem);

	CTreeCtrl::GetItem(&tItem);

	// 목적지에 새로운 아이템을 집어넣는다.
	HTREEITEM hCopyItem = AddItem(szName, _hDestItem);
	CTreeCtrl::SetItemData(hCopyItem, dwData);

	// 원본 아이템이 자식이 있는경우
	vector<HTREEITEM> vecChild;
	GetChildItem(_hSrcItem, vecChild);

	CGameObject* pSceneObj = (CGameObject*)CViewTree::CTreeCtrl::GetItemData(_hDestItem);

	if (pSceneObj != nullptr)	// 정상 노드 관계
	{
		SetItemImage(hCopyItem, 5, 5);
		SetItemImage(_hDestItem, 4, 4);
	}
	else						// Dummy Scene
	{
		SetItemImage(hCopyItem, 3, 3);
	}

	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		MoveItem(vecChild[i], hCopyItem);
	}

	// Targeting
	CGameObject* pTargetObject = (CGameObject*)CViewTree::CTreeCtrl::GetItemData(hCopyItem);
	((CPropertiesWnd*)((CMainFrame*)AfxGetMainWnd())->GetPropertyView())->SetTarget(pTargetObject);
	//

	CTreeCtrl::SelectItem(hCopyItem);


	// 목적지가 있으면
	if (nullptr != _hDestItem)
	{
		CTreeCtrl::Expand(_hDestItem, TVE_EXPAND);
	}

	// 원본 삭제 직전

	//예외처리 무시해도 됨
	CGameObject* pDestObj = (CGameObject*)CViewTree::CTreeCtrl::GetItemData(GetParentItem(_hSrcItem));

	if (pDestObj != nullptr)
	{
		vector<HTREEITEM> vecChildChecker;
		GetChildItem(GetParentItem(_hSrcItem), vecChildChecker);

		CGameObject* Obj = (CGameObject*)CViewTree::CTreeCtrl::GetItemData(GetParentItem(GetParentItem(_hSrcItem)));

		if (vecChildChecker.size() == 1)
		{
			if (Obj == nullptr)	//Obj는 Scene 더미 폴더
			{
				SetItemImage(GetParentItem(_hSrcItem), 3, 3);
			}
			else
			{
				SetItemImage(GetParentItem(_hSrcItem), 5, 5);
			}
		}
	}

	// 원본 삭제
	CTreeCtrl::DeleteItem(_hSrcItem);
}

void CViewTree::GetChildItem(HTREEITEM _hParent, vector<HTREEITEM>& _vecOut)
{
	HTREEITEM hChild = CTreeCtrl::GetChildItem(_hParent);

	while (hChild)
	{
		_vecOut.push_back(hChild);
		hChild = CTreeCtrl::GetNextSiblingItem(hChild);
	}
}

bool CViewTree::IsAncestor(HTREEITEM _hParent, HTREEITEM _hChild)
{
	HTREEITEM hItem = CTreeCtrl::GetParentItem(_hChild);
	while (hItem)
	{
		if (_hParent == hItem)
			return true;

		hItem = CTreeCtrl::GetParentItem(hItem);
	}

	return false;
}

BOOL CViewTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != nullptr);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != nullptr)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}

	return bRes;
}


void CViewTree::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (GetSafeHwnd())
	{
		delete this;
	}
	CTreeCtrl::PostNcDestroy();
}


void CViewTree::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_hDragItem = pNMTreeView->itemNew.hItem;
	SelectItem(m_hDragItem);
	// 윈도우 메세지를 가로챔
//	SetCapture();

	isSelect = true;

	*pResult = 0;
}


void CViewTree::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!isSelect)
	{
		return;
	}

	ClientToScreen(&point);
	::ScreenToClient(CTreeCtrl::m_hWnd, &point);
	m_hDestItem = CTreeCtrl::HitTest(point);

	if (m_hDragItem != m_hDestItem)
	{
		CTreeCtrl::SelectDropTarget(m_hDestItem);
	}
	else
	{
		CTreeCtrl::SelectDropTarget(nullptr);
	}

	CTreeCtrl::OnMouseMove(nFlags, point);
}


void CViewTree::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// CTreeCtrlDlg 캡처 해제
	
	// ReleaseCapture();

	// 드래그 아이템을 목적지에 넣음
	if (!IsAncestor(m_hDragItem, m_hDestItem))
	{
		DragDrop(m_hDragItem, m_hDestItem);
		MoveItem(m_hDragItem, m_hDestItem);
	}

	// 드롭 타겟 해제
	CTreeCtrl::SelectDropTarget(nullptr);

	// 드래그, 목적지 초기화
	m_hDragItem = nullptr;
	m_hDestItem = nullptr;

	isSelect = false;


	CTreeCtrl::OnLButtonUp(nFlags, point);
}
