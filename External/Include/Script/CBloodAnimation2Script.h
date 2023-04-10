#pragma once
#include <Engine\CScript.h>

class CBloodAnimation2Script :
	public CScript
{
private:
	CLevel*			m_Level;
	CGameObject*	m_Owner;
	ObjDir			m_CurDir;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	void SetOwner(CGameObject* _Object) { m_Owner = _Object; }
	void SetDir(ObjDir _Dir) { m_CurDir = _Dir; }

private:
	CLONE(CBloodAnimation2Script);

public:
	CBloodAnimation2Script();
	~CBloodAnimation2Script();
};

