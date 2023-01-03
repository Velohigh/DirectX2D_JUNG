#pragma once
#include "CComponent.h"
// 헤더에서 헤더 참조는 가능하면 지양해야 하지만,
// 스크립트들이 자주 사용하게 될 기능들은 미리 include 해놓겠습니다.
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

