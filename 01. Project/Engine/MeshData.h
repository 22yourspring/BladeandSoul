#pragma once

#include "FBXLoader.h"

#include "Ptr.h"
#include "Material.h"
#include "Mesh.h"

class CGameObject;


class CMeshData :
	public CResource
{
private:
	Ptr<CMesh>				m_pMesh;
	vector<vector<Ptr<CMaterial>>>	m_vecMtrlSet;

	// Animation3D
public:
	static CMeshData* LoadFromFBX(const wstring& _strFilePath);

	virtual void Load(const wstring& _strFilePath);
	virtual void Save(const wstring& _strFilePath);

	CGameObject* Instantiate();

public:
	CMeshData();
	virtual ~CMeshData();
};