#include "pch.h"
#include "CLevelMgr.h"

#include "CLevel.h"
#include "CLayer.h"

#include "CResMgr.h"
#include "CCollisionMgr.h"

#include "CGameObject.h"
#include "components.h"

#include "CPlayerScript.h"
#include "CCameraMoveScript.h"
#include "CMonsterScript.h"

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

	// Layer 이름설정
	m_pCurLevel->GetLayer(0)->SetName(L"Default");
	m_pCurLevel->GetLayer(1)->SetName(L"Tile");
	m_pCurLevel->GetLayer(2)->SetName(L"Player");
	m_pCurLevel->GetLayer(3)->SetName(L"Monster");
	m_pCurLevel->GetLayer(4)->SetName(L"PlayerProjectile");
	m_pCurLevel->GetLayer(5)->SetName(L"MonsterProjectile");


	// Main Camera Object 생성
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCamera");

	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);
	pMainCam->AddComponent(new CCameraMoveScript);
	
	pMainCam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pMainCam->Camera()->SetCameraIndex(0);		// MainCamera 로 설정
	pMainCam->Camera()->SetLayerMaskAll(true);	// 모든 레이어 체크

	m_pCurLevel->AddGameObject(pMainCam, 0, false);

	// 광원 추가
	CGameObject* pLightObj = new CGameObject;
	pLightObj->SetName(L"Point Light");

	pLightObj->AddComponent(new CTransform);
	pLightObj->AddComponent(new CLight2D);

	pLightObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));	
	pLightObj->Transform()->SetRelativeRot(Vec3(0.f, 0.f, XM_PI / 2.f));

	pLightObj->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	pLightObj->Light2D()->SetLightDiffuse(Vec3(1.f, 1.f, 1.f));
	pLightObj->Light2D()->SetRadius(500.f);

	m_pCurLevel->AddGameObject(pLightObj, 0, false);


	/*pLightObj = pLightObj->Clone();
	pLightObj->Light2D()->SetLightDiffuse(Vec3(0.3f, 0.3f, 1.f));
	pLightObj->Transform()->SetRelativePos(Vec3(200.f, 0.f, 0.f));
	m_pCurLevel->AddGameObject(pLightObj, 0, false);*/


	// 오브젝트 생성
	CGameObject* pParent = new CGameObject;
	pParent->SetName(L"Player");
	pParent->AddComponent(new CTransform);
	pParent->AddComponent(new CMeshRender);
	pParent->AddComponent(new CCollider2D);
	pParent->AddComponent(new CAnimator2D);
	pParent->AddComponent(new CPlayerScript);

	Ptr<CMesh> pMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");
	Ptr<CMaterial> Std2DMtrl = CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DLightMtrl");				

	pParent->Transform()->SetRelativePos(Vec3(0.f, 0.f, 500.f));
	pParent->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));

	pParent->MeshRender()->SetMesh(pMesh);
	pParent->MeshRender()->SetMaterial(Std2DMtrl);

	pParent->Collider2D()->SetAbsolute(true);
	pParent->Collider2D()->SetOffsetScale(Vec2(150.f, 150.f));

	Ptr<CTexture> pAnimAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"Link");
	pParent->Animator2D()->CreateAnimation(L"WalkDown", pAnimAtlas, Vec2(0.f, 520.f), Vec2(120.f, 130.f), Vec2(300.f, 300.f), 10, 16);
	pParent->Animator2D()->Play(L"WalkDown", true);

	m_pCurLevel->AddGameObject(pParent, L"Player", false);

	// Monster
	CGameObject* pMonster = new CGameObject;
	pMonster->SetName(L"Monster");

	pMonster->AddComponent(new CTransform);
	pMonster->AddComponent(new CMeshRender);
	pMonster->AddComponent(new CCollider2D);
	pMonster->AddComponent(new CMonsterScript);

	pMonster->Transform()->SetRelativePos(Vec3(0.f, 250.f, 100.f));
	pMonster->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));
	
	pMonster->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMonster->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	pMonster->Collider2D()->SetAbsolute(true);
	pMonster->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));

	m_pCurLevel->AddGameObject(pMonster, L"Monster", false);


	// TileMap Object
	CGameObject* pTileMap = new CGameObject;

	pTileMap->AddComponent(new CTransform);
	pTileMap->AddComponent(new CTileMap);

	pTileMap->Transform()->SetRelativePos(Vec3(0.f, 0.f, 600.f));
	pTileMap->Transform()->SetRelativeScale(Vec3(500.f, 500.f, 1.f));

	pTileMap->TileMap()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"TileAtlasTex"));
	pTileMap->TileMap()->SetSliceSize(Vec2(0.125f, 0.166f));
	pTileMap->TileMap()->SetTileCount(8, 8);
	
	m_pCurLevel->AddGameObject(pTileMap, L"Tile", false);

	// 충돌 시킬 레이어 짝 지정
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Monster");
}

void CLevelMgr::tick()
{
	m_pCurLevel->clear();

	m_pCurLevel->tick();
	m_pCurLevel->finaltick();
}