#pragma once
#include <Engine\CScript.h>

class CBikeSlashScript :
	public CScript
{
private:
	CLevel*				m_Level;
	CGameObject*		m_Owner;
	Vec2				m_vAttackDir;
	ObjDir				m_CurDir;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	void SetOwner(CGameObject* _Object) { m_Owner = _Object; }
	void SetAttackDir(Vec2 _vec2) { m_vAttackDir = _vec2; }
	void SetDir(ObjDir _Dir) { m_CurDir = _Dir; }

private:
	virtual void BeginOverlap(CCollider2D* _Other) override;


private:
	CLONE(CBikeSlashScript);

public:
	CBikeSlashScript();
	~CBikeSlashScript();
};

