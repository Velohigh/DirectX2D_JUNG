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
    void render();

public:
    void AddGameObject(CGameObject* _Object, int _iLayerIndex); // �ش� ���̾ ������Ʈ �߰�.

    CLONE(CLevel); 
public:
    CLevel();
    ~CLevel();

};

