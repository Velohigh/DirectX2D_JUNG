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

	float		m_Gravity;		// �÷��̾� �߷� ���
	float		m_GravityAccel;		// �߷°��ӵ�

	float		m_JumpPower;
	float		m_LongJumpPower;
	bool		m_IsLongJump;

	int			m_AttackCount;

	Vec2		m_MoveDir;		// �̵� ���� ����
	float       m_MoveSpeed;	// �̵� �ӵ�
	Vec2		m_AttackDir;

	bool		m_bRun1SoundOn = false;	// Run ���� �������
	bool		m_bRun2SoundOn = false;	// Run ���� �������

public:
	virtual void begin() override;
	virtual void tick() override;

	virtual void BeginOverlap(CCollider2D* _Other) override;

public:
	PlayerState GetState() { return m_CurState; }
	Vec2 GetAttackDir() { return m_AttackDir; }

private:
	void Shoot();

	void DirAnimationCheck();
	void StateChange(PlayerState _State);
	void StateUpdate();

	bool IsMoveKey();
	void SetSize2x();

	void MoveDir(const Vec2& Dir);
	void MoveValue(const Vector2& MoveValue);  //Move();
	void SetPos(const Vector2& Pos);
	void SetPivot();

public:
	virtual void SaveToLevelFile(FILE* _File) override;
	virtual void LoadFromLevelFile(FILE* _FILE) override;
	CLONE(CPlayerScript);
public:
	CPlayerScript();
	~CPlayerScript();

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

