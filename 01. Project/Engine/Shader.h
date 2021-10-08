#pragma once
#include "Res.h"

class CBlendState;
class CDepthStencilState;

struct tShaderParam
{
	wstring			strParamName;
	SHADER_PARAM	eType;
};

class CShader :
	public CResource
{	
public:
	static vector<D3D11_INPUT_ELEMENT_DESC> g_vecLayout;
	static int g_iOffset;
	static int g_iOffsetInst;

	static void CreateLayoutDesc();
	static void AddInputLayout(const char * _pSemanticName
		, UINT _iSemanticIdx, DXGI_FORMAT _eFormat
		, UINT _iInputSlot, UINT _iInstancing
		, D3D11_INPUT_CLASSIFICATION _eClass = D3D11_INPUT_PER_VERTEX_DATA);

private:
	ID3D11VertexShader	*	m_pVS;
	ID3D11VertexShader  *	m_pVSInst;
	ID3D11HullShader	*	m_pHS;
	ID3D11DomainShader	*	m_pDS;
	ID3D11GeometryShader*	m_pGS;	
	ID3D11PixelShader	*	m_pPS;

	ID3DBlob*				m_pVSBlob;
	ID3DBlob*				m_pVSInstBlob;
	ID3DBlob*				m_pHSBlob;
	ID3DBlob*				m_pDSBlob;
	ID3DBlob*				m_pGSBlob;
	ID3DBlob*				m_pPSBlob;

	ID3DBlob*				m_pErrBlob;

	ID3D11ComputeShader		*m_pCS;
	ID3DBlob*				m_pCSBlob;

	ID3D11InputLayout*		m_pLayout;
	vector<tShaderParam>	m_vecShaderPamam;

	CBlendState*			m_pBlendState;
	CDepthStencilState*		m_pDepthStencilState;
	RS_TYPE					m_eRSType;
	SHADER_POV				m_ePOV;
	D3D11_PRIMITIVE_TOPOLOGY m_eTopology;


public:
	void CreateVertexShader(const wstring& _strPath, const string& _strFuncName, const string& _strhlslVersion);
	void CreateVertexInstShader(const wstring & _strPath, const string & _strFuncName, const string & _strhlslVersion);
	void CreateHullShader(const wstring& _strPath, const string& _strFuncName, const string& _strhlslVersion);
	void CreateDomainShader(const wstring& _strPath, const string& _strFuncName, const string& _strhlslVersion);	
	void CreateGeometryShader(const wstring& _strPath, const string& _strFuncName, const string& _strhlslVersion);
	void CreateComputeShader(const wstring& _strPath, const string& _strFuncName, const string& _strhlslVersion);
	void CreatePixelShader(const wstring& _strPath, const string& _strFuncName, const string& _strhlslVersion);
	
	void UpdateData(UINT _iInstancing = 0);
	void AddShaderParam(const tShaderParam& _param) { m_vecShaderPamam.push_back(_param); }

	ID3DBlob* GetVtxBlob() { return m_pVSBlob; }
	void SetBlendState(CBlendState* _pState) { m_pBlendState = _pState; }
	void SetDepthStencilState(CDepthStencilState* _pState) { m_pDepthStencilState = _pState; }

	void SetRSType(RS_TYPE _eType) { m_eRSType = _eType; }
	void SetShaderPOV(SHADER_POV _ePOV) /* 쉐이더 사용 시점 설정*/{ m_ePOV = _ePOV; }
	SHADER_POV GetShaderPOV() { return m_ePOV; }
	void SetTopology(D3D11_PRIMITIVE_TOPOLOGY _eTopology) { m_eTopology = _eTopology; }

	ID3D11VertexShader* GetInstVS() {return m_pVSInst;}

	ID3D11VertexShader* GetVS() { return m_pVS; }
	ID3D11PixelShader* GetPS() { return m_pPS; }
	ID3D11ComputeShader* GetCS() { return m_pCS; }
	ID3D11GeometryShader* GetGS() { return m_pGS; }

public:
	CShader();
	virtual ~CShader();
};

