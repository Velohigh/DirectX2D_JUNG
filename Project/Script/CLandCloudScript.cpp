#include "pch.h"
#include "CLandCloudScript.h"

CLandCloudScript::CLandCloudScript()
	: CScript((UINT)SCRIPT_TYPE::LANDCLOUDSCRIPT)

{
}

CLandCloudScript::~CLandCloudScript()
{
}

void CLandCloudScript::begin()
{
}

void CLandCloudScript::tick()
{
	if (Animator2D()->IsEndAnimation() == true)
	{
		Destroy();
	}
}

