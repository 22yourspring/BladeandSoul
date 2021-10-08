
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CViewTree 창입니다.

class CViewTree : public CTreeCtrl
{
	// 생성입니다.
public:
	CViewTree() noexcept;

protected:
	map<DWORD_PTR, HTREEITEM>	m_mapHandle;
	map<CString, HTREEITEM>		m_mapString;

	HTREEITEM					m_hDragItem;	// 드래그 중인 아이템 핸들
	HTREEITEM					m_hDestItem;    // 드래그 목적지 아이템 핸들


public:
	virtual void init() = 0;
	virtual void update() = 0;
	virtual void Renew() = 0;


	bool		isSelect;

protected:
	HTREEITEM AddItem(CString _strItemName, HTREEITEM _hParent, DWORD_PTR _dwData = 0);
	HTREEITEM FindItemByData(DWORD_PTR _dwData)
	{
		map<DWORD_PTR, HTREEITEM>::iterator iter = m_mapHandle.find(_dwData);
		if (iter == m_mapHandle.end())
			return nullptr;
		return iter->second;
	}

	HTREEITEM FindItemByString(CString _dwData)
	{
		map<CString, HTREEITEM>::iterator iter = m_mapString.find(_dwData);
		if (iter == m_mapString.end())
			return nullptr;
		return iter->second;
	}

	void DeleteItemByData(DWORD_PTR _dwData)
	{
		map<DWORD_PTR, HTREEITEM>::iterator iter = m_mapHandle.find(_dwData);
		if (iter != m_mapHandle.end())
		{
			m_mapHandle.erase(iter);
		}
	}

	void DeleteItem(HTREEITEM _hItem);
	void MoveItem(HTREEITEM _hSrcItem, HTREEITEM _hDestItem);
	void GetChildItem(HTREEITEM _hParent, vector<HTREEITEM>& _vecOut);
	bool IsAncestor(HTREEITEM _hParent, HTREEITEM _hChild);

	virtual void DragDrop(HTREEITEM _hSrcItem, HTREEITEM _hDestItem) {};



	// 재정의입니다.
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);

	// 구현입니다.
public:
	virtual ~CViewTree();

public:
	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
public:
	afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
