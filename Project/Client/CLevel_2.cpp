#include "pch.h"

#include <Engine\components.h>

#include <Engine\CResMgr.h>
#include <Engine\CCollisionMgr.h>


#include <Script\CPlayerScript.h>
#include <Script\CMonsterScript.h>
#include <Script\CMainCameraScript.h>
#include <Script\CMouseScript.h>
#include <Script\CGruntScript.h>

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

	// Layer �̸�����
	pCurLevel->GetLayer(0)->SetName(L"Default");
	pCurLevel->GetLayer(1)->SetName(L"Tile");
	pCurLevel->GetLayer(2)->SetName(L"PlayerHitBox");
	pCurLevel->GetLayer(3)->SetName(L"MonsterHitBox");
	pCurLevel->GetLayer(4)->SetName(L"PlayerProjectile");
	pCurLevel->GetLayer(5)->SetName(L"MonsterProjectile");
	pCurLevel->GetLayer(6)->SetName(L"MonsterView");
	pCurLevel->GetLayer(7)->SetName(L"MonsterAttackRange");
	pCurLevel->GetLayer(31)->SetName(L"ViewPort UI");


	// Main Camera Object ����
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCamera");

	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);
	pMainCam->AddComponent(new CMainCameraScript);
	pMainCam->GetScript<CMainCameraScript>()->SetMapsize(Vec2(2176.f, 3500.f));

	pMainCam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pMainCam->Camera()->SetCameraIndex(0);		// MainCamera �� ����
	pMainCam->Camera()->SetLayerMaskAll(true);	// ��� ���̾� üũ
	pMainCam->Camera()->SetLayerMask(31, false);// UI Layer �� ���������� �ʴ´�.

	SpawnGameObject(pMainCam, Vec3(0.f, 0.f, 0.f), 0);

	// UI cameara
	CGameObject* pUICam = new CGameObject;
	pUICam->SetName(L"UICamera");

	pUICam->AddComponent(new CTransform);
	pUICam->AddComponent(new CCamera);

	pUICam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pUICam->Camera()->SetCameraIndex(1);		// SubCamera �� ����
	pUICam->Camera()->SetLayerMask(31, true);	// ��� ���̾� üũ

	SpawnGameObject(pUICam, Vec3(0.f, 0.f, 0.f), 0);


	// ���� �߰�
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


	// ������Ʈ ����
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
		//// Grunt_2	2�� ����
		//CGameObject* pGrunt = CreateGrunt();
		//pGrunt->GetScript<CGruntScript>()->SetDir(ObjDir::Right);
		//pGrunt->GetScript<CGruntScript>()->SetBeginState(ObjState::Walk);
		//pGrunt->GetScript<CGruntScript>()->SetPatrol(true, 4.f);
		//SpawnGameObject(pGrunt, Vec3(1054, -383, 500.f), L"MonsterHitBox");

		//// Grunt_2	2�� ���
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

	//SpawnGameObject(pBackGround, Vec3(900.f, -392.f, 1000.f), L"Default");	//��������1 ������ġ
	SpawnGameObject(pBackGround, Vec3(2176.f/2.f, -3500.f/2.f, 1000.f), L"Default");	// ��������2 ������ġ


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



	// �浹 ��ų ���̾� ¦ ����
	CCollisionMgr::GetInst()->LayerCheck(L"PlayerProjectile", L"MonsterHitBox");
	CCollisionMgr::GetInst()->LayerCheck(L"MonsterProjectile", L"PlayerHitBox");
	CCollisionMgr::GetInst()->LayerCheck(L"MonsterView", L"PlayerHitBox");
	CCollisionMgr::GetInst()->LayerCheck(L"MonsterAttackRange", L"PlayerHitBox");
}
