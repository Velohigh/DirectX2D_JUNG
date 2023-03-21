#pragma once
#include "CEntity.h"

#include "ptr.h"
#include "CTexture.h"

class CAnimator2D;

class CAnim2D :
    public CEntity
{
private:
    CAnimator2D*        m_pOwner;
    vector<tAnim2DFrm>  m_vecFrm;
    Vec2                m_vBackSize;
    Ptr<CTexture>       m_AtlasTex;

    vector<Ptr<CTexture>>   m_vecFolderTex;
    float                   m_fDuration;

    int                 m_iCurFrm;
    float               m_fTime;

    bool                m_bFinish;
    bool                    m_bFolderTex;

public:
    void finaltick();
    void Create(const wstring& _strAnimName, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _BackSize, int _FrameCount, int _FPS);
    void CreateFolderAnim(const wstring& _strAnimName, const wstring& _RelativePath, int _FrameCount, int _FPS);


    const tAnim2DFrm& GetCurFrame() { return m_vecFrm[m_iCurFrm];}
    int GetCurFrmCount() { return m_iCurFrm; }
    Vec2 GetBackSize() { return m_vBackSize; }
    Ptr<CTexture> GetAtlasTex() { return m_AtlasTex; }
    vector<Ptr<CTexture>>&   GetFolderTex() { return m_vecFolderTex; }
    bool IsFolderTex() { return m_bFolderTex; }

    bool IsFinish() { return m_bFinish; }
    void Reset()
    {
        m_iCurFrm = 0;
        m_fTime = 0.f;
        m_bFinish = false;
    }

    void SaveToLevelFile(FILE* _File);
    void LoadFromLevelFile(FILE* _File);


    CLONE(CAnim2D);
public:
    CAnim2D();
    ~CAnim2D();

    friend class CAnimator2D;
};

