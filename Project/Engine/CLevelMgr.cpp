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
	pObj->AddComponent(new CTransform);	// 위치담당 컴포넌트
	pObj->AddComponent(new CMeshRender);	// 그리기 담당 컴포넌트
	pObj->AddComponent(new CPlayerScript);	// 플레이어 스크립트 컴포넌트

	// 리소스 매니저에서 메쉬, 셰이더를 찾아와서 넣어준다.
	Ptr<CMesh> RectMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
	Ptr<CGraphicsShader> TestShader = CResMgr::GetInst()->FindRes<CGraphicsShader>(L"TestShader");

	pObj->MeshRender()->SetMesh(RectMesh);
	pObj->MeshRender()->SetShader(TestShader);

	m_pCurLevel->AddGameObject(pObj, 0);

	// 오브젝트2 생성
	pObj = new CGameObject;
	pObj->AddComponent(new CTransform);	// 위치담당 컴포넌트
	pObj->AddComponent(new CMeshRender);	// 그리기 담당 컴포넌트

	pObj->MeshRender()->SetMesh(RectMesh);
	pObj->MeshRender()->SetShader(TestShader);

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
