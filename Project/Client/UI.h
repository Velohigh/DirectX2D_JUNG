#pragma once


#include "ImGuiMgr.h"

class UI
{
private:
	const string	m_strName;		// 타이틀 이름, ID

	UI* m_ParentUI;		// 부모 UI
	vector<UI*>		m_vecChildUI;	// 자식 UI 목록

public:
	virtual void tick() {}
	virtual void finaltick();
	virtual void render_update() = 0;


public:
	const string& GetName() { return m_strName; }
	void AddChildUI(UI* _UI)
	{
		_UI->m_ParentUI = this;
		m_vecChildUI.push_back(_UI);
	}


public:
	UI(const string& _Name);
	~UI();
};

