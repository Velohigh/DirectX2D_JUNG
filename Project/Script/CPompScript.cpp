#include "pch.h"
#include "CPompScript.h"
#include "CMainCameraScript.h"
#include "CPlayerScript.h"
#include "CViewScript.h"
#include "CAttackRangeScript.h"
#include "CEnemy_FollowScript.h"
#include "CBloodAnimationScript.h"
#include "CBloodAnimation2Script.h"
#include "CViewScript.h"
#include "CAttackRangeScript.h"
#include "CGruntSlashScript.h"
#include "CBikeSlashScript.h"
#include "CReflectScript.h"
#include <random>

CPompScript::CPompScript()
	: CMonsterScript()
{
}

CPompScript::~CPompScript()
{
}

void CPompScript::begin()
{
	CMonsterScript::begin();

	// �þ� �浹ü �߰�
	m_ViewCollider = new CGameObject;
	m_ViewCollider->SetName(L"ViewCollider");
	m_ViewCollider->AddComponent(new CTransform);
	m_ViewCollider->AddComponent(new CCollider2D);
	m_ViewCollider->AddComponent(new CViewScript);

	m_ViewCollider->Transform()->SetRelativeScale(500.f, 100.f, 1.f);
	m_ViewCollider->Collider2D()->SetAbsolute(true);
	m_ViewCollider->Collider2D()->SetOffsetScale(Vec2(500.f, 100.f));
	m_ViewCollider->GetScript<CViewScript>()->SetOwner(GetOwner());

	SpawnGameObject(m_ViewCollider, Vec3(Transform()->GetRelativePos()), L"MonsterView");

	// ���� ���� �浹ü �߰�
	m_AttackRangeCollider = new CGameObject;
	m_AttackRangeCollider->SetName(L"AttackRangeCollider");
	m_AttackRangeCollider->AddComponent(new CTransform);
	m_AttackRangeCollider->AddComponent(new CCollider2D);
	m_AttackRangeCollider->AddComponent(new CAttackRangeScript);

	m_AttackRangeCollider->Transform()->SetRelativeScale(140.f, 140.f, 1.f);
	m_AttackRangeCollider->Collider2D()->SetAbsolute(true);
	m_AttackRangeCollider->Collider2D()->SetOffsetScale(Vec2(160.f, 100.f));
	m_AttackRangeCollider->GetScript<CAttackRangeScript>()->SetOwner(GetOwner());

	SpawnGameObject(m_AttackRangeCollider, Vec3(Transform()->GetRelativePos()), L"MonsterAttackRange");

}

void CPompScript::tick()
{
	CMonsterScript::tick();

}


// FSM //
void CPompScript::IdleStart()
{
	m_StateTime[(UINT)ObjState::Idle] = 0.f;
	Animator2D()->Play(L"texture\\pomp\\spr_pomp_idle", true);
	SetSpeed(0.f);

	SetSize2x();

}

void CPompScript::WalkStart()
{
	m_StateTime[static_cast<int>(ObjState::Walk)] = 0.f;
	Animator2D()->Play(L"texture\\pomp\\spr_pomp_walk", true);
	SetSpeed(80.f);

	SetSize2x();

}

void CPompScript::TurnStart()
{
	m_StateTime[static_cast<int>(ObjState::Turn)] = 0.f;
	Animator2D()->Play(L"texture\\pomp\\spr_pomp_turn", true);
	SetSpeed(0.f);

	SetSize2x();
}

void CPompScript::RunStart()
{
	m_StateTime[(int)ObjState::Run] = 0.f;
	Animator2D()->Play(L"texture\\pomp\\spr_pomp_idle", true);
	SetSpeed(0.f);

	SetSize2x();

	// Enemy_Follow ����Ʈ �߰�
	if (m_bEffect_EnemyFollow == false)
	{
		CGameObject* pEnemyFollow = new CGameObject;
		pEnemyFollow->SetName(L"EnemyFollow");
		pEnemyFollow->AddComponent(new CTransform);
		pEnemyFollow->AddComponent(new CMeshRender);
		pEnemyFollow->AddComponent(new CAnimator2D);
		pEnemyFollow->AddComponent(new CEnemy_FollowScript);

		pEnemyFollow->Transform()->SetRelativeScale(34.f, 40.f, 1.f);

		pEnemyFollow->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pEnemyFollow->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"EnemyFollowMtrl"));

		pEnemyFollow->Animator2D()->Create_Effect_Animation();
		pEnemyFollow->Animator2D()->Play(L"texture\\effect\\spr_enemy_follow", true);

		pEnemyFollow->GetScript<CEnemy_FollowScript>()->SetOwner(GetOwner());

		SpawnGameObject(pEnemyFollow, Transform()->GetRelativePos() + Vec3(0.f, 80.f, 0.f), L"Default");
		m_bEffect_EnemyFollow = true;
	}

	//

}

void CPompScript::AttackStart()
{
	Animator2D()->Play(L"texture\\pomp\\spr_pomp_attack", true);
	SetSpeed(0.f);
	SetSize2x();

}

void CPompScript::HurtGroundStart()
{
	// �Ǻ��� �ִϸ��̼�
	{
		CGameObject* pBloodAnimation = new CGameObject;
		pBloodAnimation->SetName(L"BloodAnimation2");
		pBloodAnimation->AddComponent(new CTransform);
		pBloodAnimation->AddComponent(new CMeshRender);
		pBloodAnimation->AddComponent(new CAnimator2D);
		pBloodAnimation->AddComponent(new CBloodAnimation2Script);

		pBloodAnimation->Transform()->SetRelativeScale(80.f, 79.f, 1.f);

		pBloodAnimation->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_Pivot"));
		pBloodAnimation->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BloodAnim2Mtrl"));

		pBloodAnimation->Animator2D()->Create_Effect_Animation();
		pBloodAnimation->Animator2D()->Play(L"texture\\effect\\spr_effect_bloodanimation2", false);

		pBloodAnimation->GetScript<CBloodAnimation2Script>()->SetOwner(GetOwner());
		if (m_CurDir == ObjDir::Right)
			pBloodAnimation->GetScript<CBloodAnimation2Script>()->SetDir(ObjDir::Left);
		else if (m_CurDir == ObjDir::Left)
			pBloodAnimation->GetScript<CBloodAnimation2Script>()->SetDir(ObjDir::Right);


		SpawnGameObject(pBloodAnimation, Transform()->GetRelativePos() + Vec3(0.f, 80.f, 0.f), L"Default");
	}



	m_StateTime[static_cast<int>(ObjState::HurtGround)] = 0.f;
	Animator2D()->Play(L"texture\\pomp\\spr_pomp_hurtground", false);

	SetSize2x();

}

void CPompScript::HurtFlyStart()
{
	// ų ī��Ʈ ����
	int TempKillCount = CLevelMgr::GetInst()->GetCurLevel()->GetKillCount();
	++TempKillCount;
	CLevelMgr::GetInst()->GetCurLevel()->SetKillCount(TempKillCount);


	// Blood Splat ���� ���
	{
		// �õ尪�� ��� ���� random_device ����.
		std::random_device rd;
		// random_device �� ���� ���� ���� ������ �ʱ�ȭ �Ѵ�.
		std::mt19937 gen(rd());
		// 0 ���� 99 ���� �յ��ϰ� ��Ÿ���� �������� �����ϱ� ���� �յ� ���� ����.
		std::uniform_int_distribution<int> IntRange(0, 3);
		int Value = IntRange(gen);
		switch (Value)
		{
		case 0:
		{
			Ptr<CSound> pBloodSplatSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_enemy_bloodsplat1.wav");
			pBloodSplatSound->Play(1, 1.f, true);
		}
		break;
		case 1:
		{
			Ptr<CSound> pBloodSplatSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_enemy_bloodsplat2.wav");
			pBloodSplatSound->Play(1, 1.f, true);
		}
		break;
		case 2:
		{
			Ptr<CSound> pBloodSplatSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_enemy_bloodsplat3.wav");
			pBloodSplatSound->Play(1, 1.f, true);
		}
		break;
		case 3:
		{
			Ptr<CSound> pBloodSplatSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_enemy_bloodsplat4.wav");
			pBloodSplatSound->Play(1, 1.f, true);
		}
		break;
		default:
			break;
		}


	}

	Vec2 PlayerAttackDir = m_Level->FindParentObjectByName(L"Player")->GetScript<CPlayerScript>()->GetAttackDir();
	PlayerAttackDir.Normalize();

	// �÷��̾��� ���ݹ��⿡ ���� ���ư��� �¿� ����� �����ش�.
	if (PlayerAttackDir.x >= 0.f)
	{
		m_CurDir = ObjDir::Left;
	}
	else if (PlayerAttackDir.x < 0.f)
	{
		m_CurDir = ObjDir::Right;
	}

	//DirAnimationCheck();

	// ���� �ڸ��� ���ڱ� ����
	if (m_EnemyAttackDir == Vec2(0.f, 0.f))
	{

	}

	// �Ѿ˿� ���� ���� Į�� ���� ��� BloodRemain ����Ʈ�� �������� ���� �����Ѵ�.
	if (m_EnemyAttackDir == Vector2{ 0.f, 0.f })
	{
		// Į�� �״� ȿ����
		Ptr<CSound> pDeathSwordSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\death_sword1.wav");
		pDeathSwordSound->Play(1, 1.f, true);

		if (PlayerAttackDir.x >= 0.f)
		{
			// ������ ����Ʈ �߰�
			CGameObject* pBloodAnim = new CGameObject;
			pBloodAnim->SetName(L"BloodAnimation");
			pBloodAnim->AddComponent(new CTransform);
			pBloodAnim->AddComponent(new CMeshRender);
			pBloodAnim->AddComponent(new CAnimator2D);
			pBloodAnim->AddComponent(new CBloodAnimationScript);

			pBloodAnim->Transform()->SetRelativeScale(80.f, 79.f, 1.f);

			pBloodAnim->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_Pivot"));
			pBloodAnim->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BloodAnimMtrl"));

			pBloodAnim->Animator2D()->Create_Effect_Animation();
			pBloodAnim->Animator2D()->Play(L"texture\\effect\\spr_effect_bloodanimation", true);
			pBloodAnim->GetScript<CBloodAnimationScript>()->SetDir(ObjDir::Right);
			pBloodAnim->GetScript<CBloodAnimationScript>()->SetOwner(GetOwner());
			SpawnGameObject(pBloodAnim, Transform()->GetRelativePos(), L"Default");
		}
		else if (PlayerAttackDir.x < 0.f)
		{
			// ������ ����Ʈ �߰�
			CGameObject* pBloodAnim = new CGameObject;
			pBloodAnim->SetName(L"BloodAnimation");
			pBloodAnim->AddComponent(new CTransform);
			pBloodAnim->AddComponent(new CMeshRender);
			pBloodAnim->AddComponent(new CAnimator2D);
			pBloodAnim->AddComponent(new CBloodAnimationScript);

			pBloodAnim->Transform()->SetRelativeScale(80.f, 79.f, 1.f);

			pBloodAnim->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_Pivot"));
			pBloodAnim->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BloodAnimMtrl"));

			pBloodAnim->Animator2D()->Create_Effect_Animation();
			pBloodAnim->Animator2D()->Play(L"texture\\effect\\spr_effect_bloodanimation", true);
			pBloodAnim->GetScript<CBloodAnimationScript>()->SetDir(ObjDir::Left);
			pBloodAnim->GetScript<CBloodAnimationScript>()->SetOwner(GetOwner());
			SpawnGameObject(pBloodAnim, Transform()->GetRelativePos(), L"Default");
		}
	}
	else
	{
		// �Ѿ˿� �״� ȿ����
		Ptr<CSound> pDeathBulletSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\death_bullet.wav");
		pDeathBulletSound->Play(1, 1.f, true);
	}


	// ��Ʈ ������ ����Ʈ �߰�
	// ��������Ʈ ����Ʈ �߰�

	// ��Ʈ�� ȭ�� ��鸲
	CGameObject* MainCam = m_Level->FindParentObjectByName(L"MainCamera");
	MainCam->GetScript<CMainCameraScript>()->SetCameraShakeOn(true);

	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	Vec2 m_PosyReverse = Vec2(m_Pos3.x, -m_Pos3.y);

	SetPos(m_Pos + Vector2{ 0, 2 });
	m_StateTime[static_cast<int>(ObjState::HurtGround)] = 0.f;
	Animator2D()->Play(L"texture\\pomp\\spr_pomp_hurtfly", false);
	SetSpeed(0.f);


	m_MoveDir = PlayerAttackDir * 800.f;	// �¾����� �˹�

	SetSize2x();

}

void CPompScript::KnockDownStart()
{
	m_StateTime[(UINT)ObjState::KnockDown] = 0.f;
	Animator2D()->Play(L"texture\\pomp\\spr_pomp_knockdown", true);

	// �и� ����Ʈ
	CGameObject* pReflect = new CGameObject;
	pReflect->SetName(L"Reflect");
	pReflect->AddComponent(new CTransform);
	pReflect->AddComponent(new CMeshRender);
	pReflect->AddComponent(new CAnimator2D);
	pReflect->AddComponent(new CReflectScript);

	pReflect->Transform()->SetRelativeScale(140.f, 128.f, 1.f);

	pReflect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pReflect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ReflectMtrl"));

	pReflect->Animator2D()->Create_Effect_Animation();
	pReflect->Animator2D()->Play(L"texture\\effect\\spr_reflect", false);
	pReflect->GetScript<CReflectScript>()->SetOwner(GetOwner());
	if (m_CurDir == ObjDir::Right)
		pReflect->GetScript<CReflectScript>()->SetDir(ObjDir::Right);
	else if (m_CurDir == ObjDir::Left)
		pReflect->GetScript<CReflectScript>()->SetDir(ObjDir::Left);

	Vec3 PlayerPos3 = m_Level->FindParentObjectByName(L"Player")->Transform()->GetRelativePos();
	Vec3 MyPos3 = Transform()->GetRelativePos();

	Vec2 PlayerPos = Vec2(PlayerPos3.x, PlayerPos3.y);
	Vec2 MyPos = Vec2(MyPos3.x, MyPos3.y);

	Vec2 EffectPos = ((MyPos + Vec2(0.f, 35.f)) + (PlayerPos + Vec2(0.f, 35.f)))/2.f;
	SpawnGameObject(pReflect, Vec3(EffectPos.x, EffectPos.y, MyPos3.z), L"Default");

	// �и� ����
	Ptr<CSound> pswordcrash = CResMgr::GetInst()->FindRes<CSound>(L"sound\\swordcrash.wav");
	pswordcrash->Play(1, 1.f, true);

	// �÷��̾� ���ݹ������� �˹�
	Vec2 PlayerAttackDir = m_Level->FindParentObjectByName(L"Player")->GetScript<CPlayerScript>()->GetAttackDir();
	m_MoveDir = Vec2(PlayerAttackDir.x, 0.f);
	SetSpeed(400.f);

	// �÷��̾ ���ۿ����� �˹�
	Vec2 PlayerMoveDir = m_Level->FindParentObjectByName(L"Player")->GetScript<CPlayerScript>()->GetMoveDir();
	m_Level->FindParentObjectByName(L"Player")->GetScript<CPlayerScript>()->SetMoveDir((PlayerMoveDir * -1.f) + Vec2(0.f, 200.f));

	SetSize2x();

}

void CPompScript::DeadStart()
{
	// ��������ü ����

	SetSize2x();
}

void CPompScript::IdleUpdate()
{
	// ���� �ൿ
	m_StateTime[static_cast<int>(ObjState::Idle)] += DT;
	if (m_StateTime[static_cast<int>(ObjState::Idle)] >= 2 && m_bPatrol == true)
	{
		StateChange(ObjState::Turn);
		return;
	}

	// �÷��̾� ���ݿ� ������ ��� (BeginOverlap)
	if (m_bHitBoxOn)
	{
		StateChange(ObjState::HurtFly);
		return;
	}

	// �÷��̾� �߽߰� Run ���·� �Ѿƿ��°� �߰� (ViewCollider)
	if (m_bViewColliderOn)
	{
		StateChange(ObjState::Run);
		return;
	}

	// �þ߿� ���ᰡ Run, Dead �����̸� �÷��̾ �Ѿƿ´�.
	if (m_bViewColliderOn_Monster)
	{
		StateChange(ObjState::Run);
		return;
	}

}

void CPompScript::WalkUpdate()
{
	m_StateTime[static_cast<int>(ObjState::Walk)] += DT;

	if (m_StateTime[static_cast<int>(ObjState::Walk)] >= m_fPatrolTime)
	{
		StateChange(ObjState::Idle);
		return;
	}

	// �÷��̾� ���ݿ� ������ ��� (BeginOverlap)
	if (m_bHitBoxOn)
	{
		StateChange(ObjState::HurtFly);
		return;
	}


	// �¿� �̵�
	if (m_CurDir == ObjDir::Right)
	{
		m_MoveDir = Vector2{ 1.f, 0.f };
	}
	else if (m_CurDir == ObjDir::Left)
	{
		m_MoveDir = Vector2{ -1.f, 0.f };
	}

	// �÷��̾� �߽߰� Run ���·� �Ѿƿ´�. (ViewCollider�� ����)
	if (m_bViewColliderOn)
	{
		StateChange(ObjState::Run);
		return;
	}

	// �þ߿� ���ᰡ Run, Dead �����̸� �÷��̾ �Ѿƿ´�.
	if (m_bViewColliderOn_Monster)
	{
		StateChange(ObjState::Run);
		return;
	}

	MapCollisionCheckMoveGround();

}

void CPompScript::TurnUpdate()
{
	if (true == Animator2D()->IsEndAnimation())
	{
		StateChange(ObjState::Walk);
		if (m_CurDir == ObjDir::Right)
		{
			m_CurDir = ObjDir::Left;
		}
		else if (m_CurDir == ObjDir::Left)
		{
			m_CurDir = ObjDir::Right;
		}
		return;
	}

	// �÷��̾� ���ݿ� ������ ��� BeginOverlap
	if (m_bHitBoxOn)
	{
		StateChange(ObjState::HurtFly);
		return;
	}

}

void CPompScript::RunUpdate()
{
	Vec3 PlayerPos = m_Level->FindParentObjectByName(L"Player")->Transform()->GetRelativePos();
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	Vec2 m_PosyReverse = Vec2(m_Pos3.x, -m_Pos3.y);


	m_StateTime[(int)ObjState::Run] += DT;

	if (m_StateTime[(int)ObjState::Run] >= 0.32f)
	{
		Animator2D()->Play(L"texture\\pomp\\spr_pomp_run", true);
		SetSpeed(360.f);


		// �÷��̾ �Ѿư����� �¿� ���� ����
		if (PlayerPos.x >= m_Pos.x)
			SetDir(ObjDir::Right);
		else if (PlayerPos.x < m_Pos.x)
			SetDir(ObjDir::Left);


		// �÷��̾ ���������� ������ ���� (AttackRangeScript)
		if (m_bAttackRangeOn)
		{
			StateChange(ObjState::Attack);
			return;
		}


		// �¿� �̵�
		if (m_CurDir == ObjDir::Right)
		{
			m_MoveDir = Vector2{ 1.f, 0.f };
		}

		else if (m_CurDir == ObjDir::Left)
		{
			m_MoveDir = Vector2{ -1.f, 0.f };
		}

	}

	// �÷��̾� ���ݿ� ������ �˴ٿ� ���·� (BeginOverlap)
	Vec2 PlayerAttackDir = m_Level->FindParentObjectByName(L"Player")->GetScript<CPlayerScript>()->GetAttackDir();
	if (m_bHitBoxOn)
	{
		if (m_CurDir == ObjDir::Left &&
			PlayerAttackDir.x > 0.f)
		{
			StateChange(ObjState::KnockDown);
			return;
		}

		else if (m_CurDir == ObjDir::Right &&
			PlayerAttackDir.x <= 0.f)
		{
			StateChange(ObjState::KnockDown);
			return;
		}

		// �÷��̾ �ڽ��� �Ĺ濡�� �����ϸ� ���
		else
		{
			StateChange(ObjState::HurtFly);
			return;
		}
	}

	MapCollisionCheckMoveGround();

}

void CPompScript::AttackUpdate()
{
	// �÷��̾� ������¸� Idle ���·�
	PlayerState PState = m_Level->FindParentObjectByName(L"Player")->GetScript<CPlayerScript>()->GetState();
	if (PlayerState::Dead == PState)
	{
		StateChange(ObjState::Idle);
		return;
	}

	// �÷��̾� ���ݿ� ������ �˴ٿ� ���·� (BeginOverlap)
	Vec2 PlayerAttackDir = m_Level->FindParentObjectByName(L"Player")->GetScript<CPlayerScript>()->GetAttackDir();
	if (m_bHitBoxOn)
	{
		if (m_CurDir == ObjDir::Left &&
			PlayerAttackDir.x > 0.f)
		{
			StateChange(ObjState::KnockDown);
			return;
		}

		else if (m_CurDir == ObjDir::Right &&
			PlayerAttackDir.x <= 0.f)
		{
			StateChange(ObjState::KnockDown);
			return;
		}

		// �÷��̾ �ڽ��� �Ĺ濡�� �����ϸ� ���
		else
		{
			StateChange(ObjState::HurtFly);
			return;

		}
	}


	// ���� ����� ������ �ٽ� Run ���·�
	if (true == Animator2D()->IsEndAnimation())
	{
		SetAttackOn(false);
		StateChange(ObjState::Run);
		return;
	}

	if (m_bAttackOn == false
		&& Animator2D()->GetCurAnim()->GetCurFrmCount() == 3)
	{
		// BikeSlash ����Ʈ �߰�
		CGameObject* pGruntSlash = new CGameObject;
		pGruntSlash->SetName(L"BikeSlash");
		pGruntSlash->AddComponent(new CTransform);
		pGruntSlash->AddComponent(new CMeshRender);
		pGruntSlash->AddComponent(new CAnimator2D);
		pGruntSlash->AddComponent(new CCollider2D);
		pGruntSlash->AddComponent(new CBikeSlashScript);

		pGruntSlash->Transform()->SetRelativeScale(128.f, 128.f, 1.f);

		pGruntSlash->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pGruntSlash->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BikeSlashMtrl"));

		pGruntSlash->Animator2D()->Create_Effect_Animation();
		pGruntSlash->Animator2D()->Play(L"texture\\effect\\spr_enemy_bikeslash", false);
		pGruntSlash->GetScript<CBikeSlashScript>()->SetOwner(GetOwner());

		if (m_CurDir == ObjDir::Right)
			pGruntSlash->GetScript<CBikeSlashScript>()->SetDir(ObjDir::Right);
		else if (m_CurDir == ObjDir::Left)
			pGruntSlash->GetScript<CBikeSlashScript>()->SetDir(ObjDir::Left);

		SpawnGameObject(pGruntSlash, Transform()->GetRelativePos(), L"MonsterProjectile");
		SetAttackOn(true);

		// Swing ����
		Ptr<CSound> pSwingSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\swing.wav");
		pSwingSound->Play(1, 0.8f, true);

	}


}

void CPompScript::HurtGroundUpdate()
{
	// �������� ����� ������, ��� ���·�
	if (true == Animator2D()->IsEndAnimation())
	{
		StateChange(ObjState::Dead);
		return;
	}

	m_MoveDir += -(m_MoveDir * DT * 3.4f);

	if (1.f >= m_MoveDir.Length())
	{
		m_MoveDir = Vector2{ 0,0 };
	}

	MapCollisionCheckMoveGroundDie();

}

void CPompScript::HurtFlyUpdate()
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	Vec2 m_PosyReverse = Vec2(m_Pos3.x, -m_Pos3.y);
	CTexture* m_MapColTexture = GetOwner()->GetColMapTexture();


	// ���߿� �� �����ϰ�� �߷¿����� �޴´�.
	// �߷� ���ӵ��� ���� ���� �ӵ�.
	{
		// �������ǿ��� ���ϴ� ��ġ�� �ȼ��� ������ ���� �� �ִ�.
		int RColor = m_MapColTexture->GetPixelColor(m_PosyReverse + Vector2{ 0,1 });
		m_Gravity += m_GravityAccel * DT;
		if (m_MoveDir.y < 0.f)	// ����������
		{
			if (RGB(0, 0, 0) == RColor || RGB(255, 0, 0) == RColor
				|| m_Move.y == 0.f)	// ���� ���� ���, y �̵����� 0 �ϰ��
			{
				StateChange(ObjState::HurtGround);
				return;
			}
		}
		m_MoveDir += Vector2{ 0.f, -1.f } *m_Gravity * DT;
	}

	MapCollisionCheckMoveAirDie();

}

void CPompScript::KnockDownUpdate()
{
	m_StateTime[(UINT)ObjState::KnockDown] += DT;

	// �˴ٿ� �ִϸ��̼��� ������ �ٽ� �÷��̾� ����
	if (true == Animator2D()->IsEndAnimation())
	{
		StateChange(ObjState::Run);
		return;
	}

	if (m_StateTime[(UINT)ObjState::KnockDown] >= 0.2f)
	{
		// �÷��̾� ���ݿ� ������ Hurtfly
		if (m_bHitBoxOn == true)
		{
			StateChange(ObjState::HurtFly);
			return;
		}
	}

	m_MoveSpeed -= (m_MoveSpeed)*DT;

	if (8 == Animator2D()->GetCurAnim()->GetCurFrmCount())
	{
		SetSpeed(0.f);
	}

	MapCollisionCheckMoveGround();

}

void CPompScript::DeadUpdate()
{
	return;
}