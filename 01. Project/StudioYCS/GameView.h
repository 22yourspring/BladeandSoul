#pragma once

class CGameObject;

#include <Engine/Ptr.h>
#include <Engine/Mesh.h>
#include <Engine/Material.h>

#include "StudioYCSView.h"

// CGameView 보기
class CGameView : public CStudioYCSView
{
	DECLARE_DYNCREATE(CGameView)
private:
	CGameObject*			m_pToolCam;
	CGameObject*			m_pMainCam;
	CGameObject*			m_pGridObject;
	vector<CGameObject*>	m_vecTargetUI;
	Ptr<CMaterial>			m_pDirMtrl;
	Ptr<CMesh>				m_pDirMesh;

	Ptr<CShader>			m_pGridShader;
	Ptr<CMaterial>			m_pGridMtrl;

	ID3D11Texture2D* m_pHDRTexture = NULL;
	ID3D11RenderTargetView* m_HDRRTV = NULL;
	ID3D11ShaderResourceView* m_HDRSRV = NULL;

public:
	void init();
	void update();

	void SetMainCam(CGameObject* _MainCam);

	Vec3 TessLevel;
	Vec3 PrevLevel;

	static bool ViewFocus;

protected:
	CGameView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CGameView();

public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


