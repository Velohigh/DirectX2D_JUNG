#include "pch.h"
#include "CBatteryScript.h"


CBatteryScript::CBatteryScript()
	: CScript((UINT)SCRIPT_TYPE::BATTERYSCRIPT)
{
}

CBatteryScript::~CBatteryScript()
{
}
void CBatteryScript::begin()
{
	m_Level = CLevelMgr::GetInst()->GetCurLevel();
}

void CBatteryScript::tick()
{
}
