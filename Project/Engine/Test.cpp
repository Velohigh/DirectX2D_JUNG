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
#include "CResMgr.h"

CGameObject* g_Obj = nullptr;

void Init()
{




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
}
