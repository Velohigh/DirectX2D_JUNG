#pragma once
#include <Engine\CScript.h>

class CAttackRangeScript :
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
	virtual void BeginOverlap(CCollider2D* _Other);
	virtual void OnOverlap(CCollider2D* _Other);
	virtual void EndOverlap(CCollider2D* _Other);


private:
	CLONE(CAttackRangeScript);

public:
	CAttackRangeScript();
	~CAttackRangeScript();
};

