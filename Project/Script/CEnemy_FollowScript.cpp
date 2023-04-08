#include "pch.h"
#include "CEnemy_FollowScript.h"
#include "CGruntScript.h"

CEnemy_FollowScript::CEnemy_FollowScript()
	: CScript((UINT)SCRIPT_TYPE::ENEMY_FOLLOWSCRIPT)
{
}

CEnemy_FollowScript::~CEnemy_FollowScript()
{
}

void CEnemy_FollowScript::begin()
{
	m_Level = CLevelMgr::GetInst()->GetCurLevel();

}

void CEnemy_FollowScript::tick()
{
	// 몬스터 위에 느낌표 표시
	if (nullptr != m_Owner)
	{
		Vec3 OwnerPos = Transform()->GetRelativePos();
		Transform()->SetRelativePos(OwnerPos + Vec3(0.f, 80.f, 0.f));
	}

	// 몬스터 사망시 오브젝트 파괴
	if (m_Owner->GetScript<CGruntScript>()->GetState() == ObjState::HurtFly
		|| m_Owner->GetScript<CGruntScript>()->GetState() == ObjState::HurtGround
		|| m_Owner->GetScript<CGruntScript>()->GetState() == ObjState::Dead)
	{
		Destroy();
	}
}


