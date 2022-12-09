#pragma once
#include "CEntity.h"

class CLayer;
class CGameObject;

// 오브젝트들을 들고있는 레이어들을 관리하는 클래스, 크게보면 오브젝트들을 관리한다.
class CLevel :
    public CEntity
{
private:
    CLayer*     m_arrLayer[MAX_LAYER];

public:
    void tick();
    void finaltick();
    void render();

public:
    void AddGameObject(CGameObject* _Object, int _iLayerIndex); // 해당 레이어에 오브젝트 추가.

    CLONE(CLevel); 
public:
    CLevel();
    ~CLevel();

};

