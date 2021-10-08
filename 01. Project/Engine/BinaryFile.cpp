#include "stdafx.h"
#include "BinaryFile.h"

#include <atlstr.h>


CBinaryFile::CBinaryFile(const wchar_t* _Path, const wchar_t* _Mode) : mFile(nullptr)
{
	Open(_Path, _Mode);
}

CBinaryFile::CBinaryFile() : mFile(nullptr)
{
}

CBinaryFile::CBinaryFile(const CBinaryFile & other)
{
	Copy(other);
}

CBinaryFile::CBinaryFile(const wchar_t* _FileName) : mFile(nullptr)
{
	// mFileName = _FileName;

	PathSetting(_FileName);
}

CBinaryFile::~CBinaryFile()
{
	Close();
}

void CBinaryFile::PathSetting(const char* _Path)
{
	PathSetting(CA2W(_Path).m_psz);
}

void CBinaryFile::PathSetting(const wchar_t* _Path)
{
	mPath = _Path;
	m_strFileName = mPath;
	int LastFolderIndex = m_strFileName.find_last_of(L"\\");
	m_strFileName = m_strFileName.replace(0, LastFolderIndex + 1, L"");

	m_strExt = m_strFileName;
	LastFolderIndex = m_strExt.find_last_of(L".");
	m_strExt = m_strExt.replace(0, LastFolderIndex + 1, L"");
}

void CBinaryFile::PathSetting(const std::wstring & path)
{
	PathSetting(path.c_str());
}

void CBinaryFile::FullPath(const std::wstring & path)
{
	m_strFullPath = path;
}

bool CBinaryFile::Open()
{
	return Open(mPath.c_str(), m_strOpenMode.c_str());
}

bool CBinaryFile::Open(const wchar_t* _Path, const wchar_t* _Mode)
{
	PathSetting(_Path);
	m_strOpenMode = _Mode;
	_wfopen_s(&mFile, mPath.c_str(), m_strOpenMode.c_str());

	if (nullptr == mFile)
	{
		assert(false);
		return false;
	}

	return true;
}

void CBinaryFile::Close() {
	if (nullptr != mFile)
	{
		fclose(mFile);
		mFile = nullptr;
	}
}

void CBinaryFile::Copy(const CBinaryFile & other)
{
	PathSetting(other.mPath);
	mFile = other.mFile;
	m_strFullPath = other.m_strFullPath;
}

