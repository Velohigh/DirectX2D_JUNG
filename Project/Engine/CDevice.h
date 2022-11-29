#pragma once
class CDevice
	: public CSingleton<CDevice>
{
private:

public:
	int Init(HWND _hWnd);

public:
	CDevice();
	~CDevice();
};

