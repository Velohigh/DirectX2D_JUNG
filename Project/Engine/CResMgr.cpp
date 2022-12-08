#include "pch.h"
#include "CResMgr.h"

#include "CMesh.h"
#include "CGraphicsShader.h"


CResMgr::CResMgr()
{
}

CResMgr::~CResMgr()
{
}

void CResMgr::init()
{


}

void CResMgr::CreateDefaultMesh()
{
	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;
	Vtx v;

	Ptr<CMesh> pMesh = nullptr;
	// =============
	// RectMesh 생성
	// =============
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

	pMesh = new CMesh;
	pMesh->SetKey(L"RectMesh");

	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	// pMesh.Get() 으로 해야 부모 포인터배열에 입력으로 들어갈 수 있다.
	m_arrRes[(UINT)RES_TYPE::MESH].insert(make_pair(pMesh->GetKey(), pMesh.Get()));	// 리소스 매니저에 등록
}

void CResMgr::CreateDefaultGraphicsShader()
{
	// Shader 생성
	g_pShader = new CGraphicsShader;
	g_pShader->CreateVertexShader(L"shader\\test.fx", "VS_Test");
	g_pShader->CreatePixelShader(L"shader\\test.fx", "PS_Test");

}
