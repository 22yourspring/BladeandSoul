#include "stdafx.h"
#include "ResMgr.h"

#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "Sound.h"

#include "RenderMgr.h"


CResMgr::CResMgr()
{
}

CResMgr::~CResMgr()
{
	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		Safe_Delete_Map(m_mapRes[i]);
	}

	for (UINT i = 0; i < (UINT)RES_TYPE::END; ++i)
	{
		Safe_Delete_Vector(m_vecCloneRes[i]);
	}
}


Ptr<CTexture> CResMgr::CreateTexture(const wstring _strKey, float _fWidth, float _iHeight
	, UINT _iBindFlag, DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage)
{
	Ptr<CTexture> pTex = FindRes<CTexture>(_strKey);

	if (nullptr != pTex)
		assert(nullptr);

	pTex = new CTexture;
	if (!pTex->Create(_fWidth, _iHeight, _iBindFlag, _eFormat, _eUsage))
	{
		MessageBox(nullptr, _strKey.c_str(), L"텍스쳐 생성 실패", MB_OK);
		assert(nullptr);
	}

	pTex->SetName(_strKey);

	m_mapRes[(UINT)RES_TYPE::TEXTURE].insert(make_pair(_strKey, pTex.GetPointer()));

	return pTex;
}

Ptr<CTexture> CResMgr::CreateTexture(const wstring & _strKey, ID3D11Texture2D * _pTex2D)
{
	Ptr<CTexture> pTex = FindRes<CTexture>(_strKey);

	if (nullptr != pTex)
		assert(nullptr);

	pTex = new CTexture;
	if (!pTex->Create(_pTex2D))
	{
		MessageBox(nullptr, _strKey.c_str(), L"텍스쳐 생성 실패", MB_OK);
		assert(nullptr);
	}

	pTex->SetName(_strKey);

	m_mapRes[(UINT)RES_TYPE::TEXTURE].insert(make_pair(_strKey, pTex.GetPointer()));

	return pTex;
}

Ptr<CTexture> CResMgr::CreateArrayTexture(const wstring & _strKey, const vector<Ptr<CTexture>>& _vecTex)
{
	Ptr<CTexture> pTex = new CTexture;
	if (!pTex->CreateArrayTexture(_vecTex))
	{
		MessageBox(nullptr, _strKey.c_str(), L"텍스쳐 생성 실패", MB_OK);
		assert(nullptr);
	}

	AddRes<CTexture>(_strKey, pTex);
	return pTex;
}

Ptr<CMeshData> CResMgr::LoadFBX(const wstring & _strPath)
{
	wstring strFileName = CPathMgr::GetFileName(_strPath.c_str());

	wstring strName = L"MeshData\\";
	strName += strFileName + L".mdat";

	Ptr<CMeshData> pMeshData = FindRes<CMeshData>(strName);

	if (nullptr != pMeshData)
		return pMeshData;

	pMeshData = CMeshData::LoadFromFBX(_strPath);
	pMeshData->SetName(strName);
	pMeshData->SetPath(strName);
	m_mapRes[(UINT)RES_TYPE::MESHDATA].insert(make_pair(strName, pMeshData.GetPointer()));

	return pMeshData;
}

FMOD_RESULT CHANNEL_CALLBACK(FMOD_CHANNELCONTROL *channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype
	, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype
	, void *commanddata1, void *commanddata2);

void CResMgr::InitSound()
{
	FMOD::System_Create(&CSound::g_pFMOD);

	if (nullptr == CSound::g_pFMOD)
	{
		assert(nullptr);
	}

	// 32개 채널 생성
	CSound::g_pFMOD->init(32, FMOD_DEFAULT, nullptr);
}
