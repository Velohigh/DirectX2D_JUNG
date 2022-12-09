#include "pch.h"
#include "Test.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"
#include "CPlayerScript.h"

#include "CDevice.h" // ����̽��� �������� ���� : GPU�� ����ϱ����� Device, Context�� ����ϱ� ����
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
	// �� ������Ʈ���� tick, finaltick �� ȣ��
	g_Obj->tick();

	g_Obj->finaltick();
}

void Render()
{
	// �� ������Ʈ�� ������
	g_Obj->render();
}

void Release()
{
	delete g_Obj;
}
