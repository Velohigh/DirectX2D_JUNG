#include "pch.h"
#include "Test.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"

#include "CDevice.h" // 디바이스를 가져오는 이유 : GPU에 명령하기위한 Device, Context를 사용하기 위해
#include "CPathMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"

#include "CMesh.h"
#include "CGraphicsShader.h"

ComPtr<ID3D11Buffer> g_CB;	// 상수 버퍼

Vec4	g_PlayerPos;
CGameObject* g_Obj = nullptr;

CMesh*				g_RectMesh = nullptr;
CGraphicsShader*	g_pShader = nullptr;


void Init()
{
	// RectMesh 생성
	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;
	Vtx v;
	// 0 --- 1
	// |	 |
	// 3 --- 2
	// 정점
	v.vPos = Vec3(-0.5f, 0.5f, 0.5f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, 0.5f, 0.5f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, -0.5f, 0.5f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(-0.5f, -0.5f, 0.5f);
	v.vColor = Vec4(0.f, 0.f, 0.f, 1.f);
	vecVtx.push_back(v);

	// 인덱스
	vecIdx.push_back(0);
	vecIdx.push_back(2);
	vecIdx.push_back(3);

	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);

	g_RectMesh = new CMesh;
	g_RectMesh->Create(vecVtx.data(), vecVtx.size(), vecIdx.data(), vecIdx.size());

	// Shader 생성
	g_pShader = new CGraphicsShader;
	g_pShader->CreateVertexShader(L"shader\\test.fx", "VS_Test");
	g_pShader->CreatePixelShader(L"shader\\test.fx", "PS_Test");


	// 오브젝트 생성
	g_Obj = new CGameObject;
	g_Obj->AddComponent(new CTransform);	// 위치담당 컴포넌트
	g_Obj->AddComponent(new CMeshRender);	// 그리기 담당 컴포넌트

	g_Obj->MeshRender()->SetMesh(g_RectMesh);
	g_Obj->MeshRender()->SetShader(g_pShader);


	// 상수 버퍼
	// 16바이트 단위 메모리 정렬
	D3D11_BUFFER_DESC tBufferDesc = {};

	tBufferDesc.ByteWidth = sizeof(Vec4);
	tBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	tBufferDesc.Usage = D3D11_USAGE_DYNAMIC;	// 상수버퍼는 수정할것이기때문에 다이나믹
	tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	DEVICE->CreateBuffer(&tBufferDesc, nullptr, g_CB.GetAddressOf());

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

	// 이제 위의 과정을 오브젝트 단위로 사용한다.
	g_Obj->render();

}

void Release()
{
	delete g_Obj;
	delete g_RectMesh;
	delete g_pShader;
}
