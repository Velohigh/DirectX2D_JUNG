#pragma once
#include <Engine\CScript.h>

class CSlashScript :
	public CScript
{
private:
	CLevel*				m_Level;
	CGameObject*		m_Owner;
	Vec2				m_vAttackDir;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	void SetOwner(CGameObject* _Object) { m_Owner = _Object; }
	void SetAttackDir(Vec2 _vec2) { m_vAttackDir = _vec2; }

private:
	CLONE(CSlashScript);

public:
	CSlashScript();
	~CSlashScript();
};
