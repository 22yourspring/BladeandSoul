#pragma once

// 06/09
// 애니메이션 

class CAnim3DMgr
{
	SINGLE(CAnim3DMgr);

	vector<class CAnimClip*> m_vecAnimClip;

public:
	void AddAnimClip(class CAnimClip* _animInfo);
	struct CAnimClip* FindAnimClipByName(const wchar_t* _infoName);
	struct CAnimClip* FindAnimClipByClipName(const wchar_t* _infoName);

	void SaveAll();
	void SaveAll(const TCHAR * _psaJob, const TCHAR * _psaName);
	void LoadAll();
	class CAnimClip* Load(const wchar_t* _strName);
};

