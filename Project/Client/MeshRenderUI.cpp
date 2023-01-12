#include "pch.h"
#include "MeshRenderUI.h"

#include <Engine\CMeshRender.h>

MeshRenderUI::MeshRenderUI()
	: UI("MeshRender")
	, m_Target(nullptr)
{
}

MeshRenderUI::~MeshRenderUI()
{
}


void MeshRenderUI::render_update()
{
	if (nullptr == m_Target || !m_Target->MeshRender())
		return;

	ImGui::Text("MeshRenderUI");
}