#include "pch.h"
#include "CViewScript.h"
#include "CGruntScript.h"

CViewScript::CViewScript()
	: CScript((UINT)SCRIPT_TYPE::VIEWSCRIPT)
{
}

CViewScript::~CViewScript()
{
}

void CViewScript::begin()
{
	m_Level = CLevelMgr::GetInst()->GetCurLevel();

}

void CViewScript::tick()
{
	Vec3 CameraPos = m_Level->FindParentObjectByName(L"MainCamera")->Transform()->GetRelativePos();
	Vec3 OwnerPos = m_Owner->Transform()->GetRelativePos();
	Transform()->SetRelativePos(OwnerPos);

	//Collider2D()->SetOffsetPos(Vec2( -CameraPos.x, -CameraPos.y));

	//if (m_Owner->GetScript<CGruntScript>()->GetDir() == ObjDir::Right)
	//{
	//	Collider2D()->SetOffsetPos(250.f, -60.f);
	//}

}

void CViewScript::BeginOverlap(CCollider2D* _Other)
{
	m_Owner->GetScript<CGruntScript>()->SetViewColliderOn(true);
}

void CViewScript::OnOverlap(CCollider2D* _Other)
{
	m_Owner->GetScript<CGruntScript>()->SetViewColliderOn(true);
}

void CViewScript::EndOverlap(CCollider2D* _Other)
{
	m_Owner->GetScript<CGruntScript>()->SetViewColliderOn(false);
}

