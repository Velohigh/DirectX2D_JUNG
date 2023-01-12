#pragma once
#include <Engine\CSingleton.h>

#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_dx11.h"
#include "ImGui\imgui_impl_win32.h"

class InspectorUI;

class ImGuiMgr :
    public CSingleton<ImGuiMgr>
{
    SINGLE(ImGuiMgr);
private:
    HWND            m_hMainHwnd;

    InspectorUI* m_Inspector;



public:
    void init(HWND _hWnd);
    void progress();

private:
    void CreateUI();

    void begin();
    void tick();
    void finaltick();
    void render();
};

