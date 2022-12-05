#include "pch.h"
#include "Test.h"

#include "CDevice.h" // 디바이스를 가져오는 이유 : GPU에 명령하기위한 Device, Context를 사용하기 위해

#include "CPathMgr.h"

// 정점 버퍼, 정점 정보를 저장할 버퍼
ComPtr<ID3D11Buffer> g_VB;

// 쉐이더
ComPtr<ID3DBlob>	g_VSBlob;
ComPtr<ID3DBlob>	g_PSBlob;
ComPtr<ID3DBlob>	g_ErrBlob;

ComPtr<ID3D11VertexShader>	g_VS;
ComPtr<ID3D11PixelShader>	g_PS;

// InputLayut
ComPtr<ID3D11InputLayout>	g_Layout;



void Init()
{
	Vtx arrVtx[3] = {};

	arrVtx[0].vPos = Vec3(0.f, 1.f, 0.5f);
	arrVtx[0].vColor = Vec4(1.f, 1.f, 1.f, 1.f);

	arrVtx[1].vPos = Vec3(1.f, -1.f, 0.5f);
	arrVtx[1].vColor = Vec4(1.f, 1.f, 1.f, 1.f);

	arrVtx[2].vPos = Vec3(-1.f, -1.f, 0.5f);
	arrVtx[2].vColor = Vec4(1.f, 1.f, 1.f, 1.f);


	// 버퍼 Desc 를 채워서, VertexBuffer 를 만들어낸다.
	D3D11_BUFFER_DESC tBufferDesc = {};

	// 정점 저장용도
	tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// SystemMemory 에서 수정 가능한 버퍼
	tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tBufferDesc.Usage = D3D11_USAGE_DYNAMIC; // CPU에서 접근할때 어떤식으로 접근할지 용도

	// 버퍼 크기
	tBufferDesc.ByteWidth = sizeof(Vtx) * 3; // GPU 메모리에 할당

	// Buffer 생성
	D3D11_SUBRESOURCE_DATA tSub = {};
	tSub.pSysMem = arrVtx;
	if (FAILED(DEVICE->CreateBuffer(&tBufferDesc, &tSub, g_VB.GetAddressOf())))
	{
		assert(nullptr);
	}

	// Shader 파일 경로
	wstring strShaderFile = CPathMgr::GetInst()->GetContentPath();
	strShaderFile += L"shader\\test.fx";

	// VertexShader Compile
	if (FAILED(D3DCompileFromFile(strShaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, "VS_Test", "vs_5_0", 0, 0, g_VSBlob.GetAddressOf(), g_ErrBlob.GetAddressOf())))
	{
		MessageBoxA(nullptr, (const char*)g_ErrBlob->GetBufferPointer()
			, "Vertex Shader Compile Failed!!", MB_OK);
	}

	// PixelShader Compile
	if (FAILED(D3DCompileFromFile(strShaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, "PS_Test", "ps_5_0", 0, 0, g_PSBlob.GetAddressOf(), g_ErrBlob.GetAddressOf())))
	{
		MessageBoxA(nullptr, (const char*)g_ErrBlob->GetBufferPointer()
			, "Pixel Shader Compile Failed!!", MB_OK);
	}

	// 컴파일된 객체로 VertexShader, PixelShader 를 만든다.
	DEVICE->CreateVertexShader(g_VSBlob->GetBufferPointer(), g_VSBlob->GetBufferSize()
		, nullptr, g_VS.GetAddressOf());
	DEVICE->CreatePixelShader(g_PSBlob->GetBufferPointer(), g_PSBlob->GetBufferSize()
		, nullptr, g_PS.GetAddressOf());


	// InputLayout 생성
	D3D11_INPUT_ELEMENT_DESC LayoutDesc[2] = { };

	LayoutDesc[0].SemanticName = "POSITION";
	LayoutDesc[0].SemanticIndex = 0;
	LayoutDesc[0].AlignedByteOffset = 0;	// 정점버퍼에서의 해당 데이터 시작 지점
	LayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;  // float3 개짜리 이므로 12바이트에 맞는 자료형 입력
	LayoutDesc[0].InputSlot = 0;			// 나중에 설명
	LayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc[0].InstanceDataStepRate = 0;	// 안씀

	LayoutDesc[1].SemanticName = "COLOR";
	LayoutDesc[1].SemanticIndex = 0;
	LayoutDesc[1].AlignedByteOffset = 12;	// 정점버퍼에서의 해당 데이터 시작 지점
	LayoutDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;	// 해당 레이아웃 데이터 크기
	LayoutDesc[1].InputSlot = 0;			// 나중에 설명
	LayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	LayoutDesc[1].InstanceDataStepRate = 0;	// 안씀

	if (FAILED(DEVICE->CreateInputLayout(LayoutDesc, 2
		, g_VSBlob->GetBufferPointer(), g_VSBlob->GetBufferSize()
		, g_Layout.GetAddressOf())))
	{
		assert(nullptr);
	}


}

void Tick()
{

}

void Render()
{
	// IA 단계에서 사용할 버텍스 버퍼 전달, 세팅하는 것뿐이고, IA단계가 실행되는것이 아니다.
	UINT iStride = sizeof(Vtx);
	UINT iOffset = 0;	// 정점에서 특정 정점부터 렌더링하고 싶을떄 사용, 지금 X
	CONTEXT->IASetVertexBuffers(0, 1, g_VB.GetAddressOf(), &iStride, &iOffset);
	CONTEXT->IASetInputLayout(g_Layout.Get());	// IA에서 사용될 레이아웃을 세팅
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 3개씩 끊어서 면으로 보겠다는 세팅

	CONTEXT->VSSetShader(g_VS.Get(), nullptr, 0); // 버텍스 셰이더 세팅
	CONTEXT->PSSetShader(g_PS.Get(), nullptr, 0); // 픽셀 셰이더 세팅

	// 실제 렌더링 파이프라인 실행 함수.
	CONTEXT->Draw(3, 0);
}

void Release()
{
}
