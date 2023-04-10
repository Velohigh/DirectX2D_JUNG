#include "pch.h"
#include "CGruntSlashScript.h"
#include "CGruntScript.h"

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

}

void CGruntSlashScript::tick()
{
	if (Animator2D()->IsEndAnimation() == true)
	{
		Destroy();
	}
	// �׷�Ʈ�� ��������̸� ���� ����Ʈ �ı�
	if (m_Owner->GetScript<CGruntScript>()->GetState() == ObjState::HurtFly
		|| m_Owner->GetScript<CGruntScript>()->GetState() == ObjState::HurtGround
		|| m_Owner->GetScript<CGruntScript>()->GetState() == ObjState::Dead)
	{
		Destroy();
	}

	// ȸ�� ����
	Vec3 Rot = Transform()->GetRelativeRot();
	if (m_CurDir == ObjDir::Right)
		Rot = Vec3(Rot.x, 0.f, Rot.z);
	else if (m_CurDir == ObjDir::Left)
		Rot = Vec3(Rot.x, XM_PI, Rot.z);

	Transform()->SetRelativeRot(Rot);

	Vec3 OwnerPos = m_Owner->Transform()->GetRelativePos();

	if (nullptr != m_Owner)
	{
		Vec3 OwnerPos = m_Owner->Transform()->GetRelativePos();
		OwnerPos += Vec3(0.f, 50.f, 0.f);

		if(m_CurDir == ObjDir::Right)
			Transform()->SetRelativePos(OwnerPos+Vec3(30.f, 0.f, 0.f));
		else if (m_CurDir == ObjDir::Left)
			Transform()->SetRelativePos(OwnerPos + Vec3(-30.f, 0.f, 0.f));

	}

}

