#pragma once

class CConstBuffer;
class CStructuredBuffer;

class CDevice
{
	SINGLE(CDevice);
private:
	ID3D11Device*				m_pDevice;	// 메모리 할당, 객체 생성
	ID3D11DeviceContext*		m_pContext; // rendering 기능

	ID3D11RasterizerState*		m_arrRSState[4];
	
	map<wstring, CConstBuffer*> m_mapCB;


public:
	int init();
	
	ID3D11Device* GetDevice() { return m_pDevice; }
	ID3D11DeviceContext* GetContext() { return m_pContext; }

	void CreateConstBuffer(const wstring& _strKey, UINT _iSize, UINT _iRegisterNum);
	CConstBuffer* FindConstBuffer(const wstring& _strKey);
	ID3D11RasterizerState* GetRSState(RS_TYPE _eType) { return m_arrRSState[(UINT)_eType]; }


private:
	
	
	void CreateRaterizerState();
};

