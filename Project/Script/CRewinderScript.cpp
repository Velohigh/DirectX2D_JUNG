#include "pch.h"
#include "CRewinderScript.h"
#include <Engine\CDevice.h>
#include <Engine\CEventMgr.h>
#include "CPlayerScript.h"
#include <Engine\CLevelMgr.h>
#include <Engine\CLevel.h>

CRewinderScript::CRewinderScript()
	: CScript((UINT)SCRIPT_TYPE::REWINDERSCRIPT)

{
}

CRewinderScript::~CRewinderScript()
{
}


void CRewinderScript::begin()
{
	m_bSaveOn = true;
	m_bReverseOn = false;
	m_bReplayOn = false;
}

void CRewinderScript::tick()
{
	m_Time += DT;

	// 1 / 60�ʸ� ����������, ���� ����Ÿ�� �ؽ����� �̹����� �����Ͽ� �����Ѵ�.
	if (m_Time >= 1.f / 60.f &&
		m_bSaveOn == true &&
		m_bReverseOn == false &&
		m_bReplayOn == false)
	{
		m_Time -= 1.f / 60.f;

        // �� ����� ���� �ؽ��� ���� @@@
		Vec2 vResolution = GlobalData.Resolution;
		Ptr<CTexture> pNewCopyTex = CResMgr::GetInst()->CreateTexture(L"NewCopyTex"
		    , (UINT)vResolution.x, (UINT)vResolution.y
		    , DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE
		    , D3D11_USAGE_DEFAULT);
		
		//CResMgr::GetInst()->FindRes<CMaterial>(L"GrayMtrl")->SetTexParam(TEX_0, pNewCopyTex);


		// ����Ÿ���� ���ͼ� �����Ѵ�.
		Ptr<CTexture> pRTTex = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex");
		CONTEXT->CopyResource(pNewCopyTex->GetTex2D().Get(), pRTTex->GetTex2D().Get());

		// vector �� �ִ´�.
		m_vecRewinder.push_back(pNewCopyTex);

		// ���ҽ� �Ŵ������� ����
		tEvent evn;
		evn.Type = EVENT_TYPE::DELETE_RESOURCE;
		evn.wParam = (DWORD_PTR)RES_TYPE::TEXTURE;
		evn.lParam = (DWORD_PTR)CResMgr::GetInst()->FindRes<CTexture>(L"NewCopyTex").Get();
		CEventMgr::GetInst()->AddEvent(evn);
	}

	// ������� ������, �������� ó�� ���·�.
	if (m_bSaveOn == false &&
		m_bReverseOn == true &&
		Animator2D()->IsEndAnimation() == true)
	{
		//CLevelMgr::GetInst()->GetCurLevel()->DestroyAllObject();
		Destroy();
		CLevelMgr::GetInst()->GetCurLevel()->FindParentObjectByName(L"PostProcess")->SetLifeSpan(0.f);
		CLevelMgr::GetInst()->GetCurLevel()->FindParentObjectByName(L"Distortion")->SetLifeSpan(0.f);

		wstring StageName = CLevelMgr::GetInst()->GetCurLevel()->GetName();
		
		CTimeMgr::GetInst()->SetTimeScale(1.f);

		// ���ҽ� �Ŵ������� ����
		tEvent evn;
		evn.Type = EVENT_TYPE::DELETE_RESOURCE;
		evn.wParam = (DWORD_PTR)RES_TYPE::TEXTURE;
		evn.lParam = (DWORD_PTR)CResMgr::GetInst()->FindRes<CTexture>(L"NewCopyTex").Get();
		if (evn.lParam != 0)
			CEventMgr::GetInst()->AddEvent(evn);

		if (StageName == L"Stage_1")
		{
			ResetStage_1();
			return;
		}
		else if (StageName == L"Stage_2")
		{
			ResetStage_2();
			return;
		}
		else if (StageName == L"Stage_3")
		{
			ResetStage_3();
			return;
		}



	}

	// Ŭ������, ���÷��� ����� ������, ���� ����������.
	else if (m_bSaveOn == false &&
		m_bReplayOn == true &&
		Animator2D()->IsEndAnimation() == true)
	{
		Destroy();
		CLevelMgr::GetInst()->GetCurLevel()->FindParentObjectByName(L"PostProcess")->SetLifeSpan(0.f);
		CLevelMgr::GetInst()->GetCurLevel()->FindParentObjectByName(L"Distortion")->SetLifeSpan(0.f);
		wstring StageName = CLevelMgr::GetInst()->GetCurLevel()->GetName();

		CTimeMgr::GetInst()->SetTimeScale(1.f);

		// ���ҽ� �Ŵ������� ����
		tEvent evn;
		evn.Type = EVENT_TYPE::DELETE_RESOURCE;
		evn.wParam = (DWORD_PTR)RES_TYPE::TEXTURE;
		evn.lParam = (DWORD_PTR)CResMgr::GetInst()->FindRes<CTexture>(L"NewCopyTex").Get();
		if(evn.lParam != 0)
			CEventMgr::GetInst()->AddEvent(evn);

		if (StageName == L"Stage_1")
		{
			ResetStage_2();
			return;
		}
		else if (StageName == L"Stage_2")
		{
			ResetStage_3();
			return;
		}
		else if (StageName == L"Stage_3")
		{
			ResetStage_3();
			return;
		}


	}

	if (KEY_TAP(KEY::_1))
	{
		CTimeMgr::GetInst()->SetTimeScale(1.f);
	}

	else if (KEY_TAP(KEY::_2))
	{
		CTimeMgr::GetInst()->SetTimeScale(2.f);
	}

	else if (KEY_TAP(KEY::_3))
	{
		CTimeMgr::GetInst()->SetTimeScale(3.f);
	}

	else if (KEY_TAP(KEY::_4))
	{
		CTimeMgr::GetInst()->SetTimeScale(8.f);
	}

	else if (KEY_TAP(KEY::_0))
	{
		CTimeMgr::GetInst()->SetTimeScale(0.f);
	}

}

void CRewinderScript::CreateFolderAnim_Rewinder()
{
	Animator2D()->CreateFolderAnim_Rewinder(m_vecRewinder, m_vecRewinderReverse);
}

