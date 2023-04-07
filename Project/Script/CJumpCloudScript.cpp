#include "pch.h"
#include "CJumpCloudScript.h"

CJumpCloudScript::CJumpCloudScript()
	: CScript((UINT)SCRIPT_TYPE::JUMPCLOUDSCRIPT)

{
}

CJumpCloudScript::~CJumpCloudScript()
{
}

void CJumpCloudScript::begin()
{
}

void CJumpCloudScript::tick()
{
	if (Animator2D()->IsEndAnimation() == true)
	{
		Destroy();
	}
}
