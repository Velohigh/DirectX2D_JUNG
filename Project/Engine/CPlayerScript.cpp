#include "pch.h"
#include "CPlayerScript.h"

#include "CMeshRender.h"
#include "CMaterial.h"

CPlayerScript::CPlayerScript()
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
			vCurPos.y += (float)DT * 1.f;
		}
	}

	if (KEY_PRESSED(KEY::DOWN))
	{
		for (int i = 0; i < 4; ++i)
		{
			vCurPos.y -= (float)DT * 1.f;

		}
	}

	if (KEY_PRESSED(KEY::LEFT))
	{
		for (int i = 0; i < 4; ++i)
		{
			vCurPos.x -= (float)DT * 1.f;
		}
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		for (int i = 0; i < 4; ++i)
		{
			vCurPos.x += (float)DT * 1.f;
		}
	}
	
	Transform()->SetRelativePos(vCurPos);

	if (KEY_TAP(KEY::_1))
	{
		int a = 0;
		MeshRender()->GetMaterial()->SetScalarParam(INT_0, &a);
	}

	else if (KEY_TAP(KEY::_2))
	{
		int a = 1;
		MeshRender()->GetMaterial()->SetScalarParam(INT_0, &a);
	}

	// z축 회전 시켜보기
	if (KEY_PRESSED(KEY::A))
	{
		Vec3 vRot = Transform()->GetRelativeRot();
		vRot.z += DT * XM_PI;
		Transform()->SetRelativeRot(vRot);
	}


}

