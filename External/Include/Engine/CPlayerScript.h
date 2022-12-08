#pragma once
#include "CScript.h"

class CPlayerScript :
    public CScript
{
private:

public:
    virtual void tick() override;

    

    CLONE(CPlayerScript);
public:
    CPlayerScript();
    ~CPlayerScript();
};

