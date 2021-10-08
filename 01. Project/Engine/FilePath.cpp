
#include "stdafx.h"

#include "FilePath.h"
#include <atlstr.h>
#include "BinaryFile.h"
// #include "MyDirectory.h"
// HANDLE

CFilePath::CFilePath()
{
}


CFilePath::~CFilePath()
{
}


void CFilePath::PathSetting(const char* _Path)
{
	PathSetting(CA2W(_Path).m_psz);
}
void CFilePath::PathSetting(const wchar_t* _Path)
{
	mPath = _Path;
}
