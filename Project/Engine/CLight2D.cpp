#include "pch.h"
#include "CLight2D.h"

#include "CRenderMgr.h"
#include "CTransform.h"

CLight2D::CLight2D()
	: CComponent(COMPONENT_TYPE::LIGHT2D)
{
}

CLight2D::~CLight2D()
{
}


void CLight2D::finaltick()
{
	m_LightInfo.vWorldPos = Transform()->GetWorldPos();

	CRenderMgr::GetInst()->RegisterLight2D(m_LightInfo);
}