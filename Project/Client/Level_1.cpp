#include "pch.h"
#include "Level_1.h"

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

#include "CLevelSaveLoad.h"



void CreateLevel_1()
{
	//return;

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	pCurLevel->ChangeState(LEVEL_STATE::STOP);

	// Layer 이름설정
	pCurLevel->GetLayer(0)->SetName(L"Default");
	pCurLevel->GetLayer(1)->SetName(L"Tile");
	pCurLevel->GetLayer(2)->SetName(L"Player");
	pCurLevel->GetLayer(3)->SetName(L"Monster");
	pCurLevel->GetLayer(4)->SetName(L"PlayerProjectile");
	pCurLevel->GetLayer(5)->SetName(L"MonsterProjectile");
	pCurLevel->GetLayer(31)->SetName(L"ViewPort UI");


	// Main Camera Object 생성
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCamera");

	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);
	pMainCam->AddComponent(new CMainCameraScript);
	CMainCameraScript* pScript = pMainCam->GetScript<CMainCameraScript>();
	pScript->SetMapsize(Vec2(1800.f, 784.f));

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

	pLightObj->Light2D()->SetLightType(LIGHT_TYPE::POINT);
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

	pParent->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pParent->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimLightMtrl"));

	pParent->Collider2D()->SetAbsolute(true);
	pParent->Collider2D()->SetOffsetScale(Vec2(150.f, 150.f));

	Ptr<CTexture> pAnimAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\link.png");
	pParent->Animator2D()->CreateAnimation(L"WalkDown", pAnimAtlas, Vec2(0.f, 520.f), Vec2(120.f, 130.f), Vec2(300.f, 300.f), 10, 16);
	pParent->Animator2D()->Play(L"WalkDown", true);
	CreatePlayerAnimation(pParent);
	pParent->SetColMapTexture((CResMgr::GetInst()->FindRes<CTexture>(L"texture\\map\\room_factory_2_ColMap.png")).Get());
	SpawnGameObject(pParent, Vec3(230.f, -671.f, 500.f), 0);


	/*CGameObject* pChild = new CGameObject;

	pChild->SetName(L"Child");
	pChild->AddComponent(new CTransform);
	pChild->AddComponent(new CMeshRender);

	pChild->Transform()->SetAbsolute(true);
	pChild->Transform()->SetRelativePos(200.f, 0.f, 0.f);
	pChild->Transform()->SetRelativeScale(100.f, 100.f, 1.f);

	pChild->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pChild->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DLightMtrl"));
	pChild->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\Fighter.bmp"));

	pParent->AddChild(pChild);*/

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

	SpawnGameObject(pMonster, Vec3(0.f, 250.f, 100.f), L"Monster");

	// Mouse
	CGameObject* pMouse = new CGameObject;
	pMouse->SetName(L"Mouse");

	pMouse->AddComponent(new CTransform);
	pMouse->AddComponent(new CMeshRender);
	pMouse->AddComponent(new CCollider2D);

	pMouse->Transform()->SetRelativePos(Vec3(0.f, 250.f, 100.f));
	pMouse->Transform()->SetRelativeScale(Vec3(25.f, 25.f, 1.f));

	pMouse->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMouse->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"MouseDMtrl"));
	pMouse->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\spr_cursor.png"));

	pMouse->Collider2D()->SetAbsolute(true);
	pMouse->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));

	SpawnGameObject(pMonster, Vec3(0.f, 250.f, 100.f), L"Mouse");


	//// TileMap Object
	//CGameObject* pTileMap = new CGameObject;
	//pTileMap->SetName(L"TileMap");

	//pTileMap->AddComponent(new CTransform);
	//pTileMap->AddComponent(new CTileMap);

	//pTileMap->Transform()->SetRelativePos(Vec3(0.f, 0.f, 600.f));
	//pTileMap->Transform()->SetRelativeScale(Vec3(500.f, 500.f, 1.f));

	//pTileMap->TileMap()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\TILE.bmp"));
	//pTileMap->TileMap()->SetSliceSize(Vec2(0.125f, 0.166f));
	//pTileMap->TileMap()->SetTileCount(8, 8);

	//SpawnGameObject(pTileMap, Vec3(0.f, 0.f, 600.f), L"Tile");

	// BackGround Object
	CGameObject* pBackGround = new CGameObject;
	pBackGround->SetName(L"BackGround");

	pBackGround->AddComponent(new CTransform);
	pBackGround->AddComponent(new CMeshRender);

	pBackGround->Transform()->SetRelativeScale(Vec3(1800.f, 784.f, 1.f));
	//pBackGround->Transform()->SetRelativePos(Vec3(900.f, 392.f, 600.f));

	pBackGround->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pBackGround->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	pBackGround->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\map\\room_factory_2.png"));

	SpawnGameObject(pBackGround, Vec3(900.f, -392.f, 1000.f), L"Default");


	// Particle Object
	CGameObject* pParticleObj = new CGameObject;
	pParticleObj->SetName(L"ParticleObject");
	pParticleObj->AddComponent(new CTransform);
	pParticleObj->AddComponent(new CParticleSystem);

	SpawnGameObject(pParticleObj, Vec3(0.f, 0.f, 0.f), 0);


	/*CGameObject* pPostProcess = new CGameObject;
	pPostProcess->SetName(L"PostProcess");
	pPostProcess->AddComponent(new CTransform);
	pPostProcess->AddComponent(new CMeshRender);
	pPostProcess->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pPostProcess->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"GrayMtrl"));
	SpawnGameObject(pPostProcess, Vec3(0.f, 0.f, 0.f), 0);*/

	CGameObject* pDistortion = new CGameObject;
	pDistortion->SetName(L"Distortion");
	pDistortion->AddComponent(new CTransform);
	pDistortion->AddComponent(new CMeshRender);
	pDistortion->Transform()->SetRelativeScale(200.f, 200.f, 1.f);
	pDistortion->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pDistortion->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DistortionMtrl"));
	SpawnGameObject(pDistortion, Vec3(0.f, 0.f, 500.f), 0);





	// 충돌 시킬 레이어 짝 지정
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Monster");
}

void CreatePlayerAnimation(CGameObject* _player)
{
	_player->Animator2D()->CreateFolderAnimation(L"texture\\player\\spr_idle", 11, 10.f);
	_player->Animator2D()->CreateFolderAnimation(L"texture\\player\\spr_idle_to_run", 4, 25.f);
	_player->Animator2D()->CreateFolderAnimation(L"texture\\player\\spr_run", 10, 14.2857f);
	_player->Animator2D()->CreateFolderAnimation(L"texture\\player\\spr_run_to_idle", 5, 14.28f);
	_player->Animator2D()->CreateFolderAnimation(L"texture\\player\\spr_jump", 4, 11.428f);
	_player->Animator2D()->CreateFolderAnimation(L"texture\\player\\spr_fall", 4, 11.428f);
	_player->Animator2D()->CreateFolderAnimation(L"texture\\player\\spr_attack", 7, 35.714f);
	_player->Animator2D()->CreateFolderAnimation(L"texture\\player\\spr_landing", 5, 16.667f);
	_player->Animator2D()->CreateFolderAnimation(L"texture\\player\\spr_roll", 7, 22.222f);
	_player->Animator2D()->CreateFolderAnimation(L"texture\\player\\spr_player_playsong", 31, 8.857f);
	_player->Animator2D()->CreateFolderAnimation(L"texture\\player\\spr_hurtfly_begin", 2, 14.2857f);
	_player->Animator2D()->CreateFolderAnimation(L"texture\\player\\spr_hurtfly_loop", 4, 14.2857f);
	_player->Animator2D()->CreateFolderAnimation(L"texture\\player\\spr_hurtground", 6, 10.909f);
	_player->Animator2D()->CreateFolderAnimation(L"texture\\player\\spr_wallgrab", 1, 1.f);
	_player->Animator2D()->CreateFolderAnimation(L"texture\\player\\spr_player_flip", 11, 14.2857f);




}
