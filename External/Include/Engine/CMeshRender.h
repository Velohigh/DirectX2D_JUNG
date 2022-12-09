#pragma once
#include "CComponent.h"

#include "ptr.h"
// ���ø��� ������� �˾ƾ� ��밡���ϹǷ� ���� �߰����ش�. 
#include "CMesh.h"
#include "CGraphicsShader.h"

class CMesh;
class CGraphicsShader;

class CMeshRender :
    public CComponent
{
private:
    Ptr<CMesh>              m_pMesh;    // ����� �޽�
    Ptr<CGraphicsShader>    m_pShader;  // ��� �׸�������.
     
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

