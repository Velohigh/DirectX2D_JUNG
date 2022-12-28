#pragma once
#include "CEntity.h"


class CComponent;
class CTransform;
class CMeshRender;
class CCamera;
class CRenderComponent;
class CScript;

class CGameObject :
    public CEntity
{
private:
    CComponent*             m_arrCom[(UINT)COMPONENT_TYPE::END];
    CRenderComponent*       m_RenderCom;
    vector<CScript*>        m_vecScript;

    CGameObject* m_Parent;
    vector<CGameObject*>    m_vecChild;

    int                     m_iLayerIdx;


public:
    void begin();       // 레벨이 시작될 때 호출 or 시작 된 레벨에 합류할 때
    void tick();
    void finaltick();   // 틱이 다 끝나고 계산같은 걸 마무리하는 단계

    void render();

public:
    void AddComponent(CComponent* _Component);
    void AddChild(CGameObject* _Object);

    CGameObject* GetParent() const { return m_Parent; }

    CTransform* Transform() const { return (CTransform*)m_arrCom[(UINT)COMPONENT_TYPE::TRANSFORM]; }
    CMeshRender* MeshRender() const { return (CMeshRender*)m_arrCom[(UINT)COMPONENT_TYPE::MESHRENDER]; }
    CCamera* Camera() const { return (CCamera*)m_arrCom[(UINT)COMPONENT_TYPE::CAMERA]; }
    CRenderComponent* GetRenderComponent() const { return m_RenderCom; }

    CLONE(CGameObject);
public:
    CGameObject();
    ~CGameObject();

    friend class CLayer;
};

