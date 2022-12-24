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
	CreateDefaultMaterial();
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
	v.vPos = Vec3(-0.5f, 0.5f, 0.f);
	v.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, 0.5f, 0.f);
	v.vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	v.vUV = Vec2(1.f, 0.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(0.5f, -0.5f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	v.vUV = Vec2(1.f, 1.f);
	vecVtx.push_back(v);

	v.vPos = Vec3(-0.5f, -0.5f, 0.f);
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
	vecVtx.clear();
	vecIdx.clear();

	// ===========
	// Circle Mesh
	// ===========
	//      3
	//   4     2
	// 5  --0-- 1

	// 반지름
	float fRadius = 0.5f;

	// 각도
	UINT fSlice = 40.f;
	float fTheta = XM_2PI / (float)fSlice;

	// 중심점
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	v.vUV = Vec2(0.5f, 0.5f);
	vecVtx.push_back(v);

	// 정점 위치 지정
	for (UINT i = 0; i < fSlice; ++i)
	{
		v.vPos = Vec3(fRadius * cosf(fTheta * (float)i), fRadius * sinf(fTheta * (float)i), 0.f);
		v.vUV = Vec2(v.vPos.x + 0.5f, -v.vPos.y + 0.5f);
		vecVtx.push_back(v);
	}

	// 인덱스 설정
	for (UINT i = 0; i < fSlice - 1; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	// 마지막 삼각형
	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(fSlice);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes<CMesh>(L"CircleMesh", pMesh);	// 리소스 매니저에 등록
	vecVtx.clear();
	vecIdx.clear();

}

void CResMgr::CreateDefaultGraphicsShader()
{
	// Shader 생성
	Ptr<CGraphicsShader> pShader = nullptr;

	// ===========
	// Test Shader
	// ===========
	pShader = new CGraphicsShader;
	pShader->SetKey(L"TestShader");
	pShader->CreateVertexShader(L"shader\\test.fx", "VS_Test");
	pShader->CreatePixelShader(L"shader\\test.fx", "PS_Test");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_WRITE); // 깊이 테스트 진행, 깊이 기록 X
	pShader->SetBSType(BS_TYPE::ALPHA_BLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddRes<CGraphicsShader>(L"TestShader", pShader);	// 리소스 매니저에 등록


	// ============================
	// Std2DShader
	// RasterizerState      : None
	// BlendState           : Mask
	// DepthStencilState    : Less
	//
	// Parameter
	// g_tex_0              : Output Texture
	// ============================
	pShader = new CGraphicsShader;
	pShader->SetKey(L"Std2DShader");
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::MASK);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASK);

	AddRes(pShader->GetKey(), pShader);

}

void CResMgr::CreateDefaultMaterial()
{
	Ptr<CMaterial> pMtrl = nullptr;

	// Test Material
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"TestShader"));
	AddRes<CMaterial>(L"TestMtrl", pMtrl);


	// Std2D Material
	pMtrl = new CMaterial;
	pMtrl->SetShader(FindRes<CGraphicsShader>(L"Std2DShader"));
	AddRes(L"Std2DMtrl", pMtrl);
}

void CResMgr::LoadDefaultTexture()
{
	Load<CTexture>(L"PlayerTex", L"texture\\Fighter.bmp");
	Load<CTexture>(L"SmokeTex", L"texture\\smokeparticle.png");
	Load<CTexture>(L"CharacterTex", L"texture\\Character.png");
}
