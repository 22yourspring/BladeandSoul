#include "stdafx.h"
#include "Scene.h"

#include "Layer.h"
#include "GameObject.h"
#include "Camera.h"

#include "EventMgr.h"

#include "SceneMgr.h"

CScene::CScene()
	: m_arrLayer{}
	, Be_update(true)
	, FirstTime(true)
	, CycleCount(0)
	, _SceneCameraMOD(CAMERA_MOD::CLIENT_MOD)
	, m_bWeather(false)
	, LightObject(nullptr)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		AddLayer(L"", i);
	}	
}

CScene::~CScene()
{
	Safe_Delete_Array(m_arrLayer);
}

void CScene::awake()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (nullptr != m_arrLayer[i])
			m_arrLayer[i]->awake();
	}
}

void CScene::start()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (nullptr != m_arrLayer[i])
			m_arrLayer[i]->start();
	}

	
}

void CScene::update()
{
	if (CycleCount != 0 && FirstTime)
	{
		FirstTime = false;
		Be_update = false;
	}
	else if (CycleCount == 0)
	{
		++CycleCount;
	}
	
	if (Be_update)
	{
		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			if (nullptr != m_arrLayer[i])
				m_arrLayer[i]->update();
		}
	}
	else
	{
		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			if (nullptr != m_arrLayer[i])
				m_arrLayer[i]->pause_update();
		}
	}
}

void CScene::lateupdate()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (nullptr != m_arrLayer[i])
			m_arrLayer[i]->lateupdate();
	}
}

void CScene::finalupdate()
{
	if (Be_update)
	{
		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			if (nullptr != m_arrLayer[i])
				m_arrLayer[i]->finalupdate();
		}
	}
	else
	{
		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			if (nullptr != m_arrLayer[i])
				m_arrLayer[i]->puase_finalupdate();
		}
	}
}

void CScene::EngineUpdate()
{
	if (FindLayer(L"Decal") != nullptr)
	{
		FindLayer(L"Decal")->EngineUpdate();	// 주석의 이유는 3인칭 시점으로 Update 확인하기 위해.
	}
	if (FindLayer(L"Particle") != nullptr)
	{
		FindLayer(L"Particle")->EngineUpdate();	// 날씨 때문.
	}

	if (CycleCount != 0 && FirstTime)
	{
		FirstTime = false;
		Be_update = false;
		_SceneCameraMOD = CAMERA_MOD::ENGINE_MOD;
	}
	else if (CycleCount == 0)
	{
		++CycleCount;
	}

	if (Be_update)
	{
		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			if (nullptr != m_arrLayer[i])
				m_arrLayer[i]->update();
		}
	}
	else
	{
		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			if (nullptr != m_arrLayer[i])
				m_arrLayer[i]->pause_update();
		}
	}
}


void CScene::AddLayer(const wstring & _strLayerName, int _iIdx)
{	
	// 비정상적인 LayerIndex 거르기
	assert(-1 < _iIdx && _iIdx <= 31 && (nullptr == m_arrLayer[_iIdx]));

	m_arrLayer[_iIdx] = new CLayer;
	m_arrLayer[_iIdx]->SetName(_strLayerName);
	m_arrLayer[_iIdx]->SetLayerIdx(_iIdx);
}

void CScene::SetLayerName(int _iIdx, const wstring & _strName)
{
	assert(-1 < _iIdx && _iIdx <= 31);
	m_arrLayer[_iIdx]->SetName(_strName);
}

void CScene::AddGameObject(const wstring & _strLayerName, CGameObject * _pObject, bool _bMoveAll)
{
	CLayer* pLayer = FindLayer(_strLayerName);
	
	if (pLayer == nullptr)
	{
		CScene* CurScene = CSceneMgr::GetInst()->GetCurScene();
		CurScene->SetLayerName(CurScene->GetEmptyLayer()->GetLayerIdx(), _strLayerName);
		pLayer = FindLayer(_strLayerName);
	}

	pLayer->AddGameObject(_pObject, _bMoveAll);
}

void CScene::AddGameObject(int _iLayerIdx, CGameObject * _pObject, bool _bMoveAll)
{
	assert(-1 < _iLayerIdx && _iLayerIdx <= 31);
	CLayer* pLayer = GetLayer(_iLayerIdx);
	pLayer->AddGameObject(_pObject, _bMoveAll);
}

CLayer * CScene::FindLayer(const wstring & _strLayerName)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (nullptr == m_arrLayer[i])
			continue;

		if (_strLayerName == m_arrLayer[i]->GetName())
			return m_arrLayer[i];
	}

	return nullptr;
}

CLayer * CScene::GetLayer(const wstring & _strLayerName)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (nullptr == m_arrLayer[i])
			continue;

		if (_strLayerName == m_arrLayer[i]->GetName())
			return m_arrLayer[i];
	}

	return nullptr;
}

CLayer * CScene::GetEmptyLayer()
{
	return FindLayer(L"");
}
