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

