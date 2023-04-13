#pragma once
#include <Engine\CScript.h>

class CMonsterScript :
    public CScript
{
protected:
	ObjDir		m_PreDir;	// 이전에 바라보고있던 방향
	ObjDir		m_CurDir = ObjDir::End;	// 현재 바라보는 방향
	ObjState	m_CurState;		// 현재 상태
	ObjState	m_BeginState;	// 시작 상태
	CLevel*		m_Level;	// 속한 레벨
	float		m_StateTime[static_cast<int>(ObjState::END)];	// 해당 상태가 되고 지난 시간

	Vec2		m_Move;			// 이동량
	Vec3		m_PrePos;		// 이전 프레임 위치
	Vec3		m_CurPos;		
	Vec2		m_MoveDir;		// 이동방향
	float		m_MoveSpeed;	// 이동속도
	float		m_Gravity;		//  중력 계수
	float		m_GravityAccel;		// 중력가속도

	Vec2		m_EnemyAttackDir;

	float		m_fPatrolTime;	// 순찰시간

	bool		m_bEffect_EnemyFollow;			// 플레이어 발견시 이펙트 생성
	bool		m_bPatrol;						// 정찰 행동 여부
	bool		m_bViewColliderOn;				// 시야 범위 충돌체 작동
	bool		m_bAttackRangeOn;				// 공격 범위 충돌체 작동
	bool		m_bHitBoxOn;					// 히트 박스 충돌체 작동
	bool		m_bAttackOn;						// 공격 충돌체 활성화

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
	void SetState(ObjState _State) { m_CurState = _State; }
	void SetBeginState(ObjState _State) { m_BeginState = _State; }
	ObjState GetState() { return m_CurState; }
	ObjDir GetDir() { return m_CurDir; }
	void SetSpeed(float _Speed) { m_MoveSpeed = _Speed; }
	void SetPatrol(bool _bool, float _Time = 4.f) { m_bPatrol = _bool; m_fPatrolTime = _Time; }
	void SetViewColliderOn(bool _bool) { m_bViewColliderOn = _bool; }
	void SetAttackRangeOn(bool _bool) { m_bAttackRangeOn = _bool; }
	void SetHitBoxOn(bool _bool) { m_bHitBoxOn = _bool; }
	void SetAttackOn(bool _bool) { m_bAttackOn = _bool; }

protected:
	void SetSize2x();	// 사이즈를 원본 이미지의 2배로 지정
	void MoveDir(const Vec2& Dir);
	void MoveValue(const Vector2& MoveValue);  //Move();
	virtual void BeginOverlap(CCollider2D* _Other);
	virtual void OnOverlap(CCollider2D* _Other);
	virtual void EndOverlap(CCollider2D* _Other);

    CLONE(CMonsterScript);
public:
    CMonsterScript();
    ~CMonsterScript();

protected:
	///////////////////////////////
	////	FSM
	///////////////////////////////
	virtual void IdleStart() {};
	virtual void WalkStart() {};
	virtual void TurnStart() {};
	virtual void RunStart() {};
	virtual void AttackStart() {};
	virtual void HurtGroundStart() {};
	virtual void HurtFlyStart() {};
	virtual void KnockDownStart() {};
	virtual void DeadStart() {};


	virtual void IdleUpdate() {};
	virtual void WalkUpdate() {};
	virtual void TurnUpdate() {};
	virtual void RunUpdate() {};
	virtual void AttackUpdate() {};
	virtual void HurtGroundUpdate() {};
	virtual void HurtFlyUpdate() {};
	virtual void KnockDownUpdate() {};
	virtual void DeadUpdate() {};

protected:
	// 미래의 위치를 계산하여 그곳의 RGB값을 체크하고, 이동 가능한 곳이면 이동한다.
	void MapCollisionCheckMoveGround();
	void MapCollisionCheckMoveAir();
	void MapCollisionCheckMoveGroundDie();
	void MapCollisionCheckMoveAirDie();
};



