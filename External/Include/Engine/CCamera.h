#pragma once
#include "CComponent.h"

class CCamera :
    public CComponent

{
private:
    float       m_fAspectRatio;   // 종횡비
    float       m_fScale;       // Orthograpic 에서 사용하는 카메라 배율

    PROJ_TYPE   m_ProjType;     // 투영 방식


    Matrix      m_matView;      // 뷰 행렬
    Matrix      m_matProj;      // 투영 행렬


    UINT        m_iLayerMask;

    int         m_iCamIdx;  // 카메라 우선순위


    vector<CGameObject*>    m_vecOpaque;
    vector<CGameObject*>    m_vecMask;
    vector<CGameObject*>    m_vecTransparent;
    vector<CGameObject*>    m_vecPost;
    vector<CGameObject*>    m_vecUI;


public:
    void SetProjType(PROJ_TYPE _Type) { m_ProjType = _Type; }
    PROJ_TYPE GetProjType() { return m_ProjType; }

    void SetScale(float _fScale) { m_fScale = _fScale; }
    float GetScale() { return m_fScale; }

    void SetLayerMask(int _iLayer, bool _Visible);
    void SetLayerMaskAll(bool _Visible);

    void SetCameraIndex(int _idx);

    const Matrix& GetViewMat() { return m_matView; }
    const Matrix& GetProjMat() { return m_matProj; }

public:
    void SortObject();      // 오브젝트의 사용 셰이더에 따른 도메인 분류
    void render();

public:
    virtual void finaltick() override;


private:
    void clear();
    void render_opaque();
    void render_mask();
    void render_transparent();
    void render_postprocess();
    void render_ui();


    void CalcViewMat();
    void CalcProjMat();

    CLONE(CCamera);
public:
    CCamera();
    CCamera(const CCamera& _Other);
    ~CCamera();

};

