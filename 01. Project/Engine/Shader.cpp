#include "stdafx.h"
#include "Shader.h"

#include "Device.h"
#include "PathMgr.h"

#include "BlendState.h"
#include "DepthStencilState.h"

#include "RenderMgr.h"

vector<D3D11_INPUT_ELEMENT_DESC> CShader::g_vecLayout;
int CShader::g_iOffset = 0;
int CShader::g_iOffsetInst = 0;


void CShader::AddInputLayout(const char * _pSemanticName
	, UINT _iSemanticIdx, DXGI_FORMAT _eFormat
	, UINT _iInputSlot, UINT _iInstancing, D3D11_INPUT_CLASSIFICATION _eClass)
{
	D3D11_INPUT_ELEMENT_DESC tDesc = {};

	tDesc.SemanticName = _pSemanticName;
	tDesc.SemanticIndex = _iSemanticIdx; // 같은 종류의 시멘틱에 대해서 증가하는 인덱스
	tDesc.Format = _eFormat;
	tDesc.InputSlot = _iInputSlot; // 0 ~ 15 사이의 값
	tDesc.InputSlotClass = _eClass;
	tDesc.InstanceDataStepRate = _iInstancing; // 인스턴싱

	if (0 == _iInputSlot)
	{
		tDesc.AlignedByteOffset = g_iOffset;
		g_iOffset += GetSizeofFormat(_eFormat);
	}
	else
	{
		tDesc.AlignedByteOffset = g_iOffsetInst;
		g_iOffsetInst += GetSizeofFormat(_eFormat);
	}

	g_vecLayout.push_back(tDesc);
}


void CShader::CreateLayoutDesc()
{
	// Layout 정보 만들기	
	AddInputLayout("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0);
	AddInputLayout("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
	AddInputLayout("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0);
	AddInputLayout("TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0);
	AddInputLayout("NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0);	
	AddInputLayout("BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0);
	AddInputLayout("BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);
	AddInputLayout("BLENDINDICES", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);

	/*인스턴싱 데이터는 슬롯이 다르다*/
	AddInputLayout("WORLD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	AddInputLayout("WORLD", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	AddInputLayout("WORLD", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	AddInputLayout("WORLD", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);

	AddInputLayout("WV", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	AddInputLayout("WV", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	AddInputLayout("WV", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	AddInputLayout("WV", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);

	AddInputLayout("WVP", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	AddInputLayout("WVP", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	AddInputLayout("WVP", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	AddInputLayout("WVP", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
	AddInputLayout("ROWINDEX", 0, DXGI_FORMAT_R32_SINT, 1, 1, D3D11_INPUT_PER_INSTANCE_DATA);
}

CShader::CShader()
	: CResource(RES_TYPE::SHADER)
	, m_pVS(nullptr)
	, m_pVSInst(nullptr)
	, m_pHS(nullptr)
	, m_pDS(nullptr)
	, m_pGS(nullptr)
	, m_pPS(nullptr)
	, m_pVSBlob(nullptr)
	, m_pVSInstBlob(nullptr)
	, m_pHSBlob(nullptr)
	, m_pDSBlob(nullptr)
	, m_pGSBlob(nullptr)
	, m_pPSBlob(nullptr)
	, m_pErrBlob(nullptr)
	, m_pCS(nullptr)
	, m_pCSBlob(nullptr)
	, m_pLayout(nullptr)
	, m_pBlendState(nullptr)
	, m_pDepthStencilState(nullptr)
	, m_eRSType(RS_TYPE::CULL_BACK)
	, m_ePOV(SHADER_POV::NONE_RENDER)
	, m_eTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
}

CShader::~CShader()
{
	SAFE_RELEASE(m_pVS);
	SAFE_RELEASE(m_pVSInst);
	SAFE_RELEASE(m_pHS);
	SAFE_RELEASE(m_pDS);
	SAFE_RELEASE(m_pGS);
	SAFE_RELEASE(m_pPS);
	SAFE_RELEASE(m_pVSBlob);
	SAFE_RELEASE(m_pVSInstBlob);
	SAFE_RELEASE(m_pHSBlob);
	SAFE_RELEASE(m_pDSBlob);
	SAFE_RELEASE(m_pGSBlob);
	SAFE_RELEASE(m_pPSBlob);
	SAFE_RELEASE(m_pErrBlob);
	SAFE_RELEASE(m_pCS);
	SAFE_RELEASE(m_pCSBlob);
	SAFE_RELEASE(m_pLayout);
}

void CShader::CreateVertexShader(const wstring & _strPath, const string & _strFuncName, const string & _strhlslVersion)
{
	int iFlag = 0;
#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	wstring strPath = CPathMgr::GetResPath();
	strPath += _strPath;	
	
	char* pErr = nullptr;

	if (FAILED(D3DCompileFromFile(strPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), _strhlslVersion.c_str(), iFlag, 0, &m_pVSBlob, &m_pErrBlob)))
	{
		pErr = (char*)m_pErrBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErr, "Shader Create Failed !!!", MB_OK);
	}

	DEVICE->CreateVertexShader(m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(), nullptr, &m_pVS);

	if (FAILED(DEVICE->CreateInputLayout(&g_vecLayout[0], (UINT)g_vecLayout.size(), m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(), &m_pLayout)))
		assert(nullptr);
}

void CShader::CreateVertexInstShader(const wstring & _strPath, const string & _strFuncName, const string & _strhlslVersion)
{
	int iFlag = 0;
#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	wstring strPath = CPathMgr::GetResPath();
	strPath += _strPath;

	char* pErr = nullptr;

	if (FAILED(D3DCompileFromFile(strPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), _strhlslVersion.c_str(), iFlag, 0, &m_pVSInstBlob, &m_pErrBlob)))
	{
		pErr = (char*)m_pErrBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErr, "Shader Create Failed !!!", MB_OK);
	}

	DEVICE->CreateVertexShader(m_pVSInstBlob->GetBufferPointer(), m_pVSInstBlob->GetBufferSize(), nullptr, &m_pVSInst);

	if (nullptr != m_pLayout)
		SAFE_RELEASE(m_pLayout);

	if (FAILED(DEVICE->CreateInputLayout(&g_vecLayout[0], (UINT)g_vecLayout.size(), m_pVSInstBlob->GetBufferPointer(), m_pVSInstBlob->GetBufferSize(), &m_pLayout)))
		assert(nullptr);
}

void CShader::CreateHullShader(const wstring & _strPath, const string & _strFuncName, const string & _strhlslVersion)
{
	int iFlag = 0;
#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	wstring strPath = CPathMgr::GetResPath();
	strPath += _strPath;

	char* pErr = nullptr;

	if (FAILED(D3DCompileFromFile(strPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), _strhlslVersion.c_str(), iFlag, 0, &m_pHSBlob, &m_pErrBlob)))
	{
		pErr = (char*)m_pErrBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErr, "Shader Create Failed !!!", MB_OK);
	}

	DEVICE->CreateHullShader(m_pHSBlob->GetBufferPointer(), m_pHSBlob->GetBufferSize(), nullptr, &m_pHS);
}

void CShader::CreateDomainShader(const wstring & _strPath, const string & _strFuncName, const string & _strhlslVersion)
{
	int iFlag = 0;
#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	wstring strPath = CPathMgr::GetResPath();
	strPath += _strPath;

	char* pErr = nullptr;

	if (FAILED(D3DCompileFromFile(strPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), _strhlslVersion.c_str(), iFlag, 0, &m_pDSBlob, &m_pErrBlob)))
	{
		pErr = (char*)m_pErrBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErr, "Shader Create Failed !!!", MB_OK);
	}

	DEVICE->CreateDomainShader(m_pDSBlob->GetBufferPointer(), m_pDSBlob->GetBufferSize(), nullptr, &m_pDS);
}

void CShader::CreateGeometryShader(const wstring & _strPath, const string & _strFuncName, const string & _strhlslVersion)
{
	int iFlag = 0;
#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	wstring strPath = CPathMgr::GetResPath();
	strPath += _strPath;

	char* pErr = nullptr;

	if (FAILED(D3DCompileFromFile(strPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), _strhlslVersion.c_str(), iFlag, 0, &m_pGSBlob, &m_pErrBlob)))
	{
		pErr = (char*)m_pErrBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErr, "Shader Create Failed !!!", MB_OK);
	}

	DEVICE->CreateGeometryShader(m_pGSBlob->GetBufferPointer(), m_pGSBlob->GetBufferSize(), nullptr, &m_pGS);
}

void CShader::CreateComputeShader(const wstring & _strPath, const string & _strFuncName, const string & _strhlslVersion)
{
	int iFlag = 0;
#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	wstring strPath = CPathMgr::GetResPath();
	strPath += _strPath;

	char* pErr = nullptr;

	if (FAILED(D3DCompileFromFile(strPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), _strhlslVersion.c_str(), iFlag, 0, &m_pCSBlob, &m_pErrBlob)))
	{
		pErr = (char*)m_pErrBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErr, "Shader Create Failed !!!", MB_OK);
	}

	auto* pTemp = m_pCSBlob->GetBufferPointer();
	auto BSize = m_pCSBlob->GetBufferSize();

	DEVICE->CreateComputeShader(pTemp, BSize, nullptr, &m_pCS);
}

void CShader::CreatePixelShader(const wstring & _strPath, const string & _strFuncName, const string & _strhlslVersion)
{
	int iFlag = 0;
#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	wstring strPath = CPathMgr::GetResPath();
	strPath += _strPath;

	char* pErr = nullptr;

	if (FAILED(D3DCompileFromFile(strPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), _strhlslVersion.c_str(), iFlag, 0, &m_pPSBlob, &m_pErrBlob)))
	{
		pErr = (char*)m_pErrBlob->GetBufferPointer();
		MessageBoxA(nullptr, pErr, "Shader Create Failed !!!", MB_OK);
	}

	DEVICE->CreatePixelShader(m_pPSBlob->GetBufferPointer(), m_pPSBlob->GetBufferSize(), nullptr, &m_pPS);
}

void CShader::UpdateData(UINT _iInstancing)
{
	CONTEXT->IASetPrimitiveTopology(m_eTopology);

	if (0 == _iInstancing || nullptr == m_pVSInst)
		CONTEXT->VSSetShader(m_pVS, nullptr, 0);
	else
		CONTEXT->VSSetShader(m_pVSInst, nullptr, 0); 
	
	CONTEXT->HSSetShader(m_pHS, nullptr, 0);
	CONTEXT->DSSetShader(m_pDS, nullptr, 0);
	CONTEXT->GSSetShader(m_pGS, nullptr, 0);
	CONTEXT->CSSetShader(m_pCS, nullptr, 0);
	CONTEXT->PSSetShader(m_pPS, nullptr, 0);

	CONTEXT->IASetInputLayout(m_pLayout);

	// BlendState 설정
	if (nullptr == m_pBlendState)
		CONTEXT->OMSetBlendState(nullptr, nullptr, 0xffffffff);
	else
		m_pBlendState->UpdateData();
	
	// DepthStencilState 설정
	if (nullptr == m_pDepthStencilState)
		CONTEXT->OMSetDepthStencilState(nullptr, 0);
	else
		m_pDepthStencilState->UpdateData();

	// Shader 가 지정한 RasterrizerState 를 셋팅한다.
	ID3D11RasterizerState* pRSState = CDevice::GetInst()->GetRSState(m_eRSType);
	CONTEXT->RSSetState(pRSState);

	// 샘플러 갱신
	CRenderMgr::GetInst()->UpdateSampler();
}