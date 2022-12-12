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
	// RectMesh ����
	// =============
	// 0 --- 1
	// |	 |
	// 3 --- 2
	// ����
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

	// �ε���
	vecIdx.push_back(0);
	vecIdx.push_back(2);
	vecIdx.push_back(3);

	vecIdx.push_back(0);
	vecIdx.push_back(1);
	vecIdx.push_back(2);

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	AddRes<CMesh>(L"RectMesh", pMesh);	// ���ҽ� �Ŵ����� ���
}

void CResMgr::CreateDefaultGraphicsShader()
{
	// Shader ����
	Ptr<CGraphicsShader> pShader = nullptr;

	// ===========
	// Test Shader
	// ===========

	pShader = new CGraphicsShader;
	pShader->CreateVertexShader(L"shader\\test.fx", "VS_Test");
	pShader->CreatePixelShader(L"shader\\test.fx", "PS_Test");

	AddRes<CGraphicsShader>(L"TestShader", pShader);	// ���ҽ� �Ŵ����� ���

}

void CResMgr::LoadDefaultTexture()
{
	//wstring strContent = CPathMgr::GetInst()->GetContentPath();
	//wstring strFilePath = strContent + L"texture\\cursor.bmp";
	//// Texture �� �θ��� CRes���� Friend ResMgr ó���� �Ǿ��ֱ⶧���� �θ������ͷ� private�� Load�� �����Ѵ�.
	//Ptr<CRes> pTexture = new CTexture;		
	//pTexture->SetKey(L"PlayerTex");
	//pTexture->SetRelativePath(L"texture\\cursor.bmp");
	//pTexture->Load(strFilePath);

	//m_arrRes[(UINT)RES_TYPE::TEXTURE].insert(make_pair(pTexture->GetKey(), pTexture.Get()));	// ���ҽ� �Ŵ����� ���

	Ptr<CTexture> pTexture = Load<CTexture>(L"PlayerTex", L"texture\\cursor.bmp");

	// �ε��� �ؽ��ĸ� t0�������Ϳ� ���ε��Ѵ�.
	((CTexture*)(pTexture.Get()))->UpdateData(0);

}
