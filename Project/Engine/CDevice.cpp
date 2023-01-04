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

	// RasterizerState 생성
	if (FAILED(CreateRasterizerState()))
	{
		MessageBox(nullptr, L"레스터라이져 스테이트 생성 실패", L"Device 초기화 에러", MB_OK);
		return E_FAIL;
	}

	// DepthStencilState 생성
	if (FAILED(CreateDepthStencilState()))
	{
		MessageBox(nullptr, L"DepthStencilState 생성 실패", L"Device 초기화 에러", MB_OK);
		return E_FAIL;
	}

	// BlendState 생성
	if (FAILED(CreateBlendState()))
	{
		MessageBox(nullptr, L"BlendState 생성 실패", L"Device 초기화 에러", MB_OK);
		return E_FAIL;
	}



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

int CDevice::CreateRasterizerState()
{
	m_RSState[(UINT)RS_TYPE::CULL_BACK] = nullptr; // nullptr이면 기본 옵션을 사용하라는 것.

	D3D11_RASTERIZER_DESC Desc = {};
	Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_FRONT;
	Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;	// 면 내부를 채울지 여부, 설정안하면 기본옵션 SOLID
	DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RS_TYPE::CULL_FRONT].GetAddressOf()); 

	Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
	Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RS_TYPE::CULL_NONE].GetAddressOf());

	Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
	Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
	DEVICE->CreateRasterizerState(&Desc, m_RSState[(UINT)RS_TYPE::WIRE_FRAME].GetAddressOf());

	return S_OK;
}

int CDevice::CreateDepthStencilState()
{
	// Less
	m_DSState[(UINT)DS_TYPE::LESS] = nullptr;	// 기본값

	// Less Equal
	D3D11_DEPTH_STENCIL_DESC Desc = {};
	Desc.DepthEnable = true;					// 깊이 판정 할것인지, 말것인지
	Desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;	// 깊이 판정 방식
	Desc.StencilEnable = false;						
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;	// 본인의 깊이 저장 여부, 당연히 남겨야죠!
	DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::LESS_EQUAL].GetAddressOf());

	// Greater
	Desc.DepthEnable = true;
	Desc.DepthFunc = D3D11_COMPARISON_GREATER;
	Desc.StencilEnable = false;
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::GREATER].GetAddressOf());

	// Greater Equal
	Desc.DepthEnable = true;
	Desc.DepthFunc = D3D11_COMPARISON_GREATER_EQUAL;
	Desc.StencilEnable = false;
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::GREATER_EQUAL].GetAddressOf());

	// No Write				// 깊이 판정 ON, 저장 X
	Desc.DepthEnable = true;
	Desc.DepthFunc = D3D11_COMPARISON_LESS;
	Desc.StencilEnable = false;
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::NO_WRITE].GetAddressOf());

	// NoTest NoWrite		// 깊이 판정 X, 저장 X
	Desc.DepthEnable = false;
	Desc.StencilEnable = false;
	Desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	DEVICE->CreateDepthStencilState(&Desc, m_DSState[(UINT)DS_TYPE::NO_TEST_NO_WRITE].GetAddressOf());


	return S_OK;
}

int CDevice::CreateBlendState()
{
	// No Blend
	m_BSState[(UINT)BS_TYPE::DEFAULT] = nullptr;

	// Mask
	D3D11_BLEND_DESC Desc = {};
	Desc.AlphaToCoverageEnable = true;
	Desc.IndependentBlendEnable = false;

	Desc.RenderTarget[0].BlendEnable = true;
	Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	Desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;

	Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

	Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	DEVICE->CreateBlendState(&Desc, m_BSState[(UINT)BS_TYPE::MASK].GetAddressOf());


	// Alpha Blend
	Desc.AlphaToCoverageEnable = false;
	Desc.IndependentBlendEnable = false;

	Desc.RenderTarget[0].BlendEnable = true;
	Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

	Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	DEVICE->CreateBlendState(&Desc, m_BSState[(UINT)BS_TYPE::ALPHA_BLEND].GetAddressOf());




	// One One
	Desc.AlphaToCoverageEnable = false;
	Desc.IndependentBlendEnable = false;

	Desc.RenderTarget[0].BlendEnable = true;
	Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	Desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

	Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;

	Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	DEVICE->CreateBlendState(&Desc, m_BSState[(UINT)BS_TYPE::ONE_ONE].GetAddressOf());

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
	// Transform 상수버퍼 생성
	m_arrConstBuffer[(UINT)CB_TYPE::TRANSFORM] = new CConstBuffer((UINT)CB_TYPE::TRANSFORM);
	m_arrConstBuffer[(UINT)CB_TYPE::TRANSFORM]->Create(sizeof(tTransform), 1);

	// Material 상수버퍼 생성
	m_arrConstBuffer[(UINT)CB_TYPE::MATERIAL] = new CConstBuffer((UINT)CB_TYPE::MATERIAL);
	m_arrConstBuffer[(UINT)CB_TYPE::MATERIAL]->Create(sizeof(tMtrlConst), 1);

	// Light 상수버퍼 생성
	m_arrConstBuffer[(UINT)CB_TYPE::LIGHT] = new CConstBuffer((UINT)CB_TYPE::LIGHT);
	m_arrConstBuffer[(UINT)CB_TYPE::LIGHT]->Create(sizeof(tLightInfo) * 10 + 16, 1);
}
