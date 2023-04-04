#include "pch.h"
#include "Collider2DUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CCollider2D.h>


Collider2DUI::Collider2DUI()
    : ComponentUI("##Coillider2D", COMPONENT_TYPE::COLLIDER2D)
{
    SetName("Coillider2D");
}

Collider2DUI::~Collider2DUI()
{
}

int Collider2DUI::render_update()
{
    if (FALSE == ComponentUI::render_update())
        return FALSE;

	Vec3 vPos = GetTarget()->Collider2D()->GetOffsetPos();
	Vec3 vScale = GetTarget()->Collider2D()->GetOffsetScale();
	bAbsolute = GetTarget()->Collider2D()->GetAbsolute();

	ImGui::Text("OffsetPos  ");
	ImGui::SameLine();
	ImGui::DragFloat3("##OffsetPos", vPos);

	ImGui::Text("OffsetScale");
	ImGui::SameLine();
	ImGui::DragFloat3("##OffsetScale", vScale);

	ImGui::Text("Absolute");
	ImGui::SameLine();
	ImGui::Checkbox("##bAbsolute", &bAbsolute);

	GetTarget()->Collider2D()->SetOffsetPos(Vec2(vPos.x, vPos.y));
	GetTarget()->Collider2D()->SetOffsetScale(Vec2(vScale.x, vScale.y));

	if (nullptr != GetTarget())
	{
		if (bAbsolute)
		{
			GetTarget()->Collider2D()->SetAbsolute(true);
		}
		else if (!bAbsolute)
		{
			GetTarget()->Collider2D()->SetAbsolute(false);

		}
	}



    return TRUE;
}