
#include "stdafx.h"

#include "FileDirectory.h"

#include <Windows.h>
#include <atlstr.h>
#include "BinaryFile.h"


namespace fs = std::experimental::filesystem::v1;

CFileDirectory::CFileDirectory()
{
	wchar_t directory[1024];

	if (0 == GetCurrentDirectoryW(1024, directory))
	{
		BOOM;
	}

	PathSetting(directory);
}

CFileDirectory::~CFileDirectory()
{
}

bool CFileDirectory::Move(const wchar_t * _Name)
{
	for (const auto & entry : fs::directory_iterator(mPath))
	{
		if (entry.path() == mPath + L"\\" + _Name)
		{
			mPath.append(L"\\");
			mPath.append(_Name);

			return true;
		}
	}


	return false;
}

void CFileDirectory::MoveParent()
{
	int LastFolderIndex = mPath.find_last_of(L"\\");
	mPath = mPath.replace(LastFolderIndex, mPath.size(), L"");
}

bool CFileDirectory::IsFile(const wchar_t * _FileName)
{
	for (const auto & entry : fs::directory_iterator(mPath))
	{
		if (entry.path() == mPath + L"\\" + _FileName)
		{
			return true;
		}
	}

	return false;
}

std::vector<CBinaryFile> CFileDirectory::DirToAllFile()
{
	if (false == IsExist(mPath.c_str()))
	{
		BOOM;
	}

	//std::experimental::filesystem::v1::create_directories

	std::wstring path = mPath;
	std::vector<CBinaryFile> tempFiles;

	for (const auto & entry : fs::directory_iterator(mPath))
	{
		CBinaryFile tempFile = CBinaryFile(entry.path().c_str());

		tempFiles.push_back(tempFile);
	}
	// std::cout << entry.path() << std::endl;

	return tempFiles;
}

std::vector<CBinaryFile> CFileDirectory::DirToAllFile(const wchar_t * _Delimiter)
{
	if (_Delimiter == L"*.*")
	{
		return DirToAllFile();
	}
	else if (L'*' == _Delimiter[0])
	{
		std::wstring tempDeli = _Delimiter;
		int LastFolderIndex = tempDeli.find_last_of(L".");

		std::wstring ext = tempDeli.replace(0, LastFolderIndex + 1, L"");

		std::vector<CBinaryFile> tempFiles = DirToAllFile();
		std::vector<CBinaryFile> resultFiles;
		for (int i = 0; i < (int)tempFiles.size(); ++i)
		{
			std::wstring tempPath = tempFiles[i].GetPath();
			int LastFolderIndex = tempPath.find_last_of(L".");
			std::wstring tempExt = tempPath.replace(0, LastFolderIndex + 1, L"");

			if (ext == tempExt)
			{
				resultFiles.push_back(tempFiles[i]);
			}
		}

		return resultFiles;
	}
	else
	{
		for (const auto & entry : fs::directory_iterator(mPath))
		{
			std::wstring tempPath = mPath + L"\\" + _Delimiter;

			// 폴더일 경우 들어가기
			std::wstring checkPath = entry.path();

			int LastFolderIndex = checkPath.find_last_of(L".");
			int TotalFolderIndex = checkPath.size();


			// 파일일 경우 비교  (확장자 이름이 3개, 4개라고 가정)
			if (TotalFolderIndex == LastFolderIndex + 4 || TotalFolderIndex == LastFolderIndex + 5)
			{
				if (checkPath == tempPath)
				{
					std::vector<CBinaryFile> temp;
					CBinaryFile tempFile = CBinaryFile(_Delimiter);
					tempFile.FullPath(tempPath);
					temp.push_back(tempFile);

					return temp;
				}
			}
			// 폴더라면
			else
			{
				// 그 폴더로 들어가기 (폴더 이름을 넣어야 한다.)
				std::wstring folderName = checkPath;

				int LastFolderIndex = folderName.find_last_of(L"\\");
				folderName = folderName.replace(0, LastFolderIndex + 1, L"");

				Move(folderName.c_str());

				std::vector<CBinaryFile> result = DirToAllFile(_Delimiter);

				MoveParent();

				// 찾았다면
				if (false == result.empty())
				{
					return result;
				}
			}
		}
	}

	return std::vector<CBinaryFile>();
}



void CFileDirectory::SetCurrentPath()
{
	mPath = fs::current_path();
}

void CFileDirectory::PathSetting(const char* _Path)
{
	PathSetting(CA2W(_Path).m_psz);
}

void CFileDirectory::PathSetting(const wchar_t* _Path)
{
	// 이 함수를 아직 믿는건 아니다.
	if (0 != (FILE_ATTRIBUTE_DIRECTORY & GetFileAttributesW(_Path)))
	{
		mPath = _Path;
		return;
	}

	mPath = _Path;
	int LastFolderIndex = mPath.find_last_of(L"\\");
	mPath.replace(LastFolderIndex, mPath.size() - 1, L"");
}

CBinaryFile CFileDirectory::CreateCFile(const wchar_t* _FileName)
{
	if (false == IsExist(mPath.c_str()))
	{
		return CBinaryFile();
	}

	std::wstring tempPath = mPath + L"\\" + _FileName;

	if (true == IsFile(tempPath.c_str()))
	{
		return CBinaryFile(_FileName);
	}

	//std::experimental::filesystem::v1::create_directories

	CBinaryFile tempFile = CBinaryFile(_FileName);

	FILE* file = tempFile.GetFile();

	if (_wfopen_s(&file, _FileName, L"wb") != NULL)
	{
		std::wcout << mPath << std::endl;
	}

	// fclose(file);

	return tempFile;
}
