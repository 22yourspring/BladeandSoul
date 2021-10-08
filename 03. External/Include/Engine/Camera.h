#pragma once
#include "Component.h"

#include "Frustum.h"

enum class PROJ_TYPE
{
	ORTHOGRAPHIC,
	PERSPECTIVE,
};

class CCamera :
	public CComponent
{
private:
	CFrustum	m_frustum;

	float		m_fNear; // View Space ���� �þ� ����
	float		m_fFar;  // View Space ���� �þ� ����

	float		m_fFOV;  // ���� ���� ��, �þ߰�
	float		m_fScale; // �������� ��, ���� ���� ����
	float		m_fWidth; // ���� �ػ�
	float		m_fHeight;// ���� �ػ�

	Matrix		m_matView;		// �����
	Matrix		m_matViewInv;	// �俪���
	Matrix		m_matProj;		// �������
	Matrix		m_matProjInv;	// ���������

	PROJ_TYPE	m_eProjType;
	UINT		m_iLayerCheck; // ī�޶� ��� Layer ��Ʈ��
	tRay		m_tRay;

	bool		m_bChangeRes;
	tResolution	m_tViewRect;
	// ������
	vector<CGameObject*>				m_vecDeferred;
	vector<CGameObject*>				m_vecForward;
	vector<CGameObject*>				m_vecPostEffect;

	// �ν��Ͻ� ����
	map<ULONG64, vector<tInstObj>>		m_mapInstGroup_F; // Foward
	map<ULONG64, vector<tInstObj>>		m_mapInstGroup_D; // Deferred
	map<ULONG64, vector<tInstObj>>		m_mapInstGroup_P; // Post

	map<INT_PTR, vector<tInstObj>>		m_mapSingleObj; // Single Object
	map<INT_PTR, vector<tInstObj>>		m_mapDecalObj; // Single Object


	vector<CGameObject*>				m_vecParticle;
	vector<CGameObject*>				m_vecShadow;

	vector<CGameObject*>				m_vecDecalObj;

	

	bool								m_bModule;

public:
	CGameObject*						m_pMoon = nullptr;

private:
	static bool UpdateRegistCam;
public:
	static void SetRegistCam(bool _SetRegist) { UpdateRegistCam = _SetRegist; }

public:
	virtual void finalupdate();

	void SortGameObject(); // ������ ���� �з�
	void SortShadowObject(); // �׸��ڰ� �׷��� ������Ʈ �з�
	void render_deferred();
	void render_forward();
	void render_posteffect();
	void render_shadowmap();

	void render();
	void renderClient_ADDVer();
	void renderEngine_ADDVer();

	void ColliderRender();

	void SetChangeRes(bool _SetRes);
	void SetResolution(tResolution _m_tViewRect);

	void render_Decal();
	void DecalPushBack(CGameObject* _DecalObj);

public:
	void SetProjType(PROJ_TYPE _eType);
	PROJ_TYPE GetProjType() { return m_eProjType; }

	void SetFar(float _far) { m_fFar = _far; Changed(); }
	void SetNear(float _Near) { m_fNear = _Near; Changed(); }
	void SetScale(float _fScale) { m_fScale = _fScale; if (m_fScale < 0.01f) m_fScale = 0.01f; Changed(); }
	void SetWidth(float _fWidth) { m_fWidth = _fWidth; }
	void SetHeight(float _fHeight) { m_fHeight = _fHeight; }
	void SetFOV(float _fAngle) { m_fFOV = _fAngle; Changed(); }
	void SetModule(bool _bModule) { m_bModule = _bModule; }


	float GetFar() { return m_fFar; }
	float GetNear() { return m_fNear; }
	float GetScale() { return m_fScale; }
	float GetFOV() { return m_fFOV; }

	float GetWidth() { return m_fWidth; }
	float GetHeight() { return m_fHeight; }


	tRay GetRay() { return m_tRay; }

	void SetLayerCheck(int _iLayerIdx, bool _bAdd)
	{
		assert(_iLayerIdx != -1);
		if (_bAdd)
			m_iLayerCheck |= (1 << _iLayerIdx);
		else
			m_iLayerCheck &= ~(1 << _iLayerIdx);
	}
	void SetLayerAllCheck() { m_iLayerCheck = 0xfffffff; }
	void SetLayerAllRelease() { m_iLayerCheck = 0; }


	const Matrix& GetViewMat() { return m_matView; }
	const Matrix& GetViewMatInv() { return m_matViewInv; }
	const Matrix& GetProjMat() { return m_matProj; }
	const Matrix& GetProjMatInv() { return m_matProjInv; }

public:
	CLONE(CCamera);
	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);
	void CalRay();


public:
	CCamera();
	virtual ~CCamera();
};

