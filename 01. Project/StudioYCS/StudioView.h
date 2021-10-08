#pragma once
#include "ViewTree.h"

class CGameObject;

class CStudioView :	public CViewTree
{
//	DECLARE_DYNAMIC(CStudioView)

private:
	vector<CGameObject*>	m_vecDead;

public:
	virtual void init();
	virtual void update();

public:
	virtual void Renew();
	void SelectObject(CGameObject* _pObject);

public:
	void ExpandAll_Item();

private:
	void AddGameObject(CGameObject* _pGameObject, HTREEITEM _hParent);
	virtual void DragDrop(HTREEITEM _hSrcItem, HTREEITEM _hDestItem); // 드래그 앤 드롭 이후 처리함수

public:
	CStudioView();   // 표준 생성자입니다.
	virtual ~CStudioView();
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
};

