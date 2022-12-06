#pragma once
#include "CEntity.h"

class CGameObject;

class CComponent :
    public CEntity
{
private:
    CGameObject*            m_pOwner;
    const COMPONENT_TYPE    m_Type;

public:
    COMPONENT_TYPE GetType() { return m_Type; }

public:
    virtual void finaltick() = 0;
    virtual CComponent* Clone() = 0;

public:
    CComponent(COMPONENT_TYPE _Type);
    virtual ~CComponent();

    friend class CGameObject;
};

