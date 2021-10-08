#pragma once
#include "Res.h"

#include "Ptr.h"
#include "Shader.h"
#include "Texture.h"

class CMaterial :
	public CResource
{
	// 06/05 M - 상민규
	// Bone 행렬 정보(m_tParam.m_arrMat[]) 를 받아오기 위해
	// Animator3D에 friend 설정
	friend class CAnimator3D;

private:
	Ptr<CShader>	m_pShader;
	tMtrlParam      m_tParam;
	Ptr<CTexture>   m_arrTex[(UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0];
	Ptr<CTexture>   m_arrRWTex[(UINT)SHADER_PARAM::RWTEX_END - (UINT)SHADER_PARAM::RWTEX_0];

	bool			m_bFileSave;

public:
	void SetShader(Ptr<CShader> _pShader);
	void SetData(SHADER_PARAM _eParam, void* _pData);
	void SetMaterialCoefficient(Vec4 _vDiff, Vec4 _vSpec, Vec4 _vEmissiv)
	{
		m_tParam.m_vDiff = _vDiff;
		m_tParam.m_vSpec = _vSpec;
		m_tParam.m_vEmv = _vEmissiv;
	}
	void DisableFileSave() { m_bFileSave = false; };
	void UpdateData(UINT iInstancing = 0);

	void Dispatch(UINT _iX, UINT _iY, UINT _iZ);

	virtual void Load(const wstring& _strFullPath);
	virtual void Save(const wstring& _strPath);

	Ptr<CShader> GetShader() { return m_pShader; }

	virtual CMaterial* Clone();



public:
	CMaterial();
	virtual ~CMaterial();
};

