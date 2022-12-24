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

	// Main Camera Object 생성
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCamera");

	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);
	pMainCam->AddComponent(new CCameraMoveScript);

	pMainCam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pMainCam->Camera()->SetCameraIndex(0);		// MainCamera 로 설정
	pMainCam->Camera()->SetLayerMaskAll(true);	// 모든 레이어 체크

	m_pCurLevel->AddGameObject(pMainCam, 0);



	// 오브젝트 생성
	CGameObject* pObj = new CGameObject;
	pObj->SetName(L"Player");
	pObj->AddComponent(new CTransform);	// 위치담당 컴포넌트
	pObj->AddComponent(new CMeshRender);	// 그리기 담당 컴포넌트
	pObj->AddComponent(new CPlayerScript);	// 플레이어 스크립트 컴포넌트

	// 리소스 매니저에서 메쉬, 재질, 텍스쳐를 찾아와서 넣어준다.
	Ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"CircleMesh");
	Ptr<CMaterial> Std2DMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl");

	Ptr<CTexture> PlayerTex = CResMgr::GetInst()->FindRes<CTexture>(L"CharacterTex");
	Std2DMtrl->SetTexParam(TEX_0, PlayerTex);

	pObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));
	pObj->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));

	pObj->MeshRender()->SetMesh(pMesh);
	pObj->MeshRender()->SetMaterial(Std2DMtrl);

	m_pCurLevel->AddGameObject(pObj, 1);

	// Test 오브젝트
	pObj = new CGameObject;
	pObj->SetName(L"Test Object");
	pObj->AddComponent(new CTransform);	// 위치담당 컴포넌트
	pObj->AddComponent(new CMeshRender);	// 그리기 담당 컴포넌트

	pObj->Transform()->SetRelativePos(Vec3(200.f, 0.f, 200.f));
	pObj->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));


	pObj->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pObj->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TestMtrl"));

	m_pCurLevel->AddGameObject(pObj, 0);
}

void CLevelMgr::tick()
{
	m_pCurLevel->tick();
	m_pCurLevel->finaltick();
}
