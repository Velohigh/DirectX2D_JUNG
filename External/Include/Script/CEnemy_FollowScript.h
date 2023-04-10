#pragma once
#include <Engine\CScript.h>

class CEnemy_FollowScript :
	public CScript
{
private:
	CLevel*			m_Level;
	CGameObject*	m_Owner;


public:
	virtual void begin() override;
	virtual void tick() override;

public:
	void SetOwner(CGameObject* _Object) { m_Owner = _Object; }

private:
	CLONE(CEnemy_FollowScript);

public:
	CEnemy_FollowScript();
	~CEnemy_FollowScript();
};
