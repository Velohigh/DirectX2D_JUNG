#include "pch.h"
#include "CPlayerScript.h"

#include "CMeshRender.h"
#include "CMaterial.h"

CPlayerScript::CPlayerScript()
	: m_fSpeed(100.f)
{
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::tick()
{
	Vec3 vCurPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::UP))
	{
		for (int i = 0; i < 4; ++i)
		{
			vCurPos.y += (float)DT * m_fSpeed;
		}
	}

	if (KEY_PRESSED(KEY::DOWN))
	{
		for (int i = 0; i < 4; ++i)
		{
			vCurPos.y -= (float)DT * m_fSpeed;

		}
	}

	if (KEY_PRESSED(KEY::LEFT))
	{
		for (int i = 0; i < 4; ++i)
		{
			vCurPos.x -= (float)DT * m_fSpeed;
		}
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		for (int i = 0; i < 4; ++i)
		{
			vCurPos.x += (float)DT * m_fSpeed;
		}
	}
	
	// z축 회전 시켜보기
	if (KEY_PRESSED(KEY::Z))
	{
		Vec3 vRot = Transform()->GetRelativeRot();
		vRot.z += DT * XM_PI;
		Transform()->SetRelativeRot(vRot);

		vCurPos.z += DT * m_fSpeed;
	}

	Transform()->SetRelativePos(vCurPos);

}

