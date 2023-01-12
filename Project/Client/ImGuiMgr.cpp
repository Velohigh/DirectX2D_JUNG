#include "pch.h"
#include "ImGuiMgr.h"

#include <Engine\CDevice.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CKeyMgr.h>

#include <Engine\CGameObject.h>

#include "InspectorUI.h"


ImGuiMgr::ImGuiMgr()
    : m_hMainHwnd(nullptr)
    , m_Inspector(nullptr)
{

}

ImGuiMgr::~ImGuiMgr()
{
    // ImGui Release
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    delete m_Inspector;
}


void ImGuiMgr::init(HWND _hWnd)
{
    m_hMainHwnd = _hWnd;

    // ImGui ÃÊ±âÈ­
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;
    //io.ConfigViewportsNoDefaultParent = true;
    //io.ConfigDockingAlwaysTabBar = true;
    //io.ConfigDockingTransparentPayload = true;
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: Experimental. THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
    //io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI: Experimental.

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(m_hMainHwnd);
    ImGui_ImplDX11_Init(DEVICE, CONTEXT);

    CreateUI();
}

void ImGuiMgr::progress()
{
    begin();

    tick();

    finaltick();

    render();
}


void ImGuiMgr::begin()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void ImGuiMgr::tick()
{
    m_Inspector->tick();
}

void ImGuiMgr::finaltick()
{
    // Demo UI
    ImGui::ShowDemoWindow();

    // InspectorUI
    m_Inspector->finaltick();


    if (KEY_TAP(KEY::ENTER))
        ImGui::SetWindowFocus(nullptr);
}

void ImGuiMgr::render()
{
    // ImGui Rendering
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());


    // Update and Render additional Platform Windows
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}



void ImGuiMgr::CreateUI()
{
    m_Inspector = new InspectorUI;

}