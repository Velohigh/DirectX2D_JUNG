#pragma once
#include "CEntity.h"

class CLayer;
class CGameObject;

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
    CLayer* FindLayerByName(const wstring& _strName);

    void AddGameObject(CGameObject* _Object, int _iLayerIdx, bool _Move);
    void AddGameObject(CGameObject* _Object, const wstring& _LayerName, bool _Move);


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

