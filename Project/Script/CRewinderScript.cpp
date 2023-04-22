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
}

void CRewinderScript::tick()
{
	m_Time += DT;

	// 1 / 60초를 지날때마다, 최종 렌더타겟 텍스쳐의 이미지를 복사하여 저장한다.
	if (m_Time >= 1.f / 60.f &&
		m_bSaveOn == true)
	{
		m_Time -= 1.f / 60.f;

        // 역 재생을 위한 텍스쳐 저장 @@@
		Vec2 vResolution = GlobalData.Resolution;
		Ptr<CTexture> pNewCopyTex = CResMgr::GetInst()->CreateTexture(L"NewCopyTex"
		    , (UINT)vResolution.x, (UINT)vResolution.y
		    , DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_SHADER_RESOURCE
		    , D3D11_USAGE_DEFAULT);
		
		//CResMgr::GetInst()->FindRes<CMaterial>(L"GrayMtrl")->SetTexParam(TEX_0, pNewCopyTex);


		// 렌더타겟을 들고와서 복사한다.
		Ptr<CTexture> pRTTex = CResMgr::GetInst()->FindRes<CTexture>(L"RenderTargetTex");
		CONTEXT->CopyResource(pNewCopyTex->GetTex2D().Get(), pRTTex->GetTex2D().Get());

		// vector 에 넣는다.
		m_vecRewinder.push_back(pNewCopyTex);

		// 리소스 매니저에서 삭제
		tEvent evn;
		evn.Type = EVENT_TYPE::DELETE_RESOURCE;
		evn.wParam = (DWORD_PTR)RES_TYPE::TEXTURE;
		evn.lParam = (DWORD_PTR)CResMgr::GetInst()->FindRes<CTexture>(L"NewCopyTex").Get();
		CEventMgr::GetInst()->AddEvent(evn);
	}

	if (m_bSaveOn == false &&
		Animator2D()->IsEndAnimation() == true)
	{
		//CLevelMgr::GetInst()->GetCurLevel()->DestroyAllObject();
		Destroy();
		CLevelMgr::GetInst()->GetCurLevel()->FindParentObjectByName(L"PostProcess")->SetLifeSpan(0.f);
		CLevelMgr::GetInst()->GetCurLevel()->FindParentObjectByName(L"Distortion")->SetLifeSpan(0.f);

		wstring StageName = CLevelMgr::GetInst()->GetCurLevel()->GetName();
		
		if(StageName == L"Stage_1")
			ResetStage_1();
		else if (StageName == L"Stage_2")
			ResetStage_2();
		else if (StageName == L"Stage_3")
			ResetStage_3();

	}

}

void CRewinderScript::CreateFolderAnim_Rewinder()
{
	Animator2D()->CreateFolderAnim_Rewinder(m_vecRewinder, m_vecRewinderReverse);
}

