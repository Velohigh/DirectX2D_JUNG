#include "pch.h"
#include "CLevelMgr.h"

#include "CLevel.h"
#include "CLayer.h"

#include "CResMgr.h"

#include "CGameObject.h"
#include "component.h"
#include "CPlayerScript.h"
#include "CCameraMoveScript.h"

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

	// Main Camera Object ����
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCamera");

	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);
	pMainCam->AddComponent(new CCameraMoveScript);

	pMainCam->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);

	m_pCurLevel->AddGameObject(pMainCam, 0);



	// ������Ʈ ����
	CGameObject* pObj = new CGameObject;
	pObj->SetName(L"Player");
	pObj->AddComponent(new CTransform);	// ��ġ��� ������Ʈ
	pObj->AddComponent(new CMeshRender);	// �׸��� ��� ������Ʈ
	pObj->AddComponent(new CPlayerScript);	// �÷��̾� ��ũ��Ʈ ������Ʈ

	// ���ҽ� �Ŵ������� �޽�, ����, �ؽ��ĸ� ã�ƿͼ� �־��ش�.
	Ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh");
	Ptr<CMaterial> TestMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl");
	Ptr<CTexture> PlayerTex = CResMgr::GetInst()->FindRes<CTexture>(L"PlayerTex");

	TestMtrl->SetTexParam(TEX_0, PlayerTex);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));
	pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));

	pObj->MeshRender()->SetMesh(pMesh);
	pObj->MeshRender()->SetMaterial(TestMtrl);

	m_pCurLevel->AddGameObject(pObj, 0);

	// Test ������Ʈ
	pObj = new CGameObject;
	pObj->SetName(L"Test Object");
	pObj->AddComponent(new CTransform);	// ��ġ��� ������Ʈ
	pObj->AddComponent(new CMeshRender);	// �׸��� ��� ������Ʈ

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 200.f));
	pObj->Transform()->SetRelativeScale(Vec3(100.f, 100.f, 1.f));


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
