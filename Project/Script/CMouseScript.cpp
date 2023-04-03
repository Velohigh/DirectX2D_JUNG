#include "pch.h"
#include "CMouseScript.h"

void CMouseScript::tick()
{
	// 마우스 위치로 마우스 Obj 위치 이동
	POINT ptMouse;

	GetCursorPos(&ptMouse);

	// 스크린 좌표를 윈도우 좌표로 변경한다.
	ScreenToClient(_G_HWND, &ptMouse);

	m_MouseMove.x = (float)ptMouse.x - m_MousePos.x;
	m_MouseMove.y = (float)ptMouse.y - m_MousePos.y;


	m_MousePos.x = (float)ptMouse.x;
	m_MousePos.y = (float)ptMouse.y;

	Vec3 m_Pos3 = Transform()->GetRelativePos();

	Transform()->SetRelativePos(m_MousePos.x, m_MousePos.y, m_Pos3.z);


}

CMouseScript::CMouseScript()
	: CScript((UINT)SCRIPT_TYPE::MOUSESCRIPT)
{
}

CMouseScript::~CMouseScript()
{
}
