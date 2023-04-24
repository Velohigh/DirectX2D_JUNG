#include "pch.h"
#include "CTitleScript.h"
#include "CPlayerScript.h"
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

CTitleScript::CTitleScript()
	: CScript((UINT)SCRIPT_TYPE::TITLESCRIPT)
	, m_bEnterPush(false)
	, m_bEnd(false)
{
}

CTitleScript::~CTitleScript()
{
}


void CTitleScript::begin()
{
}

void CTitleScript::tick()
{
	if (KEY_TAP(KEY::ENTER) &&
		m_bEnterPush == false)
	{
		m_bEnterPush = true;


		Ptr<CSound> pBeepSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_menubeep_2.wav");
		pBeepSound->Play(1, 1.f, true);
	}

	if (m_bEnterPush == true)
	{
		CGameObject* UICam = CLevelMgr::GetInst()->GetCurLevel()->FindParentObjectByName(L"UICamera");

		if (nullptr == UICam)
			return;

		// UI ī�޶� ���� �ö󰡴� ����
		Vec3 CamPos = UICam->Transform()->GetRelativePos();
		CamPos += Vec3(0.f, 400.f, 0.f) * DT;
		UICam->Transform()->SetRelativePos(CamPos);

		if (CamPos.y >= 738 &&
			m_bEnd == false)
		{
			m_bEnd = true;

			// Ÿ��Ʋȭ�� Bgm 2�� ��� ����.
			m_pBgm->Stop();
			Ptr<CSound> pTitleBgm = CLevelMgr::GetInst()->GetCurLevel()->GetBgm();
			pTitleBgm->Stop();

			// ��� ������Ʈ �ı���, �������� 1�� ��ȯ.
			CLevelMgr::GetInst()->GetCurLevel()->DestroyAllObject();
			UICam->Transform()->SetRelativePos(0.f, 0.f, 0.f);
			//ResetStage_1();
			ResetStage_Last();
			return;
		}
	}
}
