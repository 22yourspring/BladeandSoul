#include "stdafx.h"
#include "ScriptMgr.h"

#include "AIScript.h"
#include "BillboardScript.h"
#include "BrushScript.h"
#include "BulletScript.h"
#include "MainCamScript.h"
#include "MonsterScript.h"
#include "PlayerScript.h"
#include "RevolveScript.h"
#include "TestScript.h"
#include "WeatherScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"AIScript");
	_vec.push_back(L"BillboardScript");
	_vec.push_back(L"BrushScript");
	_vec.push_back(L"BulletScript");
	_vec.push_back(L"MainCamScript");
	_vec.push_back(L"MonsterScript");
	_vec.push_back(L"PlayerScript");
	_vec.push_back(L"RevolveScript");
	_vec.push_back(L"TestScript");
	_vec.push_back(L"WeatherScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"AIScript" == _strScriptName)
		return new CAIScript;
	if (L"BillboardScript" == _strScriptName)
		return new CBillboardScript;
	if (L"BrushScript" == _strScriptName)
		return new CBrushScript;
	if (L"BulletScript" == _strScriptName)
		return new CBulletScript;
	if (L"MainCamScript" == _strScriptName)
		return new CMainCamScript;
	if (L"MonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"PlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"RevolveScript" == _strScriptName)
		return new CRevolveScript;
	if (L"TestScript" == _strScriptName)
		return new CTestScript;
	if (L"WeatherScript" == _strScriptName)
		return new CWeatherScript;
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::AISCRIPT:
		return L"AIScript";
		break;

	case SCRIPT_TYPE::BILLBOARDSCRIPT:
		return L"BillboardScript";
		break;

	case SCRIPT_TYPE::BRUSHSCRIPT:
		return L"BrushScript";
		break;

	case SCRIPT_TYPE::BULLETSCRIPT:
		return L"BulletScript";
		break;

	case SCRIPT_TYPE::MAINCAMSCRIPT:
		return L"MainCamScript";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"MonsterScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"PlayerScript";
		break;

	case SCRIPT_TYPE::REVOLVESCRIPT:
		return L"RevolveScript";
		break;

	case SCRIPT_TYPE::TESTSCRIPT:
		return L"TestScript";
		break;

	case SCRIPT_TYPE::WEATHERSCRIPT:
		return L"WeatherScript";
		break;

	}
	return nullptr;
}