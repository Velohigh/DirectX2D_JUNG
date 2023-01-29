#include "pch.h"
#include "Animator2DUI.h"

#include <Engine\CMeshRender.h>
#include <Engine\CResMgr.h>

#include "ImGuiMgr.h"
#include "ListUI.h"

Animator2DUI::Animator2DUI()
    : ComponentUI("##Animator2D", COMPONENT_TYPE::ANIMATOR2D)
{
    SetName("Animator2D");
}

Animator2DUI::~Animator2DUI()
{
}


int Animator2DUI::render_update()
{
    if (FALSE == ComponentUI::render_update())
        return FALSE;

	if (ImGui::Button("##AnimationSelectBtn", ImVec2(18, 18)))
	{
	}
    ImGui::SameLine();
    ImGui::Text("Animation Edit");

    //map<wstring, CAnim2D*> AnimMap = GetTarget()->Animator2D()->GetAnimMap();
    char szBuff[50] = {};
    ImGui::Text("CurAnimationName");
    ImGui::SameLine();
    //AnimMap.find(L"WalkDown");
    //ImGui::InputText("##CurAnimationName", szBuff, 50, ImGuiInputTextFlags_ReadOnly);

    if (nullptr != CResMgr::GetInst()->FindRes<CTexture>(L"Link"))
    {
        m_pTexture = CResMgr::GetInst()->FindRes<CTexture>(L"Link");

        ImGui::Text("pointer = %p", m_pTexture.Get());
        ImGui::Text("size = %d x %d", (int)m_pTexture->Width(), (int)m_pTexture->Height());
        ImGui::Image((void*)m_pTexture->GetSRV().Get(), ImVec2(m_pTexture->Width(), m_pTexture->Height()));
    }

    return TRUE;
}