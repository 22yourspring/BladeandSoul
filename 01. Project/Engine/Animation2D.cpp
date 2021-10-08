#include "stdafx.h"
#include "Animation2D.h"

#include "Device.h"
#include "ConstBuffer.h"
#include "TimeMgr.h"

#include "ResMgr.h"
#include "Texture.h"

CAnimation2D::CAnimation2D()
	: m_pAnimator(nullptr)
	, m_iCurFrm(0)
	, m_fAccTime(0.f)
	, m_bFinish(false)
{
}

CAnimation2D::~CAnimation2D()
{
}

void CAnimation2D::finalupdate()
{
	if (m_bFinish)
		return;

	m_fAccTime += DT;

	if (m_vecFrm[m_iCurFrm].fDuration < m_fAccTime)
	{
		m_iCurFrm += 1;
		m_fAccTime = 0.f;

		if (m_vecFrm.size() <= m_iCurFrm)
		{
			m_iCurFrm = (int)m_vecFrm.size() - 1;
			m_bFinish = true;
		}
	}
}

void CAnimation2D::UpdateData()
{
	static CConstBuffer* pAnimBuffer = CDevice::GetInst()->FindConstBuffer(L"Anim2D");

	m_vecFrm[m_iCurFrm].pTex->UpdateData((UINT)SHADER_TYPE::ST_PIXEL, 17);

	static tAnim2D data = {};
	data.IsAnim2D = true;
	data.vLen = m_vecFrm[m_iCurFrm].vLen;
	data.vLT = m_vecFrm[m_iCurFrm].vLT;
	data.vOffset = m_vecFrm[m_iCurFrm].vOffsetPos;

	pAnimBuffer->SetData(&data);
	pAnimBuffer->UpdateData((UINT)SHADER_TYPE::ST_VERTEX | (UINT)SHADER_TYPE::ST_PIXEL);
}

void CAnimation2D::Create(Ptr<CTexture> _pTex, Vec2 _vLT, Vec2 _vGap, int _iFrmCount, float _fDuration)
{
	for (int i = 0; i < _iFrmCount; ++i)
	{
		tAnim2DFrm frm = {};

		Vec2 vLT = _vLT;
		vLT.x += _vGap.x * i;

		frm.pTex = _pTex;
		frm.vLT = Vec2(vLT.x / _pTex->Width(), vLT.y / _pTex->Height());
		frm.vLen = Vec2(_vGap.x / _pTex->Width(), _vGap.y / _pTex->Height());
		frm.fDuration = _fDuration;
		
		m_vecFrm.push_back(frm);
	}
}

void CAnimation2D::SaveToScene(FILE * _pFile)
{	
	UINT iFrmCount = (UINT) m_vecFrm.size();

	fwrite(&iFrmCount, sizeof(UINT), 1, _pFile);
	for (size_t i = 0; i < m_vecFrm.size(); ++i)
	{
		fwrite(&m_vecFrm[i], sizeof(tAnim2DFrm), 1, _pFile);

		if (nullptr != m_vecFrm[i].pTex)
		{
			SaveWString(_pFile, m_vecFrm[i].pTex->GetName());
			SaveWString(_pFile, m_vecFrm[i].pTex->GetPath());
		}
	}

	fwrite(&m_iCurFrm, 4, 1, _pFile);	
}

void CAnimation2D::LoadFromScene(FILE * _pFile)
{
	UINT iFrmCount = 0;
	fread(&iFrmCount, sizeof(UINT), 1, _pFile);

	for (UINT i = 0; i < iFrmCount; ++i)
	{
		tAnim2DFrm tFrm = {};
		fread(&tFrm, sizeof(tAnim2DFrm), 1, _pFile);

		if (nullptr != tFrm.pTex)
		{
			wstring strTexName = LoadWString(_pFile);
			wstring strTexPath = LoadWString(_pFile);

			memset(&tFrm.pTex, 0, sizeof(void*));
			tFrm.pTex = CResMgr::GetInst()->FindRes<CTexture>(strTexName);

			if (nullptr == tFrm.pTex)
			{
				tFrm.pTex = CResMgr::GetInst()->Load<CTexture>(strTexName, strTexPath);
			}			
		}

		m_vecFrm.push_back(tFrm);
	}

	fread(&m_iCurFrm, 4, 1, _pFile);
}
