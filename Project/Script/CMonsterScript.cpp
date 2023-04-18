#include "pch.h"
#include "CMonsterScript.h"
#include "CPlayerScript.h"


CMonsterScript::CMonsterScript()
	: CScript((UINT)SCRIPT_TYPE::MONSTERSCRIPT)
	, m_CurState(ObjState::END)
	, m_CurDir(ObjDir::End)
{
}

CMonsterScript::~CMonsterScript()
{
}

void CMonsterScript::begin()
{
	MeshRender()->GetDynamicMaterial();

	m_Level = CLevelMgr::GetInst()->GetCurLevel();
	wstring LevelName = m_Level->GetName();

	// 충돌맵 설정
	if (LevelName == L"Stage_1")
	{
		Ptr<CTexture> Colmap = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\map\\room_factory_2_ColMap.png");
		GetOwner()->SetColMapTexture(Colmap.Get());
	}
	else if (LevelName == L"Stage_2")
	{
		Ptr<CTexture> Colmap = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\map\\stage2_bg_collision.png");
		GetOwner()->SetColMapTexture(Colmap.Get());
	}
	else if (LevelName == L"Stage_3")
	{
		Ptr<CTexture> Colmap = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\map\\stage3_bg_collision.png");
		GetOwner()->SetColMapTexture(Colmap.Get());
	}


	m_Gravity = 1000.f;
	m_GravityAccel = 2000.f;

	if (m_BeginDir != ObjDir::End)
	{
		m_PreDir = ObjDir::End;
		m_CurDir = m_BeginDir;
		DirAnimationCheck();
	}

	if (m_BeginState != ObjState::END)
		StateChange(m_BeginState);

	SetSize2x();

	m_bViewColliderOn = false;				// 시야 범위 충돌체
	m_bAttackRangeOn = false;				// 공격 범위 충돌체
	m_bHitBoxOn = false;					// 히트 박스 충돌체
	m_bEffect_EnemyFollow = false;
	m_bAttackOn = false;

}

void CMonsterScript::tick()
{
	CGameObject* Player = m_Level->FindParentObjectByName(L"Player");
	if (nullptr == Player)
		return;

	m_PrePos = Transform()->GetRelativePos();

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
	m_CurPos = Transform()->GetRelativePos();

	// 이동량
	m_Move = Vec2(m_PrePos.x - m_CurPos.x, m_PrePos.y - m_CurPos.y);

	// 시야 충돌체 위치 지정
	Vec3 CameraPos = m_Level->FindParentObjectByName(L"MainCamera")->Transform()->GetRelativePos();
	if (m_CurDir == ObjDir::Right)
		m_ViewCollider->Collider2D()->SetOffsetPos(-CameraPos.x + 250.f, -CameraPos.y + 50.f);
	else if (m_CurDir == ObjDir::Left)
		m_ViewCollider->Collider2D()->SetOffsetPos(-CameraPos.x - 250.f, -CameraPos.y + 50.f);

	// 공격 범위 충돌체 위치 지정
	m_AttackRangeCollider->Collider2D()->SetOffsetPos(-CameraPos.x, -CameraPos.y + 40.f);

	// HitBox 충돌체 위치 지정
	Collider2D()->SetOffsetPos(Vec2(-CameraPos.x, -CameraPos.y + 35.f));

	m_PrePos = m_CurPos;

}

void CMonsterScript::DirAnimationCheck()
{
	// 바라보는 방향에 따라 좌우 반전
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

void CMonsterScript::StateChange(ObjState _State)
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
		case ObjState::KnockDown:
			KnockDownStart();
			break;
		case ObjState::Dead:
			DeadStart();
			break;

		}
		m_CurState = _State;
	}

}

void CMonsterScript::StateUpdate()
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
	case ObjState::KnockDown:
		KnockDownUpdate();
		break;
	case ObjState::Dead:
		DeadUpdate();
		break;
	}

}


void CMonsterScript::SetPos(Vec2 _vec2)
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Transform()->SetRelativePos(_vec2.x, _vec2.y, m_Pos3.z);

}

void CMonsterScript::SetSize2x()
{
	vector<Ptr<CTexture>> VecFolderTex = Animator2D()->GetCurAnim()->GetvecFolderTex();
	int CurFrmCount = Animator2D()->GetCurAnim()->GetCurFrmCount();

	float Width = VecFolderTex[CurFrmCount]->Width();
	float Height = VecFolderTex[CurFrmCount]->Height();
	Transform()->SetRelativeScale(Width * 2, Height * 2, 1);
}

void CMonsterScript::BeginOverlap(CCollider2D* _Other)
{
	if(_Other->GetOwner()->GetName() == L"Slash")
		m_bHitBoxOn = true;
}

void CMonsterScript::OnOverlap(CCollider2D* _Other)
{
	if (_Other->GetOwner()->GetName() == L"Slash")
		m_bHitBoxOn = true;
}

void CMonsterScript::EndOverlap(CCollider2D* _Other)
{
	if (_Other->GetOwner()->GetName() == L"Slash")
		m_bHitBoxOn = false;
}

void CMonsterScript::MoveDir(const Vec2& Dir)
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	m_Pos += Dir * m_MoveSpeed * DT;

	Transform()->SetRelativePos(m_Pos.x, m_Pos.y, m_Pos3.z);
}

void CMonsterScript::MoveValue(const Vector2& MoveValue)
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);

	m_Pos += MoveValue /*m_TimeScale*/;

	Transform()->SetRelativePos(m_Pos.x, m_Pos.y, m_Pos3.z);

}

void CMonsterScript::MapCollisionCheckMoveGround()
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

void CMonsterScript::MapCollisionCheckMoveAir()
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

void CMonsterScript::MapCollisionCheckMoveGroundDie()
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
			MoveValue(Vector2{ 0.f, m_MoveDir.y } * DT);
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

		if (RGB(0, 0, 0) != Color &&
			RGB(0, 0, 0) != TopRightColor &&
			RGB(0, 0, 0) != TopLeftColor &&
			RGB(255, 0, 255) != Color &&
			RGB(255, 0, 255) != TopRightColor &&
			RGB(255, 0, 255) != TopLeftColor)
		{
			MoveValue(Vector2{ m_MoveDir.x,0.f } * DT);
		}
	}

}


void CMonsterScript::MapCollisionCheckMoveAirDie()
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
