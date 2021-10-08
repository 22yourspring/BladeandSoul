#pragma once

class CDebugMgr
{
	SINGLE(CDebugMgr);
private:
	vector<tDBG>	m_vecDebug;

public:
	void AddDbgMsg(DBG_TYPE _eType, const wstring& _strMsg){m_vecDebug.push_back(tDBG{ _eType, _strMsg });}

};

