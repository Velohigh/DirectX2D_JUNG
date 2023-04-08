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
	// ���� ���� ����ǥ ǥ��
	if (nullptr != m_Owner)
	{
		Vec3 OwnerPos = Transform()->GetRelativePos();
		Transform()->SetRelativePos(OwnerPos + Vec3(0.f, 80.f, 0.f));
	}

	// ���� ����� ������Ʈ �ı�
	if (m_Owner->GetScript<CGruntScript>()->GetState() == ObjState::HurtFly
		|| m_Owner->GetScript<CGruntScript>()->GetState() == ObjState::HurtGround
		|| m_Owner->GetScript<CGruntScript>()->GetState() == ObjState::Dead)
	{
		Destroy();
	}
}


