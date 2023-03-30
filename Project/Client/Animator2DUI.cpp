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
    , SelectAnim(nullptr)
    , vLT{}
    , vSlice{}
    , vBackSize{}
    , FrameCount(0)
    , FPS(0)
    , vOffSet{}
    , RelativePath{}
    , AnimName{}
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

    map<wstring, CAnim2D*> AnimMap = GetTarget()->Animator2D()->GetAnimMap();
    auto iter = AnimMap.begin();

    if (AnimMap.size() != m_vecStrData.size())
    {
        m_vecStrData.clear();

        for (; iter != AnimMap.end(); ++iter)
        {
            m_vecStrData.push_back(string(iter->first.begin(), iter->first.end()));
        }
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

            // 특정 아이템(인덱스 i) 위에서 더블클릭이 발생하면
            if (ImGui::IsItemHovered() &&
                ImGui::IsMouseDoubleClicked(ImGuiMouseButton_::ImGuiMouseButton_Left))
            {
                strcpy_s(szBuff, m_vecStrData[m_iSelectedIdx].c_str());

                wstring szCurAnim(m_vecStrData[m_iSelectedIdx].begin(), m_vecStrData[m_iSelectedIdx].end());

                GetTarget()->Animator2D()->Play(szCurAnim, true);

            }

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndListBox();
    }

    if (m_bcheck)
    {
        ImGui::Begin("Animation Editor");

        ImGui::Text("RelativePath : ");
        ImGui::SameLine();
        ImGui::InputText("##RelativePath", RelativePath, 255);

        //ImGui::Text("  AnimName   : ");
        //ImGui::SameLine();
        //ImGui::InputText("##AnimName", AnimName, 255);

        ImGui::Text("  FrameCount : ");
        ImGui::SameLine();
        ImGui::InputInt("##FrameCount", &FrameCount);

        ImGui::Text("    FPS      : ");
        ImGui::SameLine();
        ImGui::InputInt("##FPS", &FPS);

        //wstring wAnimName = wstring(AnimName, AnimName + strlen(AnimName));
        wstring wRelativePath = wstring(RelativePath, RelativePath + strlen(RelativePath));

        ImGui::Text("Create Folder Anim");
        ImGui::SameLine();
        if (ImGui::Button("##CreateFolderAnim", ImVec2(18.f, 18.f)))
        {
            GetTarget()->Animator2D()->CreateFolderAnimation(wRelativePath, FrameCount, FPS);
        }
        
        ImGui::End();
    }

    return TRUE;
}