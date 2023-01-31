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

    if (nullptr != CResMgr::GetInst()->FindRes<CTexture>(L"Link"))
    {
        m_pTexture = CResMgr::GetInst()->FindRes<CTexture>(L"Link");

        ImGui::Text("pointer = %p", m_pTexture.Get());
        ImGui::Text("size = %d x %d", (int)m_pTexture->Width(), (int)m_pTexture->Height());
        ImGui::Image((void*)m_pTexture->GetSRV().Get(), ImVec2(m_pTexture->Width(), m_pTexture->Height()));
    }

	string szName = m_vecStrData[m_iSelectedIdx].c_str();
	wstring szWName(szName.begin(), szName.end());

	SelectAnim = GetTarget()->Animator2D()->FindAnim(szWName);


    if (m_bcheck)
    {
        ImGui::Begin("##AnimationEditTool1", 0,ImGuiWindowFlags_::ImGuiWindowFlags_HorizontalScrollbar);

        // 최근 UI 의 작업영역 사이즈를 알아낸다.
        ImVec2 ListUIContentSize = ImGui::GetContentRegionAvail();

        m_pTexture = CResMgr::GetInst()->FindRes<CTexture>(L"Link");

        // 이미지 마우스 주변 확대
        ImGuiIO& io = ImGui::GetIO();
        ImTextureID my_tex_id = (void*)(m_pTexture->GetSRV().Get());
        float my_tex_w = (float)m_pTexture->Width();
        float my_tex_h = (float)m_pTexture->Height();
        {
            ImGui::Text("Size : %.0fx%.0f", my_tex_w, my_tex_h);
            ImVec2 pos = ImGui::GetCursorScreenPos();
            ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
            ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
            ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
            ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
            ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                float region_sz = 64.0f;
                float region_x = io.MousePos.x - pos.x - region_sz * 0.5f;
                float region_y = io.MousePos.y - pos.y - region_sz * 0.5f;
                float zoom = 4.0f;
                if (region_x < 0.0f) { region_x = 0.0f; }
                else if (region_x > my_tex_w - region_sz) { region_x = my_tex_w - region_sz; }
                if (region_y < 0.0f) { region_y = 0.0f; }
                else if (region_y > my_tex_h - region_sz) { region_y = my_tex_h - region_sz; }
                ImGui::Text("Min:(%.0f, %.0f)", region_x, region_y);
                ImGui::SameLine();
                ImGui::Text("  Mouse:(%.0f, %.0f)", io.MousePos.x - pos.x, io.MousePos.y - pos.y);
                ImGui::Text("Max:(%.0f, %.0f)", region_x + region_sz, region_y + region_sz);
                ImVec2 uv0 = ImVec2((region_x) / my_tex_w, (region_y) / my_tex_h);
                ImVec2 uv1 = ImVec2((region_x + region_sz) / my_tex_w, (region_y + region_sz) / my_tex_h);
                ImGui::Image(my_tex_id, ImVec2(region_sz * zoom, region_sz * zoom), uv0, uv1, tint_col, border_col);
                ImGui::EndTooltip();

                // 우클릭으로 범위 지정
                if (ImGui::IsMouseDown(ImGuiMouseButton_Right))
                {
                    DragLT = Vec2(io.MousePos.x - pos.x, io.MousePos.y - pos.y);
                }

                else if (ImGui::IsMouseReleased(ImGuiMouseButton_Right))
                {
                    DragRB = Vec2(io.MousePos.x - pos.x, io.MousePos.y - pos.y);

                }
            }
        }

        ImGui::End();


        ImGui::Begin("##AnimationEditTool2");

        SetName("AnimatorEditor");

        ImGui::PushID(0);
        ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.6f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.f / 7.0f, 0.6f, 0.6f));
        ImGui::Button(GetName().c_str());
        ImGui::PopStyleColor(3);
        ImGui::PopID();

        ImGui::Checkbox("AutoMode", &m_AutoMode);

        Vec2 TexResolution = { m_pTexture->Width(), m_pTexture->Height() };

        if (m_AutoMode)
        {
            ImGui::Image((void*)m_pTexture->GetSRV().Get(), ImVec2(300.f, 300.f),
                ImVec2(vLT.x / TexResolution.x, vLT.y / TexResolution.y),
                ImVec2((vLT.x + vSlice.x) / TexResolution.x, (vLT.y + vSlice.y) / TexResolution.y),
                ImVec4(1,1,1,1), ImVec4(1.0f, 1.0f, 1.0f, 0.5f));

            float fvLT[2] = { vLT.x, vLT.y };
            float fvSlice[2] = { vSlice.x, vSlice.y };
            float fvBackSize[2] = { vBackSize.x, vBackSize.y };
            float fvOffSet[2] = { vOffSet.x, vOffSet.y };

            ImGui::Text("\n");

            ImGui::Text("AnimName  ");
            ImGui::SameLine();
            ImGui::InputText("##AnimName", AnimName, 50, ImGuiInputTextFlags_AllowTabInput);


            ImGui::Text("LeftTop   ");
            ImGui::SameLine();
            ImGui::DragFloat2("##LeftTop", fvLT);

            ImGui::Text("Slice     ");
            ImGui::SameLine();
            ImGui::DragFloat2("##Slice", fvSlice);

            ImGui::Text("BackSize  ");
            ImGui::SameLine();
            ImGui::DragFloat2("##BackSize", fvBackSize);

            ImGui::Text("FrameCount");
            ImGui::SameLine();
            ImGui::DragInt("##FrameCount", &FrameCount);

            ImGui::Text("FPS       ");
            ImGui::SameLine();
            ImGui::DragInt("##FPS", &FPS);

            ImGui::Text("Offset    ");
            ImGui::SameLine();
            ImGui::DragFloat2("##Offset", fvOffSet);

            vLT = Vec2{ fvLT[0], fvLT[1] };
            vSlice = Vec2{ fvSlice[0], fvSlice[1] };
            vBackSize = Vec2{ fvBackSize[0], fvBackSize[1] };
            vOffSet = Vec2{ fvOffSet[0], fvOffSet[1] };

            if (ImGui::Button("CreateAnimation"))
            {
                GetTarget()->Animator2D()->CreateAnimation(wstring(AnimName, &AnimName[50]), m_pTexture,
                    vLT, vSlice, vBackSize, FrameCount, FPS);
            }

        }




        ImGui::End();
    }

    return TRUE;
}