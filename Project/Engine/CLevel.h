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
    void begin();
    void tick();
    void finaltick();

public:
    void AddGameObject(CGameObject* _Object, int _iLayerIndex, bool _Move); // 해당 레이어에 오브젝트 추가.
    CLayer* GetLayer(int _iLayerIdx) { assert(!(_iLayerIdx < 0)); return m_arrLayer[_iLayerIdx]; }


private:
    // 등록된 GameObject 제거
    void clear();

    CLONE(CLevel); 
public:
    CLevel();
    ~CLevel();

    friend class CLevelMgr;
};

