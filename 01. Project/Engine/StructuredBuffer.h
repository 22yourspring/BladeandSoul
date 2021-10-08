#pragma once
#include "Entity.h"
class CStructuredBuffer :
	public CEntity
{
private:
	ID3D11Buffer*				m_pBuffer;
	ID3D11ShaderResourceView *	m_pSRV;
	ID3D11UnorderedAccessView*	m_pUAV;

	UINT						m_iElementSize;	// 요소 크기
	UINT						m_iElementCount;// 요소 개수

public:
	UINT GetElementCount() { return m_iElementCount; }
	UINT GetElementSize() { return m_iElementSize; }
	UINT GetBufferSize() { return m_iElementCount * m_iElementSize; }

public:
	void Create(UINT _iElementSize, UINT _iElementCount, void* _pSysMem);

	void UpdateData(UINT _iRegister);
	void UpdateRWData(UINT _iRegister);

	void CopiedFromSource(CStructuredBuffer* _pSrcBuffer, UINT _iElementSize, UINT _iElementCount);

	static void ClearData(UINT _iRegister);
	static void ClearRWData(UINT _iRegister);

public:
	CStructuredBuffer();
	virtual ~CStructuredBuffer();
};

