#include "stdafx.h"
#include "PathMgr.h"

wchar_t CPathMgr::g_szSolutionPath[255] = L"";
wchar_t CPathMgr::g_szResPath[255] = L"";
wchar_t CPathMgr::g_szIncPath[255] = L"";
wchar_t CPathMgr::g_szProjPath[255] = L"";
wchar_t CPathMgr::g_szRelativePath[255] = L"";
wchar_t CPathMgr::g_szFile[255] = L"";
wchar_t CPathMgr::g_szExt[50] = L"";

void CPathMgr::init()
{
	// Resources 폴더까지의 경로를 구한다.
	GetCurrentDirectory(255, g_szSolutionPath);
	
	size_t iCount = 0;
	for (size_t i = wcslen(g_szSolutionPath) - 1; i > 0; --i)
	{
		if ('\\' == g_szSolutionPath[i])
		{
			g_szSolutionPath[i + 1] = 0;
			++iCount;
		}

		if (iCount == 2)
			break;
	}

	// SolutionPath -> copy -> ResPath
	wcscpy_s(g_szResPath, 255, g_szSolutionPath);

	// -> bin -> Resources
	wcscat_s(g_szResPath, L"02. File\\bin\\content\\");
	
	// Inc 폴더 경로 
	GetCurrentDirectory(255, g_szIncPath);
	
	iCount = 0;
	for (size_t i = wcslen(g_szIncPath) - 1; i > 0; --i)
	{
		if ('\\' == g_szIncPath[i])
		{
			g_szIncPath[i + 1] = 0;
			++iCount;
		}

		if (iCount == 2)
			break;
	}

	wcscat_s(g_szIncPath, L"Project\\Include\\");


	// Proj 폴더 경로 
	GetCurrentDirectory(255, g_szProjPath);

	iCount = 0;
	for (size_t i = wcslen(g_szProjPath) - 1; i > 0; --i)
	{
		if ('\\' == g_szProjPath[i])
		{
			g_szProjPath[i + 1] = 0;
			++iCount;
		}

		if (iCount == 2)
			break;
	}

	wcscat_s(g_szProjPath, L"01. Project\\");
}

wchar_t * CPathMgr::GetResPath()
{
	return g_szResPath;
}

wchar_t * CPathMgr::GetIncludePath()
{
	return g_szIncPath;
}

wchar_t * CPathMgr::GetProjectPath()
{
	return g_szProjPath;
}

wchar_t * CPathMgr::GetFileName(const wchar_t * _strPath)
{
	_wsplitpath_s(_strPath, NULL, 0, NULL, 0, g_szFile, 255, NULL, 0);
	return g_szFile;
}

wchar_t * CPathMgr::GetExt(const wchar_t * _strPath)
{
	_wsplitpath_s(_strPath, NULL, 0, NULL, 0, nullptr, 0, g_szExt, 50);
	return g_szExt;
}

wchar_t * CPathMgr::GetRelativePath(const wchar_t * _pFullPath)
{
	wmemset(g_szRelativePath, 0, 255);

	wstring str = _pFullPath;
	size_t iLen = wcslen(g_szResPath);
	str = str.substr(iLen, str.length() - iLen).c_str();
	wcscpy_s(g_szRelativePath, 255, str.c_str());
	return g_szRelativePath;
}

wchar_t * CPathMgr::GetSolutionPath()
{
	return g_szSolutionPath;
}

wstring CPathMgr::GetTexturePath(const wstring & _strPath)
{
	wstring DevideString = L"02. File\\bin\\content\\";

	int _strPathCounting = _strPath.length();

	int DevideStringStartingPoint = _strPath.find(DevideString);

	wstring RelativePath = _strPath.substr(DevideStringStartingPoint + DevideString.length(), _strPathCounting);

	return RelativePath;
}

wstring CPathMgr::Conversion_from_absolute_path_to_relative_path(const wstring & _strPath)
{
	wstring DevideString = L"02. File\\bin\\content\\";
	
	int _strPathCounting = _strPath.length();
	
	int DevideStringStartingPoint = _strPath.find(DevideString);

	wstring RelativePath = _strPath.substr(DevideStringStartingPoint + DevideString.length(), _strPathCounting);

	return g_szResPath + RelativePath;
}
