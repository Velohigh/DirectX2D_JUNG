#include "pch.h"
#include "CSlashScript.h"
#include "CPlayerScript.h"
#include <Engine/CLevelMgr.h>


CSlashScript::CSlashScript()
	: CScript((UINT)SCRIPT_TYPE::SLASHSCRIPT)
{
}

CSlashScript::~CSlashScript()
{
	if (nullptr != m_Level)
		m_Level = nullptr;
}

void CSlashScript::begin()
{
	m_Level = CLevelMgr::GetInst()->GetCurLevel();
	m_Owner = m_Level->FindParentObjectByName(L"Player");
	m_vAttackDir = m_Owner->GetScript<CPlayerScript>()->GetAttackDir();
	Vec2 XVec2 = Vec2(1.f, 0.f);
	m_vAttackDir.Normalize();

	// 공격 방향으로 이미지 회전
	float Angle = XVec2.Dot(m_vAttackDir);
	Angle = acosf(Angle);
	if (m_vAttackDir.y < 0.f && m_vAttackDir.x >= 0.f)
		Angle = XM_PI * 2 - Angle;

	if(m_vAttackDir.x >= 0)
		Transform()->SetRelativeRot(Vec3(0.f, 0.f, Angle));
	else if (m_vAttackDir.x < 0 && m_vAttackDir.y < 0.f)
	{
		Transform()->SetRelativeRot(Vec3(0.f, XM_PI, XM_PI -Angle));
	}
	else if (m_vAttackDir.x < 0 && m_vAttackDir.y >= 0.f)
	{
		Transform()->SetRelativeRot(Vec3(0.f, XM_PI, XM_PI + Angle));
	}


	Vec3 PlayerPos = m_Owner->Transform()->GetRelativePos();
	PlayerPos += Vec3(0.f, 35.f, 0.f);
	Vec2 AttackPos = m_vAttackDir * 66.f;

	Collider2D()->SetAbsolute(true);
	Collider2D()->SetOffsetScale(Vec2(76.f, 76.f));
	Vec3 CameraPos = m_Level->FindParentObjectByName(L"MainCamera")->Transform()->GetRelativePos();
	Collider2D()->SetOffsetPos(Vec2(AttackPos.x-CameraPos.x, AttackPos.y-CameraPos.y + 35.f));



}

void CSlashScript::tick()
{
	// 애니메이션 재생이 끝나면 삭제
	if (Animator2D()->IsEndAnimation() == true)
	{
		Destroy();
	}

	// 플레이어가 사망 상태가 되면 삭제
	if (m_Owner->GetScript<CPlayerScript>()->GetState() == PlayerState::HurtFlyLoop
		|| m_Owner->GetScript<CPlayerScript>()->GetState() == PlayerState::HurtGround
		|| m_Owner->GetScript<CPlayerScript>()->GetState() == PlayerState::Dead)
	{
		Destroy();
	}

	if (nullptr != m_Owner)
	{
		Vec3 PlayerPos = m_Owner->Transform()->GetRelativePos();
		PlayerPos += Vec3(0.f, 35.f, 0.f);
		Transform()->SetRelativePos(PlayerPos);
	}
	
	Vec3 CameraPos = m_Level->FindParentObjectByName(L"MainCamera")->Transform()->GetRelativePos();
	Vec2 AttackPos = m_vAttackDir * 66.f;
	Collider2D()->SetOffsetPos(Vec2(AttackPos.x - CameraPos.x, AttackPos.y - CameraPos.y));

}

