#pragma once
#include <Engine\CScript.h>

class CBloodSplaterScript :
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
	CLONE(CBloodSplaterScript);

public:
	CBloodSplaterScript();
	~CBloodSplaterScript();
};
