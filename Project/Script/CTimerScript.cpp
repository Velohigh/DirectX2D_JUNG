#include "pch.h"
#include "CTimerScript.h"

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
}

void CTimerScript::tick()
{
}
