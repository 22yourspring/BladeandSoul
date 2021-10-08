#pragma once
#include "FilePath.h"

#include <vector>
#include <filesystem>

class CBinaryFile;

// 디렉토리를 형상화한 클래스
class CFileDirectory : public CFilePath
{
public:
	CFileDirectory();
	~CFileDirectory();


	void SetCurrentPath();

	// 파일 경로를 넣어줘도 파일은 제거해 버리고
	// 그 디렉토리로 자신의 패스를 세팅하게 한다.
	void PathSetting(const char* _Path);
	void PathSetting(const wchar_t* _Path);

	CBinaryFile CreateCFile(const wchar_t* _FileName);

	// 그 디렉토리가 없다 BOOM
	// Move(L"\\HiSuite");
	// C:\Users\JHK\Documents
	// 실행후 //  C:\Users\JHK\Documents\HiSuite
	// 디렉토리간 이동만 가능하다.
	bool Move(const wchar_t* _Name);

	// 이녀석의 경로가 자기 부모 경로로 변경된다.
	// C:\Users\JHK\Documents\HiSuite
	// 실행후 // C:\Users\JHK\Documents
	void MoveParent();

	// 그 디렉토리 내부에 이 파일이 있는지 없는지 알아낸다.
	bool IsFile(const wchar_t* _FileName);

	// 그 디렉토리 내부에 있는 파일들의 목록을 모두 알아낸다.
	std::vector<CBinaryFile> DirToAllFile();

	// *.*
	// *.png
	// *.txt
	std::vector<CBinaryFile> DirToAllFile(const wchar_t* _Delimiter);


public:
private:

};

