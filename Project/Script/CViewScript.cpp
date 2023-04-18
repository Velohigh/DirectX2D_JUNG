#include "pch.h"
#include "CViewScript.h"
#include "CGruntScript.h"
#include "CPompScript.h"
#include "CGangsterScript.h"
#include "CMonsterScript.h"

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
	//if (m_Owner->GetScript<CMonsterScript>()->GetState() == ObjState::HurtFly)
	//	Destroy();


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
	// �þ߿� �浹������ ���� ������ ������. ( �÷��̾� HitBox)
	if (_Other->GetOwner()->GetName() == L"Player")
		m_Owner->GetScript<CMonsterScript>()->SetViewColliderOn(true);

	// �þ߿� �ٸ� ���Ͱ� �÷��̾ �Ѱ��ְų�, ��� �����ϰ��, �÷��̾ �Ѿƿ´�.
	else if (_Other->GetOwner()->GetName() == L"Grunt")
	{
		if (_Other->GetOwner()->GetScript<CGruntScript>()->GetState() == ObjState::Run
			|| _Other->GetOwner()->GetScript<CGruntScript>()->GetState() == ObjState::HurtFly
			|| _Other->GetOwner()->GetScript<CGruntScript>()->GetState() == ObjState::HurtGround
			|| _Other->GetOwner()->GetScript<CGruntScript>()->GetState() == ObjState::Dead)
		{
			m_Owner->GetScript<CMonsterScript>()->SetViewColliderOn_Monster(true);
		}
	}

	else if (_Other->GetOwner()->GetName() == L"Pomp")
	{
		if (_Other->GetOwner()->GetScript<CPompScript>()->GetState() == ObjState::Run
			|| _Other->GetOwner()->GetScript<CPompScript>()->GetState() == ObjState::HurtFly
			|| _Other->GetOwner()->GetScript<CPompScript>()->GetState() == ObjState::HurtGround
			|| _Other->GetOwner()->GetScript<CPompScript>()->GetState() == ObjState::Dead)
		{
			m_Owner->GetScript<CMonsterScript>()->SetViewColliderOn_Monster(true);
		}
	}

	else if (_Other->GetOwner()->GetName() == L"Gangster")
	{
		if (_Other->GetOwner()->GetScript<CGangsterScript>()->GetState() == ObjState::Run
			|| _Other->GetOwner()->GetScript<CGangsterScript>()->GetState() == ObjState::HurtFly
			|| _Other->GetOwner()->GetScript<CGangsterScript>()->GetState() == ObjState::HurtGround
			|| _Other->GetOwner()->GetScript<CGangsterScript>()->GetState() == ObjState::Dead)
		{
			m_Owner->GetScript<CMonsterScript>()->SetViewColliderOn_Monster(true);
		}
	}


}

void CViewScript::OnOverlap(CCollider2D* _Other)
{
	if (_Other->GetOwner()->GetName() == L"Player")
		m_Owner->GetScript<CMonsterScript>()->SetViewColliderOn(true);

	// �þ߿� �ٸ� ���Ͱ� �÷��̾ �Ѱ��ְų�, ��� �����ϰ��, �÷��̾ �Ѿƿ´�.
	else if (_Other->GetOwner()->GetName() == L"Grunt")
	{
		if (_Other->GetOwner()->GetScript<CGruntScript>()->GetState() == ObjState::Run
			|| _Other->GetOwner()->GetScript<CGruntScript>()->GetState() == ObjState::HurtFly
			|| _Other->GetOwner()->GetScript<CGruntScript>()->GetState() == ObjState::HurtGround
			|| _Other->GetOwner()->GetScript<CGruntScript>()->GetState() == ObjState::Dead)
		{
			m_Owner->GetScript<CMonsterScript>()->SetViewColliderOn_Monster(true);
		}
	}

	else if (_Other->GetOwner()->GetName() == L"Pomp")
	{
		if (_Other->GetOwner()->GetScript<CPompScript>()->GetState() == ObjState::Run
			|| _Other->GetOwner()->GetScript<CPompScript>()->GetState() == ObjState::HurtFly
			|| _Other->GetOwner()->GetScript<CPompScript>()->GetState() == ObjState::HurtGround
			|| _Other->GetOwner()->GetScript<CPompScript>()->GetState() == ObjState::Dead)
		{
			m_Owner->GetScript<CMonsterScript>()->SetViewColliderOn_Monster(true);
		}
	}

	else if (_Other->GetOwner()->GetName() == L"Gangster")
	{
		if (_Other->GetOwner()->GetScript<CGangsterScript>()->GetState() == ObjState::Run
			|| _Other->GetOwner()->GetScript<CGangsterScript>()->GetState() == ObjState::HurtFly
			|| _Other->GetOwner()->GetScript<CGangsterScript>()->GetState() == ObjState::HurtGround
			|| _Other->GetOwner()->GetScript<CGangsterScript>()->GetState() == ObjState::Dead)
		{
			m_Owner->GetScript<CMonsterScript>()->SetViewColliderOn_Monster(true);
		}
	}

}

void CViewScript::EndOverlap(CCollider2D* _Other)
{
	if (_Other->GetOwner()->GetName() == L"Player")
		m_Owner->GetScript<CMonsterScript>()->SetViewColliderOn(false);



}

