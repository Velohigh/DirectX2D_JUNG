#include "pch.h"
#include "CGruntSlashScript.h"
#include "CMonsterScript.h"
#include "CGruntScript.h"
#include "CPlayerScript.h"


CGruntSlashScript::CGruntSlashScript()
	: CScript((UINT)SCRIPT_TYPE::GRUNTSLASHSCRIPT)
{
}

CGruntSlashScript::~CGruntSlashScript()
{
}

void CGruntSlashScript::begin()
{
	m_Level = CLevelMgr::GetInst()->GetCurLevel();

	if (m_CurDir == ObjDir::Right)
	{
		m_vAttackDir = Vec2(1.f, 0.f);
	}
	else if (m_CurDir == ObjDir::Left)
	{
		m_vAttackDir = Vec2(-1.f, 0.f);
	}

	Vec3 OwnerPos = m_Owner->Transform()->GetRelativePos();
	OwnerPos += Vec3(0.f, 45.f, 0.f);
	Vec2 AttackPos = m_vAttackDir * 30.f;


	Collider2D()->SetAbsolute(true);
	Collider2D()->SetOffsetScale(Vec2(60.f, 60.f));
	Vec3 CameraPos = m_Level->FindParentObjectByName(L"MainCamera")->Transform()->GetRelativePos();
	Collider2D()->SetOffsetPos(Vec2(AttackPos.x - CameraPos.x, AttackPos.y - CameraPos.y + 35.f));
}

void CGruntSlashScript::tick()
{
	// 공격 이펙트 애니메이션이 끝나면 파괴
	if (Animator2D()->IsEndAnimation() == true)
	{
		Destroy();
	}
	if (m_Owner == nullptr)
		return;

	// 주인이 사망상태이면 공격 이펙트 파괴
	if (m_Owner->GetScript<CMonsterScript>()->GetState() == ObjState::HurtFly
		|| m_Owner->GetScript<CMonsterScript>()->GetState() == ObjState::HurtGround
		|| m_Owner->GetScript<CMonsterScript>()->GetState() == ObjState::Dead)
	{
		Destroy();
	}

	// 회전 구현
	Vec3 Rot = Transform()->GetRelativeRot();
	if (m_CurDir == ObjDir::Right)
		Rot = Vec3(Rot.x, 0.f, Rot.z);
	else if (m_CurDir == ObjDir::Left)
		Rot = Vec3(Rot.x, XM_PI, Rot.z);
	Transform()->SetRelativeRot(Rot);

	// 위치 지정
	Vec3 OwnerPos = m_Owner->Transform()->GetRelativePos();
	if (nullptr != m_Owner)
	{
		Vec3 OwnerPos = m_Owner->Transform()->GetRelativePos();
		OwnerPos += Vec3(0.f, 45.f, 0.f);

		if (m_CurDir == ObjDir::Right)
			Transform()->SetRelativePos(OwnerPos + Vec3(30.f, 0.f, 0.f));
		else if (m_CurDir == ObjDir::Left)
			Transform()->SetRelativePos(OwnerPos + Vec3(-30.f, 0.f, 0.f));

	}

	Vec3 CameraPos = m_Level->FindParentObjectByName(L"MainCamera")->Transform()->GetRelativePos();
	Vec2 AttackPos = m_vAttackDir * 30.f;
	Collider2D()->SetOffsetPos(Vec2(AttackPos.x - CameraPos.x, AttackPos.y - CameraPos.y));


}

void CGruntSlashScript::BeginOverlap(CCollider2D* _Other)
{
	if (_Other->GetOwner()->GetName() == L"Player")
	{
		// 플레이어가 사망 상태가 아니고 , 구르기 판정이 아닐떄 히트판정
		PlayerState m_PState = _Other->GetOwner()->GetScript<CPlayerScript>()->GetState();
		if (m_PState != PlayerState::HurtFlyLoop &&
			m_PState != PlayerState::HurtGround &&
			m_PState != PlayerState::Dead &&
			m_PState != PlayerState::Dodge)
		{
			if (m_CurDir == ObjDir::Right)
			{
				_Other->GetOwner()->GetScript<CPlayerScript>()->SetEnemyAttackDir(Vector2{ 1.f , 0.5f } *800);
			}
			else if (m_CurDir == ObjDir::Left)
			{
				_Other->GetOwner()->GetScript<CPlayerScript>()->SetEnemyAttackDir(Vector2{ -1.f , 0.5f } *800);
			}

			_Other->GetOwner()->GetScript<CPlayerScript>()->StateChange(PlayerState::HurtFlyLoop);
			return;
		}
	}

}
