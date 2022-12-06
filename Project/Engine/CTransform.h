#pragma once
#include "CComponent.h"
class CTransform :
    public CComponent
{
private:
    Vec3    m_vRelativePos;
    Vec3    m_vRelativeScale;
    Vec3    m_vRelativeRot;

public:
    virtual void finaltick() override;

    CLONE(CTransform);
public:
    CTransform();
    ~CTransform();
};

