#pragma once

#include <string>


using namespace std;

class CGameObject;
class CSceneMaker
{
private:
	CSceneMaker();
	~CSceneMaker();

public:
	static CSceneMaker* GetInst()
	{
		static CSceneMaker mgr;
		return &mgr;
	}

public:
	void init();
	void SceneInit();
	void CreateScene();
	void update();

private:
	void LoadResource();
	void LoadResource(wstring& _strFolderPath);
	void Load(const wstring& _strFullPath);

private:
	static CGameObject* m_pMainCam;

};

