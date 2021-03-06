#pragma once
#include "Entity.h"


class CBlendState :
	public CEntity
{
private:
	ID3D11BlendState*		m_pState;
	D3D11_BLEND_DESC        m_tDesc;

public:
	void Create(const D3D11_BLEND_DESC& _tDesc);
	void UpdateData();

public:
	CBlendState();
	virtual ~CBlendState();
};

