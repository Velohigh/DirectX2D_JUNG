#include "pch.h"
#include "CLevelMgr.h"

#include "CLevel.h"
#include "CLayer.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CPlayerScript.h"

#include "CResMgr.h"

CLevelMgr::CLevelMgr()
	: m_pCurLevel(nullptr)
{
}

CLevelMgr::~CLevelMgr()
{
	if (nullptr != m_pCurLevel)
		delete m_pCurLevel;
}


void CLevelMgr::init()
{
	m_pCurLevel = new CLevel;

	// 오브젝트 생성
	CGameObject* pObj = new CGameObject;
	pObj->SetName(L"Player");
	pObj->AddComponent(new CTransform);	// 위치담당 컴포넌트
	pObj->AddComponent(new CMeshRender);	// 그리기 담당 컴포넌트
	pObj->AddComponent(new CPlayerScript);	// 플레이어 스크립트 컴포넌트

	// 리소스 매니저에서 메쉬, 재질, 텍스쳐를 찾아와서 넣어준다.
	Ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh");
	Ptr<CMaterial> TestMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
	Ptr<CTexture> PlayerTex = CResMgr::GetInst()->FindRes<CTexture>(L"PlayerTex");

	TestMtrl->SetTexParam(TEX_0, PlayerTex);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.5f));

	pObj->MeshRender()->SetMesh(pMesh);
	pObj->MeshRender()->SetMaterial(TestMtrl);

	m_pCurLevel->AddGameObject(pObj, 0);

	// Test 오브젝트
	pObj = new CGameObject;
	pObj->SetName(L"Test Object");
	pObj->AddComponent(new CTransform);	// 위치담당 컴포넌트
	pObj->AddComponent(new CMeshRender);	// 그리기 담당 컴포넌트

	pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(TestMtrl);

	m_pCurLevel->AddGameObject(pObj, 1);
}

void CLevelMgr::tick()
{
	m_pCurLevel->tick();
	m_pCurLevel->finaltick();
}

void CLevelMgr::render()
{
	m_pCurLevel->render();
}
