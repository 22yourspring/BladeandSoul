#pragma once

#include "libxl.h";
#include "Ptr.h";

using namespace libxl;

class CExcelMgr
{
public:
	SINGLE(CExcelMgr);


	bool CreateBook();
	bool LoadMonster(const TCHAR* filename);

	void Test();
	int ReadingTest();

	void LoadPSA(const TCHAR* psaName);
	void SavePSAToAnimClipFile(const TCHAR* _psaName, Ptr<class CMesh> _pMesh);
	void SavePSAToAnimClipFile(const TCHAR* _psaJob, const TCHAR* _psaName, Ptr<class CMesh> _pMesh);
};

