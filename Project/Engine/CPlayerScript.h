#pragma once
#include "CScript.h"

class CPlayerScript :
    public CScript
{
private:
    float       m_fSpeed;

public:
    virtual void tick() override;


private:
    void Shoot();

    CLONE(CPlayerScript);
public:
    CPlayerScript();
    ~CPlayerScript();
};

