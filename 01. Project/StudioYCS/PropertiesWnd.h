
enum class TESS_OPTION
{
	DISABLE,
	ENABLE,
	END,
};

#pragma once
class CPropGridSlider;
class CPropertiesToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CPropertiesWnd : public CDockablePane
{
	// 생성입니다.
public:
	CPropertiesWnd() noexcept;

	void AdjustLayout();


private:
	CGameObject*		m_pTarget;
	bool				m_bUpdate;		// 컴포넌트 내용을 컨트롤에 업데이트 시킨적이 있는지
	bool				m_bEdit;		// 에디트 컨트롤에 입력 중인지 아닌지

public:
	static CGameObject*		m_pPrevTarget;
	CGameObject*		m_TerrainTarget;

	Vec3				arrTrans[3];
	Vec3				PrevTrans[3];

	Vec3				arrCollider2D[2];
	Vec3				PrevCollider2D[2];

	int					m_iPrevmeshmapSize;
	int					m_iPrevMtrlmapSize;

	bool				ShaderClear;

	bool				TessEnable;
	bool				BrushEnable;
	bool				MaterialEnable;

	bool				LightEnable;
	bool				LightOptionEnable;
	bool				LightColorEnable;

	bool				m_bCameraWeather;
	bool				m_bCameraRange;
	bool				m_bCameraRatio;
	bool				m_bCameraResolution;

	bool				m_bParticleTime_Enable;
	bool				m_bParticleLife_Enable;
	bool				m_bParticlePace_Enable;
	bool				m_bParticleScale_Enable;
	bool				m_bParticleColor_Enable;

	TESS_OPTION			m_CurOption;
	TESS_OPTION			m_PrevOption;

	CAMERA_MOD _MOD;
	CAMERA_MOD Prev_MOD;

	bool		Rim_Light;
	bool		Prev_Light;

	CString strMesh;
	CString strMtrl;

	CString PrevstrMesh;
	CString PrevstrMtrl;

	bool		m_bWeatherEnable;


public:
	void SetTarget(CGameObject* _pTarget)
	{
		if (m_pTarget != _pTarget)
		{
			m_pTarget = _pTarget;
			m_bUpdate = false;
		}
	}

	CGameObject* GetTarget() { return m_pTarget; }


	void CtrlUpdate() { m_bUpdate = true; } // 컨트롤이 Update 되었음을 알림
	bool IsUpdate() { return m_bUpdate; } // 컨트롤이 Object 의 내용을 갱신 했는지 않했는지
	bool IsEdit() { return m_bEdit; } // 에디트 컨트롤에 입력중인지 아닌지.
	void SetEdit(bool _bTrue) { m_bEdit = _bTrue; }

	void EditSetFocus(UINT _id) { SetEdit(true); }
	void EditKillFocus(UINT _id) { SetEdit(false); }

protected:
	void SetFocusGameView();





	//ComponentDlg Func
public:
	//Transform
	void UpdateCtrl_Transform();
	CEdit	m_edit[9];

	//MeshRender
	void UpdateCtrl_MeshRender();

	//Camera
	void UpdateCtrl_Camera();

	//Collider2D
	void UpdateCtrl_Collider2D();

	//Collider3D
	void UpdateCtrl_Collider3D();

	//Animator2D
	void UpdateCtrl_Animator2D();

	//Animator3D
	void UpdateCtrl_Animator3D();

	//Light2D
	void UpdateCtrl_Light2D();

	//Light3D
	void UpdateCtrl_Light3D();

	//Script
	void UpdateCtrl_Script();

	//Terrain
	void UpdateCtrl_Terrain();

	//ParticleSystem
	void UpdateCtrl_ParticleSystem();

	void TessOptionEnable(CGameObject* _TargetObj);
	void TessOptionDisable(CGameObject* _TargetObj);

	// 특성입니다.
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}

protected:
	CFont m_fntPropList;
	//	CComboBox m_wndObjectCombo;
	//	CPropertiesToolBar m_wndToolBar;
	CMFCPropertyGridCtrl m_wndPropList;
	//	vector<CScriptDlg*> m_vecScriptDlg;

public:
	bool				m_PrevActive[(UINT)COMPONENT_TYPE::END];

public:
	void update();

public:
	static 	CGameObject* StaticTarget;


public:
	wstring	MtrL_To_apply;
	wstring	MtrL_Origin;

	wstring AnimState;
	wstring Prev_AnimState;
	wstring Prev_Prev_AnimState;

	wstring RimLightName;
	wstring PrevRimLightName;

	wstring _ParticleMtrl_Name;
	wstring _PrevParticleMtrl_Name;

	wstring _WeatherMtrl_Name;
	wstring _PrevWeatherMtrl_Name;

	CMFCPropertyGridProperty* m_PropertyName;
	CMFCPropertyGridProperty* m_PropertyLayerTitle;
	CMFCPropertyGridProperty* m_PropertyLayer;

	CMFCPropertyGridProperty* m_PropertyTransform;
	CMFCPropertyGridProperty* m_PropertyTransform_Position;
	CMFCPropertyGridProperty* m_PropertyTransform_Position_X;
	CMFCPropertyGridProperty* m_PropertyTransform_Position_Y;
	CMFCPropertyGridProperty* m_PropertyTransform_Position_Z;
	CMFCPropertyGridProperty* m_PropertyTransform_Scale;
	CMFCPropertyGridProperty* m_PropertyTransform_Scale_X;
	CMFCPropertyGridProperty* m_PropertyTransform_Scale_Y;
	CMFCPropertyGridProperty* m_PropertyTransform_Scale_Z;
	CMFCPropertyGridProperty* m_PropertyTransform_Rotation;
	CMFCPropertyGridProperty* m_PropertyTransform_Rotation_X;
	CMFCPropertyGridProperty* m_PropertyTransform_Rotation_Y;
	CMFCPropertyGridProperty* m_PropertyTransform_Rotation_Z;

	CMFCPropertyGridProperty* m_PropertyMesh_Renderer;
	CMFCPropertyGridProperty* m_PropertyMesh_MeshTitle;
	CMFCPropertyGridProperty* m_PropertyMesh_MeshData;		//0708
	CMFCPropertyGridProperty* m_PropertyMesh_Mesh;
	CMFCPropertyGridProperty* m_PropertyMesh_MaterialTitle;
	CMFCPropertyGridProperty* m_PropertyMesh_MaterialData;	//0708
	CMFCPropertyGridProperty* m_PropertyMesh_Material;

	CMFCPropertyGridProperty* m_PropertyCollider2D;
	CMFCPropertyGridProperty* Offset2D_Pos;
	CMFCPropertyGridProperty* Offset2D_PosX;
	CMFCPropertyGridProperty* Offset2D_PosY;
	CMFCPropertyGridProperty* Offset2D_Scale;
	CMFCPropertyGridProperty* Offset2D_ScaleX;
	CMFCPropertyGridProperty* Offset2D_ScaleY;

	CMFCPropertyGridProperty* m_PropertyCollider3D;
	CMFCPropertyGridProperty* Offset3D_Pos;
	CMFCPropertyGridProperty* Offset3D_PosX;
	CMFCPropertyGridProperty* Offset3D_PosY;
	CMFCPropertyGridProperty* Offset3D_PosZ;
	CMFCPropertyGridProperty* Offset3D_Scale;
	CMFCPropertyGridProperty* Offset3D_ScaleX;
	CMFCPropertyGridProperty* Offset3D_ScaleY;
	CMFCPropertyGridProperty* Offset3D_ScaleZ;

	CMFCPropertyGridProperty* m_Animator2D;
	CMFCPropertyGridProperty* m_Animation2D;

	CMFCPropertyGridProperty* m_Animator3D;
	CMFCPropertyGridProperty* m_Animation3DCurAnim;
	CMFCPropertyGridProperty* m_Animation3D;


	CMFCPropertyGridProperty* m_LightScript;

	CMFCPropertyGridProperty* LightType3D;

	CMFCPropertyGridProperty* m_LightType;
	CMFCPropertyGridProperty* m_LightColor;
	CMFCPropertyGridProperty* m_LightColor_R;
	CMFCPropertyGridProperty* m_LightColor_G;
	CMFCPropertyGridProperty* m_LightColor_B;
	CMFCPropertyGridProperty* m_LightRim;
	CMFCPropertyGridProperty* m_LightRange;
	CMFCPropertyGridProperty* m_LightInAngle;
	CMFCPropertyGridProperty* m_LightOutAngle;

	//
	CMFCPropertyGridProperty* m_Terrain;

	CMFCPropertyGridProperty* m_Terrain_Tessellation;
	CMFCPropertyGridProperty* m_Terrain_TessEnable;
	CPropGridSlider*		  m_Terrain_TessMin;
	CPropGridSlider*		  m_Terrain_TessMax;
	CPropGridSlider*		  m_Terrain_TessPhase;

	CMFCPropertyGridProperty* m_Terrain_Brush;
	CMFCPropertyGridProperty* m_Terrain_BrushType;
	CPropGridSlider*		  m_Terrain_BrushScale;

	CMFCPropertyGridProperty* m_Terrain_Landscape;
	CMFCPropertyGridProperty* m_Terrain_LandscapingState;
	CMFCPropertyGridColorProperty* m_Terrain_Splatting_Material;

	//
	
	/*static const TCHAR szFilter[];*/
	/*
	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("아이콘"), TRUE, _T(""), _T("ico"), 0, szFilter, _T("창 아이콘을 지정합니다.")));

	pGroup3->AddSubItem(new CMFCPropertyGridFileProperty(_T("폴더"), _T("c:\\")));
	*/

	//
	CMFCPropertyGridProperty* m_ParticleSystem;

	CMFCPropertyGridProperty* ParticleFrequencyTitle;
	CMFCPropertyGridFileProperty* m_ParticleFile;			// File Path
	CMFCPropertyGridProperty*	m_ParticleType;
	CPropGridSlider* m_ParticleFrequncy;				// 생성 주기

	CMFCPropertyGridProperty* ParticleLifeTime;
	CPropGridSlider*	Particle_MinimumLifeTime;		// 파티클 최소 생존 시간
	CPropGridSlider*	Particle_MaximumLifeTime;		// 파티클 최대 생존 시간

	CMFCPropertyGridProperty* ParticleSpeed;
	CPropGridSlider* Particle_MinSpeed;					// 파티클 시작 속도
	CPropGridSlider* Particle_MaxSpeed;					// 파티클 최종 속도

	CMFCPropertyGridProperty* ParticleSacle;
	CPropGridSlider*	Particle_StartScale;			// 파티클 시작 크기
	CPropGridSlider*	Particle_EndScale;				// 파티클 최종 크기

	CMFCPropertyGridProperty* ParticleColor;
	CMFCPropertyGridProperty* Particle_StartColor;		// 파티클 시작 컬러
	CPropGridSlider* Particle_StartColor_R;			
	CPropGridSlider* Particle_StartColor_G;			
	CPropGridSlider* Particle_StartColor_B;			
	CPropGridSlider* Particle_StartColor_A;			

	CMFCPropertyGridProperty* Particle_EndColor;		// 파티클 시작 컬러
	CPropGridSlider* Particle_EndColor_R;	
	CPropGridSlider* Particle_EndColor_G;	
	CPropGridSlider* Particle_EndColor_B;	
	CPropGridSlider* Particle_EndColor_A;	

	//

	CMFCPropertyGridProperty* m_Camera;

	CMFCPropertyGridProperty* CameraOption;

	CMFCPropertyGridProperty*		WeatherOption;
	CMFCPropertyGridProperty*		WeatherEnable;
	CMFCPropertyGridFileProperty*	WeatherShaderTexture;

	CMFCPropertyGridProperty* CameraRange;
	CPropGridSlider* CameraNear;
	CPropGridSlider* CameraFar;

	CMFCPropertyGridProperty* CameraRatio;
	CPropGridSlider* CameraFOV;
	CPropGridSlider* CameraScale;

	CMFCPropertyGridProperty* CameraResolution;
	CPropGridSlider* CameraWidth;
	CPropGridSlider* CameraHeight;

	//Disable
	CMFCPropertyGridProperty* TransformDisable;
	CMFCPropertyGridProperty* MeshRendererDisable;
	CMFCPropertyGridProperty* CameraDisable;
	CMFCPropertyGridProperty* Offset2DDisable;
	CMFCPropertyGridProperty* Offset3DDisable;
	CMFCPropertyGridProperty* Anim2DDisable;
	CMFCPropertyGridProperty* Anim3DDisable;
	CMFCPropertyGridProperty* LightDisable;
	CMFCPropertyGridProperty* TerrainDisable;
	CMFCPropertyGridProperty* ParticleDisable;

public:
	void ReSetParticle();

	// 구현입니다.
public:
	virtual ~CPropertiesWnd();

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnExpandAllProperties();
	afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnProperties1();
	afx_msg void OnUpdateProperties1(CCmdUI* pCmdUI);
	afx_msg void OnProperties2();
	afx_msg void OnUpdateProperties2(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();

	int m_nComboHeight;
};