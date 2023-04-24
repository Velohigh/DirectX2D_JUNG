#include "pch.h"
#include "CMainCameraScript.h"
#include "CPlayerScript.h"
#include <random>


CMainCameraScript::CMainCameraScript()
	: CScript((UINT)SCRIPT_TYPE::MAINCAMERASCRIPT)
	, m_fCamSpeed(100.f)
	, m_bFollowPlayer(true)
	, m_fCameraShakeTime(0.f)
	, m_bCameraShakeOn(false)

{
}

CMainCameraScript::~CMainCameraScript()
{
}

void CMainCameraScript::tick()
{
	// 카메라 이동
	Vec3 vCameraPos = Transform()->GetRelativePos();

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	CGameObject* vPlayer = pCurLevel->FindParentObjectByName(L"Player");
	Vec3 vPlayerPos;

	if (nullptr == vPlayer)
		return;

	if (nullptr != vPlayer)
		vPlayerPos = vPlayer->Transform()->GetRelativePos();

	// 플레이어를 따라다니는 카메라
	if (m_bFollowPlayer)
		vCameraPos = vPlayerPos;

	Vec2 Resolution = GlobalData.Resolution;

	// 카메라 맵밖으로 못나가게 가둠
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


	if (m_bCameraShakeOn)
	{
		m_vStartPos = vCameraPos;
		m_fCameraShakeTime = 0.f;
		m_bCameraShakeOn = false;
		//m_bFollowPlayer = false;
	}

	if (m_fCameraShakeTime < 0.12f)
	{
		// 시드값을 얻기 위한 random_device 생성.
		std::random_device rd;
		// random_device 를 통해 난수 생성 엔진을 초기화 한다.
		std::mt19937 gen(rd());
		// 0 부터 99 까지 균등하게 나타나는 난수열을 생성하기 위해 균등 분포 정의.
		std::uniform_int_distribution<int> IntRange(-4, 4);
		int ValueX = IntRange(gen);
		int ValueY = IntRange(gen);

		m_vCameraShakeValue = Vector2{ (float)ValueX,(float)ValueY };
		m_fCameraShakeTime += DT;
		Vec3 m_Pos3 = Transform()->GetRelativePos();
		Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
		SetPos(m_Pos + m_vCameraShakeValue);

		Vec3 vTempCameraPos = Transform()->GetRelativePos();
		// 카메라 맵밖으로 못나가게 가둠
		if (vTempCameraPos.x - (Resolution.x / 2.f) <= 0.f)
		{
			vTempCameraPos.x = (Resolution.x / 2.f);
			SetPos(vTempCameraPos);
		}
		else if (vTempCameraPos.x + (Resolution.x / 2.f) >= m_vMapsize.x)
		{
			vTempCameraPos.x = m_vMapsize.x - (Resolution.x / 2.f);
			SetPos(vTempCameraPos);
		}

		if (vTempCameraPos.y + (Resolution.y / 2.f) >= 0.f)
		{
			vTempCameraPos.y = -(Resolution.y / 2.f);
			SetPos(vTempCameraPos);
		}
		else if (vTempCameraPos.y - (Resolution.y / 2.f) <= -m_vMapsize.y)
		{
			vTempCameraPos.y = -m_vMapsize.y + (Resolution.y / 2.f);
			SetPos(vTempCameraPos);
		}


		if (m_fCameraShakeTime < 0.085f &&
			m_fCameraShakeTime >= 0.075f)
		{
			CTimeMgr::GetInst()->SetTimeScale(0.04f);
		}
		else
		{
			if(vPlayer->GetScript<CPlayerScript>()->GetIsSlowMode() == true)
				CTimeMgr::GetInst()->SetTimeScale(0.2f);
			else
				CTimeMgr::GetInst()->SetTimeScale(1.f);
		}
	}
	// 화면 흔들림 해제
	else if (m_fCameraShakeTime >= 0.12f)
	{
		m_bCameraShakeOn = false;
		//m_bFollowPlayer = true;
		Transform()->SetRelativePos(vCameraPos);
	}



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

void CMainCameraScript::SetPos(Vec3 _vec3)
{
	Transform()->SetRelativePos(_vec3.x, _vec3.y, _vec3.z);
}
