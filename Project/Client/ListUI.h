#pragma once
#include "UI.h"

class ListUI :
    public UI
{
private:
    vector<string>          m_vecStrData;
    int                     m_iSelectedIdx;

public:
    virtual int render_update() override;

public:
    void AddItem(const string& _strItem) { m_vecStrData.push_back(_strItem); }
    void Clear() { m_vecStrData.clear(); m_iSelectedIdx = -1; }

public:
    ListUI();
    ~ListUI();
};

