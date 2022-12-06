#pragma once
#include "CEntity.h"

class CRes :
    public CEntity
{
private:
    const RES_TYPE  m_Type;
    UINT            m_iRefCount; // 이 리소스가 얼마나 많은 대상으로부터 참조되고 있나 계산하고, 0이 되면 지워진다.

public:
    CRes(RES_TYPE _type);
    virtual ~CRes();
};

