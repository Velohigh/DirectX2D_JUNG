#include "pch.h"
#include "CResMgr.h"

#include "CPathMgr.h"


CResMgr::CResMgr()
{
}

CResMgr::~CResMgr()
{
}

void CResMgr::init()
{
	CreateDefaultMesh();
	CreateDefaultGraphicsShader();
	LoadDefaultTexture();
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
	v.vUV = Vec2(0.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, 0.5f, 0.5f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(1.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, -0.5f, 0.5f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(1.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(-0.5f, -0.5f, 0.5f);
	v.vColor = Vec4(0.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 1.f); 
	vecVtx.push_back(v);

	// 인덱스
	vecIdx.push_back(0);
	vecIdx.push_back(2);
	vecIdx.push_back(3);

	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes<CMesh>(L"RectMesh", pMesh);	// 리소스 매니저에 등록
}

void CResMgr::CreateDefaultGraphicsShader()
{
	// Shader 생성
	Ptr<CGraphicsShader> pShader = nullptr;

	// ===========
	// Test Shader
	// ===========

	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\test.fx", "VS_Test");
	pShader->CreatePixelShader(L"shader\\test.fx", "PS_Test");

	AddRes<CGraphicsShader>(L"TestShader", pShader);	// 리소스 매니저에 등록

}

void CResMgr::LoadDefaultTexture()
{
	//wstring strContent = CPathMgr::GetInst()->GetContentPath();
	//wstring strFilePath = strContent + L"texture\\cursor.bmp";
	//// Texture 의 부모인 CRes에서 Friend ResMgr 처리가 되어있기때문에 부모포인터로 private인 Load에 접근한다.
	//Ptr<CRes> pTexture = new CTexture;		
	//pTexture->SetKey(L"PlayerTex");
	//pTexture->SetRelativePath(L"texture\\cursor.bmp");
	//pTexture->Load(strFilePath);

	//m_arrRes[(UINT)RES_TYPE::TEXTURE].insert(make_pair(pTexture->GetKey(), pTexture.Get()));	// 리소스 매니저에 등록

	Ptr<CTexture> pTexture = Load<CTexture>(L"PlayerTex", L"texture\\cursor.bmp");

	// 로드한 텍스쳐를 t0레지스터에 바인딩한다.
	((CTexture*)(pTexture.Get()))->UpdateData(0);

}
