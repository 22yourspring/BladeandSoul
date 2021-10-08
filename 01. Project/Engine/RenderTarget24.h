#pragma once
#include "Entity.h"

#include "Ptr.h"
#include "Texture.h"

class CRenderTarget24 :
	public CEntity
{
private:
	Ptr<CTexture>		m_pTargetTex;
	float				m_ClearColor[4];
	
public:
	void Create(Ptr<CTexture> _pTargetTex, Vec4 _vClearColor);

	void SetClearColor(const Vec4& _vColor) { memcpy_s(m_ClearColor, sizeof(float) * 4, &_vColor, sizeof(float)*4); }
	void Clear();

	Ptr<CTexture>	GetRTTex() { return m_pTargetTex; }

public:
	CRenderTarget24();
	virtual ~CRenderTarget24();
};

