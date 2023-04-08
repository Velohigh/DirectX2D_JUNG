#pragma once
#include <Engine\CScript.h>

class CBloodAnimationScript :
	public CScript
{
private:
	ObjDir			m_PreDir;
	ObjDir			m_CurDir;
	CLevel*			m_Level;
	CGameObject*	m_Owner;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	void SetOwner(CGameObject* _Object) { m_Owner = _Object; }
	void SetDir(ObjDir _Dir) { m_CurDir = _Dir; }

private:
	CLONE(CBloodAnimationScript);

public:
	CBloodAnimationScript();
	~CBloodAnimationScript();
};

