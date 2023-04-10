#include "pch.h"
#include "CBloodAnimation2Script.h"

CBloodAnimation2Script::CBloodAnimation2Script()
	: CScript((UINT)SCRIPT_TYPE::BLOODANIMATION2SCRIPT)
{
}

CBloodAnimation2Script::~CBloodAnimation2Script()
{
}


void CBloodAnimation2Script::begin()
{
}

void CBloodAnimation2Script::tick()
{
	// 회전 구현
	Vec3 Rot = Transform()->GetRelativeRot();
	if (m_CurDir == ObjDir::Right)
		Rot = Vec3(Rot.x, 0.f, Rot.z);
	else if (m_CurDir == ObjDir::Left)
		Rot = Vec3(Rot.x, XM_PI, Rot.z);

	Transform()->SetRelativeRot(Rot);


	Vec3 OwnerPos = m_Owner->Transform()->GetRelativePos();

	if (m_CurDir == ObjDir::Right)
		OwnerPos += Vec3(50.f, 45.f, 10.f);
	else if (m_CurDir == ObjDir::Left)
		OwnerPos += Vec3(-50.f, 45.f, 10.f);

	Transform()->SetRelativePos(OwnerPos);

	if (Animator2D()->IsEndAnimation() == true)
	{
		Destroy();
	}
}

