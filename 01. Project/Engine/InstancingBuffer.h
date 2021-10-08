#pragma once
#include "global.h"

#include "Ptr.h"
#include "Texture.h"

struct tInstancingData
{
	Matrix matWorld;
	Matrix matWV;
	Matrix matWVP;
	int    iRowIdx;
};

class CTexture;
class CStructuredBuffer;

class CInstancingBuffer
{
	SINGLE(CInstancingBuffer);

private:
	ID3D11Buffer*				m_pInstancingBuffer;
	UINT						m_iMaxCount;
	vector<tInstancingData>		m_vecData;

	vector<CStructuredBuffer*>	m_vecBoneMat;
	int							m_iAnimInstCount;
	CStructuredBuffer*			m_pBoneBuffer;

public:
	void init();
	void Clear() { m_vecData.clear(); m_vecBoneMat.clear(); m_iAnimInstCount = 0; }
	void AddInstancingData(tInstancingData& _tData) { m_vecData.push_back(_tData); }

	UINT GetInstanceCount() { return m_vecData.size(); }
	ID3D11Buffer* GetBuffer() { return m_pInstancingBuffer; }

	void AddInstancingBoneMat(CStructuredBuffer* _pBuffer);
	void SetData();

private:
	void Resize(UINT _iCount);
};