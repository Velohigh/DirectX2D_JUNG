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
    void tick();
    void finaltick();

public:
    void AddGameObject(CGameObject* _Object, int _iLayerIndex); // �ش� ���̾ ������Ʈ �߰�.
    CLayer* GetLayer(int _iLayerIdx) { assert(!(_iLayerIdx < 0)); return m_arrLayer[_iLayerIdx]; }

    CLONE(CLevel); 
public:
    CLevel();
    ~CLevel();

};

