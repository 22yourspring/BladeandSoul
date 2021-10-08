#pragma once
#include "ViewTree.h"

class CStudioRes : public CViewTree
{

public:
	virtual void init();
	virtual void update();

public:
	virtual void Renew();
	virtual void PostNcDestroy();

private:
	void LoadResource();
	void LoadResource(const CString& _strFolderPath);
	void Load(const CString& _strFullPath);

public:
	CStudioRes();
	virtual ~CStudioRes();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
};

