#pragma once
#include "FilePath.h"

#include <vector>
#include <filesystem>

class CBinaryFile;

// ���丮�� ����ȭ�� Ŭ����
class CFileDirectory : public CFilePath
{
public:
	CFileDirectory();
	~CFileDirectory();


	void SetCurrentPath();

	// ���� ��θ� �־��൵ ������ ������ ������
	// �� ���丮�� �ڽ��� �н��� �����ϰ� �Ѵ�.
	void PathSetting(const char* _Path);
	void PathSetting(const wchar_t* _Path);

	CBinaryFile CreateCFile(const wchar_t* _FileName);

	// �� ���丮�� ���� BOOM
	// Move(L"\\HiSuite");
	// C:\Users\JHK\Documents
	// ������ //  C:\Users\JHK\Documents\HiSuite
	// ���丮�� �̵��� �����ϴ�.
	bool Move(const wchar_t* _Name);

	// �̳༮�� ��ΰ� �ڱ� �θ� ��η� ����ȴ�.
	// C:\Users\JHK\Documents\HiSuite
	// ������ // C:\Users\JHK\Documents
	void MoveParent();

	// �� ���丮 ���ο� �� ������ �ִ��� ������ �˾Ƴ���.
	bool IsFile(const wchar_t* _FileName);

	// �� ���丮 ���ο� �ִ� ���ϵ��� ����� ��� �˾Ƴ���.
	std::vector<CBinaryFile> DirToAllFile();

	// *.*
	// *.png
	// *.txt
	std::vector<CBinaryFile> DirToAllFile(const wchar_t* _Delimiter);


public:
private:

};

