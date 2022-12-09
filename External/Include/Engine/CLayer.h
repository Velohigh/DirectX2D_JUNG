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
    void render();

public:
    void AddGameObject(CGameObject* _Object) { m_vecObject.push_back(_Object); }

    CLONE(CLayer)
public:
    CLayer();
    ~CLayer();
};

