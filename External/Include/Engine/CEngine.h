#pragma once

class CEngine
	:  public CSingleton<CEngine>	// 매니저들 클래스는 항상 싱글톤<자기> 버전을 상속받는다.
{
private:
	HWND	m_hWnd;

	// 윈도우 해상도
	Vec2	m_vResolution;

public:
	Vec2 GetWindowResolution() { return m_vResolution; }


public:
	int init(HWND _hWnd, UINT _iWidth, UINT _iHeight);
	void progress();

private:
	void tick();
	void render();

public:
	CEngine();
	~CEngine();


};

