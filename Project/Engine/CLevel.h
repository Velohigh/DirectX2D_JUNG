#pragma once
#include "CEntity.h"

class CLayer;
class CGameObject;
class CSound;

class CLevel :
    public CEntity
{
private:
    CLayer*         m_arrLayer[MAX_LAYER];
    LEVEL_STATE     m_State;
    HWND            m_hWnd;

    CSound*         m_pBgm;
    bool            m_bBgmON;
    int             m_KillCount;

public:
    void begin();
    void tick();
    void finaltick(); 

public:
    CLayer* FindLayerByName(const wstring& _strName);
    CLayer* GetLayer(int _iLayerIdx) { assert(!(_iLayerIdx < 0)); return m_arrLayer[_iLayerIdx]; }

    CGameObject* FindObjectByName(const wstring& _Name);
    CGameObject* FindParentObjectByName(const wstring& _Name);
    void FindObjectByName(const wstring& _Name, vector<CGameObject*>& _Out);

    void AddGameObject(CGameObject* _Object, int _iLayerIdx, bool _Move);
    void AddGameObject(CGameObject* _Object, const wstring& _LayerName, bool _Move);
    
    void ChangeState(LEVEL_STATE _State);
    LEVEL_STATE GetState() { return m_State; }


    CSound* GetBgm() { return m_pBgm; }
    int GetKillCount() { return m_KillCount; }
    void SethWnd(HWND _handle) { m_hWnd = _handle; }
    void SetBgmON(bool _bool) { m_bBgmON = _bool; }
    void SetBgm(CSound* _pSound) { m_pBgm = _pSound; }
    void SetKillCount(int _KillCount) { m_KillCount = _KillCount; }

    void DestroyAllObject();    // 레벨안의 오브젝트들 Destroy(), 카메라, 광원, Rewinder 제외, Rewinder는 알아서 재생후 파괴됌

private:
    // 등록된 GameObject 제거 LevelMgr::tick() 시작부분에서.
    void clear();



    CLONE(CLevel);
public:
    CLevel();
    ~CLevel();

    friend class CLevelMgr;
};

