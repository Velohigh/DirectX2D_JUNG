#pragma once
#include "CEntity.h"

class CRes :
    public CEntity
{
private:
    const RES_TYPE  m_Type;
    UINT            m_iRefCount; // �� ���ҽ��� �󸶳� ���� ������κ��� �����ǰ� �ֳ� ����ϰ�, 0�� �Ǹ� ��������.

public:
    CRes(RES_TYPE _type);
    virtual ~CRes();
};

