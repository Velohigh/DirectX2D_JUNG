#include "pch.h"
#include "CDevice.h"

#include "CEngine.h"
#include "CConstBuffer.h"

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
	, m_arrConstBuffer{}

{
}

CDevice::~CDevice()
{
	Safe_Del_Array(m_arrConstBuffer);
}

int CDevice::init(HWND _hWnd, UINT _iWidth, UINT _iHeight)
{
	m_hWnd = _hWnd;
	m_vRenderResolution = Vec2((float)_iWidth, (float)_iHeight);

	int iFlag = 0;
#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL eLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;


	// 디바이스 생성
	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE
		, nullptr, iFlag
		, nullptr, 0
		, D3D11_SDK_VERSION
		, m_Device.GetAddressOf(), &eLevel
		, m_Context.GetAddressOf())))
	{
		MessageBox(nullptr, L"Device, Context 생성 실패", L"Device 초기화 에러", MB_OK);
		return E_FAIL;
	}

	// 스왑 체인 생성
	if (FAILED(CreateSwapChain()))
	{
		MessageBox(nullptr, L"스왚체인 생성 실패", L"Device 초기화 에러", MB_OK);
		return E_FAIL;
	}

	// 렌더타겟뷰, DepthStencil 뷰 생성
	if (FAILED(CreateView()))
	{
		MessageBox(nullptr, L"View 생성 실패", L"Device 초기화 에러", MB_OK);
		return E_FAIL;
	}


	// 출력 타겟 설정 (화면에 출력할 렌더타겟 텍스쳐와, 깊이텍스쳐를 지정한다)
	m_Context->OMSetRenderTargets(1, m_RTV.GetAddressOf(), m_DSV.Get());


	// ViewPort 설정
	m_ViewPort.TopLeftX = 0.f;
	m_ViewPort.TopLeftY = 0.f;

	Vec2 vWindowResol = CEngine::GetInst()->GetWindowResolution();
	m_ViewPort.Width = vWindowResol.x;
	m_ViewPort.Height = vWindowResol.y;

	m_ViewPort.MinDepth = 0.f;
	m_ViewPort.MaxDepth = 1.f;

	m_Context->RSSetViewports(1, &m_ViewPort);

	
	// 샘플러 생성
	if (FAILED(CreateSampler()))
	{
		MessageBox(nullptr, L"샘플러 생성 실패", L"Device 초기화 에러", MB_OK);
		return E_FAIL;
	}

	// 상수 버퍼 생성
	CreateConstBuffer();
	
	return S_OK; // E_FAIL;

}

int CDevice::CreateSwapChain()
{
	// 스왚체인 설정
	DXGI_SWAP_CHAIN_DESC tDesc = {};

	tDesc.OutputWindow = m_hWnd;    // 출력 윈도우
	tDesc.Windowed = true;          // 창모드, 전체화면 모드

	tDesc.BufferCount = 1;
	tDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	tDesc.BufferDesc.Width = (UINT)m_vRenderResolution.x;
	tDesc.BufferDesc.Height = (UINT)m_vRenderResolution.y;
	tDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tDesc.BufferDesc.RefreshRate.Denominator = 1;
	tDesc.BufferDesc.RefreshRate.Numerator = 60;
	tDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	tDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	tDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;

	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;
	tDesc.Flags = 0;


	// 스왚체인 생성
	ComPtr<IDXGIDevice>  pDXGIDevice;
	ComPtr<IDXGIAdapter> pAdapter;
	ComPtr<IDXGIFactory> pFactory;

	HRESULT hr = S_OK;

	hr = m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf());
	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pAdapter.GetAddressOf());
	hr = pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pFactory.GetAddressOf());

	hr = pFactory->CreateSwapChain(m_Device.Get(), &tDesc, m_SwapChain.GetAddressOf());

	return hr;
}

int CDevice::CreateView()
{
	// 스왑체인이 들고있는 렌더타겟 포인터를 얻어온다.
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)m_RTTex.GetAddressOf());

	// 얻어온 렌더타겟 포인터로 , 렌더타겟 뷰를 생성한다.
	if (FAILED(m_Device->CreateRenderTargetView(m_RTTex.Get(), nullptr, m_RTV.GetAddressOf())))
	{
		return E_FAIL;
	}

	// DepthStencil 용도 텍스쳐 생성
	D3D11_TEXTURE2D_DESC tDesc = {};

	tDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 반드시 렌더타겟과 같은 해상도로 설정해야 함
	tDesc.Width = (UINT)m_vRenderResolution.x;
	tDesc.Height = (UINT)m_vRenderResolution.y;
	tDesc.ArraySize = 1;

	tDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tDesc.Usage = D3D11_USAGE_DEFAULT;
	tDesc.CPUAccessFlags = 0;

	tDesc.MipLevels = 1;    // 원본만 생성

	tDesc.SampleDesc.Count = 1;
	tDesc.SampleDesc.Quality = 0;


	if (FAILED(m_Device->CreateTexture2D(&tDesc, nullptr, m_DSTex.GetAddressOf())))
	{
		return E_FAIL;
	}

	// DepthStencilView 생성
	if (FAILED(m_Device->CreateDepthStencilView(m_DSTex.Get(), nullptr, m_DSV.GetAddressOf())))
	{
		return E_FAIL;
	}


	return S_OK;
}

int CDevice::CreateSampler()
{
	D3D11_SAMPLER_DESC tSamDesc = {};
	// 텍스쳐 샘플링의 UV좌표가 초과 되었을때 어떻게 할것이냐
	// 1.Wrap : 이미지가 똑같이 나열되어 있는 방식 (1.1, 0.5 의 색상을 가져온다 치면 0.1, 0.5 좌표 색상을 가져오는 방식)
	// 2.Mirror : 이미지가 거울처럼 대칭되어 있는 방식 (1.1, 0.5 의 색상 -> 0.9, 0.5 좌표)
	tSamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	tSamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	tSamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	// 이방선 필터링 방식 : 보정 방식
	tSamDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	DEVICE->CreateSamplerState(&tSamDesc, m_Sampler[0].GetAddressOf());

	tSamDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	tSamDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	tSamDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	tSamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;	// 좌표값 색상 그대로 샘플하는 방식.
	DEVICE->CreateSamplerState(&tSamDesc, m_Sampler[1].GetAddressOf());

	// s0 레지스터에 샘플러 바인딩
	CONTEXT->VSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
	CONTEXT->HSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
	CONTEXT->DSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
	CONTEXT->GSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());
	CONTEXT->PSSetSamplers(0, 1, m_Sampler[0].GetAddressOf());

	// s1 레지스터에 샘플러 바인딩
	CONTEXT->VSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
	CONTEXT->HSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
	CONTEXT->DSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
	CONTEXT->GSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());
	CONTEXT->PSSetSamplers(1, 1, m_Sampler[1].GetAddressOf());


	return S_OK;
}

void CDevice::ClearTarget(float(&_color)[4])
{
	m_Context->ClearRenderTargetView(m_RTV.Get(), _color); // 뷰가 가리키는 렌더타겟 텍스쳐의 색상 지정
	m_Context->ClearDepthStencilView(m_DSV.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0); // 
}

void CDevice::CreateConstBuffer()
{
	// Transform 상수버퍼
	m_arrConstBuffer[(UINT)CB_TYPE::TRANSFORM] = new CConstBuffer((UINT)CB_TYPE::TRANSFORM);
	m_arrConstBuffer[(UINT)CB_TYPE::TRANSFORM]->Create(sizeof(tTransform), 1);

	// Material 상수버퍼 생성
	m_arrConstBuffer[(UINT)CB_TYPE::MATERIAL] = new CConstBuffer((UINT)CB_TYPE::MATERIAL);
	m_arrConstBuffer[(UINT)CB_TYPE::MATERIAL]->Create(sizeof(tMtrlConst), 1);

}
