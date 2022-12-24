#pragma once
#include "CComponent.h"

// 템플릿은 헤더에서 알아야 사용가능하므로 직접 추가해준다. 
#include "CMesh.h"
#include "CMaterial.h"
#include "ptr.h"

// 렌더 기능을 가진 컴포넌트들의 공통 부모 클래스
class CRenderComponent :
    public CComponent
{
private:
    Ptr<CMesh>              m_pMesh;        // 사용할 메쉬
    Ptr<CMaterial>          m_pMtrl;        // 어떻게 그릴것인지.

public:
    virtual void render() = 0;


public:
    void SetMesh(Ptr<CMesh> _Mesh) { m_pMesh = _Mesh; }
    void SetMaterial(Ptr<CMaterial> _Mtrl) { m_pMtrl = _Mtrl; }

    Ptr<CMesh> GetMesh() { return m_pMesh; }
    Ptr<CMaterial> GetMaterial() { return m_pMtrl; }


public:
    CRenderComponent(COMPONENT_TYPE _type);
    ~CRenderComponent();
};

