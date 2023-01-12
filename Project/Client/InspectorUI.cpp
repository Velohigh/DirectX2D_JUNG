#include "pch.h"
#include "InspectorUI.h"

#include <Engine\CGameObject.h>
#include <Engine\CTransform.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CKeyMgr.h>

#include "TransformUI.h"
#include "MeshRenderUI.h"

InspectorUI::InspectorUI()
	: UI("Inspector")
	, m_pTarget(nullptr)
	, m_TransformUI(nullptr)
{
	m_TransformUI = new TransformUI;
	//m_arrComUI[(UINT)COMPONENT_TYPE::TRANSFORM] = new TransformUI;
	AddChildUI(m_TransformUI);

	m_MeshRenderUI = new MeshRenderUI;
	AddChildUI(m_MeshRenderUI);
}

InspectorUI::~InspectorUI()
{

}

void InspectorUI::tick()
{
	if (nullptr == m_pTarget)
	{
		m_pTarget = CLevelMgr::GetInst()->FindObjectByName(L"Player");

		m_TransformUI->SetTarget(m_pTarget);
		m_MeshRenderUI->SetTarget(m_pTarget);
	}
}

void InspectorUI::render_update()
{

}