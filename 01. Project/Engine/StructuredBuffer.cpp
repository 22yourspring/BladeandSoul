#include "stdafx.h"
#include "StructuredBuffer.h"

#include "Device.h"

CStructuredBuffer::CStructuredBuffer()
	: m_pBuffer(nullptr)
	, m_pSRV(nullptr)
	, m_pUAV(nullptr)
	, m_iElementSize(0)
	, m_iElementCount(0)
{
}

CStructuredBuffer::~CStructuredBuffer()
{
	SAFE_RELEASE(m_pBuffer);
	SAFE_RELEASE(m_pSRV);
	SAFE_RELEASE(m_pUAV);
}

void CStructuredBuffer::Create(UINT _iElementSize, UINT _iElementCount, void* _pSysmem)
{
	SAFE_RELEASE(m_pBuffer);
	SAFE_RELEASE(m_pSRV);
	SAFE_RELEASE(m_pUAV);

	m_iElementSize = _iElementSize;
	m_iElementCount = _iElementCount;
	
	// RW滚欺 积己
	D3D11_BUFFER_DESC tBufferDesc = {};
	tBufferDesc.ByteWidth = m_iElementSize * m_iElementCount;

	tBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	tBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	tBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	tBufferDesc.StructureByteStride = m_iElementSize;

	D3D11_SUBRESOURCE_DATA tSub = {};
	tSub.pSysMem = _pSysmem;		

	if(nullptr == _pSysmem)
		DEVICE->CreateBuffer(&tBufferDesc, nullptr, &m_pBuffer);
	else
		DEVICE->CreateBuffer(&tBufferDesc, &tSub, &m_pBuffer);
	
	// View 积己
	D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
	tSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	tSRVDesc.BufferEx.FirstElement = 0;
	tSRVDesc.BufferEx.Flags = 0;
	tSRVDesc.BufferEx.NumElements = m_iElementCount;
	DEVICE->CreateShaderResourceView(m_pBuffer, &tSRVDesc, &m_pSRV);

	D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc = {};
	tUAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	tUAVDesc.Buffer.FirstElement = 0;
	tUAVDesc.Buffer.Flags = 0;
	tUAVDesc.Buffer.NumElements = m_iElementCount;
	DEVICE->CreateUnorderedAccessView(m_pBuffer, &tUAVDesc, &m_pUAV);
}

void CStructuredBuffer::UpdateData(UINT _iRegister)
{
	CONTEXT->VSSetShaderResources(_iRegister, 1, &m_pSRV);
	CONTEXT->HSSetShaderResources(_iRegister, 1, &m_pSRV);
	CONTEXT->DSSetShaderResources(_iRegister, 1, &m_pSRV);
	CONTEXT->GSSetShaderResources(_iRegister, 1, &m_pSRV);
	CONTEXT->CSSetShaderResources(_iRegister, 1, &m_pSRV);
	CONTEXT->PSSetShaderResources(_iRegister, 1, &m_pSRV);
}

void CStructuredBuffer::UpdateRWData(UINT _iRegister)
{
	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(_iRegister, 1, &m_pUAV, &i);
}

void CStructuredBuffer::CopiedFromSource(CStructuredBuffer * _pSrcBuffer, UINT _iElementSize, UINT _iElementCount)
{	
	// CONTEXT->CopySubresourceRegion(m_pBuffer, 0, );
	
	_pSrcBuffer->m_pBuffer;
}

void CStructuredBuffer::ClearData(UINT _iRegister)
{
	ID3D11ShaderResourceView* pSRV = nullptr;
	CONTEXT->VSSetShaderResources(_iRegister, 1, &pSRV);
	CONTEXT->HSSetShaderResources(_iRegister, 1, &pSRV);
	CONTEXT->DSSetShaderResources(_iRegister, 1, &pSRV);
	CONTEXT->GSSetShaderResources(_iRegister, 1, &pSRV);
	CONTEXT->CSSetShaderResources(_iRegister, 1, &pSRV);
	CONTEXT->PSSetShaderResources(_iRegister, 1, &pSRV);
}

void CStructuredBuffer::ClearRWData(UINT _iRegister)
{
	ID3D11UnorderedAccessView* pUAV = nullptr;
	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(_iRegister, 1, &pUAV, &i);
}
