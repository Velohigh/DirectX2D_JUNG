#pragma once
#include "CEntity.h"

#include "ptr.h"
#include "CTexture.h"

class CAnimator2D;

class CAnim2D :
    public CEntity
{
private:
    CAnimator2D* m_pOwner;
    vector<tAnim2DFrm>  m_vecFrm;
    Ptr<CTexture>       m_AtlasTex;

    int                 m_iCurFrm;

public:
    void finaltick();
    void Create(const wstring& _strAnimName, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, int _FrameCount, int _FPS);


    CLONE(CAnim2D);
public:
    CAnim2D();
    ~CAnim2D();

    friend class CAnimator2D;
};

