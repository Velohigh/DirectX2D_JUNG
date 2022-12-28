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


class CScript :
    public CComponent
{
private:


public:
    virtual void finaltick() final {};

public:
    CScript();
    ~CScript();
};

