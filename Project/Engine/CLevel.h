#pragma once
#include "CEntity.h"

class CLayer;
class CGameObject;

// ������Ʈ���� ����ִ� ���̾���� �����ϴ� Ŭ����, ũ�Ժ��� ������Ʈ���� �����Ѵ�.
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
    void AddGameObject(CGameObject* _Object, int _iLayerIndex, bool _Move); // �ش� ���̾ ������Ʈ �߰�.
    CLayer* GetLayer(int _iLayerIdx) { assert(!(_iLayerIdx < 0)); return m_arrLayer[_iLayerIdx]; }


private:
    // ��ϵ� GameObject ����
    void clear();

    CLONE(CLevel); 
public:
    CLevel();
    ~CLevel();

    friend class CLevelMgr;
};

