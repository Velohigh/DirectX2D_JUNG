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



}

void CPlayerScript::tick()
{








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

}

void CPlayerScript::IdleToRunUpdate()
{
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
}

void CPlayerScript::IdleToRunStart()
{
	Animator2D()->Play(L"texture\\player\\spr_idle_to_run", true);
	m_MoveSpeed = 0.f;
}

void CPlayerScript::RunStart()
{
	Animator2D()->Play(L"texture\\player\\spr_run", true);
	m_MoveSpeed = 450.f;

	// 런 스타트 구름 이펙트



}

void CPlayerScript::RunToIdleStart()
{
	Animator2D()->Play(L"texture\\player\\spr_run_to_idle", true);
	m_MoveSpeed = 100.f;


}

void CPlayerScript::JumpStart()
{
	// 점프 이펙트
	// 점프 사운드


}

void CPlayerScript::LandingStart()
{
}

void CPlayerScript::AttackStart()
{
}

void CPlayerScript::FallStart()
{
}

void CPlayerScript::DodgeStart()
{
}

void CPlayerScript::PlaySongStart()
{
}

void CPlayerScript::HurtFlyLoopStart()
{
}

void CPlayerScript::HurtGroundStart()
{
}

void CPlayerScript::WallGrabStart()
{
}

void CPlayerScript::FlipStart()
{
}

void CPlayerScript::DeadStart()
{
}
