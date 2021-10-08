#pragma once
#include "Entity.h"

class CDepthStencilState :
	public CEntity
{
private:
	ID3D11DepthStencilState*	m_pState;
	D3D11_DEPTH_STENCIL_DESC	m_tDesc;

public:
	void Create(const D3D11_DEPTH_STENCIL_DESC& _tDesc);
	void UpdateData(UINT _iRef = 0);

public:
	CDepthStencilState();
	virtual ~CDepthStencilState();
};

