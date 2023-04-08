#include "pch.h"
#include "CBloodAnimationScript.h"
#include "CGruntScript.h"

CBloodAnimationScript::CBloodAnimationScript()
	: CScript((UINT)SCRIPT_TYPE::BLOODANIMATIONSCRIPT)
{
}

CBloodAnimationScript::~CBloodAnimationScript()
{
	if (nullptr != m_Level)
		m_Level = nullptr;
}

void CBloodAnimationScript::begin()
{

}

void CBloodAnimationScript::tick()
{
	// ȸ�� ����
	Vec3 Rot = Transform()->GetRelativeRot();
	if (m_CurDir == ObjDir::Right)
		Rot = Vec3(Rot.x, 0.f, Rot.z);
	else if (m_CurDir == ObjDir::Left)
		Rot = Vec3(Rot.x, XM_PI, Rot.z);

	Transform()->SetRelativeRot(Rot);


	Vec3 OwnerPos = m_Owner->Transform()->GetRelativePos();

	if (m_CurDir == ObjDir::Right)
		OwnerPos += Vec3(40.f, 50.f, 10.f);
	else if (m_CurDir == ObjDir::Left)
		OwnerPos += Vec3(-40.f, 50.f, 10.f);


	Transform()->SetRelativePos(OwnerPos);

	if (m_Owner->GetScript<CGruntScript>()->GetState() != ObjState::HurtFly)
	{
		Destroy();
	}
}


