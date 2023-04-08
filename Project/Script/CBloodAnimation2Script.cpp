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
	if (nullptr != m_Owner)
	{
		Vec3 OwnerPos = m_Owner->Transform()->GetRelativePos();
		Transform()->SetRelativePos(OwnerPos);
	}

	if (Animator2D()->IsEndAnimation() == true)
	{
		Destroy();
	}
}

