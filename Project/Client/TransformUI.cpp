#include "pch.h"
#include "TransformUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>

TransformUI::TransformUI()
	: ComponentUI("##Transform", COMPONENT_TYPE::TRANSFORM)
{
	SetName("Transform");


}

TransformUI::~TransformUI()
{
}

int TransformUI::render_update()
{
	if (FALSE == ComponentUI::render_update())
		return FALSE;

	Vec3 vPos = GetTarget()->Transform()->GetRelativePos();
	Vec3 vScale = GetTarget()->Transform()->GetRelativeScale();
	Vec3 vRotation = GetTarget()->Transform()->GetRelativeRot();
	vRotation = (vRotation / XM_PI) * 180.f;

	ImGui::Text("Position");
	ImGui::SameLine();
	ImGui::DragFloat3("##Relative Position", vPos);

	ImGui::Text("Scale   ");
	ImGui::SameLine();
	ImGui::DragFloat3("##Relative Scale", vScale);

	ImGui::Text("Rotation");
	ImGui::SameLine();
	ImGui::DragFloat3("##Relative Rotation", vRotation);

	GetTarget()->Transform()->SetRelativePos(vPos);
	GetTarget()->Transform()->SetRelativeScale(vScale);

	vRotation = (vRotation / 180.f) * XM_PI;
	GetTarget()->Transform()->SetRelativeRot(vRotation);

	if (nullptr != GetTarget())
	{
		if (GetTarget()->Transform()->GetAbsolute() != m_bAbsoluteCheck)
			m_bAbsoluteCheck = GetTarget()->Transform()->GetAbsolute();

		ImGui::Checkbox("##AnimationEditCheckBox", &m_bAbsoluteCheck);
		ImGui::SameLine();
		ImGui::Text("Absolute");
		if (m_bAbsoluteCheck)
		{
			GetTarget()->Transform()->SetAbsolute(true);
		}
		else if (!m_bAbsoluteCheck)
		{
			GetTarget()->Transform()->SetAbsolute(false);

		}
	}


	return TRUE;
}
