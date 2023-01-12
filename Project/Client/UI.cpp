#include "pch.h"
#include "UI.h"


UI::UI(const string& _Name)
	: m_strName(_Name)
	, m_ParentUI(nullptr)
{

}

UI::~UI()
{
	Safe_Del_Vec(m_vecChildUI);
}

void UI::finaltick()
{
	if (nullptr == m_ParentUI)
	{
		ImGui::Begin(m_strName.c_str());

		render_update();

		for (size_t i = 0; i < m_vecChildUI.size(); ++i)
		{
			m_vecChildUI[i]->finaltick();

			if (i != m_vecChildUI.size() - 1)
				ImGui::Separator();
		}

		ImGui::End();
	}
	else
	{
		ImGui::BeginChild(m_strName.c_str(), ImVec2(0.f, 200.f));

		render_update();

		for (size_t i = 0; i < m_vecChildUI.size(); ++i)
		{
			m_vecChildUI[i]->finaltick();

			if (i != m_vecChildUI.size() - 1)
				ImGui::Separator();
		}

		ImGui::EndChild();
	}
}