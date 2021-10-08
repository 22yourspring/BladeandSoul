#include "stdafx.h"
#include "InstancingBuffer.h"

#include "Device.h"
#include "StructuredBuffer.h"

CInstancingBuffer::CInstancingBuffer()
	: m_iMaxCount(10)
	, m_iAnimInstCount(0)
	, m_pBoneBuffer(nullptr)
{
	m_pBoneBuffer = new CStructuredBuffer;
}

CInstancingBuffer::~CInstancingBuffer()
{
	SAFE_RELEASE(m_pInstancingBuffer);
	SAFE_DELETE(m_pBoneBuffer);
}

void CInstancingBuffer::init()
{
	D3D11_BUFFER_DESC tDesc = {};

	tDesc.ByteWidth = sizeof(tInstancingData) * m_iMaxCount;
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tDesc.Usage = D3D11_USAGE_DYNAMIC;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, NULL, &m_pInstancingBuffer)))
		assert(NULL);
}

void CInstancingBuffer::AddInstancingBoneMat(CStructuredBuffer* _pBuffer)
{
	++m_iAnimInstCount;
	m_vecBoneMat.push_back(_pBuffer);
}

void CInstancingBuffer::SetData()
{
	if (m_vecData.size() > m_iMaxCount)
	{
		Resize(m_vecData.size());
	}	

	D3D11_MAPPED_SUBRESOURCE tMap = {};

	CONTEXT->Map(m_pInstancingBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tMap);
	memcpy(tMap.pData, &m_vecData[0], sizeof(tInstancingData) * m_vecData.size());
	CONTEXT->Unmap(m_pInstancingBuffer, 0);

	if (0 == m_iAnimInstCount)
		return;
		
	size_t iBufferSize = m_vecBoneMat[0]->GetBufferSize() * m_vecBoneMat.size();
	if (m_pBoneBuffer->GetBufferSize() < iBufferSize)
	{
		m_pBoneBuffer->Create(m_vecBoneMat[0]->GetElementSize(), m_vecBoneMat[0]->GetElementCount() * m_vecBoneMat.size(), nullptr);
	}
	
	// 메모리 복사
	// StructureBuffer <- m_vecBoneMat;


}

void CInstancingBuffer::Resize(UINT _iCount)
{
	SAFE_RELEASE(m_pInstancingBuffer);

	m_iMaxCount = _iCount;

	D3D11_BUFFER_DESC tDesc = {};

	tDesc.ByteWidth = sizeof(tInstancingData) * m_iMaxCount;
	tDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tDesc.Usage = D3D11_USAGE_DYNAMIC;
	tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	if (FAILED(DEVICE->CreateBuffer(&tDesc, NULL, &m_pInstancingBuffer)))
		assert(NULL);
}
