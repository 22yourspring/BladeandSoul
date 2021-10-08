#pragma once
#include "Entity.h"


class CConstBuffer :
	public CEntity
{
private:
	ID3D11Buffer*		m_pBuffer;
	UINT				m_iRegisterNum;
	UINT				m_iShader;
	UINT				m_iSize;

	UINT				m_iCurSize;
	BYTE*				m_pSystem;

public:
	void Create(UINT _iByteWidth, UINT _iRegisterNum, const wstring& _strName = L"");
	void SetData(const void* _pSrc);
	void SetData(const void* _pSrc, UINT _iSize);

	void UpdateData(UINT _iShaderType);
	ID3D11Buffer* GetBuffer() { return m_pBuffer; }
	ID3D11Buffer** GetBufferPointer() { return &m_pBuffer; }
public:
	CConstBuffer();
	virtual ~CConstBuffer();
};

