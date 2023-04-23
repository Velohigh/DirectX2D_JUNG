#include "pch.h"
#include "CLevel_1.h"
#include <Engine\CPrefab.h>

#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>
#include <Engine\CLayer.h>
#include <Engine\CGameObject.h>
#include <Engine\components.h>

#include <Engine\CResMgr.h>
#include <Engine\CCollisionMgr.h>


#include <Script\CPlayerScript.h>
#include <Script\CMonsterScript.h>
#include <Script\CMainCameraScript.h>
#include <Script\CMouseScript.h>
#include <Script\CGruntScript.h>
#include <Script\CPompScript.h>
#include <Script\CGangsterScript.h>
#include <Script\CUIScript.h>
#include <Script\CBatteryScript.h>
#include <Script\CTimerScript.h>
#include <Script\CRewinderScript.h>
#include <Script\CTitleScript.h>

#include "CLevelSaveLoad.h"
#include "CLevel_2.h"

void CreateLevel_1()
{
	//return;
	//CreateGruntPrefab();

	// 배경음 재생
	Ptr<CSound> pBGM = CResMgr::GetInst()->FindRes<CSound>(L"sound\\song_rainonbrick.ogg");
	pBGM->Play(9999, 1.f, true);

	Ptr<CSound> pBGM2 = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_ambience_rain_title_01.wav");
	pBGM2->Play(9999, 0.4f, true);


	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	pCurLevel->SetName(L"Title");
	pCurLevel->ChangeState(LEVEL_STATE::PLAY);
	pCurLevel->SetBgm(pBGM.Get());

	// Layer 이름설정
	pCurLevel->GetLayer(0)->SetName(L"Default");
	pCurLevel->GetLayer(1)->SetName(L"Tile");
	pCurLevel->GetLayer(2)->SetName(L"PlayerHitBox");
	pCurLevel->GetLayer(3)->SetName(L"MonsterHitBox");
	pCurLevel->GetLayer(4)->SetName(L"PlayerProjectile");
	pCurLevel->GetLayer(5)->SetName(L"MonsterProjectile");
	pCurLevel->GetLayer(6)->SetName(L"MonsterView");
	pCurLevel->GetLayer(7)->SetName(L"MonsterAttackRange");
	pCurLevel->GetLayer(31)->SetName(L"ViewPort UI");



	// Main Camera Object 생성
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCamera");

	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);
	pMainCam->AddComponent(new CMainCameraScript);
	pMainCam->GetScript<CMainCameraScript>()->SetMapsize(Vec2(1800.f, 784.f));

	pMainCam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pMainCam->Camera()->SetCameraIndex(0);		// MainCamera 로 설정
	pMainCam->Camera()->SetLayerMaskAll(true);	// 모든 레이어 체크
	pMainCam->Camera()->SetLayerMask(31, false);// UI Layer 는 렌더링하지 않는다.

	SpawnGameObject(pMainCam, Vec3(0.f, 0.f, 0.f), 0);

	// UI cameara
	CGameObject* pUICam = new CGameObject;
	pUICam->SetName(L"UICamera");

	pUICam->AddComponent(new CTransform);
	pUICam->AddComponent(new CCamera);

	pUICam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pUICam->Camera()->SetCameraIndex(1);		// SubCamera 로 설정
	pUICam->Camera()->SetLayerMask(31, true);	// 모든 레이어 체크

	SpawnGameObject(pUICam, Vec3(0.f, 0.f, 0.f), 0);


	// 광원 추가
	CGameObject* pLightObj = new CGameObject;
	pLightObj->SetName(L"Point Light");

	pLightObj->AddComponent(new CTransform);
	pLightObj->AddComponent(new CLight2D);

	pLightObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pLightObj->Transform()->SetRelativeRot(Vec3(0.f, 0.f, XM_PI / 2.f));

	pLightObj->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	pLightObj->Light2D()->SetLightDiffuse(Vec3(1.f, 1.f, 1.f));
	pLightObj->Light2D()->SetRadius(20000.f);


	SpawnGameObject(pLightObj, Vec3(0.f, 0.f, 0.f), 0);


	// BackGround Object
	CGameObject* pBackGround = new CGameObject;
	pBackGround->SetName(L"BackGround");

	pBackGround->AddComponent(new CTransform);
	pBackGround->AddComponent(new CMeshRender);
	pBackGround->AddComponent(new CTitleScript);

	pBackGround->Transform()->SetRelativeScale(Vec3(1280, 1440.f, 1.f));

	pBackGround->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pBackGround->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BackGroundMtrl"));
	pBackGround->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\title\\spr_title_background.png"));

	pBackGround->GetScript<CTitleScript>()->SetBgm(pBGM2.Get());

	SpawnGameObject(pBackGround, Vec3(0.f, 360.f, 1000.f), L"ViewPort UI");	//스테이지1 생성위치

	// title_fence Object
	CGameObject* ptitlefence = new CGameObject;
	ptitlefence->SetName(L"title_fence");

	ptitlefence->AddComponent(new CTransform);
	ptitlefence->AddComponent(new CMeshRender);

	ptitlefence->Transform()->SetRelativeScale(Vec3(1280, 1440.f, 1.f));

	ptitlefence->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	ptitlefence->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"title_fenceMtrl"));
	ptitlefence->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\title\\spr_title_fence.png"));

	SpawnGameObject(ptitlefence, Vec3(0.f, 360.f, 900.f), L"ViewPort UI");	//스테이지1 생성위치

	// title_grass Object
	CGameObject* ptitlegrass = new CGameObject;
	ptitlegrass->SetName(L"title_grass");

	ptitlegrass->AddComponent(new CTransform);
	ptitlegrass->AddComponent(new CMeshRender);

	ptitlegrass->Transform()->SetRelativeScale(Vec3(1280, 240, 1.f));

	ptitlegrass->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_Pivot"));
	ptitlegrass->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"title_grassMtrl"));
	ptitlegrass->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\title\\spr_title_grass.png"));

	SpawnGameObject(ptitlegrass, Vec3(0.f, -360.f, 800.f), L"ViewPort UI");	//스테이지1 생성위치



	// title_plants Object
	CGameObject* ptitleplants = new CGameObject;
	ptitleplants->SetName(L"title_plants");

	ptitleplants->AddComponent(new CTransform);
	ptitleplants->AddComponent(new CMeshRender);
	ptitleplants->AddComponent(new CAnimator2D);


	ptitleplants->Transform()->SetRelativeScale(Vec3(1280, 360, 1.f));

	ptitleplants->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_Pivot"));
	ptitleplants->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"title_PlantsMtrl"));
	ptitleplants->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\title\\spr_title_grass.png"));

	ptitleplants->Animator2D()->CreateFolderAnimation(L"texture\\title\\spr_title_plants", 12, 11.1f);
	ptitleplants->Animator2D()->Play(L"texture\\title\\spr_title_plants", true);

	SpawnGameObject(ptitleplants, Vec3(0.f, -360.f, 700.f), L"ViewPort UI");	//스테이지1 생성위치

	// title_Big1 Object
	CGameObject* ptitlebig1 = new CGameObject;
	ptitlebig1->SetName(L"title_Big1");

	ptitlebig1->AddComponent(new CTransform);
	ptitlebig1->AddComponent(new CMeshRender);

	ptitlebig1->Transform()->SetRelativeScale(Vec3(319, 180, 1.f));

	ptitlebig1->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	ptitlebig1->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"title_Big1Mtrl"));
	ptitlebig1->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\title\\spr_titlegraphic_big_0.png"));

	SpawnGameObject(ptitlebig1, Vec3(0.f, 0.f, 500.f), L"ViewPort UI");	//스테이지1 생성위치

	// title_Big1_ZER Object
	CGameObject* ptitlebig1_ZER = new CGameObject;
	ptitlebig1_ZER->SetName(L"title_Big1_ZER");

	ptitlebig1_ZER->AddComponent(new CTransform);
	ptitlebig1_ZER->AddComponent(new CMeshRender);

	ptitlebig1_ZER->Transform()->SetRelativeScale(Vec3(251, 206, 1.f));

	ptitlebig1_ZER->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	ptitlebig1_ZER->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"title_Big1ZERMtrl"));
	ptitlebig1_ZER->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\title\\spr_titlegraphic_big_1.png"));

	SpawnGameObject(ptitlebig1_ZER, Vec3(-44, -2.f, 400.f), L"ViewPort UI");	//스테이지1 생성위치


	// title_Big1_G Object
	CGameObject* ptitlebig1_G = new CGameObject;
	ptitlebig1_G->SetName(L"title_Big1_G");

	ptitlebig1_G->AddComponent(new CTransform);
	ptitlebig1_G->AddComponent(new CMeshRender);

	ptitlebig1_G->Transform()->SetRelativeScale(Vec3(103, 208, 1.f));

	ptitlebig1_G->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	ptitlebig1_G->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"title_Big1GMtrl"));
	ptitlebig1_G->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\title\\spr_titlegraphic_big_2.png"));

	SpawnGameObject(ptitlebig1_G, Vec3(117.f, 0.f, 395.f), L"ViewPort UI");	//스테이지1 생성위치


	// title_Big2 Object
	CGameObject* ptitlebig2 = new CGameObject;
	ptitlebig2->SetName(L"title_Big2");

	ptitlebig2->AddComponent(new CTransform);
	ptitlebig2->AddComponent(new CMeshRender);

	ptitlebig2->Transform()->SetRelativeScale(Vec3(298, 210, 1.f));

	ptitlebig2->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	ptitlebig2->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"title_Big2Mtrl"));
	ptitlebig2->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\title\\spr_titlegraphic_big2.png"));

	SpawnGameObject(ptitlebig2, Vec3(1.f, 66.f, 300.f), L"ViewPort UI");	//스테이지1 생성위치


	// 충돌 시킬 레이어 짝 지정
	CCollisionMgr::GetInst()->LayerCheck(L"PlayerProjectile", L"MonsterHitBox");
	CCollisionMgr::GetInst()->LayerCheck(L"MonsterProjectile", L"PlayerHitBox");
	CCollisionMgr::GetInst()->LayerCheck(L"MonsterProjectile", L"PlayerProjectile");
	CCollisionMgr::GetInst()->LayerCheck(L"MonsterView", L"PlayerHitBox");
	CCollisionMgr::GetInst()->LayerCheck(L"MonsterView", L"MonsterHitBox");
	CCollisionMgr::GetInst()->LayerCheck(L"MonsterAttackRange", L"PlayerHitBox");
}

void CreateGruntPrefab()
{
	// 그런트 프리팹 등록
	CGameObject* pGrunt = new CGameObject;
	pGrunt->SetName(L"Grunt");
	pGrunt->AddComponent(new CTransform);
	pGrunt->AddComponent(new CMeshRender);
	pGrunt->AddComponent(new CCollider2D);
	pGrunt->AddComponent(new CAnimator2D);
	pGrunt->AddComponent(new CGruntScript);

	pGrunt->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));

	pGrunt->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_Pivot"));
	pGrunt->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"GruntMtrl"));

	pGrunt->Collider2D()->SetAbsolute(true);
	pGrunt->Collider2D()->SetOffsetScale(Vec2(36.f, 70.f));

	pGrunt->Animator2D()->Create_Grunt_Animation();
	pGrunt->Animator2D()->Play(L"texture\\grunt\\spr_grunt_idle", true);
	//SpawnGameObject(pGrunt, Vec3(230.f, -671.f, 500.f), L"MonsterHitBox");

	Ptr<CPrefab> GruntPrefab = new CPrefab;
	GruntPrefab->RegisterProtoObject(pGrunt);
	CResMgr::GetInst()->AddRes<CPrefab>(L"GruntPrefab", GruntPrefab);
}


