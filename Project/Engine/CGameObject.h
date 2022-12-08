#pragma once
#include "CEntity.h"

class CComponent;
class CTransform;
class CMeshRender;

class CGameObject :
    public CEntity
{
private:
    CComponent*         m_arrCom[(UINT)COMPONENT_TYPE::END];

public:
    void tick();
    void finaltick();   // 틱이 다 끝나고 계산같은 걸 마무리하는 단계

    void render();

public:
    void AddComponent(CComponent* _Component);

    CTransform* Transform() { return (CTransform*)m_arrCom[(UINT)COMPONENT_TYPE::TRANSFORM]; }
    CMeshRender* MeshRender() { return (CMeshRender*)m_arrCom[(UINT)COMPONENT_TYPE::MESHRENDER]; }

    CLONE(CGameObject);
public:
    CGameObject();
    ~CGameObject();

};

