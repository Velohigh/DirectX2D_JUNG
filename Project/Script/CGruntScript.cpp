#include "pch.h"
#include "CGruntScript.h"
#include "CPlayerScript.h"

CGruntScript::CGruntScript()
	: CScript((UINT)SCRIPT_TYPE::GRUNTSCRIPT)
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


void CGruntScript::IdleStart()
{
	m_StateTime[(UINT)ObjState::Idle] = 0.f;
	Animator2D()->Play(L"texture\\grunt\\spr_grunt_idle", true);
	SetSpeed(0.f);


}

void CGruntScript::WalkStart()
{
	m_StateTime[static_cast<int>(ObjState::Walk)] = 0.f;
	Animator2D()->Play(L"texture\\grunt\\spr_grunt_walk", true);
	SetSpeed(80.f);

}

void CGruntScript::TurnStart()
{
	m_StateTime[static_cast<int>(ObjState::Turn)] = 0.f;
	Animator2D()->Play(L"texture\\grunt\\spr_grunt_turn", true);
	SetSpeed(0.f);


}

void CGruntScript::RunStart()
{
	m_StateTime[(int)ObjState::Run] = 0.f;
	Animator2D()->Play(L"texture\\grunt\\spr_grunt_run", true);
	SetSpeed(0.f);

	// Enemy_Follow 이펙트
}

void CGruntScript::AttackStart()
{
	Animator2D()->Play(L"texture\\grunt\\spr_grunt_attack", true);
	SetSpeed(0.f);
}

void CGruntScript::HurtGroundStart()
{
	// 피분출 애니메이션

	m_StateTime[static_cast<int>(ObjState::HurtGround)] = 0.f;
	Animator2D()->Play(L"texture\\grunt\\spr_grunt_hurtground", false);

}

void CGruntScript::HurtFlyStart()
{
}

void CGruntScript::DeadStart()
{
}

void CGruntScript::IdleUpdate()
{
}

void CGruntScript::WalkUpdate()
{
}

void CGruntScript::TurnUpdate()
{
}

void CGruntScript::RunUpdate()
{
}

void CGruntScript::AttackUpdate()
{
}

void CGruntScript::HurtGroundUpdate()
{
}

void CGruntScript::HurtFlyUpdate()
{
}

void CGruntScript::DeadUpdate()
{
}
