#pragma once
#include "Entity.h"

#include "Ptr.h"
#include "Texture.h"

class CRenderTarget24;

class CMRT :
	public CEntity
{
public:
	CRenderTarget24*		m_arrRT[8];	
	UINT					m_iCount;
	Ptr<CTexture>			m_pDepthStencilTex;

	D3D11_VIEWPORT			m_tViewPortDesc; // ����Ÿ�� ������� ����Ʈ ����� ���߾� �ش�.

public:
	void Create(UINT _iCount, CRenderTarget24** _pRT, Ptr<CTexture> _pDSTex);

	void OMSet();
	void Clear();

public:
	CMRT();
	virtual ~CMRT();
};

