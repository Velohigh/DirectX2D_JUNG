#include "pch.h"
#include "CTimerScript.h"
#include "CPlayerScript.h"

CTimerScript::CTimerScript()
	: CScript((UINT)SCRIPT_TYPE::TIMERSCRIPT)
{
}

CTimerScript::~CTimerScript()
{
}

void CTimerScript::begin()
{
	m_Level = CLevelMgr::GetInst()->GetCurLevel();
	m_Size = Vec3(188.f, 22.f, 1.f);
}

void CTimerScript::tick()
{
	CGameObject* pPlayer = m_Level->FindParentObjectByName(L"Player");
	float Timer = pPlayer->GetScript<CPlayerScript>()->GetTimer();

	Transform()->SetRelativeScale(m_Size.x * Timer / 60.f, m_Size.y, m_Size.z);

}
