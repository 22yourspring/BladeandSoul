#pragma once

#include "Res.h"

class CAnimClip
{
	friend class CExcelMgr;
	friend class CAnim3DMgr;
	friend class CAnimator3D;

private:
	wstring		m_strName; // 클립 이름 (ex.Move_BladeMaster_JinF_Animset.psa)
	list<struct tAnimationTransition*>		m_TransitionList;
	wstring		m_strClipName; // 짤린 clip의 이름 (ex.BK_B_Standard_Mov_MoveToIdle)

	int			m_iStartFrame = 0;
	int			m_iEndFrame = 0;
	int			m_iFrameLength = 0;
	double		m_dStartTime = 0;
	double		m_dEndTime = 0;
	double		m_dTimeLength = 0;
	float		m_fUpdateTime = 0.f; // 이거 안씀

	int			m_iChangeFrame = 0;

	UINT m_iFrameSize = 0;

	// 06/10
	//   - Animation Clip과 같은 역할
	// class CStructuredBuffer* m_pBoneKeyFrame = nullptr;
	vector<tFrameTrans> m_vecKeyFrame;

	FbxTime::EMode m_eMode;

public:
	int GetCurFrame() { return m_iChangeFrame; };

public:
	CAnimClip();
	~CAnimClip();

	void SetEndFrame(int end) { m_iEndFrame = end; }
	int GetEndFrame() { return m_iEndFrame - 1; }
	int GetFrameLength() { return m_iEndFrame; }
	const wstring& GetName() const { return m_strName; }
	const wstring& GetClipName() const { return m_strClipName; }
	void SetBoneKeyFrame(vector<tFrameTrans>* _keyFrame);

	const vector<tFrameTrans>* GetKeyFrame() const { return&m_vecKeyFrame; }
	// class CStructuredBuffer* GetBoneFrameDataBuffer() const { return m_pBoneKeyFrame; }

public:


	void Save();
};

