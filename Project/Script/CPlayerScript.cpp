#include "pch.h"
#include "CPlayerScript.h"
#include "CMouseScript.h"

#include <Engine\CMeshRender.h>
#include <Engine\CMaterial.h>
#include <Engine\CLevelMgr.h>

#include "CMissileScript.h"

CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_MoveSpeed(50.f)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MoveSpeed, "Speed");
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_JumpPower, "JumpPower");
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_LongJumpPower, "LongJumpPower");
}

CPlayerScript::~CPlayerScript()
{

}

void CPlayerScript::begin()
{
	m_Level = CLevelMgr::GetInst()->GetCurLevel();
	MeshRender()->GetDynamicMaterial();


	StateChange(PlayerState::Idle);
}

void CPlayerScript::tick()
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();

	DirAnimationCheck();
	StateUpdate();

	Vec3 CameraPos = m_Level->FindParentObjectByName(L"MainCamera")->Transform()->GetRelativePos();
	Collider2D()->SetOffsetPos(Vec2(-CameraPos.x, -CameraPos.y + 35.f));



	//if (KEY_PRESSED(KEY::Z))
	//{
	//	Vec3 vRot = Transform()->GetRelativeRot();
	//	vRot.z += DT * XM_PI;
	//	Transform()->SetRelativeRot(vRot);
	//}

	//Transform()->SetRelativePos(vCurPos);

	//if (KEY_TAP(KEY::SPACE))
	//{
	//	DrawDebugCircle(Transform()->GetWorldPos(), 500.f, Vec4(0.f, 0.f, 1.f, 1.f), Vec3(0.f, 0.f, 0.f), 2.f);

	//	Shoot();
	//}
}

void CPlayerScript::Shoot()
{
	// 미사일 프리팹 참조
	Ptr<CPrefab> pMissilePrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"MissilePrefab");
	Vec3 vMissilePos = Transform()->GetRelativePos() + Vec3(0.f, 0.5f, 0.f) * Transform()->GetRelativeScale();
	CGameObject* pCloneMissile = pMissilePrefab->Instantiate();

	// 레벨에 추가
	SpawnGameObject(pCloneMissile, vMissilePos, L"PlayerProjectile");
}

void CPlayerScript::DirAnimationCheck()
{
	return;
	PlayerDir CheckDir = m_CurDir;

	Vec3 Rot = Transform()->GetRelativeRot();


	if (m_CurState != PlayerState::Attack && m_CurState != PlayerState::Dodge &&
		m_CurState != PlayerState::Dead && m_CurState != PlayerState::HurtFlyLoop &&
		m_CurState != PlayerState::HurtGround &&
		m_CurState != PlayerState::WallGrab &&
		m_CurState != PlayerState::Flip)
	{

		if (KEY_PRESSED(KEY::D))
		{
			CheckDir = PlayerDir::Right;
		}

		else if (KEY_PRESSED(KEY::A))
		{
			CheckDir = PlayerDir::Left;
		}

		if (CheckDir != m_CurDir)
		{
			m_CurDir = CheckDir;
		}
	}

	if (m_CurDir == PlayerDir::Right)
	{
		Rot = Vec3(Rot.x, 0.f, Rot.z);
	}
	else if (m_CurDir == PlayerDir::Left)
	{
		Rot = Vec3(Rot.x, 3.1415926535f, Rot.z);
	}


	Transform()->SetRelativeRot(Rot);
}

void CPlayerScript::StateChange(PlayerState _State)
{
	if (m_CurState != _State)	// 상태가 바뀔때 한번만 실행시켜준다.
	{
		switch (_State)
		{
		case PlayerState::Idle:
			IdleStart();
			break;
		case PlayerState::IdleToRun:
			IdleToRunStart();
			break;
		case PlayerState::Run:
			RunStart();
			break;
		case PlayerState::RunToIdle:
			RunToIdleStart();
			break;
		case PlayerState::Jump:
			JumpStart();
			break;
		case PlayerState::Landing:
			LandingStart();
			break;
		case PlayerState::Attack:
			AttackStart();
			break;
		case PlayerState::Fall:
			FallStart();
			break;
		case PlayerState::Dodge:
			DodgeStart();
			break;
		case PlayerState::PlaySong:
			PlaySongStart();
			break;
		case PlayerState::HurtFlyLoop:
			HurtFlyLoopStart();
			break;
		case PlayerState::HurtGround:
			HurtGroundStart();
			break;
		case PlayerState::WallGrab:
			WallGrabStart();
			break;
		case PlayerState::Flip:
			FlipStart();
			break;
		case PlayerState::Dead:
			DeadStart();
			break;
		}
		m_CurState = _State;
	}
}

void CPlayerScript::StateUpdate()
{
	switch (m_CurState)
	{
	case PlayerState::Idle:
		IdleUpdate();
		break;
	case PlayerState::IdleToRun:
		IdleToRunUpdate();
		break;
	case PlayerState::Run:
		RunUpdate();
		break;
	case PlayerState::RunToIdle:
		RunToIdleUpdate();
		break;
	case PlayerState::Jump:
		JumpUpdate();
		break;
	case PlayerState::Landing:
		LandingUpdate();
		break;
	case PlayerState::Attack:
		AttackUpdate();
		break;
	case PlayerState::Fall:
		FallUpdate();
		break;
	case PlayerState::Dodge:
		DodgeUpdate();
		break;
	case PlayerState::PlaySong:
		PlaySongUpdate();
		break;
	case PlayerState::HurtFlyLoop:
		HurtFlyLoopUpdate();
		break;
	case PlayerState::HurtGround:
		HurtGroundUpdate();
		break;
	case PlayerState::WallGrab:
		WallGrabUpdate();
		break;
	case PlayerState::Flip:
		FlipUpdate();
		break;
	case PlayerState::Dead:
		DeadUpdate();
		break;

	}
}

bool CPlayerScript::IsMoveKey()
{
	// 무브키 눌렀다면 true 리턴
	if (KEY_PRESSED(KEY::A) ||
		KEY_PRESSED(KEY::D))
	{
		return true;
	}
	return false;
}

void CPlayerScript::SetSize2x()
{
	vector<Ptr<CTexture>> VecFolderTex = Animator2D()->GetCurAnim()->GetvecFolderTex();
	int CurFrmCount = Animator2D()->GetCurAnim()->GetCurFrmCount();

	float Width = VecFolderTex[CurFrmCount]->Width();
	float Height = VecFolderTex[CurFrmCount]->Height();
	Transform()->SetRelativeScale(Width * 2, Height * 2, 1);
	//Transform()->SetRelativeScale(1, 1, 1);

}

void CPlayerScript::MoveDir(const Vec2& Dir)
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	m_Pos += Dir * m_MoveSpeed * DT;

	Transform()->SetRelativePos(m_Pos.x, m_Pos.y, m_Pos3.z);
}

void CPlayerScript::MoveValue(const Vector2& MoveValue)
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);

	m_Pos += MoveValue /*m_TimeScale*/;

	if (MoveValue.Length() <= 0.001f)
		int ab = 0;

	Transform()->SetRelativePos(m_Pos.x, m_Pos.y, m_Pos3.z);

}

void CPlayerScript::SetPos(const Vector2& Pos)
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Transform()->SetRelativePos(Pos.x, Pos.y, m_Pos3.z);

}

void CPlayerScript::SetPivot()
{

}



void CPlayerScript::BeginOverlap(CCollider2D* _Other)
{

}





void CPlayerScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_MoveSpeed, sizeof(float), 1, _File);
}

void CPlayerScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_MoveSpeed, sizeof(float), 1, _File);
}


///////////////////////////////////////////
	// State Update 
///////////////////////////////////////////

void CPlayerScript::IdleUpdate()
{
	// 이동키를 누르면 IdleToRun 상태로
	if (true == IsMoveKey())
	{
		StateChange(PlayerState::IdleToRun);
		return;
	}


	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	Vec2 m_PosYReverse = Vec2(m_Pos3.x, -m_Pos3.y);
	CTexture* m_MapColTexture = GetOwner()->GetColMapTexture();

	// 아래쪽에 지형이 없다면 Fall상태로
	int color = m_MapColTexture->GetPixelColor(m_PosYReverse + Vec2{ 0,10 });
	int Rcolor = m_MapColTexture->GetPixelColor(m_PosYReverse + Vec2{ 0,1 });
	if (color != RGB(0, 0, 0) && m_CurState != PlayerState::Jump &&
		Rcolor != RGB(255, 0, 0) &&
		Rcolor != RGB(0, 0, 0))
	{
		StateChange(PlayerState::Fall);
		return;
	}

	// 충돌맵 빨간색이면 아래로 이동 가능
	if (Rcolor == RGB(255, 0, 0) &&
		KEY_TAP(KEY::S))
	{
		SetPos(m_Pos + Vector2{ 0, -4 });
	}

	// 점프키를 누르면 Jump 상태로
	if (KEY_TAP(KEY::SPACE))		// @@@ 점프 추가.
	{
		StateChange(PlayerState::Jump);
		return;
	}

	// 공격키를 누르면 공격상태로
	if (KEY_TAP(KEY::LBTN))
	{
		StateChange(PlayerState::Attack);
		return;
	}

	// 회피
	if (KEY_TAP(KEY::LSHIFT))	// @@@ 회피 추가.
	{
		StateChange(PlayerState::Dodge);
		return;
	}



}

void CPlayerScript::IdleToRunUpdate()
{
	// 애니메이션 종료후 Run 모션으로
	if (true == Animator2D()->IsEndAnimation())
	{
		StateChange(PlayerState::Run);
		return;
	}

	// 이동키를 안눌렀다면 Idle 상태로
	if (false == IsMoveKey())				// 이동키를 안눌렀다면
	{
		StateChange(PlayerState::Idle);
		return;
	}

	// 점프키를 누르면 Jump 상태로
	if (KEY_TAP(KEY::SPACE))		// @@@ 점프 추가.
	{
		StateChange(PlayerState::Jump);
		return;
	}

	// 회피키를 누르면 Dodge 상태로
	if (KEY_TAP(KEY::LSHIFT))	// @@@ 회피 추가.
	{
		StateChange(PlayerState::Dodge);
		return;
	}

	// 공격
	if (KEY_TAP(KEY::LBTN))
	{
		StateChange(PlayerState::Attack);
		return;
	}

	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	Vec2 m_PosYReverse = Vec2(m_Pos3.x, -m_Pos3.y);
	CTexture* m_MapColTexture = GetOwner()->GetColMapTexture();


	// 아래쪽에 지형이 없다면 Fall상태로
	int color[5] = {};
	for (int i = 1; i <= 5; ++i)
	{
		color[i - 1] = m_MapColTexture->GetPixelColor(m_PosYReverse + Vector2{ 0.f,(float)i });
	}
	if (color[0] != RGB(0, 0, 0) && color[0] != RGB(255, 0, 0) &&
		color[1] != RGB(0, 0, 0) &&
		color[2] != RGB(0, 0, 0) &&
		color[3] != RGB(0, 0, 0) &&
		color[4] != RGB(0, 0, 0) &&
		m_CurState != PlayerState::Jump)
	{
		StateChange(PlayerState::Fall);
		return;
	}

	// 충돌맵 빨간색이면 아래로 이동 가능
	if (color[0] == RGB(255, 0, 0) &&
		KEY_TAP(KEY::S))
	{
		SetPos(m_Pos + Vector2{ 0, -4 });
	}

	m_MoveDir = Vector2{ 0,0 };

	if (KEY_PRESSED(KEY::A))
	{
		m_MoveDir = Vector2{ -1.f, 0.f };
	}

	if (KEY_PRESSED(KEY::D))
	{
		m_MoveDir = Vector2{ 1.f, 0.f };
	}

	MapCollisionCheckMoveGround();
}

void CPlayerScript::RunUpdate()
{
	m_StateTime[static_cast<int>(PlayerState::Run)] += DT;

	// 이동키를 안누르면 Idle 상태로
	if (false == IsMoveKey())
	{
		StateChange(PlayerState::RunToIdle);
		return;
	}

	// 점프키를 누르면 Jump 상태로
	if (KEY_TAP(KEY::SPACE))		// @@@ 점프 추가.
	{
		StateChange(PlayerState::Jump);
		return;
	}

	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	Vec2 m_PosYReverse = Vec2(m_Pos3.x, -m_Pos3.y);
	CTexture* m_MapColTexture = GetOwner()->GetColMapTexture();


	// 아래쪽에 지형이 없다면 Fall상태로
	int color[10] = {};
	for (int i = 1; i <= 10; ++i)
	{
		color[i - 1] = m_MapColTexture->GetPixelColor(m_PosYReverse + Vector2{ 0.f,(float)i });
	}
	if (color[0] != RGB(0, 0, 0) && color[0] != RGB(255, 0, 0) &&
		color[1] != RGB(0, 0, 0) &&
		color[2] != RGB(0, 0, 0) &&
		color[3] != RGB(0, 0, 0) &&
		color[4] != RGB(0, 0, 0) &&
		color[5] != RGB(0, 0, 0) &&
		color[6] != RGB(0, 0, 0) &&
		color[7] != RGB(0, 0, 0) &&
		color[8] != RGB(0, 0, 0) &&
		color[9] != RGB(0, 0, 0) &&
		m_CurState != PlayerState::Jump)
	{
		StateChange(PlayerState::Fall);
		return;
	}

	// 충돌맵 빨간색이면 아래로 이동 가능
	if (color[0] == RGB(255, 0, 0) &&
		KEY_TAP(KEY::S))
	{
		SetPos(m_Pos + Vector2{ 0, -4 });
	}

	// 회피키를 누르면 Dodge 상태로
	if (KEY_TAP(KEY::LSHIFT))	// @@@ 회피 추가.
	{
		StateChange(PlayerState::Dodge);
		return;
	}

	// 공격
	if (KEY_TAP(KEY::LBTN))
	{
		StateChange(PlayerState::Attack);
		return;
	}


	m_MoveDir = Vector2{ 0.f, 0.f };

	if (KEY_PRESSED(KEY::A))
	{
		m_MoveDir = Vector2{ -1.f, 0.f };
	}

	if (KEY_PRESSED(KEY::D))
	{
		m_MoveDir = Vector2{ 1.f, 0.f };
	}

	MapCollisionCheckMoveGround();

}

void CPlayerScript::RunToIdleUpdate()
{
	// 이동키를 누르면 Run 상태로
	if (true == IsMoveKey())
	{
		StateChange(PlayerState::IdleToRun);
		return;
	}

	// 점프키를 누르면 Jump 상태로
	if (KEY_TAP(KEY::SPACE))		// @@@ 점프 추가.
	{
		StateChange(PlayerState::Jump);
		return;
	}

	// 이동키를 안누르고, 애니메이션이 끝까지 재생되면 Idle 상태로
	if (false == IsMoveKey() &&
		true == Animator2D()->IsEndAnimation())
	{
		StateChange(PlayerState::Idle);
		return;
	}

	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	Vec2 m_PosYReverse = Vec2(m_Pos3.x, -m_Pos3.y);
	CTexture* m_MapColTexture = GetOwner()->GetColMapTexture();


	// 아래쪽에 지형이 없다면 Fall상태로
	int color[10] = {};
	for (int i = 1; i <= 10; ++i)
	{
		color[i - 1] = m_MapColTexture->GetPixelColor(m_PosYReverse + Vector2{ 0.f,(float)i });
	}
	if (color[0] != RGB(0, 0, 0) && color[0] != RGB(255, 0, 0) &&
		color[1] != RGB(0, 0, 0) &&
		color[2] != RGB(0, 0, 0) &&
		color[3] != RGB(0, 0, 0) &&
		color[4] != RGB(0, 0, 0) &&
		color[5] != RGB(0, 0, 0) &&
		color[6] != RGB(0, 0, 0) &&
		color[7] != RGB(0, 0, 0) &&
		color[8] != RGB(0, 0, 0) &&
		color[9] != RGB(0, 0, 0) &&
		m_CurState != PlayerState::Jump)
	{
		StateChange(PlayerState::Fall);
		return;
	}

	// 충돌맵 빨간색이면 아래로 이동 가능
	if (color[0] == RGB(255, 0, 0) &&
		KEY_TAP(KEY::S))
	{
		SetPos(m_Pos + Vector2{ 0, -4 });
	}

	// 회피키를 누르면 Dodge 상태로
	if (KEY_TAP(KEY::LSHIFT))	// @@@ 회피 추가.
	{
		StateChange(PlayerState::Dodge);
		return;
	}

	// 공격
	if (KEY_TAP(KEY::LBTN))
	{
		StateChange(PlayerState::Attack);
		return;
	}

	// 이동키를 안누르고, 애니메이션이 끝까지 재생되면 Idle 상태로
	if (false == IsMoveKey() &&
		true == Animator2D()->IsEndAnimation())
	{
		StateChange(PlayerState::Idle);
		return;
	}
	else if (false == IsMoveKey() &&
		false == Animator2D()->IsEndAnimation())
	{
		Vector2 MoveDir = Vector2{ 0.f,0.f };

		if (m_CurDir == PlayerDir::Left)
		{
			MoveDir = Vector2{ -1.f,0.f };
		}

		else if (m_CurDir == PlayerDir::Right)
		{
			MoveDir = Vector2{ 1.f,0.f };
		}

		MapCollisionCheckMoveGround();
	}

	// 멈추는중에 다시 이동키를 누르면
	if (true == IsMoveKey())
	{
		StateChange(PlayerState::IdleToRun);
		return;
	}

}

void CPlayerScript::JumpUpdate()
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	Vec2 m_PosYReverse = Vec2(m_Pos3.x, -m_Pos3.y);
	CTexture* m_MapColTexture = GetOwner()->GetColMapTexture();


	// 점프키를 오래 누르면 더 높게 뛴다.
	m_StateTime[static_cast<int>(PlayerState::Jump)] += DT;

	if (m_StateTime[static_cast<int>(PlayerState::Jump)] > 0.f &&
		m_StateTime[static_cast<int>(PlayerState::Jump)] <= 0.15f &&
		KEY_PRESSED(KEY::SPACE))
	{
		m_MoveDir += Vector2{ 0.f, 1.f } *DT * m_LongJumpPower;
	}


	m_MoveDir += Vector2{ 0.f, -1.f } *DT * 1500.f;

	// y이동량이 일정이하로 떨어지면 Fall 상태로
	Vector2 TempY = { 0.f ,m_MoveDir.y };
	if (30.0f >= TempY.Length())
	{
		m_MoveDir.y = 0;
		//MoveDir.Normal2D();
		StateChange(PlayerState::Fall);
		return;
	}

	// 공격
	if (KEY_TAP(KEY::LBTN))
	{
		StateChange(PlayerState::Attack);
		return;
	}


	if (KEY_TAP(KEY::S))
	{
		StateChange(PlayerState::Fall);
		return;
	}

	// 공중에서 키누르면 해당방향으로 가속도
	if (KEY_PRESSED(KEY::A))
	{
		m_MoveDir += Vector2{ -1.f, 0.f } *DT * 2000.f;

		// 공중에서 최대 가속도 제한
		Vector2 TempX = { m_MoveDir.x,0.f };
		if (TempX.Length() >= 450.f)
		{
			TempX.Normalize();
			TempX *= 450.f;
			m_MoveDir.x = TempX.x;
		}
	}
	if (KEY_PRESSED(KEY::D))
	{
		m_MoveDir += Vector2{ 1.f, 0.f } *DT * 2000.f;
		Vector2 TempX = { m_MoveDir.x,0.f };

		if (TempX.Length() >= 450.f)
		{
			TempX.Normalize();
			TempX *= 450.f;
			m_MoveDir.x = TempX.x;
		}
	}

	// 검은 땅에 닿지않고 벽에 부딪힐경우 wallgrab
	int Color = m_MapColTexture->GetPixelColor(m_PosYReverse + Vector2{ 0.f,1.f });
	int LCColor = m_MapColTexture->GetPixelColor(m_PosYReverse + Vector2{ -20,-35 });
	int RCColor = m_MapColTexture->GetPixelColor(m_PosYReverse + Vector2{ 20,-35 });

	if (Color != RGB(0, 0, 0))
	{
		if (LCColor == RGB(255, 0, 255))
		{
			m_CurDir = PlayerDir::Left;
			m_Gravity = 10.0f;
			StateChange(PlayerState::WallGrab);
			return;
		}
		else if (RCColor == RGB(255, 0, 255))
		{
			m_CurDir = PlayerDir::Right;
			m_Gravity = 10.0f;
			StateChange(PlayerState::WallGrab);
			return;
		}
	}



	// 검은 땅에 닿을 경우 착지
	Color = m_MapColTexture->GetPixelColor(m_PosYReverse + Vector2{ 0,1 });
	if (RGB(0, 0, 0) == Color)
	{
		m_Gravity = 10.0f;
		m_MoveDir.Normalize();

		//Effect_LandCloud* NewEffect = GetLevel()->CreateActor<Effect_LandCloud>((int)ORDER::UI);
		//NewEffect->SetPosition(GetPosition());

		StateChange(PlayerState::Landing);
		return;
	}


	MapCollisionCheckMoveAir();

}

void CPlayerScript::LandingUpdate()
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	Vec2 m_PosYReverse = Vec2(m_Pos3.x, -m_Pos3.y);
	CTexture* m_MapColTexture = GetOwner()->GetColMapTexture();

	// 충돌맵 이미지 색상 디버그용
	//int temp = m_MapColTexture->GetPixelColor(Vec2(1785, 254));

	if (true == Animator2D()->IsEndAnimation())
	{
		StateChange(PlayerState::Idle);
		return;
	}

	// 이동키를 누르면 IdleToRun 상태로
	if (true == IsMoveKey())
	{
		StateChange(PlayerState::IdleToRun);
		return;
	}

	// 아래쪽에 지형이 없다면 Fall상태로
	int color[5] = {};
	for (int i = 1; i <= 5; ++i)
	{
		color[i - 1] = m_MapColTexture->GetPixelColor(m_PosYReverse + Vector2{ 0.f,(float)i });
	}
	if (color[0] != RGB(0, 0, 0) && color[0] != RGB(255, 0, 0) &&
		color[1] != RGB(0, 0, 0) &&
		color[2] != RGB(0, 0, 0) &&
		color[3] != RGB(0, 0, 0) &&
		color[4] != RGB(0, 0, 0) &&
		m_CurState != PlayerState::Jump)
	{
		StateChange(PlayerState::Fall);
		return;
	}

	// 충돌맵 빨간색이면 아래로 이동 가능
	if (color[0] == RGB(255, 0, 0) &&
		KEY_TAP(KEY::S))
	{
		SetPos(m_Pos + Vector2{ 0, -4 });
	}

	// 회피키를 누르면 Dodge 상태로
	if (KEY_TAP(KEY::LSHIFT))	// @@@ 회피 추가.
	{
		StateChange(PlayerState::Dodge);
		return;
	}

	// 공격
	if (KEY_TAP(KEY::LBTN))
	{
		StateChange(PlayerState::Attack);
		return;
	}


	// 점프키를 누르면 Jump 상태로
	if (KEY_TAP(KEY::SPACE))
	{
		StateChange(PlayerState::Jump);
		return;
	}



}

void CPlayerScript::AttackUpdate()
{
	//// 플레이어 공격이 끝나면 플레이어 공격 충돌체 제거
	//if (true == Animator2D()->IsEndAnimation() &&
	//	m_PlayerAttackCollision != nullptr)
	//{
	//	m_PlayerAttackCollision->SetActive(false);
	//	m_PlayerAttackCollision = nullptr;
	//}

	// 공격 끝날시 Fall 상태로
	if (true == Animator2D()->IsEndAnimation())
	{
		StateChange(PlayerState::Fall);
		return;
	}

	MapCollisionCheckMoveAir();
}

void CPlayerScript::FallUpdate()
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	Vec2 m_PosYReverse = Vec2(m_Pos3.x, -m_Pos3.y);
	CTexture* m_MapColTexture = GetOwner()->GetColMapTexture();


	// 검은 땅에 닿지않고 분홍벽에 부딪힐경우 wallgrab
	int Color = m_MapColTexture->GetPixelColor(m_PosYReverse + Vector2{ 0,1 });
	int LCColor = m_MapColTexture->GetPixelColor(m_PosYReverse + Vector2{ -20,-35 });
	int RCColor = m_MapColTexture->GetPixelColor(m_PosYReverse + Vector2{ 20,-35 });

	if (Color != RGB(0, 0, 0))
	{
		if (LCColor == RGB(255, 0, 255))
		{
			m_CurDir = PlayerDir::Left;
			m_Gravity = 10.0f;
			StateChange(PlayerState::WallGrab);
			return;
		}
		else if (RCColor == RGB(255, 0, 255))
		{
			m_CurDir = PlayerDir::Right;
			m_Gravity = 10.0f;
			StateChange(PlayerState::WallGrab);
			return;
		}
	}


	// 땅에 닿을경우 착지상태로
	// 공중에 뜬 상태일경우 중력영향을 받는다.
	// 중력 가속도에 따른 낙하 속도.
	{
		// 내포지션에서 원하는 위치의 픽셀의 색상을 구할 수 있다.
		int Color = m_MapColTexture->GetPixelColor(m_PosYReverse + Vector2{ 0.f,1.f });
		m_Gravity += m_GravityAccel * DT;
		if (RGB(0, 0, 0) == Color || RGB(255, 0, 0) == Color)	// 땅에 닿을 경우 
		{
			m_Gravity = 10.0f;
			m_MoveDir.Normalize();


			StateChange(PlayerState::Landing);
			return;
		}
		MoveValue(Vector2{ 0.f, -1.f } *m_Gravity * DT);
	}



	// 공격
	if (KEY_TAP(KEY::LBTN))
	{
		StateChange(PlayerState::Attack);
		return;
	}

	if (KEY_PRESSED(KEY::A))
	{
		m_MoveDir += Vector2{ -1.f, 0.f } *DT * 2000.f;
		Vector2 TempX = { m_MoveDir.x, 0.f };

		if (TempX.Length() >= 450.f)
		{
			TempX.Normalize();
			TempX *= 450.f;
			m_MoveDir.x = TempX.x;
		}
	}
	if (KEY_PRESSED(KEY::D))
	{
		m_MoveDir += Vector2{ 1.f, 0.f } *DT * 2000.f;
		Vector2 TempX = { m_MoveDir.x, 0.f };

		if (TempX.Length() >= 450.f)
		{
			TempX.Normalize();
			TempX *= 450.f;
			m_MoveDir.x = TempX.x;
		}
	}

	if (KEY_PRESSED(KEY::S))
	{
		m_MoveDir += Vector2{ 0.f , -1.f } *DT * 4000;
	}

	MapCollisionCheckMoveAir();

}

void CPlayerScript::DodgeUpdate()
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	Vec2 m_PosYReverse = Vec2(m_Pos3.x, -m_Pos3.y);
	CTexture* m_MapColTexture = GetOwner()->GetColMapTexture();


	// 이펙트 생성

	// 닷지 종료시 RunToIdle 상태로
	if (true == Animator2D()->IsEndAnimation())
	{
		StateChange(PlayerState::RunToIdle);
		return;
	}

	// 아래쪽에 지형이 없다면 Fall상태로
	int color[10] = {};
	for (int i = 1; i <= 10; ++i)
	{
		color[i - 1] = m_MapColTexture->GetPixelColor(m_PosYReverse + Vector2{ 0.f,(float)i });
	}
	if (color[0] != RGB(0, 0, 0) && color[0] != RGB(255, 0, 0) &&
		color[1] != RGB(0, 0, 0) &&
		color[2] != RGB(0, 0, 0) &&
		color[3] != RGB(0, 0, 0) &&
		color[4] != RGB(0, 0, 0) &&
		color[5] != RGB(0, 0, 0) &&
		color[6] != RGB(0, 0, 0) &&
		color[7] != RGB(0, 0, 0) &&
		color[8] != RGB(0, 0, 0) &&
		color[9] != RGB(0, 0, 0) &&
		m_CurState != PlayerState::Jump)
	{
		StateChange(PlayerState::Fall);
		return;
	}

	// 점프키를 누르면 Jump 상태로
	if (KEY_TAP(KEY::SPACE))
	{
		StateChange(PlayerState::Jump);
		return;
	}

	// 공격
	if (KEY_TAP(KEY::LBTN))
	{
		StateChange(PlayerState::Attack);
		return;
	}

	MapCollisionCheckMoveGround();

}

void CPlayerScript::PlaySongUpdate()
{
}

void CPlayerScript::HurtFlyLoopUpdate()
{
}

void CPlayerScript::HurtGroundUpdate()
{
}

void CPlayerScript::WallGrabUpdate()
{
}

void CPlayerScript::FlipUpdate()
{
}

void CPlayerScript::DeadUpdate()
{
}

// State Start
void CPlayerScript::IdleStart()
{
	Animator2D()->Play(L"texture\\player\\spr_idle", true);
	m_MoveSpeed = 0.f;

	SetSize2x();
}

void CPlayerScript::IdleToRunStart()
{
	Animator2D()->Play(L"texture\\player\\spr_idle_to_run", true);
	m_MoveSpeed = 200.f;

	SetSize2x();

}

void CPlayerScript::RunStart()
{
	Animator2D()->Play(L"texture\\player\\spr_run", true);
	m_MoveSpeed = 450.f;
	SetSize2x();

	// 런 스타트 구름 이펙트 추가
	// 런 스타트 사운드 추가

}

void CPlayerScript::RunToIdleStart()
{
	Animator2D()->Play(L"texture\\player\\spr_run_to_idle", true);
	m_MoveSpeed = 100.f;

	SetSize2x();
}

void CPlayerScript::JumpStart()
{
	// 점프 이펙트 추가
	// 점프 사운드 추가

	Animator2D()->Play(L"texture\\player\\spr_jump", true);
	SetSize2x();

	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Transform()->SetRelativePos(m_Pos3.x, m_Pos3.y + 4.f, m_Pos3.z);

	m_MoveDir *= m_MoveSpeed;
	m_MoveDir += Vec2(0.f, 1.f) * m_JumpPower;

	m_StateTime[(UINT)PlayerState::Jump] = 0.f;
	m_IsLongJump = false;

}

void CPlayerScript::LandingStart()
{
	// 착지 이펙트 추가
	// 착지 사운드 추가

	Animator2D()->Play(L"texture\\player\\spr_landing", true);
	SetSize2x();
	m_MoveDir = Vec2(0.f, 0.f);
	m_MoveSpeed = 0.f;
	m_AttackCount = 0;

}

void CPlayerScript::AttackStart()
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	Vec2 m_PosYReverse = Vec2(m_Pos3.x, -m_Pos3.y);
	CTexture* m_MapColTexture = GetOwner()->GetColMapTexture();
	CLevel* CurLevel = CLevelMgr::GetInst()->GetCurLevel();

	Vec3 MousePos = CurLevel->FindParentObjectByName(L"Mouse")->Transform()->GetRelativePos();
	Vec3 MainCamPos = CurLevel->FindParentObjectByName(L"MainCamera")->Transform()->GetRelativePos();
	Vec3 MouseWorldPos3 = MousePos + MainCamPos;
	Vec2 MouseWorldPos = Vec2(MouseWorldPos3.x, MouseWorldPos3.y);

	// 어택 사운드 추가
	// 어택 이펙트 추가

	// 공격 방향은 마우스 방향 고정
	if (MouseWorldPos.x >= (m_Pos + Vector2{ 0.f, -35.f }).x)
	{
		m_CurDir = PlayerDir::Right;
	}
	else if (MouseWorldPos.x < (m_Pos + Vector2{ 0.f, -35.f }).x)
	{
		m_CurDir = PlayerDir::Left;
	}

	Animator2D()->Play(L"texture\\player\\spr_attack", true);
	SetSize2x();

	// 플레이어->마우스 방향 벡터 획득
	Vector2 AttackDir = MouseWorldPos - (m_Pos + Vector2{ 0,-35 });
	AttackDir.Normalize();

	// 공격방향 저장.
	m_AttackDir = AttackDir;

	// 공격 판정 충돌체 추가@@@

	m_MoveDir = Vector2{ 0.f, 0.f };
	// 공중에서 최초 한번의 공격일때만 y축 전진성을 부여한다.
	if (m_AttackCount <= 0)
	{
		m_MoveDir = AttackDir * 540.f;
		++m_AttackCount;
	}
	else if (m_AttackCount >= 1)
	{
		// 플레이어는 2회 공격이후 y축 이동 제한, 공중 무한 날기 방지용
		if (AttackDir.y > 0)
		{
			m_MoveDir = Vector2{ AttackDir.x, 0.f } *540.f;
		}
		else
		{
			m_MoveDir = Vector2{ AttackDir.x, AttackDir.y } *540.f;
		}
	}
	m_Gravity = 10.f;	// 공격 후 중력 초기화


}

void CPlayerScript::FallStart()
{
	Animator2D()->Play(L"texture\\player\\spr_fall", true);

	SetSize2x();

}

void CPlayerScript::DodgeStart()
{
	// 닷지 사운드

	m_StateTime[(UINT)PlayerState::Dodge] = 0.f;


	// 점프 후 착지시 방향벡터가 0이 되는순간에 구르기할시 제자리에서 구르기 방지.
	if (m_CurDir == PlayerDir::Right)
		m_MoveDir = Vec2{ 1.f, 0.f };
	else if (m_CurDir == PlayerDir::Left)
		m_MoveDir = Vec2{ -1.f, 0.f };


	Animator2D()->Play(L"texture\\player\\spr_roll", true);
	m_MoveSpeed = 680.f;
	SetSize2x();

}

void CPlayerScript::PlaySongStart()
{
	Animator2D()->Play(L"texture\\player\\spr_player_playsong", true);


	SetSize2x();

}

void CPlayerScript::HurtFlyLoopStart()
{
	Animator2D()->Play(L"texture\\player\\spr_hurtfly_begin", true);

	SetSize2x();

}

void CPlayerScript::HurtGroundStart()
{
	Animator2D()->Play(L"texture\\player\\spr_hurtground", true);


	SetSize2x();

}

void CPlayerScript::WallGrabStart()
{
	Animator2D()->Play(L"texture\\player\\spr_wallgrab", true);


	SetSize2x();

}

void CPlayerScript::FlipStart()
{
	Animator2D()->Play(L"texture\\player\\spr_player_flip", true);

	SetSize2x();

}

void CPlayerScript::DeadStart()
{
	SetSize2x();

}


void CPlayerScript::MapCollisionCheckMoveGround()
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

void CPlayerScript::MapCollisionCheckMoveAir()
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
