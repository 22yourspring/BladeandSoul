#pragma once
#include "Entity.h"

class CLayer;
class CGameObject;
class CCamera;

class CScene :
	public CEntity
{
private:
	CLayer*				m_arrLayer[MAX_LAYER];

	bool		Be_update;
	bool		FirstTime;
	int			CycleCount;

	wstring		str_WeatherTexture;
	bool		m_bWeather;

	CGameObject* LightObject;
public:
	void _SetUpdate(bool _SetValue) { Be_update = _SetValue; };
	bool _GetUpdate() { return Be_update; };

	CAMERA_MOD _SceneCameraMOD;


	wstring* GetWeatherTextureName() { return &str_WeatherTexture; }
	void SetWeatherTextureName(wstring& _SetName) { str_WeatherTexture = _SetName; }

	bool GetWeatherEnable() { return m_bWeather; }
	void SetWeatherEnable(bool _SetValue) { m_bWeather = _SetValue; }

	void SetLightObject(CGameObject* _SetObject) { LightObject = _SetObject; }
	CGameObject* GetLightObject() { return LightObject; }
public:
	void awake();
	void start();
	void update();
	void lateupdate();
	void finalupdate();

	void EngineUpdate();
public:
	void SetLayerName(int _iIdx, const wstring& _strName);
	void AddGameObject(const wstring& _strLayerName, CGameObject* _pObject, bool _bMoveAll);
	void AddGameObject(int _iLayerIdx, CGameObject* _pObject, bool _bMoveAll);
	CLayer* FindLayer(const wstring& _strLayerName);
	CLayer* GetLayer(int _iIdx) { return m_arrLayer[_iIdx]; };
	CLayer* GetLayer(const wstring& _strLayerName);
	CLayer* GetEmptyLayer();


private:
	void AddLayer(const wstring& _strLayerName, int _iIdx = -1);



public:
	CScene();
	virtual ~CScene();
};

