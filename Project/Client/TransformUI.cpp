#include "pch.h"
#include "TransformUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>

TransformUI::TransformUI()
	: UI("Transform")
	, m_Target(nullptr)
{
}

TransformUI::~TransformUI()
{
}

void TransformUI::render_update()
{
	if (nullptr == m_Target)
		return;

	Vec3 vPos = m_Target->Transform()->GetRelativePos();
	Vec3 vScale = m_Target->Transform()->GetRelativeScale();
	Vec3 vRotation = m_Target->Transform()->GetRelativeRot();
	vRotation = (vRotation / XM_PI) * 180.f;

	ImGui::Text("Position");
	ImGui::SameLine();
	ImGui::InputFloat3("##Relative Position", vPos);

	ImGui::Text("Scale   ");
	ImGui::SameLine();
	ImGui::InputFloat3("##Relative Scale", vScale);

	ImGui::Text("Rotation");
	ImGui::SameLine();
	ImGui::InputFloat3("##Relative Rotation", vRotation);

	m_Target->Transform()->SetRelativePos(vPos);
	m_Target->Transform()->SetRelativeScale(vScale);

	vRotation = (vRotation / 180.f) * XM_PI;
	m_Target->Transform()->SetRelativeRot(vRotation);
}
