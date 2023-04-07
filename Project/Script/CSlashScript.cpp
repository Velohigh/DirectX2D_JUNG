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


}

void CSlashScript::tick()
{
	if (Animator2D()->IsEndAnimation() == true)
	{
		Destroy();
	}

	if (nullptr != m_Owner)
	{
		Vec3 PlayerPos = m_Owner->Transform()->GetRelativePos();
		PlayerPos += Vec3(0.f, 35.f, 0.f);
		Transform()->SetRelativePos(PlayerPos);
	}
	
}

