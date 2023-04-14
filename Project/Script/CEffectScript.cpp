#include "pch.h"
#include "CEffectScript.h"


CEffectScript::CEffectScript()
	: CScript((UINT)SCRIPT_TYPE::EFFECTSCRIPT)
{
}

CEffectScript::~CEffectScript()
{
}

void CEffectScript::begin()
{
	m_Level = CLevelMgr::GetInst()->GetCurLevel();
}

void CEffectScript::tick()
{
	if (Animator2D()->IsEndAnimation() == true)
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

}
