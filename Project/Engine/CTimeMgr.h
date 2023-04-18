#pragma once


class CTimeMgr
	: public CSingleton<CTimeMgr>
{	
	SINGLE(CTimeMgr);
private:
	LARGE_INTEGER	m_llPrevCount;
	LARGE_INTEGER	m_llCurCount;
	LARGE_INTEGER	m_llFrequency;

	UINT			m_iCallCount;
	float			m_fDeltaTime;	// 프레임 간격 시간
	float			m_fTime;		// 누적 시간

	float			m_TimeScale;	// 시간 배속 배율



public:
	void init();
	void tick();
	void render();

public:
	float GetDeltaTime() { return m_fDeltaTime * m_TimeScale; }
	
	float const GetTimeScale() { return m_TimeScale; }
	void SetTimeScale(float _Scale) { m_TimeScale = _Scale; }
};

