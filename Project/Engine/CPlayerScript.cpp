#include "pch.h"
#include "CPlayerScript.h"

#include "CMeshRender.h"
#include "CMaterial.h"

#include "CMissileScript.h"


CPlayerScript::CPlayerScript()
	: m_fSpeed(100.f)
{
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::tick()
{
	Vec3 vCurPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::UP))
	{
		for (int i = 0; i < 4; ++i)
		{
			vCurPos.y += DT * m_fSpeed;
		}
	}

	if (KEY_PRESSED(KEY::DOWN))
	{
		for (int i = 0; i < 4; ++i)
		{
			vCurPos.y -= DT * m_fSpeed;
		}
	}

	if (KEY_PRESSED(KEY::LEFT))
	{
		for (int i = 0; i < 4; ++i)
		{
			vCurPos.x -= DT * m_fSpeed;
		}
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		for (int i = 0; i < 4; ++i)
		{
			vCurPos.x += DT * m_fSpeed;
		}
	}

	if (KEY_PRESSED(KEY::Z))
	{
		Vec3 vRot = Transform()->GetRelativeRot();
		vRot.z += DT * XM_PI;
		Transform()->SetRelativeRot(vRot);
	}

	Transform()->SetRelativePos(vCurPos);

	if (KEY_TAP(KEY::SPACE))
	{
		Shoot();
	}
}

void CPlayerScript::Shoot()
{
	// 미사일 오브젝트 생성
	CGameObject* pMissile = new CGameObject;

	pMissile->AddComponent(new CTransform);
	pMissile->AddComponent(new CMeshRender);
	pMissile->AddComponent(new CMissileScript);

	pMissile->Transform()->SetRelativePos(Transform()->GetRelativePos() + Vec3(0.f, 0.5f, 0.f) * Transform()->GetRelativeScale());
	pMissile->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 50.f));

	pMissile->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMissile->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));

	CMissileScript* pMissileScript = pMissile->GetScript<CMissileScript>();
	if (nullptr != pMissileScript)
		pMissileScript->SetSpeed(500.f);

	//// 미사일 프리팹 참조
	//Ptr<CPrefab> pMissilePrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"MissilePrefab");
	//CGameObject* pCloneMissile = pMissilePrefab->Instantiate();
	
	// 레벨에 추가
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	pCurLevel->AddGameObject(pMissile, L"PlayerProjectile", false);
}
