#pragma once
#include "Res.h"

#include "Texture.h"
#include "Ptr.h"

#include "FBXLoader.h"
#include "StructuredBuffer.h"





class CMesh :
	public CResource
{
private:
	vector<tMeshContainer*>	m_vecMeshContainer;
	ID3D11InputLayout*		m_pLayout;
	// class CMaterial*		m_pMaterial;
	Vec3					m_vMin;
	Vec3					m_vMax;


	// Animation3D 정보
	vector<tMTAnimClip>		 m_vecAnimClip;
	vector<tMTBone>			 m_vecBones;

	CStructuredBuffer*		 m_pBoneFrameData; // 전체 본 프레임 정보
	CStructuredBuffer*		 m_pBoneOffset;	   // 각 뼈의 offset 행렬

	vector<Matrix>			 m_vecBoneMatrix;
	vector<vector<tFrameTrans>>		m_vecFrameTrans;
	int m_iFrameCount;

public:
	bool AddVertexBuffer(const void* pVertices,
		int iVtxSize, int iVtxCount, D3D11_USAGE eVtxUsage,
		D3D11_PRIMITIVE_TOPOLOGY ePrimitive,
		tMeshContainer* pContainer);
	bool AddIndexBuffer(const void* pIndices, int iIdxSize, int iIdxCount,
		D3D11_USAGE eIdxUsage, DXGI_FORMAT eFmt, tMeshContainer* pContainer);

	void Create(void* _pVtxMem, UINT _iSize, UINT _iVtxCount, D3D11_USAGE _eVtxUsage,
		void* _pIdxMem, DXGI_FORMAT _eIdxFormat, UINT _iIdxCount, D3D11_USAGE _IdxUsage);

	static CMesh* CreateFromContainer(CFBXLoader & _loader);

	void render(UINT _iSubset = 0, UINT _iContainer = 0);
	void render_particle(UINT _iInstCount, UINT _iSubset = 0);
	void render_instancing(UINT _iSubset, UINT _iContainer);

public:
	void SetKeyFrame(vector<vector<tFrameTrans>>& vecFrameTrans);
	const vector<vector<tFrameTrans>>* GetKeyFrame() const { return &m_vecFrameTrans; }

	const vector<Matrix>* GetBoneMatrix() const { return &m_vecBoneMatrix; }

	UINT GetContainerCount() const { return m_vecMeshContainer.size(); }
	UINT GetSubsetCount(int iContainer) { return (UINT)m_vecMeshContainer[iContainer]->vecIB.size(); }
	void AddContainer(vector<tMeshContainer*> _OtherContainer, CMesh &_Mesh);


	vector<tMeshContainer*>& GetSubset() { return m_vecMeshContainer; }

	const vector<tMTBone>* GetBones() { return &m_vecBones; }
	UINT GetBoneCount() { return (UINT)m_vecBones.size(); }
	const vector<tMTAnimClip>* GetAnimClip() { return &m_vecAnimClip; }

	bool IsAnimMesh() { return !m_vecAnimClip.empty(); }

	CStructuredBuffer*	GetBoneFrameDataBuffer() { return m_pBoneFrameData; } // 전체 본 프레임 정보
	CStructuredBuffer*	GetBoneOffsetBuffer() { return  m_pBoneOffset; }	   // 각 뼈의 offset 행렬	
	int GetFrameCount() const { return m_iFrameCount; }
public:
	virtual void Load(const wstring& _strFullPath);
	virtual void Save(FILE * _pFile, const wstring& _strPath/*상대 경로*/);

public:
	CMesh();
	virtual ~CMesh();
};

