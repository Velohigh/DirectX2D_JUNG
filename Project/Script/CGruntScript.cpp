#include "pch.h"
#include "CGruntScript.h"
#include "CPlayerScript.h"
#include "CMainCameraScript.h"
#include "CEnemy_FollowScript.h"
#include "CBloodAnimationScript.h"
#include "CBloodAnimation2Script.h"
#include <random>

CGruntScript::CGruntScript()
	: CScript((UINT)SCRIPT_TYPE::GRUNTSCRIPT)
	, m_CurState(ObjState::END)
{
}

CGruntScript::~CGruntScript()
{
}

void CGruntScript::begin()
{
	MeshRender()->GetDynamicMaterial();

	StateChange(ObjState::Idle);

	m_Level = CLevelMgr::GetInst()->GetCurLevel();

	// 충돌체 추가
	//Collider2D()->
	//	= AddCollider<CColliderBox>("Body");
	//Box->SetExtent(36.f, 70.f);
	//Box->SetOffset(0.f, -35.f);
	//Box->SetCollisionProfile("MonsterHitBox");



}

void CGruntScript::tick()
{
	CGameObject* Player = m_Level->FindParentObjectByName(L"Player");
	if (nullptr == Player)
		return;

	CPlayerScript* PlayerScript = Player->GetScript<CPlayerScript>();
	if (PlayerScript->GetState() == PlayerState::Dead &&
		m_CurState != ObjState::Dead &&
		m_CurState != ObjState::HurtFly &&
		m_CurState != ObjState::HurtGround)
		StateChange(ObjState::Idle);
	else
	{
		DirAnimationCheck();
		StateUpdate();
	}

	// 시야 충돌체등 추가

	Vec3 CameraPos = m_Level->FindParentObjectByName(L"MainCamera")->Transform()->GetRelativePos();
	Collider2D()->SetOffsetPos(Vec2(-CameraPos.x, -CameraPos.y + 35.f));


}

void CGruntScript::DirAnimationCheck()
{
	Vec3 Rot = Transform()->GetRelativeRot();

	if (m_PreDir != m_CurDir)
	{
		if (m_CurDir == ObjDir::Right)
		{
			Rot = Vec3(Rot.x, 0.f, Rot.z);
		}
		else if (m_CurDir == ObjDir::Left)
		{
			Rot = Vec3(Rot.x, XM_PI, Rot.z);
		}

		Transform()->SetRelativeRot(Rot);
		m_PreDir = m_CurDir;
	}

}

void CGruntScript::StateChange(ObjState _State)
{
	if (_State != m_CurState)
	{
		switch (_State)
		{
		case ObjState::Idle:
			IdleStart();
			break;
		case ObjState::Walk:
			WalkStart();
			break;
		case ObjState::Turn:
			TurnStart();
			break;
		case ObjState::Run:
			RunStart();
			break;
		case ObjState::Attack:
			AttackStart();
			break;
		case ObjState::HurtGround:
			HurtGroundStart();
			break;
		case ObjState::HurtFly:
			HurtFlyStart();
			break;
		case ObjState::Dead:
			DeadStart();
			break;

		}
		m_CurState = _State;
	}

}

void CGruntScript::StateUpdate()
{
	switch (m_CurState)
	{
	case ObjState::Idle:
		IdleUpdate();
		break;
	case ObjState::Walk:
		WalkUpdate();
		break;
	case ObjState::Turn:
		TurnUpdate();
		break;
	case ObjState::Run:
		RunUpdate();
		break;
	case ObjState::Attack:
		AttackUpdate();
		break;
	case ObjState::HurtGround:
		HurtGroundUpdate();
		break;
	case ObjState::HurtFly:
		HurtFlyUpdate();
		break;
	case ObjState::Dead:
		DeadUpdate();
		break;
	}

}

void CGruntScript::SetPos(Vec2 _vec2)
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Transform()->SetRelativePos(_vec2.x, _vec2.y, m_Pos3.z);

}

void CGruntScript::SetSize2x()
{
	vector<Ptr<CTexture>> VecFolderTex = Animator2D()->GetCurAnim()->GetvecFolderTex();
	int CurFrmCount = Animator2D()->GetCurAnim()->GetCurFrmCount();

	float Width = VecFolderTex[CurFrmCount]->Width();
	float Height = VecFolderTex[CurFrmCount]->Height();
	Transform()->SetRelativeScale(Width * 2, Height * 2, 1);
	//Transform()->SetRelativeScale(1, 1, 1);

}

void CGruntScript::BeginOverlap(CCollider2D* _Other)
{
	// 플레이어 공격에 맞으면 날아간다.
	if (m_CurState == ObjState::Idle
		|| m_CurState == ObjState::Walk
		|| m_CurState == ObjState::Turn)
	{
		StateChange(ObjState::HurtFly);
		return;
	}
}

void CGruntScript::OnOverlap(CCollider2D* _Other)
{
}

void CGruntScript::EndOverlap(CCollider2D* _Other)
{
}

void CGruntScript::MoveDir(const Vec2& Dir)
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	m_Pos += Dir * m_MoveSpeed * DT;

	Transform()->SetRelativePos(m_Pos.x, m_Pos.y, m_Pos3.z);
}

void CGruntScript::MoveValue(const Vector2& MoveValue)
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);

	m_Pos += MoveValue /*m_TimeScale*/;

	if (MoveValue.Length() <= 0.001f)
		int ab = 0;

	Transform()->SetRelativePos(m_Pos.x, m_Pos.y, m_Pos3.z);

}

void CGruntScript::IdleStart()
{
	m_StateTime[(UINT)ObjState::Idle] = 0.f;
	Animator2D()->Play(L"texture\\grunt\\spr_grunt_idle", true);
	SetSpeed(0.f);

	SetSize2x();

	//
}

void CGruntScript::WalkStart()
{
	m_StateTime[static_cast<int>(ObjState::Walk)] = 0.f;
	Animator2D()->Play(L"texture\\grunt\\spr_grunt_walk", true);
	SetSpeed(80.f);

	SetSize2x();
	//
}

void CGruntScript::TurnStart()
{
	m_StateTime[static_cast<int>(ObjState::Turn)] = 0.f;
	Animator2D()->Play(L"texture\\grunt\\spr_grunt_turn", true);
	SetSpeed(0.f);

	SetSize2x();
	//
}

void CGruntScript::RunStart()
{
	m_StateTime[(int)ObjState::Run] = 0.f;
	Animator2D()->Play(L"texture\\grunt\\spr_grunt_run", true);
	SetSpeed(0.f);

	SetSize2x();

	// Enemy_Follow 이펙트 추가
	if (!m_bEffect_EnemyFollow)
	{
		CGameObject* pEnemyFollow = new CGameObject;
		pEnemyFollow->SetName(L"EnemyFollow");
		pEnemyFollow->AddComponent(new CTransform);
		pEnemyFollow->AddComponent(new CMeshRender);
		pEnemyFollow->AddComponent(new CAnimator2D);
		pEnemyFollow->AddComponent(new CEnemy_FollowScript);

		pEnemyFollow->Transform()->SetRelativeScale(34.f, 40.f, 1.f);

		pEnemyFollow->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_Pivot"));
		pEnemyFollow->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"EnemyFollowMtrl"));

		pEnemyFollow->Animator2D()->Create_Effect_Animation();
		pEnemyFollow->Animator2D()->Play(L"texture\\effect\\spr_enemy_follow", false);

		pEnemyFollow->GetScript<CEnemy_FollowScript>()->SetOwner(GetOwner());

		SpawnGameObject(pEnemyFollow, Transform()->GetRelativePos() + Vec3(0.f, 80.f, 0.f), L"Default");
		m_bEffect_EnemyFollow = !m_bEffect_EnemyFollow;
	}

	//
}

void CGruntScript::AttackStart()
{
	Animator2D()->Play(L"texture\\grunt\\spr_grunt_attack", true);
	SetSpeed(0.f);
	SetSize2x();

	//
}

void CGruntScript::HurtGroundStart()
{
	// 피분출 애니메이션
	{
		CGameObject* pBloodAnimation = new CGameObject;
		pBloodAnimation->SetName(L"BloodAnimation2");
		pBloodAnimation->AddComponent(new CTransform);
		pBloodAnimation->AddComponent(new CMeshRender);
		pBloodAnimation->AddComponent(new CAnimator2D);
		pBloodAnimation->AddComponent(new CEnemy_FollowScript);

		pBloodAnimation->Transform()->SetRelativeScale(80.f, 79.f, 1.f);

		pBloodAnimation->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_Pivot"));
		pBloodAnimation->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BloodAnim2Mtrl"));

		pBloodAnimation->Animator2D()->Create_Effect_Animation();
		pBloodAnimation->Animator2D()->Play(L"texture\\effect\\spr_effect_bloodanimation2", false);

		pBloodAnimation->GetScript<CEnemy_FollowScript>()->SetOwner(GetOwner());

		SpawnGameObject(pBloodAnimation, Transform()->GetRelativePos() + Vec3(0.f, 80.f, 0.f), L"Default");
	}



	m_StateTime[static_cast<int>(ObjState::HurtGround)] = 0.f;
	Animator2D()->Play(L"texture\\grunt\\spr_grunt_hurtground", false);

	SetSize2x();

}

void CGruntScript::HurtFlyStart()
{
	// Blood Splat 사운드 재생
	{
		// 시드값을 얻기 위한 random_device 생성.
		std::random_device rd;
		// random_device 를 통해 난수 생성 엔진을 초기화 한다.
		std::mt19937 gen(rd());
		// 0 부터 99 까지 균등하게 나타나는 난수열을 생성하기 위해 균등 분포 정의.
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

	// 플레이어의 공격방향에 따라 날아가는 좌우 모션을 정해준다.
	if (PlayerAttackDir.x >= 0.f)
	{
		m_CurDir = ObjDir::Left;
	}
	else if (PlayerAttackDir.x < 0.f)
	{
		m_CurDir = ObjDir::Right;
	}

	//DirAnimationCheck();

	// 맞은 자리에 핏자국 생성
	if (m_EnemyAttackDir == Vec2(0.f, 0.f))
	{

	}

	// 총알에 맞은 경우와 칼에 맞은 경우 BloodRemain 이펙트를 생성할지 말지 결정한다.
	if (m_EnemyAttackDir == Vector2{ 0.f, 0.f })
	{
		// 칼에 죽는 효과음
		Ptr<CSound> pDeathSwordSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\death_sword1.wav");
		pDeathSwordSound->Play(1, 1.f, true);

		if (PlayerAttackDir.x >= 0.f)
		{
			// 블러드 이펙트 추가
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
			// 블러드 이펙트 추가
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
		// 총알에 죽는 효과음
		Ptr<CSound> pDeathBulletSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\death_bullet.wav");
		pDeathBulletSound->Play(1, 1.f, true);
	}


	// 히트 레이저 이펙트 추가
	// 슬래시히트 이펙트 추가

	// 히트시 화면 흔들림
	CGameObject* MainCam = m_Level->FindParentObjectByName(L"MainCamera");
	MainCam->GetScript<CMainCameraScript>()->SetCameraShakeOn(true);

	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	Vec2 m_PosyReverse = Vec2(m_Pos3.x, -m_Pos3.y);

	SetPos(m_Pos + Vector2{ 0, 2 });
	m_StateTime[static_cast<int>(ObjState::HurtGround)] = 0.f;
	Animator2D()->Play(L"texture\\grunt\\spr_grunt_hurtfly", false);
	SetSpeed(0.f);


	m_MoveDir = PlayerAttackDir * 800;	// 맞았을때 넉백

	SetSize2x();
}

void CGruntScript::DeadStart()
{
	// 공격판정체 삭제

	SetSize2x();

}

	///////////////////////////
	// StateUpdate
	//////////////////////////
void CGruntScript::IdleUpdate()
{
	// 정찰 행동
	m_StateTime[static_cast<int>(ObjState::Idle)] += DT;
	if (m_StateTime[static_cast<int>(ObjState::Idle)] >= 2 && m_bPatrol == true)
	{
		StateChange(ObjState::Turn);
		return;
	}

	// 플레이어 공격에 맞으면 사망 (BeginOverlap 구현)

	// 플레이어 발견시 Run 상태로 쫓아오는것 추가

	// 시야에 동료가 Run 상태이면 플레이어를 쫓아온다.


}

void CGruntScript::WalkUpdate()
{
	m_StateTime[static_cast<int>(ObjState::Walk)] += DT;

	if (m_StateTime[static_cast<int>(ObjState::Walk)] >= m_fPatrolTime)
	{
		StateChange(ObjState::Idle);
		return;
	}

	// 플레이어 공격에 맞으면 사망 (BeginOverlap 구현)

	// 좌우 이동
	if (m_CurDir == ObjDir::Right)
	{
		m_MoveDir = Vector2{ 1.f, 0.f };
	}
	else if (m_CurDir == ObjDir::Left)
	{
		m_MoveDir = Vector2{ -1.f, 0.f };
	}

	// 플레이어 발견시 Run 상태로 쫓아온다.

	// 시야에 동료가 Run상태이면 플레이어를 쫓아온다..

	MapCollisionCheckMoveGround();

}

void CGruntScript::TurnUpdate()
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

	// 플레이어 공격에 맞으면 사망 BeginOverlap

}

void CGruntScript::RunUpdate()
{
	Vec3 PlayerPos = m_Level->FindParentObjectByName(L"Player")->Transform()->GetRelativePos();
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	Vec2 m_PosyReverse = Vec2(m_Pos3.x, -m_Pos3.y);


	m_StateTime[(int)ObjState::Run] += DT;

	if (m_StateTime[(int)ObjState::Run] >= 0.22f)
	{
		Animator2D()->Play(L"spr_grunt_run", true);
		SetSpeed(360.f);


		// 플레이어를 쫓아가도록 좌우 방향 조정
		if (PlayerPos.x >= m_Pos.x)
			SetDir(ObjDir::Right);
		else if (PlayerPos.x < m_Pos.x)
			SetDir(ObjDir::Left);

		// 플레이어가 일정범위에 들어오면 공격 추가

		// 좌우 이동
		if (m_CurDir == ObjDir::Right)
		{
			m_MoveDir = Vector2{ 1.f, 0.f };
		}

		else if (m_CurDir == ObjDir::Left)
		{
			m_MoveDir = Vector2{ -1.f, 0.f };
		}

	}

	// 플레이어 공격에 맞으면 사망 BeginOverlap

	MapCollisionCheckMoveGround();
}

void CGruntScript::AttackUpdate()
{
}

void CGruntScript::HurtGroundUpdate()
{
	// 쓰러지는 모션이 끝나면, 사망 상태로
	if (true == Animator2D()->IsEndAnimation())
	{
		//FindCollider("Box")->SetActive(false);
		//SetEnable(false);
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

void CGruntScript::HurtFlyUpdate()
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	Vec2 m_PosyReverse = Vec2(m_Pos3.x, -m_Pos3.y);
	CTexture* m_MapColTexture = GetOwner()->GetColMapTexture();


	// 공중에 뜬 상태일경우 중력영향을 받는다.
	// 중력 가속도에 따른 낙하 속도.
	{
		// 내포지션에서 원하는 위치의 픽셀의 색상을 구할 수 있다.
		int RColor = m_MapColTexture->GetPixelColor(m_PosyReverse + Vector2{ 0,1 });
		m_Gravity += m_GravityAccel * DT;
		if (m_MoveDir.y < 0.f)	// 떨어질때만
		{
			if (RGB(0, 0, 0) == RColor || RGB(255, 0, 0) == RColor)	// 땅에 닿을 경우, y
			{
				StateChange(ObjState::HurtGround);
				return;
			}
		}
		m_MoveDir += Vector2{ 0.f, -1.f } * m_Gravity * DT;
	}

	MapCollisionCheckMoveAirDie();

}

void CGruntScript::DeadUpdate()
{
}



void CGruntScript::MapCollisionCheckMoveGround()
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	Vec2 m_PosyReverse = Vec2(m_Pos3.x, -m_Pos3.y);
	CTexture* m_MapColTexture = GetOwner()->GetColMapTexture();

	{
		// 미래의 위치를 계산하여 그곳의 RGB값을 체크하고, 이동 가능한 곳이면 이동한다.
		Vector2 NextPos = m_PosyReverse + (Vec2{ 0.f,m_MoveDir.y } *DT * m_MoveSpeed);
		Vector2 CheckPos = NextPos + Vector2{ 0,0 };	// 미래 위치의 발기준 색상
		Vector2 CheckPosTopRight = NextPos + Vector2{ 18,-70 };	// 미래 위치의 머리기준 색상
		Vector2 CheckPosTopLeft = NextPos + Vector2{ -18,-70 };	// 미래 위치의 머리기준 색상

		int Color = m_MapColTexture->GetPixelColor(CheckPos);
		int TopRightColor = m_MapColTexture->GetPixelColor(CheckPosTopRight);
		int TopLeftColor = m_MapColTexture->GetPixelColor(CheckPosTopLeft);

		if (RGB(0, 0, 0) != Color &&
			RGB(0, 0, 0) != TopRightColor &&
			RGB(0, 0, 0) != TopLeftColor &&
			RGB(255, 0, 255) != Color &&
			RGB(255, 0, 255) != TopRightColor &&
			RGB(255, 0, 255) != TopLeftColor)
		{
			MoveDir(Vector2{ 0.f, -m_MoveDir.y });
		}
	}

	{
		// 미래의 위치를 계산하여 그곳의 RGB값을 체크하고, 이동 가능한 곳이면 이동한다.
		Vector2 NextPos = m_PosyReverse + (Vec2{ m_MoveDir.x,0.f } *DT * m_MoveSpeed);
		Vector2 CheckPos = NextPos + Vec2{ 0.f,0.f };	// 미래 위치의 발기준 색상
		Vector2 CheckPosTopRight = NextPos + Vec2{ 18,-70 };	// 미래 위치의 머리기준 색상
		Vector2 CheckPosTopLeft = NextPos + Vec2{ -18,-70 };	// 미래 위치의 머리기준 색상
		Vector2 ForDownPos = m_PosyReverse + Vec2{ 0.f, 1.f };	// 발 아래 색상

		int CurColor = m_MapColTexture->GetPixelColor(m_Pos);
		int ForDownColor = m_MapColTexture->GetPixelColor(ForDownPos);
		int Color = m_MapColTexture->GetPixelColor(CheckPos);
		int TopRightColor = m_MapColTexture->GetPixelColor(CheckPosTopRight);
		int TopLeftColor = m_MapColTexture->GetPixelColor(CheckPosTopLeft);


		// 항상 땅에 붙어있기
		if (RGB(0, 0, 0) != ForDownColor && RGB(255, 0, 0) != ForDownColor)
		{
			SetPos(Vector2{ m_Pos.x, m_Pos.y - 1.f });
		}

		// 계단 올라가기
		while (RGB(0, 0, 0) == Color &&
			TopRightColor != RGB(0, 0, 0) && TopLeftColor != RGB(0, 0, 0))
		{
			CheckPos.y -= 1.0f;
			Color = m_MapColTexture->GetPixelColor(CheckPos);
			SetPos(Vector2{ m_Pos.x, m_Pos.y + 1.0f });
		}

		if (RGB(0, 0, 0) != Color &&
			RGB(0, 0, 0) != TopRightColor &&
			RGB(0, 0, 0) != TopLeftColor &&
			RGB(255, 0, 255) != Color &&
			RGB(255, 0, 255) != TopRightColor &&
			RGB(255, 0, 255) != TopLeftColor)
		{
			MoveDir(Vector2{ m_MoveDir.x,0.f });
		}
	}


}

void CGruntScript::MapCollisionCheckMoveAir()
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	Vec2 m_PosyReverse = Vec2(m_Pos3.x, -m_Pos3.y);
	CTexture* m_MapColTexture = GetOwner()->GetColMapTexture();

	{
		// 미래의 위치를 계산하여 그곳의 RGB값을 체크하고, 이동 가능한 곳이면 이동한다.
		Vector2 NextPos = m_PosyReverse + (Vector2{ 0.f, -m_MoveDir.y } *DT);
		//Vector2 NextPos = m_PosyReverse + (m_MoveDir * DT);
		Vector2 CheckPos = NextPos + Vector2{ 0,0 };	// 미래 위치의 발기준 색상
		Vector2 CheckPosTopRight = NextPos + Vector2{ 18,-70 };	// 미래 위치의 머리기준 색상
		Vector2 CheckPosTopLeft = NextPos + Vector2{ -18,-70 };	// 미래 위치의 머리기준 색상
		Vector2 CheckPosCenterRight = NextPos + Vector2{ 18,-20 };	// 몸중앙 색상
		Vector2 CheckPosCenterLeft = NextPos + Vector2{ -18,-20 };	// 몸중앙 색상

		int Color = m_MapColTexture->GetPixelColor(CheckPos);
		int TopRightColor = m_MapColTexture->GetPixelColor(CheckPosTopRight);
		int TopLeftColor = m_MapColTexture->GetPixelColor(CheckPosTopLeft);
		int CenterRightColor = m_MapColTexture->GetPixelColor(CheckPosCenterRight);
		int CenterLeftColor = m_MapColTexture->GetPixelColor(CheckPosCenterLeft);


		if (RGB(0, 0, 0) != Color &&
			RGB(0, 0, 0) != TopRightColor &&
			RGB(0, 0, 0) != TopLeftColor &&
			RGB(0, 0, 0) != CenterRightColor &&
			RGB(0, 0, 0) != CenterLeftColor &&
			RGB(255, 0, 255) != Color &&
			RGB(255, 0, 255) != TopRightColor &&
			RGB(255, 0, 255) != TopLeftColor &&
			RGB(255, 0, 255) != CenterRightColor &&
			RGB(255, 0, 255) != CenterLeftColor)
		{
			MoveValue(Vector2(0.f, m_MoveDir.y) * DT);
		}
	}

	{
		// 미래의 위치를 계산하여 그곳의 RGB값을 체크하고, 이동 가능한 곳이면 이동한다.
		Vector2 NextPos = m_PosyReverse + (Vector2{ m_MoveDir.x,0.f } *DT);
		//Vector2 NextPos = m_PosyReverse + (m_MoveDir * DT);
		Vector2 CheckPos = NextPos + Vector2{ 0.f, 0.f };	// 미래 위치의 발기준 색상
		Vector2 CheckPosTopRight = NextPos + Vector2{ 18,-70 };	// 미래 위치의 머리기준 색상
		Vector2 CheckPosTopLeft = NextPos + Vector2{ -18,-70 };	// 미래 위치의 머리기준 색상
		Vector2 CheckPosCenterRight = NextPos + Vector2{ 18,-20 };	// 몸중앙 색상
		Vector2 CheckPosCenterLeft = NextPos + Vector2{ -18,-20 };	// 몸중앙 색상

		int Color = m_MapColTexture->GetPixelColor(CheckPos);
		int TopRightColor = m_MapColTexture->GetPixelColor(CheckPosTopRight);
		int TopLeftColor = m_MapColTexture->GetPixelColor(CheckPosTopLeft);
		int CenterRightColor = m_MapColTexture->GetPixelColor(CheckPosCenterRight);
		int CenterLeftColor = m_MapColTexture->GetPixelColor(CheckPosCenterLeft);

		if (RGB(0, 0, 0) != Color &&
			RGB(0, 0, 0) != TopRightColor &&
			RGB(0, 0, 0) != TopLeftColor &&
			RGB(0, 0, 0) != CenterRightColor &&
			RGB(0, 0, 0) != CenterLeftColor &&
			RGB(255, 0, 255) != Color &&
			RGB(255, 0, 255) != TopRightColor &&
			RGB(255, 0, 255) != TopLeftColor &&
			RGB(255, 0, 255) != CenterRightColor &&
			RGB(255, 0, 255) != CenterLeftColor)
		{
			MoveValue(Vector2(m_MoveDir.x, 0.f) * DT);
		}
	}


}

void CGruntScript::MapCollisionCheckMoveGroundDie()
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	Vec2 m_PosyReverse = Vec2(m_Pos3.x, -m_Pos3.y);
	CTexture* m_MapColTexture = GetOwner()->GetColMapTexture();

	{
		// 미래의 위치를 계산하여 그곳의 RGB값을 체크하고, 이동 가능한 곳이면 이동한다.
		Vector2 NextPos = m_PosyReverse + (Vec2{ 0.f,-m_MoveDir.y } *DT * m_MoveSpeed);
		Vector2 CheckPos = NextPos + Vector2{ 0,0 };	// 미래 위치의 발기준 색상
		Vector2 CheckPosTopRight = NextPos + Vector2{ 18,-70 };	// 미래 위치의 머리기준 색상
		Vector2 CheckPosTopLeft = NextPos + Vector2{ -18,-70 };	// 미래 위치의 머리기준 색상

		int Color = m_MapColTexture->GetPixelColor(CheckPos);
		int TopRightColor = m_MapColTexture->GetPixelColor(CheckPosTopRight);
		int TopLeftColor = m_MapColTexture->GetPixelColor(CheckPosTopLeft);

		if (RGB(0, 0, 0) != Color && RGB(255, 0, 0) != Color &&
			RGB(0, 0, 0) != TopRightColor &&
			RGB(0, 0, 0) != TopLeftColor &&
			RGB(255, 0, 255) != Color &&
			RGB(255, 0, 255) != TopRightColor &&
			RGB(255, 0, 255) != TopLeftColor)
		{
			MoveDir(Vector2{ 0.f, m_MoveDir.y });
		}
	}

	{
		// 미래의 위치를 계산하여 그곳의 RGB값을 체크하고, 이동 가능한 곳이면 이동한다.
		Vector2 NextPos = m_PosyReverse + (Vec2{ m_MoveDir.x,0.f } *DT * m_MoveSpeed);
		Vector2 CheckPos = NextPos + Vec2{ 0.f,0.f };	// 미래 위치의 발기준 색상
		Vector2 CheckPosTopRight = NextPos + Vec2{ 18,-70 };	// 미래 위치의 머리기준 색상
		Vector2 CheckPosTopLeft = NextPos + Vec2{ -18,-70 };	// 미래 위치의 머리기준 색상
		Vector2 ForDownPos = m_PosyReverse + Vec2{ 0.f, 1.f };	// 발 아래 색상

		int CurColor = m_MapColTexture->GetPixelColor(m_Pos);
		int ForDownColor = m_MapColTexture->GetPixelColor(ForDownPos);
		int Color = m_MapColTexture->GetPixelColor(CheckPos);
		int TopRightColor = m_MapColTexture->GetPixelColor(CheckPosTopRight);
		int TopLeftColor = m_MapColTexture->GetPixelColor(CheckPosTopLeft);


		// 항상 땅에 붙어있기
		if (RGB(0, 0, 0) != ForDownColor && RGB(255, 0, 0) != ForDownColor)
		{
			SetPos(Vector2{ m_Pos.x, m_Pos.y - 1.f });
		}

		// 계단 올라가기
		while (RGB(0, 0, 0) == Color &&
			TopRightColor != RGB(0, 0, 0) && TopLeftColor != RGB(0, 0, 0))
		{
			CheckPos.y -= 1.0f;
			Color = m_MapColTexture->GetPixelColor(CheckPos);
			SetPos(Vector2{ m_Pos.x, m_Pos.y + 1.0f });
		}

		if (RGB(0, 0, 0) != Color &&
			RGB(0, 0, 0) != TopRightColor &&
			RGB(0, 0, 0) != TopLeftColor &&
			RGB(255, 0, 255) != Color &&
			RGB(255, 0, 255) != TopRightColor &&
			RGB(255, 0, 255) != TopLeftColor)
		{
			MoveDir(Vector2{ m_MoveDir.x,0.f });
		}
	}

}


void CGruntScript::MapCollisionCheckMoveAirDie()
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	Vec2 m_PosyReverse = Vec2(m_Pos3.x, -m_Pos3.y);
	CTexture* m_MapColTexture = GetOwner()->GetColMapTexture();

	{
		// 미래의 위치를 계산하여 그곳의 RGB값을 체크하고, 이동 가능한 곳이면 이동한다.
		Vector2 NextPos = m_PosyReverse + (Vector2{ 0.f, -m_MoveDir.y } *DT);
		//Vector2 NextPos = m_PosyReverse + (m_MoveDir * DT);
		Vector2 CheckPos = NextPos + Vector2{ 0,0 };	// 미래 위치의 발기준 색상
		Vector2 CheckPosTopRight = NextPos + Vector2{ 18,-70 };	// 미래 위치의 머리기준 색상
		Vector2 CheckPosTopLeft = NextPos + Vector2{ -18,-70 };	// 미래 위치의 머리기준 색상
		Vector2 CheckPosCenterRight = NextPos + Vector2{ 18,-20 };	// 몸중앙 색상
		Vector2 CheckPosCenterLeft = NextPos + Vector2{ -18,-20 };	// 몸중앙 색상

		int Color = m_MapColTexture->GetPixelColor(CheckPos);
		int TopRightColor = m_MapColTexture->GetPixelColor(CheckPosTopRight);
		int TopLeftColor = m_MapColTexture->GetPixelColor(CheckPosTopLeft);
		int CenterRightColor = m_MapColTexture->GetPixelColor(CheckPosCenterRight);
		int CenterLeftColor = m_MapColTexture->GetPixelColor(CheckPosCenterLeft);


		if (RGB(0, 0, 0) != Color &&
			RGB(0, 0, 0) != TopRightColor &&
			RGB(0, 0, 0) != TopLeftColor &&
			RGB(0, 0, 0) != CenterRightColor &&
			RGB(0, 0, 0) != CenterLeftColor &&
			RGB(255, 0, 255) != Color &&
			RGB(255, 0, 255) != TopRightColor &&
			RGB(255, 0, 255) != TopLeftColor &&
			RGB(255, 0, 255) != CenterRightColor &&
			RGB(255, 0, 255) != CenterLeftColor)
		{
			MoveValue(Vector2(0.f, m_MoveDir.y) * DT);
		}
	}

	{
		// 미래의 위치를 계산하여 그곳의 RGB값을 체크하고, 이동 가능한 곳이면 이동한다.
		Vector2 NextPos = m_PosyReverse + (Vector2{ m_MoveDir.x,0.f } *DT);
		//Vector2 NextPos = m_PosyReverse + (m_MoveDir * DT);
		Vector2 CheckPos = NextPos + Vector2{ 0.f, 0.f };	// 미래 위치의 발기준 색상
		Vector2 CheckPosTopRight = NextPos + Vector2{ 18,-70 };	// 미래 위치의 머리기준 색상
		Vector2 CheckPosTopLeft = NextPos + Vector2{ -18,-70 };	// 미래 위치의 머리기준 색상
		Vector2 CheckPosCenterRight = NextPos + Vector2{ 18,-20 };	// 몸중앙 색상
		Vector2 CheckPosCenterLeft = NextPos + Vector2{ -18,-20 };	// 몸중앙 색상

		int Color = m_MapColTexture->GetPixelColor(CheckPos);
		int TopRightColor = m_MapColTexture->GetPixelColor(CheckPosTopRight);
		int TopLeftColor = m_MapColTexture->GetPixelColor(CheckPosTopLeft);
		int CenterRightColor = m_MapColTexture->GetPixelColor(CheckPosCenterRight);
		int CenterLeftColor = m_MapColTexture->GetPixelColor(CheckPosCenterLeft);

		if (RGB(0, 0, 0) != Color &&
			RGB(0, 0, 0) != TopRightColor &&
			RGB(0, 0, 0) != TopLeftColor &&
			RGB(0, 0, 0) != CenterRightColor &&
			RGB(0, 0, 0) != CenterLeftColor &&
			RGB(255, 0, 255) != Color &&
			RGB(255, 0, 255) != TopRightColor &&
			RGB(255, 0, 255) != TopLeftColor &&
			RGB(255, 0, 255) != CenterRightColor &&
			RGB(255, 0, 255) != CenterLeftColor)
		{
			MoveValue(Vector2(m_MoveDir.x, 0.f) * DT);
		}
	}

}
