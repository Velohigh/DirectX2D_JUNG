#pragma once

class CDevice
	: public CSingleton<CDevice>
{
private:
	HWND					m_hWnd;

	ID3D11Device*			m_Device;	// GPU 메모리 할당
	ID3D11DeviceContext*	m_Context;	// GPU 제어, 렌더링, 동작 수행

	IDXGISwapChain*			m_SwapChain;

	ID3D11Texture2D*		m_RTTex;
	ID3D11RenderTargetView* m_RTV;

	ID3D11Texture2D*		m_DSTex;
	ID3D11DepthStencilView* m_DSV;

	D3D11_VIEWPORT			m_ViewPort;

	// 렌더타겟 해상도
	UINT					m_iRenderWidth;
	UINT					m_iRenderHeight;


public:
	int Init(HWND _hWnd, UINT _iWidth, UINT _iHeight);

private:
	int CreateSwapChain();

public:
	CDevice();
	~CDevice();
};

