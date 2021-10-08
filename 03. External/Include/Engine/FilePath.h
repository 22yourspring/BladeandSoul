#pragma once
#include <string>

#include <assert.h>

#define BOOM assert(false)

#ifdef UNICODE
namespace std
{
	typedef std::wstring tstring;
}
#else
namespace std
{
	typedef std::string tstring;
}
#endif

class CFilePath
{
public:
	static bool IsExist(const wchar_t* _Path)
	{
		if (0 != _waccess_s(_Path, 0))
		{
			return false;
		}
		return true;
	}

	const std::wstring& GetPath() const { return mPath; }

protected:
	std::wstring mPath;


public:
	void PathSetting(const char* _Path);
	void PathSetting(const wchar_t* _Path);

public:
	CFilePath();
	~CFilePath();
};

