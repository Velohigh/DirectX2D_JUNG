#pragma once

class CEngine
	:  public CSingleton<CEngine>	// 매니저들 클래스는 항상 싱글톤<자기> 버전을 상속받는다.
{
private:
	HWND	m_hWnd;

public:
	int Init(HWND _hWnd, UINT _iWidth, UINT _iHeight);
	void progress();

public:
	CEngine();
	~CEngine();


};

