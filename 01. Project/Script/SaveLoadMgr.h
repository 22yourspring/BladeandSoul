#pragma once

class CLayer;
class CScene;
class CGameObject;

class CSaveLoadMgr
{
public:
	static void SaveScene(const wstring& _strFilePath);
	static void LoadScene(const wstring& _strFilePath);

	static void SaveLayer(CLayer* _pLayer, FILE* _pFile);
	static void LoadLayer(CLayer* _pLayer, FILE* _pFile);

	static void SaveGameObject(CGameObject* _pObj, FILE* _pFile);
	static void LoadGameObject(CGameObject* _pObj, FILE* _pFile);

	static void SaveResource(FILE* _pFile);
	static void LoadResource(FILE* _pFile);






private:
	CSaveLoadMgr() {}
	virtual ~CSaveLoadMgr() {}
};

