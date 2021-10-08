#pragma once
#include "Res.h"

#include <DirectTex/DirectXTex.h>
#include <DirectTex/DirectXTex.inl>

#ifdef _DEBUG
#pragma comment(lib, "DirectTex\\DirectXTex_debug.lib")
#else
#pragma comment(lib, "DirectTex\\DirectXTex.lib")
#endif

#include "Ptr.h"


class CTexture :
	public CResource
{
private:
	ScratchImage				m_Image;
	ID3D11Texture2D*			m_pTex2D;

	ID3D11ShaderResourceView*   m_pSRV; // (t 읽기)
	ID3D11UnorderedAccessView*  m_pUAV; // (u 읽기 쓰기)
	ID3D11RenderTargetView*     m_pRTV;	
	ID3D11DepthStencilView*     m_pDSV;	

	D3D11_TEXTURE2D_DESC		m_tDesc;

	// RenderTarget
	// DepthStencil
	// ShaderResource

public:
	ID3D11ShaderResourceView* GetSRV() { return m_pSRV; }
	ID3D11RenderTargetView*   GetRTV() { return m_pRTV; }
	ID3D11DepthStencilView*   GetDSV() { return m_pDSV; }
	ID3D11Texture2D*		  GetTex2D() {return m_pTex2D;}

	void UpdateData(UINT _iShaderType, UINT _iRegisterNum);
	static void ClearData(UINT _iShaderType, UINT _iRegisterNum);

	void UpdateRWData(UINT _iRegisterNum);
	static void ClearRWData(UINT _iRegisterNum);

	static void ClearAllData();
	static void ClearAllRWData();

public:
	virtual void Load(const wstring& _strFullPath);
	virtual void Save(const wstring& _strPath/*상대 경로*/);

	bool Create(float _fWidth, float _fHeight, UINT _flag, DXGI_FORMAT _eFormat, D3D11_USAGE _eUsage);
	bool Create(ID3D11Texture2D* _pTex2D);
	bool CreateArrayTexture(const vector<Ptr<CTexture>>& _vecTex);

	float Width() { return (float)m_tDesc.Width; }
	float Height() { return (float)m_tDesc.Height;}

	const D3D11_TEXTURE2D_DESC& GetDesc() const { return m_tDesc; }
	void* GetSysMem() const { return m_Image.GetPixels();}
	const TexMetadata& GetMetaData() const { return m_Image.GetMetadata(); }
	UINT GetRowPitch() const { return (UINT)m_Image.GetImages()->rowPitch; }
	UINT GetSlicePitch()const { return (UINT)m_Image.GetImages()->slicePitch; }

	void SetData(void * _pSrc, size_t _size);
	void* GetSysMem();
	void Resize(UINT _iWidth, UINT _iHeight);

public:
	CTexture();
	virtual ~CTexture();
};

