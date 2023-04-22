#include "pch.h"
#include "CLevel_3.h"
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

#include "CLevelSaveLoad.h"
#include "CLevel_1.h"
#include "CLevel_2.h"

void CreateLevel_3()
{

	//return;

	// ¹è°æÀ½ Àç»ý
	Ptr<CSound> pBGM = CResMgr::GetInst()->FindRes<CSound>(L"sound\\song_sneaky_driver.ogg");
	pBGM->Play(999, 1.f, true);

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	pCurLevel->SetName(L"Stage_3");
	pCurLevel->ChangeState(LEVEL_STATE::STOP);
	pCurLevel->SetBgm(pBGM.Get());


	// Layer ÀÌ¸§¼³Á¤
	pCurLevel->GetLayer(0)->SetName(L"Default");
	pCurLevel->GetLayer(1)->SetName(L"Tile");
	pCurLevel->GetLayer(2)->SetName(L"PlayerHitBox");
	pCurLevel->GetLayer(3)->SetName(L"MonsterHitBox");
	pCurLevel->GetLayer(4)->SetName(L"PlayerProjectile");
	pCurLevel->GetLayer(5)->SetName(L"MonsterProjectile");
	pCurLevel->GetLayer(6)->SetName(L"MonsterView");
	pCurLevel->GetLayer(7)->SetName(L"MonsterAttackRange");
	pCurLevel->GetLayer(31)->SetName(L"ViewPort UI");

	//// Main Camera Object »ý¼º
	//CGameObject* pMainCam = new CGameObject;
	//pMainCam->SetName(L"MainCamera");

	//pMainCam->AddComponent(new CTransform);
	//pMainCam->AddComponent(new CCamera);
	//pMainCam->AddComponent(new CMainCameraScript);
	//pMainCam->GetScript<CMainCameraScript>()->SetMapsize(Vec2(4288, 1200));

	//pMainCam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	//pMainCam->Camera()->SetCameraIndex(0);		// MainCamera ·Î ¼³Á¤
	//pMainCam->Camera()->SetLayerMaskAll(true);	// ¸ðµç ·¹ÀÌ¾î Ã¼Å©
	//pMainCam->Camera()->SetLayerMask(31, false);// UI Layer ´Â ·»´õ¸µÇÏÁö ¾Ê´Â´Ù.

	//SpawnGameObject(pMainCam, Vec3(0.f, 0.f, 0.f), 0);

	//// UI cameara
	//CGameObject* pUICam = new CGameObject;
	//pUICam->SetName(L"UICamera");

	//pUICam->AddComponent(new CTransform);
	//pUICam->AddComponent(new CCamera);

	//pUICam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	//pUICam->Camera()->SetCameraIndex(1);		// SubCamera ·Î ¼³Á¤
	//pUICam->Camera()->SetLayerMask(31, true);	// ¸ðµç ·¹ÀÌ¾î Ã¼Å©

	//SpawnGameObject(pUICam, Vec3(0.f, 0.f, 0.f), 0);

	//// ±¤¿ø Ãß°¡
	//CGameObject* pLightObj = new CGameObject;
	//pLightObj->SetName(L"Point Light");

	//pLightObj->AddComponent(new CTransform);
	//pLightObj->AddComponent(new CLight2D);

	//pLightObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	//pLightObj->Transform()->SetRelativeRot(Vec3(0.f, 0.f, XM_PI / 2.f));

	//pLightObj->Light2D()->SetLightType(LIGHT_TYPE::DIRECTIONAL);
	//pLightObj->Light2D()->SetLightDiffuse(Vec3(1.f, 1.f, 1.f));
	//pLightObj->Light2D()->SetRadius(20000.f);

	//SpawnGameObject(pLightObj, Vec3(0.f, 0.f, 0.f), 0);


	// ¿ÀºêÁ§Æ® »ý¼º
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
	SpawnGameObject(pParent, Vec3(60.f, -755.f, 500.f), L"PlayerHitBox");

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
		// Grunt_1	ÁÂ 2Ãþ ÁÂÃø
		CGameObject* pGrunt = CreateGrunt();
		pGrunt->GetScript<CGruntScript>()->SetBeginDir(ObjDir::Right);
		pGrunt->GetScript<CGruntScript>()->SetBeginState(ObjState::Idle);
		pGrunt->GetScript<CGruntScript>()->SetPatrol(false, 3.f);
		SpawnGameObject(pGrunt, Vec3(611, -387.f, 500.f), L"MonsterHitBox");

		// Grunt_1	ÁÂ 2Ãþ »¡°£¶¥
		pGrunt = CreateGrunt();
		pGrunt->GetScript<CGruntScript>()->SetBeginDir(ObjDir::Left);
		pGrunt->GetScript<CGruntScript>()->SetBeginState(ObjState::Idle);
		pGrunt->GetScript<CGruntScript>()->SetPatrol(false, 3.f);
		SpawnGameObject(pGrunt, Vec3(737, -387.f, 500.f), L"MonsterHitBox");

		// Grunt_1	Áß¾Ó Á¤Âû
		pGrunt = CreateGrunt();
		pGrunt->GetScript<CGruntScript>()->SetBeginDir(ObjDir::Right);
		pGrunt->GetScript<CGruntScript>()->SetBeginState(ObjState::Walk);
		pGrunt->GetScript<CGruntScript>()->SetPatrol(true, 3.f);
		SpawnGameObject(pGrunt, Vec3(2272, -515, 500.f), L"MonsterHitBox");

		// Grunt_1	Áß¾Ó ¿ìÃø
		pGrunt = CreateGrunt();
		pGrunt->GetScript<CGruntScript>()->SetBeginDir(ObjDir::Left);
		pGrunt->GetScript<CGruntScript>()->SetBeginState(ObjState::Idle);
		pGrunt->GetScript<CGruntScript>()->SetPatrol(false, 3.f);
		SpawnGameObject(pGrunt, Vec3(3422, -515, 500.f), L"MonsterHitBox");

		// Grunt_1	ÁÂ 1Ãþ
		pGrunt = CreateGrunt();
		pGrunt->GetScript<CGruntScript>()->SetBeginDir(ObjDir::Right);
		pGrunt->GetScript<CGruntScript>()->SetBeginState(ObjState::Idle);
		pGrunt->GetScript<CGruntScript>()->SetPatrol(false, 3.f);
		SpawnGameObject(pGrunt, Vec3(1155, -771, 500.f), L"MonsterHitBox");

		// Grunt_1	ÁÂ 1Ãþ
		pGrunt = CreateGrunt();
		pGrunt->GetScript<CGruntScript>()->SetBeginDir(ObjDir::Right);
		pGrunt->GetScript<CGruntScript>()->SetBeginState(ObjState::Walk);
		pGrunt->GetScript<CGruntScript>()->SetPatrol(true, 3.f);
		SpawnGameObject(pGrunt, Vec3(3110, -771, 500.f), L"MonsterHitBox");
	}

	{
		// °»½ºÅÍ ÁÂ 2Ãþ ¿ìÃø³¡
		CGameObject* pGangster = CreateGangster();
		pGangster->GetScript<CGangsterScript>()->SetBeginDir(ObjDir::Left);
		pGangster->GetScript<CGangsterScript>()->SetBeginState(ObjState::Idle);
		pGangster->GetScript<CGangsterScript>()->SetPatrol(false, 3.f);
		SpawnGameObject(pGangster, Vec3(1483, -387.f, 500.f), L"MonsterHitBox");


		// °»½ºÅÍ Áß¾Ó ÁÂÃø³¡
		pGangster = CreateGangster();
		pGangster->GetScript<CGangsterScript>()->SetBeginDir(ObjDir::Right);
		pGangster->GetScript<CGangsterScript>()->SetBeginState(ObjState::Idle);
		pGangster->GetScript<CGangsterScript>()->SetPatrol(false, 3.f);
		SpawnGameObject(pGangster, Vec3(1840, -515.f, 500.f), L"MonsterHitBox");

		// °»½ºÅÍ Áß¾Ó Á¤Âû
		pGangster = CreateGangster();
		pGangster->GetScript<CGangsterScript>()->SetBeginDir(ObjDir::Left);
		pGangster->GetScript<CGangsterScript>()->SetBeginState(ObjState::Walk);
		pGangster->GetScript<CGangsterScript>()->SetPatrol(true, 3.f);
		SpawnGameObject(pGangster, Vec3(2788, -515.f, 500.f), L"MonsterHitBox");

		// °»½ºÅÍ Áß¾Ó Á¤Âû
		pGangster = CreateGangster();
		pGangster->GetScript<CGangsterScript>()->SetBeginDir(ObjDir::Left);
		pGangster->GetScript<CGangsterScript>()->SetBeginState(ObjState::Walk);
		pGangster->GetScript<CGangsterScript>()->SetPatrol(true, 3.f);
		SpawnGameObject(pGangster, Vec3(3567, -515.f, 500.f), L"MonsterHitBox");

		// °»½ºÅÍ Áß¾Ó ¿ìÃø ³¡
		pGangster = CreateGangster();
		pGangster->GetScript<CGangsterScript>()->SetBeginDir(ObjDir::Left);
		pGangster->GetScript<CGangsterScript>()->SetBeginState(ObjState::Idle);
		pGangster->GetScript<CGangsterScript>()->SetPatrol(false, 3.f);
		SpawnGameObject(pGangster, Vec3(3684, -515.f, 500.f), L"MonsterHitBox");

		// °»½ºÅÍ ÁÂ 1Ãþ ¿ìÃø ³¡
		pGangster = CreateGangster();
		pGangster->GetScript<CGangsterScript>()->SetBeginDir(ObjDir::Left);
		pGangster->GetScript<CGangsterScript>()->SetBeginState(ObjState::Idle);
		pGangster->GetScript<CGangsterScript>()->SetPatrol(false, 3.f);
		SpawnGameObject(pGangster, Vec3(1600, -771.f, 500.f), L"MonsterHitBox");

		// °»½ºÅÍ ¿ì 1Ãþ ¿ìÃø ³¡
		pGangster = CreateGangster();
		pGangster->GetScript<CGangsterScript>()->SetBeginDir(ObjDir::Left);
		pGangster->GetScript<CGangsterScript>()->SetBeginState(ObjState::Idle);
		pGangster->GetScript<CGangsterScript>()->SetPatrol(false, 3.f);
		SpawnGameObject(pGangster, Vec3(4000, -771.f, 500.f), L"MonsterHitBox");

		// °»½ºÅÍ ¿ì 1Ãþ
		pGangster = CreateGangster();
		pGangster->GetScript<CGangsterScript>()->SetBeginDir(ObjDir::Left);
		pGangster->GetScript<CGangsterScript>()->SetBeginState(ObjState::Idle);
		pGangster->GetScript<CGangsterScript>()->SetPatrol(false, 3.f);
		SpawnGameObject(pGangster, Vec3(3425, -771.f, 500.f), L"MonsterHitBox");



		// ÆûÇÁ ÁÂ 2Ãþ
		CGameObject* pPomp = CreatePomp();
		pPomp->GetScript<CPompScript>()->SetBeginDir(ObjDir::Left);
		pPomp->GetScript<CPompScript>()->SetBeginState(ObjState::Walk);
		pPomp->GetScript<CPompScript>()->SetPatrol(true, 3.f);
		SpawnGameObject(pPomp, Vec3(1420, -387.f, 500.f), L"MonsterHitBox");

		// ÆûÇÁ Áß¾Ó 2Ãþ
		pPomp = CreatePomp();
		pPomp->GetScript<CPompScript>()->SetBeginDir(ObjDir::Left);
		pPomp->GetScript<CPompScript>()->SetBeginState(ObjState::Idle);
		pPomp->GetScript<CPompScript>()->SetPatrol(false, 3.f);
		SpawnGameObject(pPomp, Vec3(3220, -515.f, 500.f), L"MonsterHitBox");

		// ÆûÇÁ ÁÂ 1Ãþ Á¤Âû
		pPomp = CreatePomp();
		pPomp->GetScript<CPompScript>()->SetBeginDir(ObjDir::Left);
		pPomp->GetScript<CPompScript>()->SetBeginState(ObjState::Walk);
		pPomp->GetScript<CPompScript>()->SetPatrol(true, 4.f);
		SpawnGameObject(pPomp, Vec3(1530, -771.f, 500.f), L"MonsterHitBox");

		// ÆûÇÁ ¿ì 1Ãþ Áß¾Ó
		pPomp = CreatePomp();
		pPomp->GetScript<CPompScript>()->SetBeginDir(ObjDir::Right);
		pPomp->GetScript<CPompScript>()->SetBeginState(ObjState::Idle);
		pPomp->GetScript<CPompScript>()->SetPatrol(false, 4.f);
		SpawnGameObject(pPomp, Vec3(3645, -771.f, 500.f), L"MonsterHitBox");

		// ÆûÇÁ ¿ì 1Ãþ Áß¾Ó
		pPomp = CreatePomp();
		pPomp->GetScript<CPompScript>()->SetBeginDir(ObjDir::Left);
		pPomp->GetScript<CPompScript>()->SetBeginState(ObjState::Idle);
		pPomp->GetScript<CPompScript>()->SetPatrol(false, 4.f);
		SpawnGameObject(pPomp, Vec3(3710, -771.f, 500.f), L"MonsterHitBox");

	}

	// BackGround Object
	CGameObject* pBackGround = new CGameObject;
	pBackGround->SetName(L"BackGround");

	pBackGround->AddComponent(new CTransform);
	pBackGround->AddComponent(new CMeshRender);

	pBackGround->Transform()->SetRelativeScale(Vec3(4288.f, 1200.f, 1.f));

	pBackGround->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pBackGround->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	pBackGround->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\map\\stage3_bg_render.png"));

	SpawnGameObject(pBackGround, Vec3(2144, -600.f, 1000.f), L"Default");	//½ºÅ×ÀÌÁö1 »ý¼ºÀ§Ä¡

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

	pBattery->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_PivotRight"));
	pBattery->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BatteryMtrl"));
	pBattery->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\effect_battery.png"));

	SpawnGameObject(pBattery, Vec3(-617.f, 318.f, 100.f), L"ViewPort UI");

	// UI_Timer
	CGameObject* pTimer = new CGameObject;
	pTimer->SetName(L"UI_Timer");

	pTimer->AddComponent(new CTransform);
	pTimer->AddComponent(new CMeshRender);
	pTimer->AddComponent(new CTimerScript);

	pTimer->Transform()->SetRelativeScale(Vec3(188.f, 22.f, 1.f));

	pTimer->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_PivotRight"));
	pTimer->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TimerMtrl"));
	pTimer->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\effect_timer.png"));

	SpawnGameObject(pTimer, Vec3(-89.f, 324.f, 100.f), L"ViewPort UI");

	// UI_Katana
	CGameObject* pKatana = new CGameObject;
	pKatana->SetName(L"UI_Katana");

	pKatana->AddComponent(new CTransform);
	pKatana->AddComponent(new CMeshRender);

	pKatana->Transform()->SetRelativeScale(Vec3(40.f, 40.f, 1.f));

	pKatana->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_PivotRight"));
	pKatana->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"KatanaMtrl"));
	pKatana->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\spr_katanaicons.png"));

	SpawnGameObject(pKatana, Vec3(507.f, 318.f, 100.f), L"ViewPort UI");



	//// Particle Object
	//CGameObject* pParticleObj = new CGameObject;
	//pParticleObj->SetName(L"ParticleObject");
	//pParticleObj->AddComponent(new CTransform);
	//pParticleObj->AddComponent(new CParticleSystem);

	//SpawnGameObject(pParticleObj, Vec3(0.f, 0.f, 0.f), 0);


	/*CGameObject* pPostProcess = new CGameObject;
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



	// Ãæµ¹ ½ÃÅ³ ·¹ÀÌ¾î Â¦ ÁöÁ¤
	CCollisionMgr::GetInst()->LayerCheck(L"PlayerProjectile", L"MonsterHitBox");
	CCollisionMgr::GetInst()->LayerCheck(L"MonsterProjectile", L"PlayerHitBox");
	CCollisionMgr::GetInst()->LayerCheck(L"MonsterProjectile", L"PlayerProjectile");
	CCollisionMgr::GetInst()->LayerCheck(L"MonsterView", L"PlayerHitBox");
	CCollisionMgr::GetInst()->LayerCheck(L"MonsterView", L"MonsterHitBox");
	CCollisionMgr::GetInst()->LayerCheck(L"MonsterAttackRange", L"PlayerHitBox");
}
