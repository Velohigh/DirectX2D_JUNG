#pragma once
#include <Engine\CScript.h>
class CBatteryScript :
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
    CLONE(CBatteryScript);
public:
    CBatteryScript();
    ~CBatteryScript();
};

