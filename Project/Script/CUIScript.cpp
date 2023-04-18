#include "pch.h"
#include "CUIScript.h"


CUIScript::CUIScript()
	: CScript((UINT)SCRIPT_TYPE::UISCRIPT)
{
}

CUIScript::~CUIScript()
{
}


void CUIScript::begin()
{
	//// UI_Battery
	//CGameObject* pBattery = new CGameObject;
	//pBattery->SetName(L"UI_Battery");

	//pBattery->AddComponent(new CTransform);
	//pBattery->AddComponent(new CMeshRender);
	//pBattery->AddComponent(new CUIScript);

	//pBattery->Transform()->SetRelativePos(Vec3(GlobalData.Resolution.x / 2.f, GlobalData.Resolution.y / 2.f, 100.f));
	//pBattery->Transform()->SetRelativeScale(Vec3(1280.f, 46.f, 1.f));

	//pBattery->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	//pBattery->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BatteryMtrl"));
	//pBattery->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\effect_battery.png"));

	//SpawnGameObject(pBattery, Vec3(0, 360.f - 42.f, 100.f), L"ViewPort UI");



}

void CUIScript::tick()
{
}
