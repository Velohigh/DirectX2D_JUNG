#pragma once
#include <Engine\CScript.h>

class CBulletScript :
	public CScript
{
private:
	CLevel*			m_Level;
	CGameObject*	m_Owner;
	Vec2			m_MoveDir;
	Vec2			m_AttackDir;
	ObjDir			m_CurDir;
	float			m_MoveSpeed;

	float			m_Angle;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	void SetOwner(CGameObject* _Object) { m_Owner = _Object; }
	void SetMoveDir(Vec2 _vec2) { m_MoveDir = _vec2; }
	void SetDir(ObjDir _Dir) { m_CurDir = _Dir; }
	void SetAngle(float _Angle) { m_Angle = _Angle; }

public:
	void MoveDir(const Vec2& Dir);
	void MoveValue(const Vector2& MoveValue);  //Move();

private:
	virtual void BeginOverlap(CCollider2D* _Other);
	virtual void OnOverlap(CCollider2D* _Other);
	virtual void EndOverlap(CCollider2D* _Other);




private:
	CLONE(CBulletScript);

public:
	CBulletScript();
	~CBulletScript();
};

