#pragma once

class CEngine
	:  public CSingleton<CEngine>	// �Ŵ����� Ŭ������ �׻� �̱���<�ڱ�> ������ ��ӹ޴´�.
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

