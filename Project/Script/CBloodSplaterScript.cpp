#include "pch.h"
#include "CBloodSplaterScript.h"

CBloodSplaterScript::CBloodSplaterScript()
	: CScript((UINT)SCRIPT_TYPE::BLOODSPLATERSCRIPT)
{
}

CBloodSplaterScript::~CBloodSplaterScript()
{
}

void CBloodSplaterScript::begin()
{
}

void CBloodSplaterScript::tick()
{
	Vec3 OwnerPos = m_Owner->Transform()->GetRelativePos();
	Transform()->SetRelativePos(OwnerPos);
}

