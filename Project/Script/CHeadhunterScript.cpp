#include "pch.h"
#include "CHeadhunterScript.h"

CHeadhunterScript::CHeadhunterScript()
	: CMonsterScript()
	, m_CurState(BossState::End)
{
}

CHeadhunterScript::~CHeadhunterScript()
{
}

void CHeadhunterScript::BeginOverlap(CCollider2D* _Other)
{
}

void CHeadhunterScript::begin()
{
	MeshRender()->GetDynamicMaterial();

	m_Level = CLevelMgr::GetInst()->GetCurLevel();
	wstring LevelName = m_Level->GetName();

	m_Gravity = 1000.f;
	m_GravityAccel = 2000.f;



	Ptr<CTexture> Colmap = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\map\\LastStage_collision.png");
	GetOwner()->SetColMapTexture(Colmap.Get());

	if (m_CurState != m_BeginState)
	{
		StateChange(m_BeginState);
	}

	if (m_BeginDir != ObjDir::End)
	{
		m_CurDir = m_BeginDir;
		m_PreDir = ObjDir::End;
		DirAnimationCheck();
	}
	SetSize2x();

}

void CHeadhunterScript::tick()
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();

	DirAnimationCheck();
	StateUpdate();

	Vec3 CameraPos = m_Level->FindParentObjectByName(L"MainCamera")->Transform()->GetRelativePos();
	Collider2D()->SetOffsetPos(Vec2(-CameraPos.x, -CameraPos.y + 35.f));


}

void CHeadhunterScript::DirAnimationCheck()
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

void CHeadhunterScript::StateChange(BossState _State)
{
	if (_State != m_CurState)
	{
		switch (_State)
		{
		case BossState::Idle:
			IdleS();
			break;


		}
		m_CurState = _State;
	}

}

void CHeadhunterScript::StateUpdate()
{
	switch (m_CurState)
	{
	case BossState::Idle:
		IdleU();
		break;

	}

}


// FSM Start //
void CHeadhunterScript::IdleS()
{
	Animator2D()->Play(L"texture\\headhunter\\spr_headhunter_idle", true);
	m_MoveSpeed = 0.f;

	SetSize2x();

}

void CHeadhunterScript::TakeoutgunS()
{
}

void CHeadhunterScript::PutbackgunS()
{
}

void CHeadhunterScript::ShootS()
{
}

void CHeadhunterScript::TakeoutrifleS()
{
}

void CHeadhunterScript::PutbackrifleS()
{
}

void CHeadhunterScript::Aim_rifleS()
{
}

void CHeadhunterScript::HurtS()
{
}

void CHeadhunterScript::RecoverS()
{
}

void CHeadhunterScript::WallidleS()
{
}

void CHeadhunterScript::WalljumpS()
{
}

void CHeadhunterScript::Walljump_landS()
{
}

void CHeadhunterScript::PredashS()
{
}

void CHeadhunterScript::DashS()
{
}

void CHeadhunterScript::Dash_end_groundS()
{
}

void CHeadhunterScript::PrejumpS()
{
}

void CHeadhunterScript::JumpS()
{
}

void CHeadhunterScript::Teleport_inS()
{
}

void CHeadhunterScript::Teleport_outS()
{
}

void CHeadhunterScript::Teleport_in_groundS()
{
}

void CHeadhunterScript::Teleport_out_groundS()
{
}

void CHeadhunterScript::Teleport_in_sweepS()
{
}

void CHeadhunterScript::SweepS()
{
}

void CHeadhunterScript::Teleport_out_sweepS()
{
}

void CHeadhunterScript::DodgerollS()
{
}

void CHeadhunterScript::HurtflyS()
{
}

void CHeadhunterScript::HurtgroundS()
{
}

// FSM Update //
void CHeadhunterScript::IdleU()
{
}

void CHeadhunterScript::TakeoutgunU()
{
}

void CHeadhunterScript::PutbackgunU()
{
}

void CHeadhunterScript::ShootU()
{
}

void CHeadhunterScript::TakeoutrifleU()
{
}

void CHeadhunterScript::PutbackrifleU()
{
}

void CHeadhunterScript::Aim_rifleU()
{
}

void CHeadhunterScript::HurtU()
{
}

void CHeadhunterScript::RecoverU()
{
}

void CHeadhunterScript::WallidleU()
{
}

void CHeadhunterScript::WalljumpU()
{
}

void CHeadhunterScript::Walljump_landU()
{
}

void CHeadhunterScript::PredashU()
{
}

void CHeadhunterScript::DashU()
{
}

void CHeadhunterScript::Dash_end_groundU()
{
}

void CHeadhunterScript::PrejumpU()
{
}

void CHeadhunterScript::JumpU()
{
}

void CHeadhunterScript::Teleport_inU()
{
}

void CHeadhunterScript::Teleport_outU()
{
}

void CHeadhunterScript::Teleport_in_groundU()
{
}

void CHeadhunterScript::Teleport_out_groundU()
{
}

void CHeadhunterScript::Teleport_in_sweepU()
{
}

void CHeadhunterScript::SweepU()
{
}

void CHeadhunterScript::Teleport_out_sweepU()
{
}

void CHeadhunterScript::DodgerollU()
{
}

void CHeadhunterScript::HurtflyU()
{
}

void CHeadhunterScript::HurtgroundU()
{
}
