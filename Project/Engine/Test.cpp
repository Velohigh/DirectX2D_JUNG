#include "pch.h"
#include "Test.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CPlayerScript.h"

#include "CDevice.h" // 디바이스를 가져오는 이유 : GPU에 명령하기위한 Device, Context를 사용하기 위해
#include "CPathMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"

#include "ptr.h"
#include "CResMgr.h"
#include "CMesh.h"
#include "CGraphicsShader.h"



CGameObject* g_Obj = nullptr;

CGraphicsShader*	g_pShader = nullptr;


void Init()
{


	// 오브젝트 생성
	g_Obj = new CGameObject;
	g_Obj->AddComponent(new CTransform);	// 위치담당 컴포넌트
	g_Obj->AddComponent(new CMeshRender);	// 그리기 담당 컴포넌트
	g_Obj->AddComponent(new CPlayerScript);	// 플레이어 스크립트 컴포넌트

	// 리소스 매니저에서 메쉬를 찾아와서 넣어준다.
	Ptr<CMesh> RectMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");

	g_Obj->MeshRender()->SetMesh(RectMesh.Get());
	g_Obj->MeshRender()->SetShader(g_pShader);



}

void Tick()
{
	// 각 오브젝트들의 tick, finaltick 을 호출
	g_Obj->tick();

	g_Obj->finaltick();
}

void Render()
{
	// 각 오브젝트들 렌더링
	g_Obj->render();
}

void Release()
{
	delete g_Obj;
	delete g_pShader;
}
