#pragma once
#include "Component.h"

#include "Ptr.h"
#include "Material.h"
#include "Mesh.h"

class CStructuredBuffer;

class CParticleSystem :
	public CComponent
{
private:	
	CStructuredBuffer*	m_pParticleBuffer; // 파티클 데이터 저장
	CStructuredBuffer*	m_pSharedBuffer;   // 파티클 공유 메모리
	UINT				m_iMaxParticle;    // 최대 파티클 개수

	Ptr<CMaterial>		m_pUpdateMtrl;		// 컴퓨트 쉐이더
	Ptr<CMaterial>		m_pMtrl;			// 파티클 렌더링용
	Ptr<CMesh>			m_pMesh;			// 사용 메쉬(Rect Mesh 기본)

	float				m_fFrequency;		// 파티클 생성 주기
	float				m_fAccTime;		   // 누적시간 체크용도

	float				m_fMinLifeTime;   // 파티클 최소 생존 시간
	float				m_fMaxLifeTime;   // 파티클 최대 생존 시간

	float				m_fMinSpeed;	  // 파티클 시작 속도
	float				m_fMaxSpeed;	  // 파티클 최종 속도

	float				m_fStartScale;   // 파티클 시작 크기
	float				m_fEndScale;     // 파티클 최종 크기

	Vec4				m_vStartColor;
	Vec4				m_vEndColor;

	float				m_fStartColor_Red;
	float				m_fStartColor_Greed;
	float				m_fStartColor_Blue;
	float				m_fStartColor_Alpha;

	float				m_fEndColor_Red;
	float				m_fEndColor_Greed;
	float				m_fEndColor_Blue;
	float				m_fEndColor_Alpha;

	wstring				ParticleMtrlName;
	PARTICLE_TYPE		ParticleType;

public:
	bool				m_bWeatherParticleRendering;

	PARTICLE_TYPE	GetParticleType() { return ParticleType; }
	void	SetParticleType(PARTICLE_TYPE _ParticleTYPE) { ParticleType = _ParticleTYPE; }

	wstring		GetParticleName() { return ParticleMtrlName; }
	void	 SetParticleName(wstring _SetName) { ParticleMtrlName = _SetName; }

public:
	virtual void finalupdate();
	virtual void render();

public:
	void SetParticleMtrl(wstring* _ParticleName);

public:
	void SetMaxParticle(UINT SetValue) { m_iMaxParticle = SetValue; }
	UINT GetMaxParticle() { return m_iMaxParticle; }

	void SetStartColor_Red(float _SetColor) { m_fStartColor_Red = _SetColor; }
	float GetStartColor_Red() { return m_fStartColor_Red; }
	void SetStartColor_Green(float _SetColor) { m_fStartColor_Greed = _SetColor; }
	float GetStartColor_Green() { return m_fStartColor_Greed; }
	void SetStartColor_Blue(float _SetColor) { m_fStartColor_Blue = _SetColor; }
	float GetStartColor_Blue() { return m_fStartColor_Blue; }
	void SetStartColor_Alpha(float _SetColor) { m_fStartColor_Alpha = _SetColor; }
	float GetStartColor_Alpha() { return m_fStartColor_Alpha; }
	
	void SetEndColor_Red(float _SetColor) { m_fEndColor_Red = _SetColor; }
	float GetEndColor_Red() { return m_fEndColor_Red; }
	void SetEndColor_Green(float _SetColor) { m_fEndColor_Greed = _SetColor; }
	float GetEndColor_Green() { return m_fEndColor_Greed; }
	void SetEndColor_Blue(float _SetColor) { m_fEndColor_Blue = _SetColor; }
	float GetEndColor_Blue() { return m_fEndColor_Blue; }
	void SetEndColor_Alpha(float _SetColor) { m_fEndColor_Alpha = _SetColor; }
	float GetEndColor_Alpha() { return m_fEndColor_Alpha; }



	void SetStartColor(Vec4 _vColor);
	Vec4 GetStartColor() { return m_vStartColor; }

	void SetFrequency(float _Setvalue) { m_fFrequency = _Setvalue; }
	float GetFrequency() { return m_fFrequency; }

	void SetMinLifeTime(float _Setvalue) { m_fMinLifeTime = _Setvalue; }
	float GetMinLifeTime() { return m_fMinLifeTime; }

	void SetMaxLifeTime(float _Setvalue) { m_fMaxLifeTime = _Setvalue; }
	float GetMaxLifeTime() { return m_fMaxLifeTime; }

	void SetMinSpeed(float _Setvalue) { m_fMinSpeed = _Setvalue; }
	float GetMinSpeed() { return m_fMinSpeed; }

	void SetMaxSpeed(float _Setvalue) { m_fMaxSpeed = _Setvalue; }
	float GetMaxSpeed() { return m_fMaxSpeed; }

	void SetStartScale(float _Setvalue) { m_fStartScale = _Setvalue; }
	float GetStartScale() { return m_fStartScale; }

	void SetEndScale(float _Setvalue) { m_fEndScale = _Setvalue; }
	float GetEndScale() { return m_fEndScale; }

	void SetEndColor(Vec4 _Setvalue) { m_vEndColor = _Setvalue; }
	Vec4 GetEndColor() { return m_vEndColor; }

	void SetStartColor();
	void SetEndColor();
	void SortColor();

public:
	CLONE(CParticleSystem);
	virtual void SaveToScene(FILE* _pFile);
	virtual void LoadFromScene(FILE* _pFile);

public:
	CParticleSystem();
	virtual ~CParticleSystem();
};

