#pragma once
#include "CComponent.h"

// ������� ��� ������ �����ϸ� �����ؾ� ������,
// ��ũ��Ʈ���� ���� ����ϰ� �� ��ɵ��� �̸� include �س��ڽ��ϴ�.

//#include "CLevelMgr"
//#include "CRes.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"

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

