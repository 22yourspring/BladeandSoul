#include "stdafx.h"
#include "TimeMgr.h"

#include "FontMgr.h"

CTimeMgr::CTimeMgr()
	: m_llCurCount{}
	, m_llOldCount{}
	, m_llFrequency{}
	, m_fDeltaTime(0.f)
	, m_fAccTime(0.f)
	, m_secondTime(0.f)
	, m_mmTime(0.f)
	, m_bFreeze(false)
{
}

CTimeMgr::~CTimeMgr()
{
}

void CTimeMgr::init()
{
	QueryPerformanceFrequency(&m_llFrequency);
	QueryPerformanceCounter(&m_llOldCount);
}

void CTimeMgr::update()
{
	QueryPerformanceCounter(&m_llCurCount);

	m_fDeltaTime = (float)(m_llCurCount.QuadPart - m_llOldCount.QuadPart) / (float)m_llFrequency.QuadPart;
	m_llOldCount = m_llCurCount;

	m_fAccTime += m_fDeltaTime;
	m_secondTime += m_fDeltaTime;
	m_mmTime += m_fDeltaTime;

	if (m_bFreeze)
	{
		m_bFreeze = false;
		m_fDeltaTime = 1.f / 60.f;
		m_secondTime = 1.f / 60.f;
		m_mmTime = 1.f / 60.f;
	}

	g_global.fDT = m_fDeltaTime;
	g_global.fAccTime += m_fDeltaTime;
}
void CTimeMgr::render()
{
	if (m_fAccTime > 1.f)
	{
		QueryPerformanceFrequency(&m_llFrequency);
		m_fAccTime = 0.f;
		m_fFPS = 1 / m_fDeltaTime;
	}

	wchar_t szFPS[255] = {};

	wsprintf(szFPS, L"FPS : %d", (UINT)m_fFPS);
	CFontMgr::GetInst()->DrawFont(szFPS, 10, 10, 16, FONT_RGBA(255, 0, 0, 255));
}

float CTimeMgr::renderTime()
{
	if (m_secondTime > 1.f)
	{
		QueryPerformanceFrequency(&m_llFrequency);
		m_secondTime = 0.f;
		m_sccondFPS = 1 / m_fDeltaTime;
	}

	return m_sccondFPS;
}

float CTimeMgr::renderMilliTime()
{
	if (m_mmTime > 0.1f)
	{
		QueryPerformanceFrequency(&m_llFrequency);
		m_mmTime = 0.f;
		m_mmfFPS = 1 / m_fDeltaTime;
	}

	return m_mmfFPS;
}
