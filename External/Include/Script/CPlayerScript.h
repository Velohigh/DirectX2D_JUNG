#pragma once
#include <Engine\CScript.h>

enum class PlayerDir
{
	Left,
	Right,
	END
};

enum class PlayerState
{
	Idle,
	IdleToRun,
	Run,
	RunToIdle,
	Jump,
	Landing,
	Attack,
	Fall,
	Dodge,
	PlaySong,
	HurtFlyLoop,
	HurtGround,
	WallGrab,
	Flip,
	Dead,
	END
};

class CPlayerScript :
	public CScript
{
private:
	CLevel* m_Level;
	PlayerDir	m_CurDir = PlayerDir::Right;
	PlayerState	m_CurState = PlayerState::END;
	float		m_StateTime[(UINT)(PlayerState::END)] = {};

	float		m_fBattery;		// ���͸� ��
	float		m_BatteryRechargeTime;	// ���͸� ������ �ð�
	float		m_BatteryPushTime;		// ���� �ð�

	float		m_Timer;		// ���ѽð�


	float		m_Gravity;			// �÷��̾� �߷� ���
	float		m_GravityAccel;		// �߷°��ӵ�

	float		m_JumpPower;
	float		m_LongJumpPower;
	bool		m_IsLongJump;

	int			m_AttackCount;

	Vec2		m_Move;			// �̵���
	Vec3		m_PrePos;		// ���� ������ ��ġ
	Vec3		m_CurPos;

	Vec2		m_MoveDir;		// �̵� ���� ����
	float       m_MoveSpeed;	// �̵� �ӵ�
	Vec2		m_AttackDir;	// �� ���� ����
	Vec2		m_EnemyAttackDir;	// ���� ���� ����

	bool		m_bRun1SoundOn;	// Run ���� �������
	bool		m_bRun2SoundOn;	// Run ���� �������
	bool		m_bHitOn;				// ���� �޾Ҵ��� ����

	bool		m_IsSlowMode;	// ���ο� ������� ����
	bool		m_bReplayStage;	// Ŭ������ ���÷��� �������

public:
	virtual void begin() override;
	virtual void tick() override;


public:
	PlayerState GetState() { return m_CurState; }
	Vec2& GetAttackDir() { return m_AttackDir; }
	Vec2& GetMoveDir() { return m_MoveDir; }
	bool& GetIsSlowMode() { return m_IsSlowMode; }
	float& GetBattery() { return m_fBattery; }
	float& GetTimer() { return m_Timer; }

	void SetMoveDir(Vec2 _vec2) { m_MoveDir = _vec2; }
	void SetEnemyAttackDir(Vec2 _vec2) { m_EnemyAttackDir = _vec2; }
	void SetHitOn(bool _bool) { m_bHitOn = _bool; }


	void StateChange(PlayerState _State);

private:
	void Shoot();

	void DirAnimationCheck();
	void StateUpdate();

	bool IsMoveKey();
	void SetSize2x();

	void MoveDir(const Vec2& Dir);
	void MoveValue(const Vector2& MoveValue);  //Move();
	void SetPos(const Vector2& Pos);

public:
	virtual void SaveToLevelFile(FILE* _File) override;
	virtual void LoadFromLevelFile(FILE* _FILE) override;
	CLONE(CPlayerScript);
public:
	CPlayerScript();
	~CPlayerScript();

private:
	virtual void BeginOverlap(CCollider2D* _Other) override;
	void SlowModeIn();
	void SlowModeOut();

private:
	void IdleUpdate();
	void IdleToRunUpdate();
	void RunUpdate();
	void RunToIdleUpdate();
	void JumpUpdate();
	void LandingUpdate();
	void AttackUpdate();
	void FallUpdate();
	void DodgeUpdate();
	void PlaySongUpdate();
	void HurtFlyLoopUpdate();
	void HurtGroundUpdate();
	void WallGrabUpdate();
	void FlipUpdate();
	void DeadUpdate();


	void IdleStart();
	void IdleToRunStart();
	void RunStart();
	void RunToIdleStart();
	void JumpStart();
	void LandingStart();
	void AttackStart();
	void FallStart();
	void DodgeStart();
	void PlaySongStart();
	void HurtFlyLoopStart();
	void HurtGroundStart();
	void WallGrabStart();
	void FlipStart();
	void DeadStart();

private:
	// �̷��� ��ġ�� ����Ͽ� �װ��� RGB���� üũ�ϰ�, �̵� ������ ���̸� �̵��Ѵ�.
	void MapCollisionCheckMoveGround();
	void MapCollisionCheckMoveAir();
};

// ���� ��������, ����
void ResetStage_1();
void ResetStage_2();
void ResetStage_3();
void ResetStage_Ending();

CGameObject* CreateGrunt();
CGameObject* CreatePomp();
CGameObject* CreateGangster();