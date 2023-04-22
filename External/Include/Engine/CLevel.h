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
    void SethWnd(HWND _handle) { m_hWnd = _handle; }
    void SetBgmON(bool _bool) { m_bBgmON = _bool; }
    void SetBgm(CSound* _pSound) { m_pBgm = _pSound; }

    void DestroyAllObject();

private:
    // ��ϵ� GameObject ���� LevelMgr::tick() ���ۺκп���.
    void clear();



    CLONE(CLevel);
public:
    CLevel();
    ~CLevel();

    friend class CLevelMgr;
};

