#include "pch.h"

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

#include "CLevelSaveLoad.h"
#include "CLevel_2.h"
#include "CLevel_1.h"

void CreateLevel_2()
{
	//return;
	CreateGruntPrefab();

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	pCurLevel->SetName(L"Stage_2");
	pCurLevel->ChangeState(LEVEL_STATE::STOP);

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
	pMainCam->GetScript<CMainCameraScript>()->SetMapsize(Vec2(2176.f, 3500.f));

	pMainCam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pMainCam->Camera()->SetCameraIndex(0);		// MainCamera 로 설정
	pMainCam->Camera()->SetLayerMaskAll(true);	// 모든 레이어 체크
	pMainCam->Camera()->SetLayerMask(31, false);// UI Layer 는 렌더링하지 않는다.

	SpawnGameObject(pMainCam, Vec3(0.f, 0.f, 0.f), 0);

	// UI
	CGameObject* pUI = new CGameObject;
	pUI->SetName(L"UI");

	pUI->AddComponent(new CTransform);
	pUI->AddComponent(new CMeshRender);
	pUI->AddComponent(new CUIScript);

	pUI->Transform()->SetRelativePos(Vec3(GlobalData.Resolution.x / 2.f, GlobalData.Resolution.y / 2.f, 100.f));
	pUI->Transform()->SetRelativeScale(Vec3(1280.f, 46.f, 1.f));

	pUI->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pUI->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UIMtrl"));
	pUI->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\hud_collapse.png"));

	SpawnGameObject(pUI, Vec3(0, 360.f - 42.f, 200.f), L"ViewPort UI");

	// UI_Battery
	CGameObject* pBattery = new CGameObject;
	pBattery->SetName(L"UI_Battery");

	pBattery->AddComponent(new CTransform);
	pBattery->AddComponent(new CMeshRender);
	pBattery->AddComponent(new CBatteryScript);

	pBattery->Transform()->SetRelativeScale(Vec3(110.f, 20.f, 1.f));

	pBattery->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pBattery->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BatteryMtrl"));
	pBattery->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\effect_battery.png"));

	SpawnGameObject(pBattery, Vec3(-561.f, 318.f, 100.f), L"ViewPort UI");

	// UI_Timer
	CGameObject* pTimer = new CGameObject;
	pTimer->SetName(L"UI_Timer");

	pTimer->AddComponent(new CTransform);
	pTimer->AddComponent(new CMeshRender);
	pTimer->AddComponent(new CTimerScript);

	pTimer->Transform()->SetRelativeScale(Vec3(188.f, 22.f, 1.f));

	pTimer->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pTimer->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TimerMtrl"));
	pTimer->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\effect_timer.png"));

	SpawnGameObject(pTimer, Vec3(5.f, 324.f, 100.f), L"ViewPort UI");


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


	// 오브젝트 생성
	CGameObject* pParent = new CGameObject;
	pParent->SetName(L"Player");
	pParent->AddComponent(new CTransform);
	pParent->AddComponent(new CMeshRender);
	pParent->AddComponent(new CCollider2D);
	pParent->AddComponent(new CAnimator2D);
	pParent->AddComponent(new CPlayerScript);

	pParent->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));

	pParent->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_Pivot"));
	pParent->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimLightMtrl"));

	Vec2 _Resolution = GlobalData.Resolution;
	pParent->Collider2D()->SetAbsolute(true);
	pParent->Collider2D()->SetOffsetScale(Vec2(36.f, 70.f));
	//pParent->Collider2D()->SetOffsetPos(Vec2( -_Resolution.x/2.f, _Resolution.y/2.f + 35.f));

	Ptr<CTexture> pAnimAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\link.png");
	pParent->Animator2D()->CreateAnimation(L"WalkDown", pAnimAtlas, Vec2(0.f, 520.f), Vec2(120.f, 130.f), Vec2(300.f, 300.f), 10, 16);
	pParent->Animator2D()->Play(L"WalkDown", true);
	pParent->Animator2D()->Create_Player_Animation();
	SpawnGameObject(pParent, Vec3(375.f, -300.f, 500.f), L"PlayerHitBox");

	// Mouse
	CGameObject* pMouse = new CGameObject;
	pMouse->SetName(L"Mouse");

	pMouse->AddComponent(new CTransform);
	pMouse->AddComponent(new CMeshRender);
	pMouse->AddComponent(new CCollider2D);
	pMouse->AddComponent(new CMouseScript);

	pMouse->Transform()->SetRelativePos(Vec3(0.f, 250.f, 100.f));
	pMouse->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));


	pMouse->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMouse->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"MouseDMtrl"));
	pMouse->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\spr_cursor.png"));

	pMouse->Collider2D()->SetAbsolute(true);
	pMouse->Collider2D()->SetOffsetScale(Vec2(35.f, 35.f));

	SpawnGameObject(pMouse, Vec3(0.f, 0.f, 100.f), L"ViewPort UI");

	{
		//// Grunt_2	2층 정찰
		//CGameObject* pGrunt = CreateGrunt();
		//pGrunt->GetScript<CGruntScript>()->SetDir(ObjDir::Right);
		//pGrunt->GetScript<CGruntScript>()->SetBeginState(ObjState::Walk);
		//pGrunt->GetScript<CGruntScript>()->SetPatrol(true, 4.f);
		//SpawnGameObject(pGrunt, Vec3(1054, -383, 500.f), L"MonsterHitBox");

		//// Grunt_2	2층 방안
		//pGrunt = CreateGrunt();
		//pGrunt->GetScript<CGruntScript>()->SetDir(ObjDir::Right);
		//pGrunt->GetScript<CGruntScript>()->SetBeginState(ObjState::Idle);
		//SpawnGameObject(pGrunt, Vec3(338, -383, 500.f), L"MonsterHitBox");

	}

	// BackGround Object
	CGameObject* pBackGround = new CGameObject;
	pBackGround->SetName(L"BackGround");

	pBackGround->AddComponent(new CTransform);
	pBackGround->AddComponent(new CMeshRender);

	pBackGround->Transform()->SetRelativeScale(Vec3(2176.f, 3500.f, 1.f));

	pBackGround->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pBackGround->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	//pBackGround->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\map\\room_factory_2.png"));
	pBackGround->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\map\\stage2_bg_render.png"));

	//SpawnGameObject(pBackGround, Vec3(900.f, -392.f, 1000.f), L"Default");	//스테이지1 생성위치
	SpawnGameObject(pBackGround, Vec3(2176.f/2.f, -3500.f/2.f, 1000.f), L"Default");	// 스테이지2 생성위치


	//// Particle Object
	//CGameObject* pParticleObj = new CGameObject;
	//pParticleObj->SetName(L"ParticleObject");
	//pParticleObj->AddComponent(new CTransform);
	//pParticleObj->AddComponent(new CParticleSystem);

	//SpawnGameObject(pParticleObj, Vec3(0.f, 0.f, 0.f), 0);


	/*CGameObject* p
	cess = new CGameObject;
	pPostProcess->SetName(L"PostProcess");
	pPostProcess->AddComponent(new CTransform);
	pPostProcess->AddComponent(new CMeshRender);
	pPostProcess->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pPostProcess->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"GrayMtrl"));
	SpawnGameObject(pPostProcess, Vec3(0.f, 0.f, 0.f), 0);*/

	//CGameObject* pDistortion = new CGameObject;
	//pDistortion->SetName(L"Distortion");
	//pDistortion->AddComponent(new CTransform);
	//pDistortion->AddComponent(new CMeshRender);
	//pDistortion->Transform()->SetRelativeScale(200.f, 200.f, 1.f);
	//pDistortion->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pDistortion->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DistortionMtrl"));
	//SpawnGameObject(pDistortion, Vec3(0.f, 0.f, 500.f), 0);



	// 충돌 시킬 레이어 짝 지정
	CCollisionMgr::GetInst()->LayerCheck(L"PlayerProjectile", L"MonsterHitBox");
	CCollisionMgr::GetInst()->LayerCheck(L"MonsterProjectile", L"PlayerHitBox");
	CCollisionMgr::GetInst()->LayerCheck(L"MonsterView", L"PlayerHitBox");
	CCollisionMgr::GetInst()->LayerCheck(L"MonsterAttackRange", L"PlayerHitBox");
}
