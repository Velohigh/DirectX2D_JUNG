#include "pch.h"
#include "Test.h"

#include "CDevice.h" // 디바이스를 가져오는 이유 : GPU에 명령하기위한 Device, Context를 사용하기 위해

// 정점 버퍼, 정점 정보를 저장할 버퍼
ComPtr<ID3D11Buffer> g_VB;

// 쉐이더



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
	
}

void Tick()
{

}

void Render()
{
}

void Release()
{
}
