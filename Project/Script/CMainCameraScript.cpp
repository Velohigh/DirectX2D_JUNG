#include "pch.h"
#include "CMainCameraScript.h"
#include <random>


CMainCameraScript::CMainCameraScript()
	: CScript((UINT)SCRIPT_TYPE::MAINCAMERASCRIPT)
	, m_fCamSpeed(100.f)
	, m_bFollowPlayer(true)
{
}

CMainCameraScript::~CMainCameraScript()
{
}

void CMainCameraScript::tick()
{
	// ī�޶� �̵�
	Vec3 vCameraPos = Transform()->GetRelativePos();

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	CGameObject* vPlayer = pCurLevel->FindParentObjectByName(L"Player");
	Vec3 vPlayerPos;

	if (nullptr != vPlayer)
		vPlayerPos = vPlayer->Transform()->GetRelativePos();

	// �÷��̾ ����ٴϴ� ī�޶�
	if (m_bFollowPlayer)
		vCameraPos = vPlayerPos;

	Vec2 Resolution = GlobalData.Resolution;

	// ī�޶� �ʹ����� �������� ����
	if (vCameraPos.x - (Resolution.x / 2.f) <= 0.f)
	{
		vCameraPos.x = (Resolution.x / 2.f);
	}
	else if (vCameraPos.x + (Resolution.x / 2.f) >= m_vMapsize.x)
	{
		vCameraPos.x = m_vMapsize.x - (Resolution.x / 2.f);
	}
 
	if (vCameraPos.y + (Resolution.y / 2.f) >= 0.f)
	{
		vCameraPos.y = -(Resolution.y / 2.f);
	}
	else if (vCameraPos.y - (Resolution.y / 2.f) <= -m_vMapsize.y)
	{
		vCameraPos.y = -m_vMapsize.y + (Resolution.y / 2.f);
	}


	CameraShakeEffect();
	Transform()->SetRelativePos(vCameraPos);
}

void CMainCameraScript::SetMapsize(Vec2 _Size)
{
	m_vMapsize = _Size;
}

void CMainCameraScript::SetFollowPlayer(bool _bool)
{
	m_bFollowPlayer = _bool;
}

void CMainCameraScript::CameraShakeEffect()
{
	if (m_bCameraShakeOn)
	{
		m_fCameraShakeTime = 0.f;
		m_bCameraShakeOn = true;
	}

	if (m_fCameraShakeTime < 0.15f)
	{
		// �õ尪�� ��� ���� random_device ����.
		std::random_device rd;
		// random_device �� ���� ���� ���� ������ �ʱ�ȭ �Ѵ�.
		std::mt19937 gen(rd());
		// 0 ���� 99 ���� �յ��ϰ� ��Ÿ���� �������� �����ϱ� ���� �յ� ���� ����.
		std::uniform_int_distribution<int> IntRange(-30, 30);
		int Value = IntRange(gen);

		m_vCameraShakeValue = Vector2{ (float)Value,(float)Value };
		m_fCameraShakeTime += DT;
		Vec3 m_Pos3 = Transform()->GetRelativePos();
		Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
		SetPos(m_Pos + m_vCameraShakeValue);

	}

	// ȭ�� ��鸲 ����
	else if (m_fCameraShakeTime >= 0.15f)
	{
		m_bCameraShakeOn = false;
	}


}

void CMainCameraScript::SetCameraShakeOn(bool _bool)
{
	m_bCameraShakeOn = _bool;
}

void CMainCameraScript::SetPos(Vec2 _vec2)
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Transform()->SetRelativePos(_vec2.x, _vec2.y, m_Pos3.z);
}
