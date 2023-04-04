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

	//if (m_CurDir == ObjDir::Right)
	//{
	//	m_ViewCollider->SetOffset(250.f, -60.f);
	//	m_MyAttackDir = Vector2{ 1.f, -0.5f } *800;
	//}
	//else if (m_CurDir == ObjDir::Left)
	//{
	//	m_ViewCollider->SetOffset(-250.f, -60.f);
	//	m_MyAttackDir = Vector2{ -1.f, -0.5f } *800;
	//}


}

void CGruntScript::DirAnimationCheck()
{

}

void CGruntScript::StateChange(ObjState _State)
{
}

void CGruntScript::StateUpdate()
{
}

