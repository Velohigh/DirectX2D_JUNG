#pragma once
#include <Engine\CScript.h>

class CBloodAnimation2Script :
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
	CLONE(CBloodAnimation2Script);

public:
	CBloodAnimation2Script();
	~CBloodAnimation2Script();
};

