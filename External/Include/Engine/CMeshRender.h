#pragma once
#include "CComponent.h"

#include "ptr.h"
// 템플릿은 헤더에서 알아야 사용가능하므로 직접 추가해준다. 
#include "CMesh.h"
#include "CGraphicsShader.h"

class CMesh;
class CGraphicsShader;

class CMeshRender :
    public CComponent
{
private:
    Ptr<CMesh>              m_pMesh;    // 사용할 메쉬
    Ptr<CGraphicsShader>    m_pShader;  // 어떻게 그릴것인지.
     
public:
	void SetMesh(Ptr<CMesh> _Mesh) { m_pMesh = _Mesh; }
	void SetShader(Ptr<CGraphicsShader> _Shader) { m_pShader = _Shader; }

    Ptr<CMesh> GetMesh() { return m_pMesh; }
    Ptr<CGraphicsShader> GetShader() { return m_pShader; }

public:
    virtual void finaltick() override;
    void render();






    CLONE(CMeshRender);
public:
    CMeshRender();
    ~CMeshRender();

};

