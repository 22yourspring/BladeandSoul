#include "stdafx.h"
#include "WeatherScript.h"

#include <Engine/RenderMgr.h>
#include <Engine/Camera.h>

#include <Engine/Core.h>
#include <Engine/ParticleSystem.h>

CWeatherScript::CWeatherScript()
	: CScript((UINT)SCRIPT_TYPE::WEATHERSCRIPT)
	, m_bStart(true)
{
}

CWeatherScript::~CWeatherScript()
{
}

void CWeatherScript::AutoPosition()
{
	if (CCore::GetInst()->GetSceneMod() == SCENE_MOD::SCENE_PLAY)
	{
		ClimatePosX = 4000.f;
		ClimatePosY = 5000.f;
		ClimatePosZ = 4000.f;
	}
	else
	{
		vPos = CRenderMgr::GetInst()->GetMainCam()->Transform()->GetWorldPos();
		ClimatePosX = vPos.x;
		ClimatePosY = vPos.y += 1400.f;
		ClimatePosZ = vPos.z;
	}


	if (Object()->GetName() == L"_Weather Particle01")
	{
		Transform()->SetLocalPos(Vec3(ClimatePosX + 800.f, ClimatePosY, ClimatePosZ - 800.f));
	}
	else if (Object()->GetName() == L"_Weather Particle02")
	{
		Transform()->SetLocalPos(Vec3(ClimatePosX + 800.f, ClimatePosY, ClimatePosZ - 400.f));
	}
	else if (Object()->GetName() == L"_Weather Particle03")
	{
		Transform()->SetLocalPos(Vec3(ClimatePosX + 800.f, ClimatePosY, ClimatePosZ));
	}
	else if (Object()->GetName() == L"_Weather Particle04")
	{
		Transform()->SetLocalPos(Vec3(ClimatePosX + 800.f, ClimatePosY, ClimatePosZ + 400.f));
	}
	else if (Object()->GetName() == L"_Weather Particle05")
	{
		Transform()->SetLocalPos(Vec3(ClimatePosX + 800.f, ClimatePosY, ClimatePosZ + 800.f));
	}
	else if (Object()->GetName() == L"_Weather Particle06")
	{
		Transform()->SetLocalPos(Vec3(ClimatePosX + 400.f, ClimatePosY, ClimatePosZ - 800.f));
	}
	else if (Object()->GetName() == L"_Weather Particle07")
	{
		Transform()->SetLocalPos(Vec3(ClimatePosX + 400.f, ClimatePosY, ClimatePosZ - 400.f));
	}
	else if (Object()->GetName() == L"_Weather Particle08")
	{
		Transform()->SetLocalPos(Vec3(ClimatePosX + 400.f, ClimatePosY, ClimatePosZ));
	}
	else if (Object()->GetName() == L"_Weather Particle09")
	{
		Transform()->SetLocalPos(Vec3(ClimatePosX + 400.f, ClimatePosY, ClimatePosZ + 400.f));
	}
	else if (Object()->GetName() == L"_Weather Particle10")
	{
		Transform()->SetLocalPos(Vec3(ClimatePosX + 400.f, ClimatePosY, ClimatePosZ + 800.f));
	}
	else if (Object()->GetName() == L"_Weather Particle11")
	{
		Transform()->SetLocalPos(Vec3(ClimatePosX, ClimatePosY, ClimatePosZ - 1000.f));
	}
	else if (Object()->GetName() == L"_Weather Particle12")
	{
		Transform()->SetLocalPos(Vec3(ClimatePosX, ClimatePosY, ClimatePosZ - 400.f));
	}
	else if (Object()->GetName() == L"_Weather Particle13")
	{
		Transform()->SetLocalPos(Vec3(ClimatePosX, ClimatePosY, ClimatePosZ));	//4 5 4
	}
	else if (Object()->GetName() == L"_Weather Particle14")
	{
		Transform()->SetLocalPos(Vec3(ClimatePosX, ClimatePosY, ClimatePosZ + 400.f));
	}
	else if (Object()->GetName() == L"_Weather Particle15")
	{
		Transform()->SetLocalPos(Vec3(ClimatePosX, ClimatePosY, ClimatePosZ + 1000.f));
	}
	else if (Object()->GetName() == L"_Weather Particle16")
	{
		Transform()->SetLocalPos(Vec3(ClimatePosX - 400.f, ClimatePosY, ClimatePosZ - 800.f));
	}
	else if (Object()->GetName() == L"_Weather Particle17")
	{
		Transform()->SetLocalPos(Vec3(ClimatePosX - 400.f, ClimatePosY, ClimatePosZ - 400.f));
	}
	else if (Object()->GetName() == L"_Weather Particle18")
	{
		Transform()->SetLocalPos(Vec3(ClimatePosX - 400.f, ClimatePosY, ClimatePosZ));
	}
	else if (Object()->GetName() == L"_Weather Particle19")
	{
		Transform()->SetLocalPos(Vec3(ClimatePosX - 400.f, ClimatePosY, ClimatePosZ + 400.f));
	}
	else if (Object()->GetName() == L"_Weather Particle20")
	{
		Transform()->SetLocalPos(Vec3(ClimatePosX - 400.f, ClimatePosY, ClimatePosZ + 800.f));
	}
	else if (Object()->GetName() == L"_Weather Particle21")
	{
		Transform()->SetLocalPos(Vec3(ClimatePosX - 800.f, ClimatePosY, ClimatePosZ - 800.f));
	}
	else if (Object()->GetName() == L"_Weather Particle22")
	{
		Transform()->SetLocalPos(Vec3(ClimatePosX - 800.f, ClimatePosY, ClimatePosZ - 400.f));
	}
	else if (Object()->GetName() == L"_Weather Particle23")
	{
		Transform()->SetLocalPos(Vec3(ClimatePosX - 800.f, ClimatePosY, ClimatePosZ));
	}
	else if (Object()->GetName() == L"_Weather Particle24")
	{
		Transform()->SetLocalPos(Vec3(ClimatePosX - 800.f, ClimatePosY, ClimatePosZ + 400.f));
	}
	else if (Object()->GetName() == L"_Weather Particle25")
	{
		Transform()->SetLocalPos(Vec3(ClimatePosX - 800.f, ClimatePosY, ClimatePosZ + 800.f));
	}
}


void CWeatherScript::update()
{
	wstring* str_ClimateTexture = CSceneMgr::GetInst()->GetCurScene()->GetWeatherTextureName();
	if (Script_strWeatherShader != *str_ClimateTexture && *str_ClimateTexture != L"")
	{
		Object()->Particlesystem()->SetParticleMtrl(str_ClimateTexture);
		Script_strWeatherShader = *str_ClimateTexture;
	}

	Object()->Particlesystem()->m_bWeatherParticleRendering = CSceneMgr::GetInst()->GetCurScene()->GetWeatherEnable();

	if (m_bStart)
	{
		Object()->Particlesystem()->SetParticleType(PARTICLE_TYPE::FALL);
		Object()->Particlesystem()->SetStartScale(0.f);
		Object()->Particlesystem()->SetEndScale(5.f);
		Object()->Particlesystem()->SetStartColor(Vec4(10.f, 10.f, 10.f, 1.f));
		Object()->Particlesystem()->SetEndColor(Vec4(10.f, 10.f, 10.f, 0.f));
		Object()->Particlesystem()->SetMinSpeed(80.f);
		Object()->Particlesystem()->SetMaxSpeed(30.f);

		m_bStart = false;
	}

	AutoPosition();
}
