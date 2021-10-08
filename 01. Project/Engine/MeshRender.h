#pragma once
#include "Component.h"


#include "Ptr.h"

#include "Mesh.h"
#include "Material.h"

class CMeshRender :
	public CComponent
{
private:
	Ptr<CMesh>				m_pMesh;
	vector<vector<Ptr<CMaterial>>>  m_vecMtrlSet;
	bool					m_bShadow;

	/*int m_iMaterialConatinerCount;
	int m_iMaterialSubsetCount;*/

public:
	void render();
	void render_shadowmap();

public:
	void SetMaterial(Ptr<CMaterial> _pMtrl, UINT _iSubset = 0, UINT _iContainer = 0);
	void SetMesh(Ptr<CMesh> _pMesh) { m_pMesh = _pMesh; Changed(); }

	void ShadowCheck(bool _bCheck) { m_bShadow = _bCheck; }
	bool GetShadowCheck() { return m_bShadow; }

	Ptr<CMaterial> GetCloneMaterial(UINT _iSubset = 0, UINT _iContainer = 0);
	Ptr<CMaterial> GetSharedMaterial(UINT _iSubset = 0, UINT _iContainer = 0);
	vector<vector<Ptr<CMaterial>>>* GetVectorMaterial() { return &m_vecMtrlSet; }

	Ptr<CMesh> GetMesh() { return m_pMesh; }

	UINT GetMaterialSubsetCount(UINT _iSubset = 0) { return (UINT)m_vecMtrlSet[_iSubset].size(); }
	UINT GetMaterialContainerCount() { return (UINT)m_vecMtrlSet.size(); }
	void CheckDynamicShadow(bool _bTrue) { m_bShadow = _bTrue; }
	bool IsDynamicShadow() { return m_bShadow; }


	ULONG64 GetInstID(UINT _iMtrlIdx, UINT _iContainer = 0);


public:
	CMeshRender* Clone() { return new CMeshRender(*this); }

	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

public:
	CMeshRender();
	virtual ~CMeshRender();
};

