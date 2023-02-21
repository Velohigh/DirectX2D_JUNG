#include "pch.h"
#include "CLevelMgr.h"

#include "CLevel.h"
#include "CLayer.h"

#include "CResMgr.h"
#include "CCollisionMgr.h"

#include "CGameObject.h"
#include "components.h"

#include "CPlayerScript.h"
#include "CCameraMoveScript.h"
#include "CMonsterScript.h"

#include "CDevice.h"
#include "CSetColorShader.h"

#include "CStructuredBuffer.h"

CLevelMgr::CLevelMgr()
	: m_pCurLevel(nullptr)
{
}

CLevelMgr::~CLevelMgr()
{
	if (nullptr != m_pCurLevel)
		delete m_pCurLevel;
}


void CLevelMgr::init()
{
	// 구조화버퍼 테스트
	struct tTestStruct
	{
		int a;
		float f;
		long long l;
	};

	CStructuredBuffer* pBuffer = new CStructuredBuffer;
	pBuffer->Create(sizeof(tTestStruct), 1, SB_TYPE::READ_ONLY, true);

	tTestStruct testParam = {};
	testParam.a = 10;
	testParam.f = 1.4f;
	testParam.l = 1000000000000;
	pBuffer->SetData(&testParam);

	tTestStruct testParam2 = {};
	pBuffer->GetData(&testParam2);

	delete pBuffer;


	// 텍스쳐 색칠하기
	// 텍스쳐 생성(UnorderedAccess)
	Ptr<CTexture> pCreateTex = CResMgr::GetInst()->CreateTexture(
		L"SampleTexture"
		, 1280, 768
		, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM
		, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS
		, D3D11_USAGE_DEFAULT);

	// ComputeShader 로 텍스쳐 색 변경하기
	Ptr<CSetColorShader> pCS = (CSetColorShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"SetColorCS").Get();
	pCS->SetTargetTexture(pCreateTex);
	pCS->SetColor(Vec3(0.f, 1.f, 1.f));
	pCS->Execute();

	m_pCurLevel = new CLevel;
	m_pCurLevel->ChangeState(LEVEL_STATE::STOP);

	// Layer 이름설정
	m_pCurLevel->GetLayer(0)->SetName(L"Default");
	m_pCurLevel->GetLayer(1)->SetName(L"Tile");
	m_pCurLevel->GetLayer(2)->SetName(L"Player");
	m_pCurLevel->GetLayer(3)->SetName(L"Monster");
	m_pCurLevel->GetLayer(4)->SetName(L"PlayerProjectile");
	m_pCurLevel->GetLayer(5)->SetName(L"MonsterProjectile");


	// Main Camera Object 생성
	CGameObject* pMainCam = new CGameObject;
	pMainCam->SetName(L"MainCamera");

	pMainCam->AddComponent(new CTransform);
	pMainCam->AddComponent(new CCamera);

	pMainCam->Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	pMainCam->Camera()->SetCameraIndex(0);		// MainCamera 로 설정
	pMainCam->Camera()->SetLayerMaskAll(true);	// 모든 레이어 체크

	SpawnGameObject(pMainCam, Vec3(0.f, 0.f, 0.f), 0);

	// 광원 추가
	CGameObject* pLightObj = new CGameObject;
	pLightObj->SetName(L"Point Light");

	pLightObj->AddComponent(new CTransform);
	pLightObj->AddComponent(new CLight2D);

	pLightObj->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	pLightObj->Transform()->SetRelativeRot(Vec3(0.f, 0.f, XM_PI / 2.f));

	pLightObj->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	pLightObj->Light2D()->SetLightDiffuse(Vec3(1.f, 1.f, 1.f));
	pLightObj->Light2D()->SetRadius(500.f);

	SpawnGameObject(pLightObj, Vec3(0.f, 0.f, 0.f), 0);


	// 오브젝트 생성
	CGameObject* pParent = new CGameObject;
	pParent->SetName(L"Player");
	pParent->AddComponent(new CTransform);
	pParent->AddComponent(new CMeshRender);
	pParent->AddComponent(new CCollider2D);
	pParent->AddComponent(new CAnimator2D);
	pParent->AddComponent(new CPlayerScript);

	pParent->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));

	pParent->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pParent->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimLightMtrl"));

	pParent->Collider2D()->SetAbsolute(true);
	pParent->Collider2D()->SetOffsetScale(Vec2(150.f, 150.f));

	Ptr<CTexture> pAnimAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"Link");
	pParent->Animator2D()->CreateAnimation(L"WalkDown", pAnimAtlas, Vec2(0.f, 520.f), Vec2(120.f, 130.f), Vec2(300.f, 300.f), 10, 16);
	pParent->Animator2D()->Play(L"WalkDown", true);

	SpawnGameObject(pParent, Vec3(0.f, 0.f, 500.f), 0);


	CGameObject* pChild = new CGameObject;

	pChild->SetName(L"Child");
	pChild->AddComponent(new CTransform);
	pChild->AddComponent(new CMeshRender);

	pChild->Transform()->SetAbsolute(true);
	pChild->Transform()->SetRelativePos(200.f, 0.f, 0.f);
	pChild->Transform()->SetRelativeScale(100.f, 100.f, 1.f);

	pChild->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pChild->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DLightMtrl"));
	pChild->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"PlayerTex"));


	pParent->AddChild(pChild);

	// Monster
	CGameObject* pMonster = new CGameObject;
	pMonster->SetName(L"Monster");

	pMonster->AddComponent(new CTransform);
	pMonster->AddComponent(new CMeshRender);
	pMonster->AddComponent(new CCollider2D);
	pMonster->AddComponent(new CMonsterScript);

	pMonster->Transform()->SetRelativePos(Vec3(0.f, 250.f, 100.f));
	pMonster->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));

	pMonster->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMonster->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	pMonster->MeshRender()->GetMaterial()->SetTexParam(TEX_0, pCreateTex);

	pMonster->Collider2D()->SetAbsolute(true);
	pMonster->Collider2D()->SetOffsetScale(Vec2(100.f, 100.f));

	SpawnGameObject(pMonster, Vec3(0.f, 250.f, 100.f), L"Monster");



	// TileMap Object
	CGameObject* pTileMap = new CGameObject;
	pTileMap->SetName(L"TileMap");

	pTileMap->AddComponent(new CTransform);
	pTileMap->AddComponent(new CTileMap);

	pTileMap->Transform()->SetRelativePos(Vec3(0.f, 0.f, 600.f));
	pTileMap->Transform()->SetRelativeScale(Vec3(500.f, 500.f, 1.f));

	pTileMap->TileMap()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"TileAtlasTex"));
	pTileMap->TileMap()->SetSliceSize(Vec2(0.125f, 0.166f));
	pTileMap->TileMap()->SetTileCount(8, 8);

	SpawnGameObject(pTileMap, Vec3(0.f, 0.f, 600.f), L"Tile");


	// Particle Object
	CGameObject* pParticleObj = new CGameObject;
	pParticleObj->SetName(L"ParticleObject");
	pParticleObj->AddComponent(new CTransform);
	pParticleObj->AddComponent(new CParticleSystem);

	SpawnGameObject(pParticleObj, Vec3(0.f, 0.f, 0.f), 0);


	// 충돌 시킬 레이어 짝 지정
	CCollisionMgr::GetInst()->LayerCheck(L"Player", L"Monster");
}

void CLevelMgr::tick()
{
	m_pCurLevel->clear();

	if (LEVEL_STATE::PLAY == m_pCurLevel->GetState())
	{
		m_pCurLevel->tick();
	}

	m_pCurLevel->finaltick();
}


CGameObject* CLevelMgr::FindObjectByName(const wstring& _strName)
{
	return m_pCurLevel->FindObjectByName(_strName);
}

void CLevelMgr::FindObjectByName(const wstring& _strName, vector<CGameObject*>& _vec)
{
	m_pCurLevel->FindObjectByName(_strName, _vec);
}