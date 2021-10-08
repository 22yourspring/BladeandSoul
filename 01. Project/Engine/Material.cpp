#include "stdafx.h"
#include "Material.h"

#include "Device.h"
#include "ConstBuffer.h"
#include "ResMgr.h"


#include "Core.h"

CMaterial::CMaterial()
	: CResource(RES_TYPE::MATERIAL)
	, m_bFileSave(true)
{
	m_tParam.m_vDiff = Vec4(1.f, 1.f, 1.f, 1.f);
	m_tParam.m_vSpec = Vec4(0.2f, 0.2f, 0.2f, 1.f);
	m_tParam.m_vEmv = Vec4(1.f, 1.f, 1.f, 1.f);
}

CMaterial::~CMaterial()
{
}

void CMaterial::SetShader(Ptr<CShader> _pShader)
{
	m_pShader = _pShader;
}

void CMaterial::SetData(SHADER_PARAM _eParam, void * _pData)
{
	switch (_eParam)
	{
	case SHADER_PARAM::INT_0:
	case SHADER_PARAM::INT_1:
	case SHADER_PARAM::INT_2:
	case SHADER_PARAM::INT_3:
		m_tParam.m_arrInt[(UINT)_eParam - (UINT)SHADER_PARAM::INT_0] = *((int*)_pData);
		break;
	case SHADER_PARAM::FLOAT_0:
	case SHADER_PARAM::FLOAT_1:
	case SHADER_PARAM::FLOAT_2:
	case SHADER_PARAM::FLOAT_3:
		m_tParam.m_arrFloat[(UINT)_eParam - (UINT)SHADER_PARAM::FLOAT_0] = *((float*)_pData);
		break;
	case SHADER_PARAM::VEC2_0:
	case SHADER_PARAM::VEC2_1:
	case SHADER_PARAM::VEC2_2:
	case SHADER_PARAM::VEC2_3:
		m_tParam.m_arrVec2[(UINT)_eParam - (UINT)SHADER_PARAM::VEC2_0] = *((Vec2*)_pData);
		break;
	case SHADER_PARAM::VEC4_0:
	case SHADER_PARAM::VEC4_1:
	case SHADER_PARAM::VEC4_2:
	case SHADER_PARAM::VEC4_3:
		m_tParam.m_arrVec4[(UINT)_eParam - (UINT)SHADER_PARAM::VEC4_0] = *((Vec4*)_pData);
		break;
	case SHADER_PARAM::MATRIX_0:
	case SHADER_PARAM::MATRIX_1:
	case SHADER_PARAM::MATRIX_2:
	case SHADER_PARAM::MATRIX_3:

		m_tParam.m_arrMat[(UINT)_eParam - (UINT)SHADER_PARAM::MATRIX_0] = *((Matrix*)_pData);
		break;

	case SHADER_PARAM::TEX_0:
	case SHADER_PARAM::TEX_1:
	case SHADER_PARAM::TEX_2:
	case SHADER_PARAM::TEX_3:
	case SHADER_PARAM::TEX_4:
	case SHADER_PARAM::TEX_5:
	case SHADER_PARAM::TEX_6:
	case SHADER_PARAM::TEX_7:
	case SHADER_PARAM::TEXARR_0:
	case SHADER_PARAM::TEXARR_1:
	case SHADER_PARAM::TEXARR_2:
	case SHADER_PARAM::TEXARR_3:
		m_arrTex[(UINT)_eParam - (UINT)SHADER_PARAM::TEX_0] = *((Ptr<CTexture>*)_pData);
		break;
	case SHADER_PARAM::RWTEX_0:
	case SHADER_PARAM::RWTEX_1:
	case SHADER_PARAM::RWTEX_2:
	case SHADER_PARAM::RWTEX_3:
		m_arrRWTex[(UINT)_eParam - (UINT)SHADER_PARAM::RWTEX_0] = *((Ptr<CTexture>*)_pData);
		break;
	default:
		break;
	}

	//if (SCENE_MOD::SCENE_STOP == CCore::GetInst()->GetSceneMod())
	//{
	//	Save(GetPath());
	//}
}

void CMaterial::UpdateData(UINT _iInstancing)
{
	// Texture Register Update
	for (UINT i = 0; i < (UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0; ++i)
	{
		if (nullptr != m_arrTex[i])
		{
			m_arrTex[i]->UpdateData(ST_ALL, i);
			m_tParam.m_iArrTex[i] = 1;
		}
		else
		{
			CTexture::ClearData(ST_ALL, i);
			m_tParam.m_iArrTex[i] = 0;
		}
	}

	// RWTexture Register Update
	for (UINT i = 0; i < (UINT)SHADER_PARAM::RWTEX_END - (UINT)SHADER_PARAM::RWTEX_0; ++i)
	{
		if (nullptr != m_arrRWTex[i])
		{
			m_arrRWTex[i]->UpdateRWData(i);
			m_tParam.m_iArrRWTex[i] = 1;
		}
		else
		{
			CTexture::ClearRWData(i);
			m_tParam.m_iArrRWTex[i] = 0;
		}
	}

	static CConstBuffer* pCB = CDevice::GetInst()->FindConstBuffer(L"Material Parameter");

	pCB->SetData(&m_tParam);
	pCB->UpdateData(ST_ALL);

	if (nullptr != m_pShader)
	{
		m_pShader->UpdateData(_iInstancing);
	}
}

void CMaterial::Dispatch(UINT _iX, UINT _iY, UINT _iZ)
{
	UpdateData();
	CONTEXT->Dispatch(_iX, _iY, _iZ);

	CTexture::ClearAllData();
	CTexture::ClearAllRWData();
}

void CMaterial::Load(const wstring & _strFullPath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _strFullPath.c_str(), L"rb");

	// 쉐이더 키값
	wstring strName;
	strName = LoadWString(pFile);
	m_pShader = CResMgr::GetInst()->FindRes<CShader>(strName);

	// 쉐이더 파라미터
	fread(&m_tParam, sizeof(tMtrlParam), 1, pFile);

	UINT iMaxCount = (UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0;
	for (UINT i = 0; i < iMaxCount; ++i)
	{
		UINT iExist = 0;
		fread(&iExist, 4, 1, pFile);

		if (iExist)
		{
			wstring strKey = LoadWString(pFile);
			wstring strPath = LoadWString(pFile);

			m_arrTex[i] = CResMgr::GetInst()->FindRes<CTexture>(strKey);

			// FullPath 인지 아닌지 구분 
			wchar_t zero = strPath[0];
			wchar_t first = strPath[1];

			bool bFullPath = false;

			if (L'D' == zero && L':' == first)
			{
				bFullPath = true;
			}

			if (nullptr == m_arrTex[i])
			{
				m_arrTex[i] = CResMgr::GetInst()->Load<CTexture>(strKey, strPath, bFullPath);
			}
		}
	}

	fclose(pFile);
}

void CMaterial::Save(const wstring & _strPath)
{
	if (!m_bFileSave)
		return;

	wstring strFilePath = CPathMgr::GetResPath();
	strFilePath += _strPath;
	SetPath(_strPath);

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, strFilePath.c_str(), L"wb");

	// 쉐이더 키값
	if (m_pShader != nullptr)
		SaveWString(pFile, m_pShader->GetName());

	// 쉐이더 파라미터
	fwrite(&m_tParam, sizeof(tMtrlParam), 1, pFile);

	UINT iMaxCount = (UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0;
	for (UINT i = 0; i < iMaxCount; ++i)
	{
		UINT iExist = 1;
		if (nullptr == m_arrTex[i])
		{
			iExist = 0;
			fwrite(&iExist, 4, 1, pFile);
			continue;
		}

		fwrite(&iExist, 4, 1, pFile);
		SaveWString(pFile, m_arrTex[i]->GetName());
		SaveWString(pFile, m_arrTex[i]->GetPath());
	}

	fclose(pFile);
}

CMaterial * CMaterial::Clone()
{
	CResource::Clone();

	CMaterial* pClone = new CMaterial(*this);

	wchar_t szAdd[50] = {};
	wsprintf(szAdd, L"_Clone_%d.mtrl", GetCloneCount());

	// Naming
	wstring strPath = L"Material\\";
	strPath += CPathMgr::GetFileName(GetName().c_str());
	strPath += szAdd;

	pClone->SetName(strPath);
	pClone->SetPath(strPath);

	CResMgr::GetInst()->AddCloneRes<CMaterial>(pClone);

	if (SCENE_MOD::SCENE_STOP == CCore::GetInst()->GetSceneMod())
	{
		pClone->m_bFileSave = true;
		pClone->Save(pClone->GetPath());
	}

	return pClone;
}
