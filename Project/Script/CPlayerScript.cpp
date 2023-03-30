#include "pch.h"
#include "CPlayerScript.h"

#include <Engine\CMeshRender.h>
#include <Engine\CMaterial.h>

#include "CMissileScript.h"


CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_MoveSpeed(400.f)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MoveSpeed, "Speed");
}

CPlayerScript::~CPlayerScript()
{

}

void CPlayerScript::begin()
{
	MeshRender()->GetDynamicMaterial();


	StateChange(PlayerState::Idle);
}

void CPlayerScript::tick()
{

	StateUpdate();







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
	Transform()->SetRelativeScale(Width*2, Height*2, 1);
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
	CTexture* m_MapColTexture = GetOwner()->GetColMapTexture();
	const Image* Img = m_MapColTexture->GetImage();

	////// 아래쪽에 지형이 없다면 Fall상태로
	//int color = m_MapColTexture->GetPixelColor(m_Pos + Vec2{ 0,10 });
	//int Rcolor = m_MapColTexture->GetPixelColor(m_Pos + Vec2{ 0,1 });
	//if (color != RGB(0, 0, 0) && m_CurState != PlayerState::Jump &&
	//	Rcolor != RGB(255, 0, 0) &&
	//	Rcolor != RGB(0, 0, 0))
	//{
	//	StateChange(PlayerState::Fall);
	//	return;
	//}




}

void CPlayerScript::IdleToRunUpdate()
{
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

}

void CPlayerScript::RunUpdate()
{
}

void CPlayerScript::RunToIdleUpdate()
{
}

void CPlayerScript::JumpUpdate()
{
}

void CPlayerScript::LandingUpdate()
{
}

void CPlayerScript::AttackUpdate()
{
}

void CPlayerScript::FallUpdate()
{
}

void CPlayerScript::DodgeUpdate()
{
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
	m_MoveSpeed = 0.f;

	SetSize2x();

}

void CPlayerScript::RunStart()
{
	Animator2D()->Play(L"texture\\player\\spr_run", true);
	m_MoveSpeed = 450.f;

	// 런 스타트 구름 이펙트

	SetSize2x();
}

void CPlayerScript::RunToIdleStart()
{
	Animator2D()->Play(L"texture\\player\\spr_run_to_idle", true);
	m_MoveSpeed = 100.f;

	SetSize2x();
}

void CPlayerScript::JumpStart()
{
	Animator2D()->Play(L"texture\\player\\spr_jump", true);

	SetSize2x();

	// 점프 이펙트
	// 점프 사운드


}

void CPlayerScript::LandingStart()
{
	Animator2D()->Play(L"texture\\player\\spr_landing", true);


	SetSize2x();

}

void CPlayerScript::AttackStart()
{
	Animator2D()->Play(L"texture\\player\\spr_attack", true);


	SetSize2x();

}

void CPlayerScript::FallStart()
{
	Animator2D()->Play(L"texture\\player\\spr_fall", true);

	SetSize2x();

}

void CPlayerScript::DodgeStart()
{
	Animator2D()->Play(L"texture\\player\\spr_roll", true);


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
