#pragma once
#include <Engine\CScript.h>
class CTimerScript :
    public CScript
{
private:
    CLevel*         m_Level;
    CGameObject*    m_Owner;
    Vec3            m_Size;


public:
    virtual void begin() override;
    virtual void tick() override;

public:
    void SetOwner(CGameObject* _Object) { m_Owner = _Object; }


private:
    CLONE(CTimerScript);
public:
    CTimerScript();
    ~CTimerScript();
};

