#include "stdafx.h"
#include "Texture.h"

#include "Device.h"
#include "PathMgr.h"

CTexture::CTexture()
	: CResource(RES_TYPE::TEXTURE)
	, m_pTex2D(nullptr)
	, m_pSRV(nullptr)
	, m_pUAV(nullptr)
	, m_pRTV(nullptr)
	, m_pDSV(nullptr)
{
}

CTexture::~CTexture()
{
	SAFE_RELEASE(m_pTex2D);
	SAFE_RELEASE(m_pSRV);
	SAFE_RELEASE(m_pUAV);
	SAFE_RELEASE(m_pRTV);
	SAFE_RELEASE(m_pDSV);
}

void CTexture::Load(const wstring & _strFullPath)
{
	wstring ConvertedPath = CPathMgr::Conversion_from_absolute_path_to_relative_path(_strFullPath);

	// 확장자명 얻기
	wchar_t szExt[50] = L"";	
	_wsplitpath_s(ConvertedPath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 50);

	wstring strExt = szExt;

	if (L".dds" == strExt || L".DDS" == strExt)
	{
		if(FAILED(LoadFromDDSFile(ConvertedPath.c_str(), DDS_FLAGS_NONE, nullptr, m_Image)))
		{
			assert(nullptr);
		}
	}
	else if (L".tga" == strExt || L".TGA" == strExt)
	{
		if (FAILED(LoadFromTGAFile(ConvertedPath.c_str(), nullptr, m_Image)))
		{
			assert(nullptr);
		}
	}
	else // png, jpg, jpeg, bmp (WIC)
	{
		if (FAILED(LoadFromWICFile(ConvertedPath.c_str(), WIC_FLAGS_FORCE_RGB, nullptr, m_Image)))
		{
			assert(nullptr);
		}
	}	

	// SRV
	CreateShaderResourceView(DEVICE, m_Image.GetImages(), m_Image.GetImageCount(), m_Image.GetMetadata(), &m_pSRV);			
	m_pSRV->GetResource( ((ID3D11Resource**)&m_pTex2D));
	m_pTex2D->GetDesc(&m_tDesc);
}

void CTexture::Save(const wstring & _strPath)
{
	CaptureTexture(DEVICE, CONTEXT, m_pTex2D, m_Image);

	wstring strPath = CPathMgr::GetResPath();
	strPath += _strPath;

	size_t arrsize = m_Image.GetMetadata().arraySize;

	if (1 < arrsize)
	{
		SaveToDDSFile(m_Image.GetImages(), arrsize, m_Image.GetMetadata(), DDS_FLAGS::DDS_FLAGS_NONE, strPath.c_str());
	}
	else
	{
		SaveToWICFile(*(m_Image.GetImages())
			, WIC_FLAGS_NONE
			, GetWICCodec(static_cast<WICCodecs>(WICCodecs::WIC_CODEC_PNG))
			, strPath.c_str());

		//SaveToDDSFile(*(m_Image.GetImages()), DDS_FLAGS_NONE, strPath.c_str());
	}
}

bool CTexture::Create(float _fWidth, float _fHeight, UINT _iflag, DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage)
{
	m_tDesc.ArraySize = 1;
	m_tDesc.BindFlags = _iflag; // DepthStencil 용도
	
	m_tDesc.Usage = _eUsage;
	if (m_tDesc.Usage == D3D11_USAGE_DYNAMIC)
		m_tDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	else
		m_tDesc.CPUAccessFlags = 0;

	m_tDesc.Width = (UINT)_fWidth;
	m_tDesc.Height = (UINT)_fHeight;

	m_tDesc.Format = _eFormat;
	m_tDesc.MipLevels = 1; // 2 제곱으로 나눈 크기의 텍스쳐를 추가 생성

	m_tDesc.SampleDesc.Count = 1;
	m_tDesc.SampleDesc.Quality = 0;

	if (FAILED(DEVICE->CreateTexture2D(&m_tDesc, nullptr, &m_pTex2D)))
	{
		MessageBox(nullptr, L"Fail Texture Create!!!", L"텍스쳐 생성 실패", MB_OK);
		return false;
	}

	if (m_tDesc.BindFlags &(UINT)D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL)
	{
		if (FAILED(DEVICE->CreateDepthStencilView(m_pTex2D, nullptr, &m_pDSV)))
			return false;
	}
	else
	{
		if (m_tDesc.BindFlags &(UINT)D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC tViewDesc = {};
			tViewDesc.Texture2D.MipLevels = 1;
			tViewDesc.Texture2D.MostDetailedMip = 0;
			tViewDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
			tViewDesc.Format = m_tDesc.Format;

			if (FAILED(DEVICE->CreateShaderResourceView(m_pTex2D, &tViewDesc, &m_pSRV)))
				return false;
		}

		if (m_tDesc.BindFlags &(UINT)D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS)
		{
			D3D11_UNORDERED_ACCESS_VIEW_DESC tViewDesc = {};
			tViewDesc.Texture2D.MipSlice = 0;
			tViewDesc.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D;
			tViewDesc.Format = m_tDesc.Format;

			if (FAILED(DEVICE->CreateUnorderedAccessView(m_pTex2D, &tViewDesc, &m_pUAV)))
				return false;
		}

		if (m_tDesc.BindFlags &(UINT)D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET)
		{
			if (FAILED(DEVICE->CreateRenderTargetView(m_pTex2D, nullptr, &m_pRTV)))
				return false;
		}
	}

	return true;
}

bool CTexture::Create(ID3D11Texture2D * _pTex2D)
{
	m_pTex2D = _pTex2D;
	m_pTex2D->AddRef();
		
	m_pTex2D->GetDesc(&m_tDesc);

	if (m_tDesc.BindFlags &(UINT)D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL)
	{
		if(FAILED(DEVICE->CreateDepthStencilView(m_pTex2D, nullptr, &m_pDSV)))
			return false;
	}
	else
	{
		if (m_tDesc.BindFlags &(UINT)D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC tViewDesc = {};
			tViewDesc.Texture2D.MipLevels = 1;
			tViewDesc.Texture2D.MostDetailedMip = 0;
			tViewDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;
			tViewDesc.Format = m_tDesc.Format;

			if (FAILED(DEVICE->CreateShaderResourceView(m_pTex2D, &tViewDesc, &m_pSRV)))
				return false;
		}

		if (m_tDesc.BindFlags &(UINT)D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS)
		{
			D3D11_UNORDERED_ACCESS_VIEW_DESC tViewDesc = {};
			tViewDesc.Texture2D.MipSlice = 0;
			tViewDesc.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D;
			tViewDesc.Format = m_tDesc.Format;

			if(FAILED(DEVICE->CreateUnorderedAccessView(m_pTex2D, &tViewDesc, &m_pUAV)))
				return false;
		}

		if (m_tDesc.BindFlags &(UINT)D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET)
		{
			if (FAILED(DEVICE->CreateRenderTargetView(m_pTex2D, nullptr, &m_pRTV)))
				return false;
		}
	}

	return true;
}

bool CTexture::CreateArrayTexture(const vector<Ptr<CTexture>>& _vecTex)
{	
	m_tDesc = _vecTex[0]->GetDesc();
	m_tDesc.ArraySize = _vecTex.size();
	m_tDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;	

	vector<D3D11_SUBRESOURCE_DATA> vecSub;

	for (size_t i = 0; i < _vecTex.size(); ++i)
	{
		D3D11_SUBRESOURCE_DATA tSub = {};

		tSub.pSysMem = _vecTex[i]->GetSysMem();
		tSub.SysMemPitch = _vecTex[i]->GetRowPitch();
		tSub.SysMemSlicePitch = _vecTex[i]->GetSlicePitch();

		vecSub.push_back(tSub);
	}

	HRESULT hr = DEVICE->CreateTexture2D(&m_tDesc, vecSub.data(), &m_pTex2D);
	if (FAILED(hr))
		return false;
	
	if (FAILED(DEVICE->CreateShaderResourceView(m_pTex2D, nullptr, &m_pSRV)))
		return false;	

	return true;
}

void CTexture::UpdateData(UINT _iShaderType, UINT _iRegisterNum)
{
	if ((UINT)SHADER_TYPE::ST_VERTEX &  _iShaderType)
		CONTEXT->VSSetShaderResources(_iRegisterNum, 1, &m_pSRV);
	if ((UINT)SHADER_TYPE::ST_HULL &  _iShaderType)
		CONTEXT->HSSetShaderResources(_iRegisterNum, 1, &m_pSRV);
	if ((UINT)SHADER_TYPE::ST_DOMAIN &  _iShaderType)
		CONTEXT->DSSetShaderResources(_iRegisterNum, 1, &m_pSRV);
	if ((UINT)SHADER_TYPE::ST_GEOMETRY & _iShaderType)
		CONTEXT->GSSetShaderResources(_iRegisterNum, 1, &m_pSRV);
	if ((UINT)SHADER_TYPE::ST_COMPUTE &  _iShaderType)
		CONTEXT->CSSetShaderResources(_iRegisterNum, 1, &m_pSRV);
	if ((UINT)SHADER_TYPE::ST_PIXEL &  _iShaderType)
		CONTEXT->PSSetShaderResources(_iRegisterNum, 1, &m_pSRV);
}

void CTexture::ClearData(UINT _iShaderType, UINT _iRegisterNum)
{
	ID3D11ShaderResourceView* pSRV = nullptr;

	if ((UINT)SHADER_TYPE::ST_VERTEX &  _iShaderType)
		CONTEXT->VSSetShaderResources(_iRegisterNum, 1, &pSRV);
	if ((UINT)SHADER_TYPE::ST_HULL &  _iShaderType)
		CONTEXT->HSSetShaderResources(_iRegisterNum, 1, &pSRV);
	if ((UINT)SHADER_TYPE::ST_DOMAIN &  _iShaderType)
		CONTEXT->DSSetShaderResources(_iRegisterNum, 1, &pSRV);
	if ((UINT)SHADER_TYPE::ST_GEOMETRY & _iShaderType)
		CONTEXT->GSSetShaderResources(_iRegisterNum, 1, &pSRV);
	if ((UINT)SHADER_TYPE::ST_COMPUTE &  _iShaderType)
		CONTEXT->CSSetShaderResources(_iRegisterNum, 1, &pSRV);
	if ((UINT)SHADER_TYPE::ST_PIXEL &  _iShaderType)
		CONTEXT->PSSetShaderResources(_iRegisterNum, 1, &pSRV);
}

void CTexture::UpdateRWData(UINT _iRegisterNum)
{
	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(_iRegisterNum, 1, &m_pUAV, &i);	
}

void CTexture::ClearRWData(UINT _iRegisterNum)
{
	ID3D11UnorderedAccessView* pView = nullptr;
	UINT i = -1;
	CONTEXT->CSSetUnorderedAccessViews(_iRegisterNum, 1, &pView, &i);
}

void CTexture::ClearAllData()
{
	static const int iCount = (UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0;
	ID3D11ShaderResourceView* arrSRV[iCount] = {};
	CONTEXT->VSSetShaderResources(0, iCount, arrSRV);
	CONTEXT->HSSetShaderResources(0, iCount, arrSRV);
	CONTEXT->DSSetShaderResources(0, iCount, arrSRV);
	CONTEXT->GSSetShaderResources(0, iCount, arrSRV);
	CONTEXT->CSSetShaderResources(0, iCount, arrSRV);
	CONTEXT->PSSetShaderResources(0, iCount, arrSRV);
}

void CTexture::ClearAllRWData()
{
	static const int iCount = (UINT)SHADER_PARAM::RWTEX_END - (UINT)SHADER_PARAM::RWTEX_0;
	ID3D11UnorderedAccessView* arrUAV[iCount] = {};
	CONTEXT->CSSetUnorderedAccessViews(0, iCount, arrUAV, nullptr);
}

void CTexture::SetData(void * _pSrc, size_t _size)
{
	D3D11_MAPPED_SUBRESOURCE tData = {};

	CONTEXT->Map(m_pTex2D, 0, D3D11_MAP_WRITE_DISCARD, 0, &tData);

	memcpy(tData.pData, _pSrc, _size);

	CONTEXT->Unmap(m_pTex2D, 0);
}

void * CTexture::GetSysMem()
{
	CaptureTexture(DEVICE, CONTEXT, m_pTex2D, m_Image);
	return m_Image.GetPixels();
}

void CTexture::Resize(UINT _iWidth, UINT _iHeight)
{
	SAFE_RELEASE(m_pSRV);
	SAFE_RELEASE(m_pRTV);
	SAFE_RELEASE(m_pDSV);
	SAFE_RELEASE(m_pUAV);
	SAFE_RELEASE(m_pTex2D);

	ID3D11Texture2D* pTex2D = NULL;

	m_tDesc.Width = _iWidth;
	m_tDesc.Height = _iHeight;

	DEVICE->CreateTexture2D(&m_tDesc, NULL, &pTex2D);

	Create(pTex2D);
}