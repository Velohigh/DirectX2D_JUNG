#include "pch.h"
#include "Test.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"

#include "CDevice.h" // 디바이스를 가져오는 이유 : GPU에 명령하기위한 Device, Context를 사용하기 위해
#include "CPathMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"


// 정점 버퍼, 정점 정보를 저장할 버퍼
ComPtr<ID3D11Buffer> g_VB;
ComPtr<ID3D11Buffer> g_IB;	// 인덱스 버퍼
ComPtr<ID3D11Buffer> g_CB;	// 상수 버퍼

// 쉐이더
ComPtr<ID3DBlob>	g_VSBlob;
ComPtr<ID3DBlob>	g_PSBlob;
ComPtr<ID3DBlob>	g_ErrBlob;

ComPtr<ID3D11VertexShader>	g_VS;
ComPtr<ID3D11PixelShader>	g_PS;

// InputLayut
ComPtr<ID3D11InputLayout>	g_Layout;


// Vertex
Vtx		g_arrVtx[4] = {};
UINT	g_arrIdx[6] = {};

Vec4	g_PlayerPos;

CGameObject* g_Obj = nullptr;


void Init()
{
	// 오브젝트 생성
	g_Obj = new CGameObject;
	g_Obj->AddComponent(new CTransform);
	g_Obj->AddComponent(new CMeshRender);



	// 상수 버퍼
	// 16바이트 단위 메모리 정렬
	D3D11_BUFFER_DESC tBufferDesc = {};

	tBufferDesc.ByteWidth = sizeof(Vec4);
	tBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tBufferDesc.Usage = D3D11_USAGE_DYNAMIC;	// 상수버퍼는 수정할것이기때문에 다이나믹
	tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	DEVICE->CreateBuffer(&tBufferDesc, nullptr, g_CB.GetAddressOf());


	// 0 --- 1
	// |	 |
	// 3 --- 2
	g_arrVtx[0].vPos = Vec3(-0.5f, 0.5f, 0.5f);
	g_arrVtx[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);

	g_arrVtx[1].vPos = Vec3(0.5f, 0.5f, 0.5f);
	g_arrVtx[1].vColor = Vec4(0.f, 1.f, 0.f, 1.f);

	g_arrVtx[2].vPos = Vec3(0.5f, -0.5f, 0.5f);
	g_arrVtx[2].vColor = Vec4(0.f, 0.f, 1.f, 1.f);

	g_arrVtx[3].vPos = Vec3(-0.5f, -0.5f, 0.5f);
	g_arrVtx[3].vColor = Vec4(0.f, 0.f, 0.f, 1.f);


	// 버퍼 Desc 를 채워서, VertexBuffer 를 만들어낸다.

	// 정점 저장용도
	tBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	// SystemMemory 에서 수정 가능한 버퍼
	tBufferDesc.CPUAccessFlags = 0;
	tBufferDesc.Usage = D3D11_USAGE_DEFAULT; // CPU에서 접근할때 어떤식으로 접근할지 용도

	// 버퍼 크기
	tBufferDesc.ByteWidth = sizeof(Vtx) * 4; // GPU 메모리에 할당할 크기

	// Buffer 생성
	D3D11_SUBRESOURCE_DATA tSub = {};
	tSub.pSysMem = g_arrVtx;
	if (FAILED(DEVICE->CreateBuffer(&tBufferDesc, &tSub, g_VB.GetAddressOf())))
	{
		assert(nullptr);
	}


	// 인덱스
	g_arrIdx[0] = 0;
	g_arrIdx[1] = 2;
	g_arrIdx[2] = 3;

	g_arrIdx[3] = 0;
	g_arrIdx[4] = 1;
	g_arrIdx[5] = 2;

	// 인덱스 저장용도
	tBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	// SystemMemory 에서 수정 불가능
	tBufferDesc.CPUAccessFlags = 0;			// 수정 X
	tBufferDesc.Usage = D3D11_USAGE_DEFAULT; // CPU에서 접근할때 어떤식으로 접근할지 용도

	// 인덱스 버퍼 크기
	tBufferDesc.ByteWidth = sizeof(UINT) * 6; // GPU 메모리에 할당할 크기

	// 인덱스 버퍼 생성
	tSub.pSysMem = g_arrIdx;
	if (FAILED(DEVICE->CreateBuffer(&tBufferDesc, &tSub, g_IB.GetAddressOf()))) // 인덱스 버퍼에 결과값 얻어오기
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
	if (KEY_PRESSED(KEY::UP))
	{
		for (int i = 0; i < 4; ++i)
		{
			g_PlayerPos.y += (float)DT * 1.f;
		}
	}

	if (KEY_PRESSED(KEY::DOWN))
	{
		for (int i = 0; i < 4; ++i)
		{
			g_PlayerPos.y -= (float)DT * 1.f;

		}
	}

	if (KEY_PRESSED(KEY::LEFT))
	{
		for (int i = 0; i < 4; ++i)
		{
			g_PlayerPos.x -= (float)DT * 1.f;
		}
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		for (int i = 0; i < 4; ++i)
		{
			g_PlayerPos.x += (float)DT * 1.f;
		}
	}

	// g_PlayerPos ==> g_CB
	D3D11_MAPPED_SUBRESOURCE tSubRes = {};
	if (!FAILED(CONTEXT->Map(g_CB.Get(), 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &tSubRes)))
	{
	// 맵핑으로 시스템메모리에 동적할당하여 복사한다.
	memcpy(tSubRes.pData, g_PlayerPos, sizeof(Vec4));  // tSubRes.pData 맵핑으로 가져온, 동적할당된 곳의 주소
	CONTEXT->Unmap(g_CB.Get(), 0);	// 동적할당하여 가져온 곳의 수정한 정보들을 GPU로 다시 보낸다.
	}



	



}

void Render()
{
	// IA 단계에서 사용할 버텍스 버퍼 전달, 세팅하는 것뿐이고, IA단계가 실행되는것이 아니다.
	UINT iStride = sizeof(Vtx);
	UINT iOffset = 0;	// 정점에서 특정 정점부터 렌더링하고 싶을떄 사용, 지금 X
	CONTEXT->IASetVertexBuffers(0, 1, g_VB.GetAddressOf(), &iStride, &iOffset);
	CONTEXT->IASetIndexBuffer(g_IB.Get(), DXGI_FORMAT_R32_UINT, 0);  // 인덱스 버퍼 세팅
	CONTEXT->IASetInputLayout(g_Layout.Get());	// IA에서 사용될 레이아웃을 세팅
	CONTEXT->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST); // 3개씩 끊어서 면으로 보겠다는 세팅

	// 상수 버퍼 레지스터 세팅
	// #1 : slot 레지스터 번호, b0 으로 지정해두었다.
	// #2 : 버퍼 개수 1
	// #3 : 상수 버퍼 주소
	CONTEXT->VSSetConstantBuffers(0, 1, g_CB.GetAddressOf());

	CONTEXT->VSSetShader(g_VS.Get(), nullptr, 0); // 버텍스 셰이더 세팅
	CONTEXT->PSSetShader(g_PS.Get(), nullptr, 0); // 픽셀 셰이더 세팅

	// 실제 렌더링 파이프라인 실행 함수.
	CONTEXT->DrawIndexed(6, 0, 0);
}

void Release()
{
	delete g_Obj;
}
