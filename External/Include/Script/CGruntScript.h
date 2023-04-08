#pragma once
#include <Engine\CScript.h>


class CGruntScript :
	public CScript
{
private:
	ObjDir		m_PreDir;	// 이전에 바라보고있던 방향
	ObjDir		m_CurDir = ObjDir::Right;	// 현재 바라보는 방향
	ObjState	m_CurState;	// 현재 상태
	CLevel*		m_Level;	// 속한 레벨
	float		m_StateTime[static_cast<int>(ObjState::END)];	// 해당 상태가 되고 지난 시간

	Vec2		m_MoveDir;		// 이동방향
	float		m_MoveSpeed;	// 이동속도
	float		m_Gravity = 1000.f;		//  중력 계수
	float		m_GravityAccel = 2000.f;		// 중력가속도

	Vec2		m_EnemyAttackDir;

	float		m_fPatrolTime;	// 순찰시간
	
	bool		m_bEffect_EnemyFollow = false;	// 플레이어 발견시 이펙트 생성
	bool		m_bPatrol;						// 정찰 행동 여부

	CGameObject* m_ViewCollider;				// 시야 충돌체
	CGameObject* m_AttackRangeCollider;			// 공격범위 충돌체

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	void DirAnimationCheck();
	void StateChange(ObjState _State);
	void StateUpdate();

public:
	void SetPos(Vec2 _vec2);
	void SetDir(ObjDir _Dir) { m_CurDir = _Dir; }
	ObjState GetState() { return m_CurState; }
	ObjDir GetDir() { return m_CurDir; }
	void SetSpeed(float _Speed) { m_MoveSpeed = _Speed; }

private:
	CLONE(CGruntScript);
public:
	CGruntScript();
	~CGruntScript();

private:
	void SetSize2x();	// 사이즈를 원본 이미지의 2배로 지정
	void MoveDir(const Vec2& Dir);
	void MoveValue(const Vector2& MoveValue);  //Move();
	virtual void BeginOverlap(CCollider2D* _Other);
	virtual void OnOverlap(CCollider2D* _Other);
	virtual void EndOverlap(CCollider2D* _Other);

private:
	///////////////////////////////
	////	FSM
	///////////////////////////////
	void IdleStart();
	void WalkStart();
	void TurnStart();
	void RunStart();
	void AttackStart();
	void HurtGroundStart();
	void HurtFlyStart();
	void DeadStart();


	void IdleUpdate();
	void WalkUpdate();
	void TurnUpdate();
	void RunUpdate();
	void AttackUpdate();
	void HurtGroundUpdate();
	void HurtFlyUpdate();
	void DeadUpdate();

private:
	// 미래의 위치를 계산하여 그곳의 RGB값을 체크하고, 이동 가능한 곳이면 이동한다.
	void MapCollisionCheckMoveGround();
	void MapCollisionCheckMoveAir();
	void MapCollisionCheckMoveGroundDie();
	void MapCollisionCheckMoveAirDie();

};

