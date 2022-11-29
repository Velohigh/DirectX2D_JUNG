#include "pch.h"
#include "CDevice.h"

CDevice::CDevice()
	: m_hWnd(nullptr)
	, m_Device(nullptr)
	, m_Context(nullptr)
	, m_SwapChain(nullptr)
	, m_RTTex(nullptr)
	, m_RTV(nullptr)
	, m_DSTex(nullptr)
	, m_DSV(nullptr)
	, m_ViewPort{}

{
}

CDevice::~CDevice()
{
	if (nullptr != m_Device)	m_Device->Release();
	if (nullptr != m_Context)	m_Context->Release();
	if (nullptr != m_SwapChain)	m_SwapChain->Release();
	if (nullptr != m_RTTex)	m_RTTex->Release();
	if (nullptr != m_RTV)	m_RTV->Release();
	if (nullptr != m_DSTex)	m_DSTex->Release();
	if (nullptr != m_DSV)	m_DSV->Release();

}

int CDevice::Init(HWND _hWnd, UINT _iWidth, UINT _iHeight)
{
	m_hWnd = _hWnd;
	m_iRenderWidth = _iWidth;
	m_iRenderHeight = _iHeight;

	UINT iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL eLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE
		, nullptr, iFlag
		, nullptr, 0
		, D3D11_SDK_VERSION
		, &m_Device, &eLevel
		, &m_Context)))
	{
		MessageBox(nullptr, L"Device, Context 생성 실패", L"Device 초기화 에러", MB_OK);
		return E_FAIL;
	}

	if (FAILED(CreateSwapChain()))
	{
		MessageBox(nullptr, L"스왑체인 생성 실패", L"Device 초기화 에러", MB_OK);
		return E_FAIL;
	}

	//m_SwapChain->Present(0, 0);	// 백버퍼와 프론트 버퍼를 바꾸는 함수


	return S_OK;	// E_FAIL;
}

int CDevice::CreateSwapChain()
{
	// 스왑체인 설정
	DXGI_SWAP_CHAIN_DESC tDesc = {};

	tDesc.OutputWindow = m_hWnd;    // 출력 윈도우
	tDesc.Windowed = true;          // 창모드, 전체화면 모드

	tDesc.BufferCount = 1;
	tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	tDesc.BufferDesc.Width = m_iRenderWidth;
	tDesc.BufferDesc.Height = m_iRenderHeight;
	tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tDesc.BufferDesc.RefreshRate.Denominator = 1;
	tDesc.BufferDesc.RefreshRate.Numerator = 60;
	tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	tDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;

	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.Flags = 0;



	// 스왑체인 생성
	IDXGIDevice* pDXGIDevice = nullptr;
	IDXGIAdapter* pAdapter = nullptr;
	IDXGIFactory* pFactory = nullptr;

	HRESULT hr = S_OK;

	hr = m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDXGIDevice);
	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pAdapter);
	hr = pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);

	hr = pFactory->CreateSwapChain(m_Device, &tDesc, &m_SwapChain);

	pDXGIDevice->Release();
	pAdapter->Release();
	pFactory->Release();


	return hr;
}

