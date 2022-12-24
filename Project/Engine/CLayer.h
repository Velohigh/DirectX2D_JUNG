#pragma once
#include "CEntity.h"

class CGameObject;

class CLayer :
    public CEntity
{
private:
    vector<CGameObject*>    m_vecObject;

public:
    void tick();
    void finaltick();

public:
    void AddGameObject(CGameObject* _Object) { m_vecObject.push_back(_Object); }
    const vector<CGameObject*>& GetObjects() { return m_vecObject; }

    CLONE(CLayer)
public:
    CLayer();
    ~CLayer();
};

