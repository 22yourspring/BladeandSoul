#include "stdafx.h"
#include "ParticleSystem.h"

#include "StructuredBuffer.h"
#include "ResMgr.h"
#include "KeyMgr.h"
#include "TimeMgr.h"

#include "Transform.h"

CParticleSystem::CParticleSystem()
	: CComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_pParticleBuffer(nullptr)
	, m_pSharedBuffer(nullptr)
	, m_iMaxParticle(20000)
	, m_fFrequency(0.002f)
	, m_fAccTime(0.f)
	, m_fMinLifeTime(0.5f)
	, m_fMaxLifeTime(1.f)
	, m_fMinSpeed(100.f)
	, m_fMaxSpeed(50.f)
	, m_fStartScale(30.f)
	, m_fEndScale(10.f)
	, m_vStartColor(Vec4(0.7f, 0.7f, 0.7f, 1.4f))
	, m_vEndColor(Vec4(0.1f, 0.1f, 0.1f, 1.0f))
	, m_fStartColor_Red(0.7f)
	, m_fStartColor_Greed(0.7f)
	, m_fStartColor_Blue(0.7f)
	, m_fStartColor_Alpha(1.4f)
	, m_fEndColor_Red(0.1f)
	, m_fEndColor_Greed(0.1f)
	, m_fEndColor_Blue(0.1f)
	, m_fEndColor_Alpha(1.0f)
	, ParticleType(PARTICLE_TYPE::FLUTTER)
	, m_bWeatherParticleRendering(false)
{
	// 구조화 버퍼 생성
	m_pParticleBuffer = new CStructuredBuffer;	
	m_pParticleBuffer->Create(sizeof(tParticle), m_iMaxParticle, nullptr);

	m_pSharedBuffer = new CStructuredBuffer;	
	m_pSharedBuffer->Create(sizeof(tParticleShared), 1, nullptr);
	   	 
	// 사각형 Mesh
	m_pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh");

	// Material
	m_pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleMtrl");
	Ptr<CTexture> pParticle = CResMgr::GetInst()->Load<CTexture>(L"Texture\\Particle\\particle_00.png", L"Texture\\Particle\\particle_00.png");

	m_pMtrl->SetData(SHADER_PARAM::TEX_0, &pParticle);
	
	// Mtrl Name
	ParticleMtrlName = L"smokeparticle";

	// ParticleUpdate
	m_pUpdateMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleUpdateMtrl");	
}

CParticleSystem::~CParticleSystem()
{
	SAFE_DELETE(m_pParticleBuffer);
	SAFE_DELETE(m_pSharedBuffer);
}

void CParticleSystem::finalupdate()
{
	// 추가될 파티클 개수 계산
	m_fAccTime += DT;

	int iAdd = 0;
	if (m_fFrequency < m_fAccTime)
	{		
		m_fAccTime = m_fAccTime - m_fFrequency;
		iAdd = 1;
	}
	   	
	m_pParticleBuffer->UpdateRWData(6);
	m_pSharedBuffer->UpdateRWData(7);
	
	m_pUpdateMtrl->SetData(SHADER_PARAM::INT_0, &m_iMaxParticle);
	m_pUpdateMtrl->SetData(SHADER_PARAM::INT_1, &iAdd);
	m_pUpdateMtrl->SetData(SHADER_PARAM::FLOAT_0, &m_fMinLifeTime);
	m_pUpdateMtrl->SetData(SHADER_PARAM::FLOAT_1, &m_fMaxLifeTime);
	//
	m_pUpdateMtrl->SetData(SHADER_PARAM::INT_2, &ParticleType);
	//
	m_pUpdateMtrl->SetData(SHADER_PARAM::FLOAT_2, &m_fMinSpeed);
	m_pUpdateMtrl->SetData(SHADER_PARAM::FLOAT_3, &m_fMaxSpeed);
	
	m_pUpdateMtrl->Dispatch(1, 1, 1);

	m_pParticleBuffer->ClearRWData(6);
	m_pSharedBuffer->ClearRWData(7);
}

void CParticleSystem::render()
{
	if (m_bWeatherParticleRendering == false)
	{
		return;
	}
	Transform()->UpdateData();

	m_pParticleBuffer->UpdateData(20);
	SortColor();	// Start, End 넣기

	m_pMtrl->SetData(SHADER_PARAM::VEC4_0, &m_vStartColor);
	m_pMtrl->SetData(SHADER_PARAM::VEC4_1, &m_vEndColor);
	m_pMtrl->SetData(SHADER_PARAM::FLOAT_0, &m_fStartScale);
	m_pMtrl->SetData(SHADER_PARAM::FLOAT_1, &m_fEndScale);
	
	m_pMtrl->UpdateData();		
	m_pMesh->render_particle(m_iMaxParticle);	

	m_pParticleBuffer->ClearData(20);
}

void CParticleSystem::SetParticleMtrl(wstring * _ParticleName)
{
	if (*_ParticleName != ParticleMtrlName)
	{
		Ptr<CTexture> pParticle = CResMgr::GetInst()->Load<CTexture>(*_ParticleName, *_ParticleName);

		wstring Particlename = CPathMgr::GetFileName(_ParticleName->c_str());
		Particlename += L"ParticleMtrl";
		m_pMtrl = CResMgr::GetInst()->FindRes<CMaterial>(Particlename);

		if (m_pMtrl == nullptr)
		{
			m_pMtrl = new CMaterial;
			m_pMtrl->DisableFileSave();
			m_pMtrl->SetShader(CResMgr::GetInst()->FindRes<CShader>(L"ParticleShader"));
			CResMgr::GetInst()->AddRes(Particlename, m_pMtrl);
		}

		m_pMtrl->SetData(SHADER_PARAM::TEX_0, &pParticle);
	}
	ParticleMtrlName = *_ParticleName;
}

void CParticleSystem::SetStartColor(Vec4 _vColor)
{
	m_vStartColor = _vColor;

	m_fStartColor_Red = _vColor.x;
	m_fStartColor_Greed = _vColor.y;
	m_fStartColor_Blue = _vColor.z;
	m_fStartColor_Alpha = _vColor.w;
}

void CParticleSystem::SetStartColor()
{
	m_vStartColor.x = m_fStartColor_Red;
	m_vStartColor.y = m_fStartColor_Greed;
	m_vStartColor.z = m_fStartColor_Blue;
	m_vStartColor.w = m_fStartColor_Alpha;
}

void CParticleSystem::SetEndColor()
{
	m_vEndColor.x = m_fEndColor_Red;
	m_vEndColor.y = m_fEndColor_Greed;
	m_vEndColor.z = m_fEndColor_Blue;
	m_vEndColor.w = m_fEndColor_Alpha;
}

void CParticleSystem::SortColor()
{
	SetStartColor();
	SetEndColor();
}

void CParticleSystem::SaveToScene(FILE * _pFile)
{
}

void CParticleSystem::LoadFromScene(FILE * _pFile)
{
}
