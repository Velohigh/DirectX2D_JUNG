#include "pch.h"
#include "CBatteryScript.h"
#include "CPlayerScript.h"


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
	m_Size = Vec3(110.f, 20.f, 1.f);
}

void CBatteryScript::tick()
{
	CGameObject* pPlayer = m_Level->FindParentObjectByName(L"Player");
	float Battery = pPlayer->GetScript<CPlayerScript>()->GetBattery();

	Transform()->SetRelativeScale(m_Size.x * Battery, m_Size.y, m_Size.z);
}
