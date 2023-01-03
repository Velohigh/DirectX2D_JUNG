#pragma once
#include "CComponent.h"
// ������� ��� ������ �����ϸ� �����ؾ� ������,
// ��ũ��Ʈ���� ���� ����ϰ� �� ��ɵ��� �̸� include �س��ڽ��ϴ�.
#include "CLevelMgr.h"
#include "CLevel.h"
#include "CLayer.h"

#include "CResMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"

#include "CGameObject.h"
#include "components.h"


class CCollider2D;

class CScript :
    public CComponent
{
private:



public:
    void Destroy() { DestroyObject(GetOwner()); }
    void SetLifeSpan(float _Time) { GetOwner()->SetLifeSpan(_Time); }


public:
    virtual void finaltick() final {};

    virtual void BeginOverlap(CCollider2D* _Other) {}
    virtual void OnOverlap(CCollider2D* _Other) {}
    virtual void EndOverlap(CCollider2D* _Other) {}

public:
    CScript();
    ~CScript();
};

