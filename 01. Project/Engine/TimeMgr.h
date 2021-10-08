#pragma once

class CTimeMgr
{
private:
	LARGE_INTEGER	m_llOldCount;
	LARGE_INTEGER   m_llCurCount;
	LARGE_INTEGER   m_llFrequency;

	float			m_fDeltaTime;
	float			m_fAccTime;
	float			m_mmTime;
	float			m_secondTime;
	float			m_fFPS;
	float			m_mmfFPS;
	float			m_sccondFPS;

	bool			m_bFreeze; // DT 가 과하게 넘어갈때 동결

public:
	void init();
	void update();
	void render();

	float renderTime();
	float renderMilliTime();

public:
	float GetDeltaTime() { return m_fDeltaTime; }
	void DTFreeze() { m_bFreeze = true; }
	SINGLE(CTimeMgr);
};

