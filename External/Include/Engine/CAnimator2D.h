#pragma once
#include "CComponent.h"

#include "ptr.h"
#include "CTexture.h"
#include "CAnim2D.h"

class CAnimator2D :
    public CComponent
{
private:
    map<wstring, CAnim2D*>  m_mapAnim;  // Animation 목록
    CAnim2D*                m_pCurAnim; // 현재 재생중인 Animation
    bool                    m_bRepeat;  // 반복


public:
    virtual void finaltick() override;
    void UpdateData();
    void Clear();

public:
    void Play(const wstring& _strName, bool _bRepeat);
    CAnim2D* FindAnim(const wstring& _strName);

    void CreateAnimation(const wstring& _strAnimName, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _vBackSize, int _FrameCount, int _FPS);
    void CreateFolderAnimation(const wstring& _RelativePath, int _FrameCount, float _FPS);
    void CreateFolderAnim_Rewinder(vector<Ptr<CTexture>>& _vecTex, vector<Ptr<CTexture>>& _vecTexReverse);

    map<wstring, CAnim2D*>& GetAnimMap() { return m_mapAnim; }
    CAnim2D* GetCurAnim() { return m_pCurAnim; }


    bool IsEndAnimation()
    {
        return m_pCurAnim->IsFinish();
    }

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

public:
    void Create_Player_Animation();
    void Create_Grunt_Animation();
    void Create_Pomp_Animation();
    void Create_Gangster_Animation();
    void Create_Effect_Animation();

    CLONE(CAnimator2D);
public:
    CAnimator2D();
    ~CAnimator2D();
};

