#include "pch.h"
#include "CMouseScript.h"

void CMouseScript::tick()
{
	// ���콺 ��ġ�� ���콺 Obj ��ġ �̵�
	POINT ptMouse;

	GetCursorPos(&ptMouse);

	// ��ũ�� ��ǥ�� ������ ��ǥ�� �����Ѵ�.
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
