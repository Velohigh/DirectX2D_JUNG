#pragma once
#include "CComponent.h"

#include "ptr.h"
#include "CTexture.h"

class CAnim2D;

class CAnimator2D :
    public CComponent
{
private:
    map<wstring, CAnim2D*>  m_mapAnim;  // Animation 목록
    CAnim2D* m_pCurAnim; // 현재 재생중인 Animation
    bool                    m_bRepeat;  // 반복


public:
    virtual void finaltick() override;


public:
    void CreateAnimation(const wstring& _strAnimName, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, int _FrameCount, int _FPS);

    CLONE(CAnimator2D);
public:
    CAnimator2D();
    ~CAnimator2D();
};

