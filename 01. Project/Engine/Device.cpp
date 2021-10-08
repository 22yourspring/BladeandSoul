#include "stdafx.h"
#include "Device.h"

#include "ConstBuffer.h"
#include "ResMgr.h"

CDevice::CDevice()
	: m_pDevice(nullptr)
	, m_pContext(nullptr)
{
}

CDevice::~CDevice()
{
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pContext);
	
	Safe_Delete_Map(m_mapCB);
	
	for (UINT i = 0; i < (UINT)RS_TYPE::END; ++i)
	{
		SAFE_RELEASE(m_arrRSState[i]);
	}
}

int CDevice::init()
{	
	UINT iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_FEATURE_LEVEL eLvl = (D3D_FEATURE_LEVEL)0;
		
	D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr
		, iFlag, nullptr, 0
		, D3D11_SDK_VERSION
		, &m_pDevice
		, &eLvl
		, &m_pContext);

	// 멀티 샘플 레벨 체크
	UINT iQulityLvl = 0;
	m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &iQulityLvl);

	if (0 == iQulityLvl)
	{
		MessageBox(nullptr, L"Sample Count 4 Level 지원 안됨", L"Dvice 초기화 실패", MB_OK);
		return E_FAIL;
	}
	
	// Rasterizer State 만들기
	CreateRaterizerState();
	   
	return S_OK;
}


void CDevice::CreateConstBuffer(const wstring & _strKey, UINT _iSize, UINT _iRegisterNum)
{
	CConstBuffer* pCB = FindConstBuffer(_strKey);

	if (nullptr != pCB)
		assert(nullptr);

	pCB = new CConstBuffer;
	pCB->Create(_iSize, _iRegisterNum, _strKey);
	m_mapCB.insert(make_pair(_strKey, pCB));
}

CConstBuffer * CDevice::FindConstBuffer(const wstring & _strKey)
{
	map<wstring, CConstBuffer*>::iterator iter = m_mapCB.find(_strKey);	
	if (m_mapCB.end() == iter)
		return nullptr;

	return iter->second;
}


void CDevice::CreateRaterizerState()
{
	m_arrRSState[(UINT)RS_TYPE::CULL_BACK] = nullptr;

	D3D11_RASTERIZER_DESC tDesc = {};
	tDesc.FillMode = D3D11_FILL_SOLID;
	tDesc.CullMode = D3D11_CULL_FRONT;
	tDesc.DepthClipEnable = true;
	m_pDevice->CreateRasterizerState(&tDesc, &m_arrRSState[(UINT)RS_TYPE::CULL_FRONT]);

	tDesc = {};
	tDesc.FillMode = D3D11_FILL_SOLID;
	tDesc.CullMode = D3D11_CULL_NONE;
	tDesc.DepthClipEnable = true;
	m_pDevice->CreateRasterizerState(&tDesc, &m_arrRSState[(UINT)RS_TYPE::CULL_NONE]);

	tDesc = {};
	tDesc.FillMode = D3D11_FILL_WIREFRAME;
	tDesc.CullMode = D3D11_CULL_NONE;
	tDesc.DepthClipEnable = true;
	m_pDevice->CreateRasterizerState(&tDesc, &m_arrRSState[(UINT)RS_TYPE::WIRE_FRAME]);
}
