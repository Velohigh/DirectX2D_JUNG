#include "pch.h"
#include "CDustCloudScript.h"
#include <random>

CDustCloudScript::CDustCloudScript()
	: CScript((UINT)SCRIPT_TYPE::DUSTCLOUDSCRIPT)
{
}

CDustCloudScript::~CDustCloudScript()
{
}

void CDustCloudScript::begin()
{
	// �ܺ� ����̽� ���������� �������� �޾ƿ´�.
	std::random_device rd;
	std::mt19937_64 mt_(rd());	// ���������� �õ带 �����Ѵ�.
	std::uniform_int_distribution<int> IntRange(80, 320);	// ���� ����
	m_MoveSpeed = static_cast<float>(IntRange(mt_));
	std::uniform_real_distribution<float> FloatRange(0, 0.7f);
	m_MoveDir = Vector2{ 0.f,-FloatRange(mt_) };
	RandomValueX = Vector2{ FloatRange(mt_) - 0.35f , 0.f };

}

void CDustCloudScript::tick()
{
	if (m_CurDir == ObjDir::Right)
	{
		MoveDir((Vector2{ 1.f, 0.f } - m_MoveDir));
	}
	else if (m_CurDir == ObjDir::Left)
	{
		MoveDir((Vector2{ -1.f, 0.f } - m_MoveDir));
	}
	else
	{
		MoveDir((Vector2{ RandomValueX.x, 0.f } - m_MoveDir));
	}

	if (Animator2D()->IsEndAnimation() == true)
	{
		Destroy();
	}
}

void CDustCloudScript::MoveDir(const Vec2& Dir)
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	m_Pos += Dir * m_MoveSpeed * DT;

	Transform()->SetRelativePos(m_Pos.x, m_Pos.y, m_Pos3.z);
}

void CDustCloudScript::MoveValue(const Vector2& MoveValue)
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);

	m_Pos += MoveValue /*m_TimeScale*/;

	if (MoveValue.Length() <= 0.001f)
		int ab = 0;

	Transform()->SetRelativePos(m_Pos.x, m_Pos.y, m_Pos3.z);

}

