#include "stdafx.h"
#include "Anim3DMgr.h"

#include "AnimClip.h"

#include "FileDirectory.h"
#include "BinaryFile.h"
#include "StructuredBuffer.h"

#include "PathMgr.h""

CAnim3DMgr::CAnim3DMgr()
{
}

CAnim3DMgr::~CAnim3DMgr()
{
	// SAFE_DELETE_VECLIST(m_vecAnimInfo);
	int size = (int)m_vecAnimClip.size();

	for (int j = 0; j < size; ++j)
	{
		SAFE_DELETE(m_vecAnimClip[j]);
	}

	m_vecAnimClip.clear();
}




void CAnim3DMgr::AddAnimClip(CAnimClip * _animInfo)
{
	m_vecAnimClip.push_back(_animInfo);
}

CAnimClip* CAnim3DMgr::FindAnimClipByName(const wchar_t * _infoName)
{
	int size = m_vecAnimClip.size();

	for (int i = 0; i < size; ++i)
	{
		if (_infoName == m_vecAnimClip[i]->GetName())
		{
			return m_vecAnimClip[i];
		}
	}

	return nullptr;
}

CAnimClip * CAnim3DMgr::FindAnimClipByClipName(const wchar_t * _infoName)
{
	int size = m_vecAnimClip.size();

	for (int i = 0; i < size; ++i)
	{
		if (_infoName == m_vecAnimClip[i]->GetClipName())
		{
			return m_vecAnimClip[i];
		}
	}

	return nullptr;
}

// 짤린 Animclip 들을 저장
void CAnim3DMgr::SaveAll()
{
	int totalSize = m_vecAnimClip.size();

	if (0 >= totalSize)
	{
		return;
	}

	CFileDirectory newDir;
	newDir.MoveParent();
	newDir.MoveParent();
	newDir.Move(L"02. File");
	newDir.Move(L"bin");
	newDir.Move(L"content");
	newDir.Move(L"AnimClip");

	for (int i = 0; i < totalSize; ++i)
	{
		FILE* pFile = NULL;

		// 파일 이름 = 클립 이름
		wstring clipName = m_vecAnimClip[i]->GetClipName();

		wstring fullPath = newDir.GetPath() + L"\\" + clipName + L".clip";

		// 파일 열기
		_wfopen_s(&pFile, fullPath.c_str(), L"wb");

		// 저장 시작
		SaveWString(pFile, m_vecAnimClip[i]->GetName().c_str());
		SaveWString(pFile, clipName.c_str());

		int	iStartFrame = m_vecAnimClip[i]->m_iStartFrame;
		fwrite(&iStartFrame, sizeof(int), 1, pFile);

		int			iEndFrame = m_vecAnimClip[i]->m_iEndFrame;
		fwrite(&iEndFrame, sizeof(int), 1, pFile);
		int			iFrameLength = m_vecAnimClip[i]->m_iFrameLength;
		fwrite(&iFrameLength, sizeof(int), 1, pFile);
		double		dStartTime = m_vecAnimClip[i]->m_dStartTime;
		fwrite(&dStartTime, sizeof(double), 1, pFile);
		double		dEndTime = m_vecAnimClip[i]->m_dEndTime;
		fwrite(&dEndTime, sizeof(double), 1, pFile);
		double		dTimeLength = m_vecAnimClip[i]->m_dTimeLength;
		fwrite(&dTimeLength, sizeof(double), 1, pFile);
		UINT		iFrameSize = m_vecAnimClip[i]->m_iFrameSize;
		fwrite(&iFrameSize, sizeof(UINT), 1, pFile);


		int iSize = m_vecAnimClip[i]->m_vecKeyFrame.size();
		fwrite(&iSize, sizeof(int), 1, pFile);

		for (int j = 0; j < iSize; ++j)
		{
			float x = m_vecAnimClip[i]->m_vecKeyFrame[j].vTranslate.x;
			float y = m_vecAnimClip[i]->m_vecKeyFrame[j].vTranslate.y;
			float z = m_vecAnimClip[i]->m_vecKeyFrame[j].vTranslate.z;
			float w = m_vecAnimClip[i]->m_vecKeyFrame[j].vTranslate.w;

			fwrite(&x, sizeof(float), 1, pFile);
			fwrite(&y, sizeof(float), 1, pFile);
			fwrite(&z, sizeof(float), 1, pFile);
			fwrite(&w, sizeof(float), 1, pFile);

			x = m_vecAnimClip[i]->m_vecKeyFrame[j].vScale.x;
			y = m_vecAnimClip[i]->m_vecKeyFrame[j].vScale.y;
			z = m_vecAnimClip[i]->m_vecKeyFrame[j].vScale.z;
			w = m_vecAnimClip[i]->m_vecKeyFrame[j].vScale.w;

			fwrite(&x, sizeof(float), 1, pFile);
			fwrite(&y, sizeof(float), 1, pFile);
			fwrite(&z, sizeof(float), 1, pFile);
			fwrite(&w, sizeof(float), 1, pFile);

			x = m_vecAnimClip[i]->m_vecKeyFrame[j].qRot.x;
			y = m_vecAnimClip[i]->m_vecKeyFrame[j].qRot.y;
			z = m_vecAnimClip[i]->m_vecKeyFrame[j].qRot.z;
			w = m_vecAnimClip[i]->m_vecKeyFrame[j].qRot.w;

			fwrite(&x, sizeof(float), 1, pFile);
			fwrite(&y, sizeof(float), 1, pFile);
			fwrite(&z, sizeof(float), 1, pFile);
			fwrite(&w, sizeof(float), 1, pFile);

		}

		fclose(pFile);
	}

	return;
}

void CAnim3DMgr::SaveAll(const TCHAR * _psaJob, const TCHAR * _psaName)
{
	int totalSize = m_vecAnimClip.size();

	if (0 >= totalSize)
	{
		return;
	}

	wstring psaName = CPathMgr::GetFileName(_psaName);

//	_psaName

	CFileDirectory newDir;
	newDir.MoveParent();
	newDir.MoveParent();
	newDir.Move(L"02. File");
	newDir.Move(L"bin");
	newDir.Move(L"content");
	newDir.Move(L"AnimClip");
	newDir.Move(_psaJob);
	newDir.Move(psaName.c_str());

	for (int i = 0; i < totalSize; ++i)
	{
		FILE* pFile = NULL;

		// 파일 이름 = 클립 이름
		wstring clipName = m_vecAnimClip[i]->GetClipName();

		wstring fullPath = newDir.GetPath() + L"\\" + clipName + L".clip";

		// 파일 열기
		_wfopen_s(&pFile, fullPath.c_str(), L"wb");

		// 저장 시작
		SaveWString(pFile, m_vecAnimClip[i]->GetName().c_str());
		SaveWString(pFile, clipName.c_str());

		int	iStartFrame = m_vecAnimClip[i]->m_iStartFrame;
		fwrite(&iStartFrame, sizeof(int), 1, pFile);

		int			iEndFrame = m_vecAnimClip[i]->m_iEndFrame;
		fwrite(&iEndFrame, sizeof(int), 1, pFile);
		int			iFrameLength = m_vecAnimClip[i]->m_iFrameLength;
		fwrite(&iFrameLength, sizeof(int), 1, pFile);
		double		dStartTime = m_vecAnimClip[i]->m_dStartTime;
		fwrite(&dStartTime, sizeof(double), 1, pFile);
		double		dEndTime = m_vecAnimClip[i]->m_dEndTime;
		fwrite(&dEndTime, sizeof(double), 1, pFile);
		double		dTimeLength = m_vecAnimClip[i]->m_dTimeLength;
		fwrite(&dTimeLength, sizeof(double), 1, pFile);
		UINT		iFrameSize = m_vecAnimClip[i]->m_iFrameSize;
		fwrite(&iFrameSize, sizeof(UINT), 1, pFile);


		int iSize = m_vecAnimClip[i]->m_vecKeyFrame.size();
		fwrite(&iSize, sizeof(int), 1, pFile);

		for (int j = 0; j < iSize; ++j)
		{
			float x = m_vecAnimClip[i]->m_vecKeyFrame[j].vTranslate.x;
			float y = m_vecAnimClip[i]->m_vecKeyFrame[j].vTranslate.y;
			float z = m_vecAnimClip[i]->m_vecKeyFrame[j].vTranslate.z;
			float w = m_vecAnimClip[i]->m_vecKeyFrame[j].vTranslate.w;

			fwrite(&x, sizeof(float), 1, pFile);
			fwrite(&y, sizeof(float), 1, pFile);
			fwrite(&z, sizeof(float), 1, pFile);
			fwrite(&w, sizeof(float), 1, pFile);

			x = m_vecAnimClip[i]->m_vecKeyFrame[j].vScale.x;
			y = m_vecAnimClip[i]->m_vecKeyFrame[j].vScale.y;
			z = m_vecAnimClip[i]->m_vecKeyFrame[j].vScale.z;
			w = m_vecAnimClip[i]->m_vecKeyFrame[j].vScale.w;

			fwrite(&x, sizeof(float), 1, pFile);
			fwrite(&y, sizeof(float), 1, pFile);
			fwrite(&z, sizeof(float), 1, pFile);
			fwrite(&w, sizeof(float), 1, pFile);

			x = m_vecAnimClip[i]->m_vecKeyFrame[j].qRot.x;
			y = m_vecAnimClip[i]->m_vecKeyFrame[j].qRot.y;
			z = m_vecAnimClip[i]->m_vecKeyFrame[j].qRot.z;
			w = m_vecAnimClip[i]->m_vecKeyFrame[j].qRot.w;

			fwrite(&x, sizeof(float), 1, pFile);
			fwrite(&y, sizeof(float), 1, pFile);
			fwrite(&z, sizeof(float), 1, pFile);
			fwrite(&w, sizeof(float), 1, pFile);

		}

		fclose(pFile);
	}

	return;
}

void CAnim3DMgr::LoadAll()
{
	CFileDirectory newDir;
	newDir.MoveParent();
	newDir.MoveParent();
	newDir.Move(L"02. File");
	newDir.Move(L"bin");
	newDir.Move(L"content");
	newDir.Move(L"AnimClip");

	vector<CBinaryFile> allFile = newDir.DirToAllFile();
	int fileSize = (int)allFile.size();

	for (size_t i = 0; i < fileSize; ++i)
	{
		if (L"clip" == allFile[i].Ext())
		{
			FILE* pFile = NULL;

			// 파일 이름 = 클립 이름
			wstring clipName = allFile[i].FileName();

			wstring fullPath = newDir.GetPath() + L"\\" + clipName;

			// 파일 열기
			_wfopen_s(&pFile, fullPath.c_str(), L"rb");

			CAnimClip* pClip = new CAnimClip();

			pClip->m_strName = LoadWString(pFile);
			pClip->m_strClipName = LoadWString(pFile);

			fread(&pClip->m_iStartFrame, sizeof(int), 1, pFile);
			fread(&pClip->m_iEndFrame, sizeof(int), 1, pFile);
			fread(&pClip->m_iFrameLength, sizeof(int), 1, pFile);
			fread(&pClip->m_dStartTime, sizeof(double), 1, pFile);
			fread(&pClip->m_dEndTime, sizeof(double), 1, pFile);
			fread(&pClip->m_dTimeLength, sizeof(double), 1, pFile);
			fread(&pClip->m_iFrameSize, sizeof(UINT), 1, pFile);

			int iSize = 0;
			fread(&iSize, sizeof(int), 1, pFile);
			pClip->m_vecKeyFrame.resize(iSize);

			for (int j = 0; j < iSize; ++j)
			{
				fread(&pClip->m_vecKeyFrame[j].vTranslate.x, sizeof(float), 1, pFile);
				fread(&pClip->m_vecKeyFrame[j].vTranslate.y, sizeof(float), 1, pFile);
				fread(&pClip->m_vecKeyFrame[j].vTranslate.z, sizeof(float), 1, pFile);
				fread(&pClip->m_vecKeyFrame[j].vTranslate.w, sizeof(float), 1, pFile);

				fread(&pClip->m_vecKeyFrame[j].vScale.x, sizeof(float), 1, pFile);
				fread(&pClip->m_vecKeyFrame[j].vScale.y, sizeof(float), 1, pFile);
				fread(&pClip->m_vecKeyFrame[j].vScale.z, sizeof(float), 1, pFile);
				fread(&pClip->m_vecKeyFrame[j].vScale.w, sizeof(float), 1, pFile);

				fread(&pClip->m_vecKeyFrame[j].qRot.x, sizeof(float), 1, pFile);
				fread(&pClip->m_vecKeyFrame[j].qRot.y, sizeof(float), 1, pFile);
				fread(&pClip->m_vecKeyFrame[j].qRot.z, sizeof(float), 1, pFile);
				fread(&pClip->m_vecKeyFrame[j].qRot.w, sizeof(float), 1, pFile);
			}

			fclose(pFile);

			// pClip->m_pBoneKeyFrame = new CStructuredBuffer;
			// pClip->m_pBoneKeyFrame->Create(sizeof(tFrameTrans), pClip->m_iFrameSize, pClip->m_vecKeyFrame.data());

			AddAnimClip(pClip);
		}
	}

	allFile.clear();

}

CAnimClip* CAnim3DMgr::Load(const wchar_t * _strName)
{
	CFileDirectory newDir;
	newDir.MoveParent();
	newDir.MoveParent();
	newDir.Move(L"02. File");
	newDir.Move(L"bin");
	newDir.Move(L"content");
	newDir.Move(L"AnimClip");

	FILE* pFile = NULL;

	// 파일 이름 = 클립 이름
	wstring clipName = _strName;
	clipName.append(L".clip");

	wstring fullPath = newDir.GetPath() + L"\\" + clipName;

	// 파일 열기
	_wfopen_s(&pFile, fullPath.c_str(), L"rb");

	if (nullptr == pFile)
	{
		// Anim clip 파일을 찾을 수 없습니다.
		assert(false);
	}

	CAnimClip* pClip = new CAnimClip();

	pClip->m_strName = LoadWString(pFile);
	pClip->m_strClipName = LoadWString(pFile);
	pClip->m_strClipName = _strName;

	fread(&pClip->m_iStartFrame, sizeof(int), 1, pFile);
	fread(&pClip->m_iEndFrame, sizeof(int), 1, pFile);
	fread(&pClip->m_iFrameLength, sizeof(int), 1, pFile);
	fread(&pClip->m_dStartTime, sizeof(double), 1, pFile);
	fread(&pClip->m_dEndTime, sizeof(double), 1, pFile);
	fread(&pClip->m_dTimeLength, sizeof(double), 1, pFile);
	fread(&pClip->m_iFrameSize, sizeof(UINT), 1, pFile);

	int iSize = 0;
	fread(&iSize, sizeof(int), 1, pFile);
	pClip->m_vecKeyFrame.resize(iSize);

	for (int j = 0; j < iSize; ++j)
	{
		fread(&pClip->m_vecKeyFrame[j].vTranslate.x, sizeof(float), 1, pFile);
		fread(&pClip->m_vecKeyFrame[j].vTranslate.y, sizeof(float), 1, pFile);
		fread(&pClip->m_vecKeyFrame[j].vTranslate.z, sizeof(float), 1, pFile);
		fread(&pClip->m_vecKeyFrame[j].vTranslate.w, sizeof(float), 1, pFile);

		fread(&pClip->m_vecKeyFrame[j].vScale.x, sizeof(float), 1, pFile);
		fread(&pClip->m_vecKeyFrame[j].vScale.y, sizeof(float), 1, pFile);
		fread(&pClip->m_vecKeyFrame[j].vScale.z, sizeof(float), 1, pFile);
		fread(&pClip->m_vecKeyFrame[j].vScale.w, sizeof(float), 1, pFile);

		fread(&pClip->m_vecKeyFrame[j].qRot.x, sizeof(float), 1, pFile);
		fread(&pClip->m_vecKeyFrame[j].qRot.y, sizeof(float), 1, pFile);
		fread(&pClip->m_vecKeyFrame[j].qRot.z, sizeof(float), 1, pFile);
		fread(&pClip->m_vecKeyFrame[j].qRot.w, sizeof(float), 1, pFile);
	}

	fclose(pFile);

	// pClip->m_pBoneKeyFrame = new CStructuredBuffer;
	// pClip->m_pBoneKeyFrame->Create(sizeof(tFrameTrans), pClip->m_iFrameSize, pClip->m_vecKeyFrame.data());

	AddAnimClip(pClip);

	return pClip;
}

