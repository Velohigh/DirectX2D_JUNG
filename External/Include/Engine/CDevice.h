#pragma once

class CDevice
	: public CSingleton<CDevice>
{
private:
	HWND							m_hWnd;

	ComPtr<ID3D11Device>			m_Device;		// GPU 메모리 할당 	
	ComPtr<ID3D11DeviceContext>		m_Context;		// GPU 제어, 렌더링, 동작 수행

	ComPtr<IDXGISwapChain>			m_SwapChain;

	ComPtr<ID3D11Texture2D>			m_RTTex;
	ComPtr<ID3D11RenderTargetView>	m_RTV;

	ComPtr<ID3D11Texture2D>			m_DSTex;
	ComPtr<ID3D11DepthStencilView>	m_DSV;

	D3D11_VIEWPORT					m_ViewPort;

	// 렌더타겟 해상도
	Vec2							m_vRenderResolution;


public:
	int Init(HWND _hWnd, UINT _iWidth, UINT _iHeight);
	void ClearTarget(float(&_color)[4]);
	void Present() { m_SwapChain->Present(0, 0); }

private:
	int CreateSwapChain();
	int CreateView();

public:
	ID3D11Device* GetDevice() { return m_Device.Get(); }
	ID3D11DeviceContext* GetDeviceContext() { return m_Context.Get(); }

public:
	CDevice();
	~CDevice();
};

