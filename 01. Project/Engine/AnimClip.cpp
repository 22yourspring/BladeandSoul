#include "stdafx.h"
#include "AnimClip.h"

#include "ExcelMgr.h"


CAnimClip::CAnimClip()
{

}

CAnimClip::~CAnimClip()
{
	// SAFE_DELETE_VECLIST(m_vecKeyFrame);

	// m_vecKeyFrame.clear();

	// SAFE_DELETE(m_pBoneKeyFrame);

	SAFE_DELETE_VECLIST(m_TransitionList);
}

void CAnimClip::SetBoneKeyFrame(vector<tFrameTrans>* _keyFrame)
{
	int size = _keyFrame->size();

	m_vecKeyFrame.resize(size);

	for (int i = 0; i < size; ++i)
	{
		tFrameTrans temp = tFrameTrans();

		temp.Clone(_keyFrame->at(i));

		m_vecKeyFrame[i] = temp;
	}
}
