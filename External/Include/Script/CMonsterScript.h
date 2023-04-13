#pragma once
#include <Engine\CScript.h>

class CMonsterScript :
    public CScript
{
protected:
	ObjDir		m_PreDir;	// ������ �ٶ󺸰��ִ� ����
	ObjDir		m_CurDir = ObjDir::End;	// ���� �ٶ󺸴� ����
	ObjState	m_CurState;		// ���� ����
	ObjState	m_BeginState;	// ���� ����
	CLevel*		m_Level;	// ���� ����
	float		m_StateTime[static_cast<int>(ObjState::END)];	// �ش� ���°� �ǰ� ���� �ð�

	Vec2		m_Move;			// �̵���
	Vec3		m_PrePos;		// ���� ������ ��ġ
	Vec3		m_CurPos;		
	Vec2		m_MoveDir;		// �̵�����
	float		m_MoveSpeed;	// �̵��ӵ�
	float		m_Gravity;		//  �߷� ���
	float		m_GravityAccel;		// �߷°��ӵ�

	Vec2		m_EnemyAttackDir;

	float		m_fPatrolTime;	// �����ð�

	bool		m_bEffect_EnemyFollow;			// �÷��̾� �߽߰� ����Ʈ ����
	bool		m_bPatrol;						// ���� �ൿ ����
	bool		m_bViewColliderOn;				// �þ� ���� �浹ü �۵�
	bool		m_bAttackRangeOn;				// ���� ���� �浹ü �۵�
	bool		m_bHitBoxOn;					// ��Ʈ �ڽ� �浹ü �۵�
	bool		m_bAttackOn;						// ���� �浹ü Ȱ��ȭ

	CGameObject* m_ViewCollider;				// �þ� �浹ü
	CGameObject* m_AttackRangeCollider;			// ���ݹ��� �浹ü

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
	void SetSize2x();	// ����� ���� �̹����� 2��� ����
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
	// �̷��� ��ġ�� ����Ͽ� �װ��� RGB���� üũ�ϰ�, �̵� ������ ���̸� �̵��Ѵ�.
	void MapCollisionCheckMoveGround();
	void MapCollisionCheckMoveAir();
	void MapCollisionCheckMoveGroundDie();
	void MapCollisionCheckMoveAirDie();
};



