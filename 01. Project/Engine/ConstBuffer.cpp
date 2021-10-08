#include "stdafx.h"
#include "ConstBuffer.h"

#include "Device.h"

CConstBuffer::CConstBuffer()
	: m_pBuffer(nullptr)
	, m_iRegisterNum(0)
	, m_iShader(0)
	, m_iSize(0)
	, m_iCurSize(0)
{
}

CConstBuffer::~CConstBuffer()
{
	SAFE_RELEASE(m_pBuffer);
	SAFE_DELETE(m_pSystem);
}

void CConstBuffer::Create(UINT _iByteWidth, UINT _iRegisterNum, const wstring & _strName)
{
	SetName(_strName);

	m_iSize = _iByteWidth;
	m_iRegisterNum = _iRegisterNum;

	// 상수버퍼 생성
	D3D11_BUFFER_DESC tBufferDesc = {};
	tBufferDesc.ByteWidth = m_iSize;

	tBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;	
	tBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	
	DEVICE->CreateBuffer(&tBufferDesc, nullptr, &m_pBuffer);

	m_pSystem = (BYTE*)malloc(_iByteWidth);
}

void CConstBuffer::SetData(const void * _pSrc)
{
	D3D11_MAPPED_SUBRESOURCE tSub = {};

	CONTEXT->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, _pSrc, m_iSize);
	CONTEXT->Unmap(m_pBuffer, 0);

	m_iCurSize += m_iSize;
}

void CConstBuffer::SetData(const void * _pSrc, UINT _iSize)
{	
	memcpy(m_pSystem + m_iCurSize, _pSrc, _iSize);
	m_iCurSize += (UINT)_iSize;
	
	assert(m_iCurSize <= m_iSize);

	if (m_iCurSize == m_iSize)
	{
		D3D11_MAPPED_SUBRESOURCE tSub = {};

		CONTEXT->Map(m_pBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
		memcpy(tSub.pData, m_pSystem, m_iCurSize);
		CONTEXT->Unmap(m_pBuffer, 0);

		m_iCurSize = 0;
	}
}

void CConstBuffer::UpdateData(UINT _iShaderType)
{
	if ((UINT)SHADER_TYPE::ST_VERTEX &  _iShaderType)
		CONTEXT->VSSetConstantBuffers(m_iRegisterNum, 1, &m_pBuffer);	
	if ((UINT)SHADER_TYPE::ST_HULL &  _iShaderType)
		CONTEXT->HSSetConstantBuffers(m_iRegisterNum, 1, &m_pBuffer);
	if ((UINT)SHADER_TYPE::ST_DOMAIN &  _iShaderType)
		CONTEXT->DSSetConstantBuffers(m_iRegisterNum, 1, &m_pBuffer);	
	if ((UINT)SHADER_TYPE::ST_GEOMETRY &  _iShaderType)
		CONTEXT->GSSetConstantBuffers(m_iRegisterNum, 1, &m_pBuffer);	
	if ((UINT)SHADER_TYPE::ST_COMPUTE &  _iShaderType)
		CONTEXT->CSSetConstantBuffers(m_iRegisterNum, 1, &m_pBuffer);	
	if ((UINT)SHADER_TYPE::ST_PIXEL &  _iShaderType)
		CONTEXT->PSSetConstantBuffers(m_iRegisterNum, 1, &m_pBuffer);
}