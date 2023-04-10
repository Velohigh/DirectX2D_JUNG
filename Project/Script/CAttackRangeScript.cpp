#include "pch.h"
#include "CAttackRangeScript.h"
#include "CGruntScript.h"

CAttackRangeScript::CAttackRangeScript()
	: CScript((UINT)SCRIPT_TYPE::ATTACKRANGESCRIPT)
{
}

CAttackRangeScript::~CAttackRangeScript()
{
}


void CAttackRangeScript::begin()
{
	m_Level = CLevelMgr::GetInst()->GetCurLevel();
}

void CAttackRangeScript::tick()
{
	Vec3 CameraPos = m_Level->FindParentObjectByName(L"MainCamera")->Transform()->GetRelativePos();
	Vec3 OwnerPos = m_Owner->Transform()->GetRelativePos();
	//OwnerPos += Vec3(0.f, 35.f, 0.f);
	Transform()->SetRelativePos(OwnerPos);

}

void CAttackRangeScript::BeginOverlap(CCollider2D* _Other)
{
	m_Owner->GetScript<CMonsterScript>()->SetAttackRangeOn(true);
}

void CAttackRangeScript::OnOverlap(CCollider2D* _Other)
{
	m_Owner->GetScript<CMonsterScript>()->SetAttackRangeOn(true);
}

void CAttackRangeScript::EndOverlap(CCollider2D* _Other)
{
	m_Owner->GetScript<CMonsterScript>()->SetAttackRangeOn(false);
}


