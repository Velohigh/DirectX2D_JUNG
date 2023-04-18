#pragma once
#include <Engine\CScript.h>
class CUIScript :
    public CScript
{
private:
    CGameObject*    m_Battery;
    CGameObject*    m_Timer;


public:
    virtual void begin() override;
    virtual void tick() override;

private:
    CLONE(CUIScript);
public:
    CUIScript();
    ~CUIScript();
};

