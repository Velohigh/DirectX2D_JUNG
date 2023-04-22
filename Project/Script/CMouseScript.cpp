#include "pch.h"
#include "CMouseScript.h"

CMouseScript::CMouseScript()
	: CScript((UINT)SCRIPT_TYPE::MOUSESCRIPT)
{
}

CMouseScript::~CMouseScript()
{
}

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
	Vec2 Resolution = GlobalData.Resolution;

	Transform()->SetRelativePos(m_MousePos.x - (Resolution.x/2), -m_MousePos.y + (Resolution.y/2), m_Pos3.z);

	// ȭ�� �ȿ� Ŀ���� ������ �Ⱥ��̰�
	RECT	rc = {};
	GetClientRect(_G_HWND, &rc);

	if (rc.left <= ptMouse.x && ptMouse.x <= rc.right &&
		rc.top <= ptMouse.y && ptMouse.y <= rc.bottom)
	{
		if (m_ShowCursor)
		{
			m_ShowCursor = false;
			//ShowCursor(FALSE);
		}
	}

	else
	{
		if (!m_ShowCursor)
		{
			m_ShowCursor = true;
			//ShowCursor(TRUE);
		}
	}

	// ī�޶� ��ġ�� ���� ���콺 ��ǥ ���
	CLevel* CurLevel = CLevelMgr::GetInst()->GetCurLevel();
	Vec3 CameraPos = CurLevel->FindParentObjectByName(L"MainCamera")->Transform()->GetRelativePos();

	ComputeWorldMousePos(Vec2(CameraPos.x, CameraPos.y));



}

void CMouseScript::begin()
{
	//ShowCursor(FALSE);
	m_ShowCursor = false;
}


