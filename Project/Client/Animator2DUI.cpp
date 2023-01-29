#include "pch.h"
#include "Animator2DUI.h"

#include <Engine\CMeshRender.h>
#include <Engine\CAnimator2D.h>
#include <Engine\CAnim2D.h>
#include <Engine\CResMgr.h>

#include "ImGuiMgr.h"
#include "ListUI.h"
//#include "../Engine/CAnimator2D.h"

Animator2DUI::Animator2DUI()
    : ComponentUI("##Animator2D", COMPONENT_TYPE::ANIMATOR2D)
    , m_iSelectedIdx(0)
    , m_bcheck(false)
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

    ImGui::Checkbox("##AnimationEditCheckBox", &m_bcheck);
    ImGui::SameLine();
    ImGui::Text("Animation Edit");

    //map<wstring, CAnim2D*> AnimMap = GetTarget()->Animator2D()->GetAnimMap();
    map<wstring, CAnim2D*> AnimMap = GetTarget()->Animator2D()->GetAnimMap();
    auto iter = AnimMap.begin();

    if (0 == m_vecStrData.size())
        for (; iter != AnimMap.end(); ++iter)
        {
            m_vecStrData.push_back(string(iter->first.begin(), iter->first.end()));
        }

    ImGui::Text("SelectAnimName");
    ImGui::SameLine();
    ImGui::InputText("##SelectAnimName", szBuff, 50, ImGuiInputTextFlags_ReadOnly);

    if (ImGui::BeginListBox("##list", ImVec2{ 300.f, 300.f }))
    {
        for (int i = 0; i < m_vecStrData.size(); i++)
        {
            const bool is_selected = (m_iSelectedIdx == i);

            // 리스트 아이템 출력, 반환값은 클릭 True/ False
            if (ImGui::Selectable(m_vecStrData[i].c_str(), is_selected))
            {
                m_iSelectedIdx = i;
            }

            // 특정 아이템(인덱스 i) 위에서 클릭이 발생하면
            if (ImGui::IsItemHovered() &&
                ImGui::IsMouseClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
            {
                strcpy_s(szBuff, m_vecStrData[m_iSelectedIdx].c_str());
            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }

    if (nullptr != CResMgr::GetInst()->FindRes<CTexture>(L"Link"))
    {
        m_pTexture = CResMgr::GetInst()->FindRes<CTexture>(L"Link");

        ImGui::Text("pointer = %p", m_pTexture.Get());
        ImGui::Text("size = %d x %d", (int)m_pTexture->Width(), (int)m_pTexture->Height());
        ImGui::Image((void*)m_pTexture->GetSRV().Get(), ImVec2(m_pTexture->Width(), m_pTexture->Height()));
    }

    if (m_bcheck)
    {
        ImGui::Begin("##AnimationEditTool1");

        m_pTexture = CResMgr::GetInst()->FindRes<CTexture>(L"Link");

        ImGui::Text("Size : %d, %d", (int)m_pTexture->Width(), (int)m_pTexture->Height());
        ImGui::Image((void*)m_pTexture->GetSRV().Get(), ImVec2(m_pTexture->Width(), m_pTexture->Height()));


        ImGui::End();


        ImGui::Begin("##AnimationEditTool2");

        string szName = m_vecStrData[m_iSelectedIdx].c_str();
        wstring szWName(szName.begin(), szName.end());

        CAnim2D* CurAnim = GetTarget()->Animator2D()->FindAnim(szWName);

        Vec2 vLT = CurAnim->GetCurFrame().LeftTopUV;
        Vec2 vSlice = CurAnim->GetCurFrame().SliceUV;
        Vec2 vOffSet = CurAnim->GetCurFrame().Offset;
        float fvLT[] = { vLT.x , vLT.y };
        float fvSlice[] = { vSlice.x , vSlice.y };
        float fvOffSet[] = { vOffSet.x , vOffSet.y};
        ImGui::Text("LeftTop");
        ImGui::SameLine();
        ImGui::DragFloat2("##LeftTop", fvLT);

        ImGui::Text("Slice  ");
        ImGui::SameLine();
        ImGui::DragFloat2("##Slice", fvSlice);

        ImGui::Text("Offset ");
        ImGui::SameLine();
        ImGui::DragFloat2("##Offset", fvOffSet);

        ImGui::End();
    }

    return TRUE;
}