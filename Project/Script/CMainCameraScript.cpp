#include "pch.h"
#include "CMainCameraScript.h"


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
	// 카메라 이동
	Vec3 vCameraPos = Transform()->GetRelativePos();

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	CGameObject* vPlayer = pCurLevel->FindParentObjectByName(L"Player");
	Vec3 vPlayerPos;

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
