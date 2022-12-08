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

#include "ptr.h"
#include "CResMgr.h"
#include "CMesh.h"
#include "CGraphicsShader.h"



CGameObject* g_Obj = nullptr;

CGraphicsShader*	g_pShader = nullptr;


void Init()
{


	// ������Ʈ ����
	g_Obj = new CGameObject;
	g_Obj->AddComponent(new CTransform);	// ��ġ��� ������Ʈ
	g_Obj->AddComponent(new CMeshRender);	// �׸��� ��� ������Ʈ
	g_Obj->AddComponent(new CPlayerScript);	// �÷��̾� ��ũ��Ʈ ������Ʈ

	// ���ҽ� �Ŵ������� �޽��� ã�ƿͼ� �־��ش�.
	Ptr<CMesh> RectMesh = CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh");

	g_Obj->MeshRender()->SetMesh(RectMesh.Get());
	g_Obj->MeshRender()->SetShader(g_pShader);



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
	delete g_pShader;
}
