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
	PlayerDir	m_CurDir = PlayerDir::Right;
	PlayerState	m_CurState = PlayerState::END;
	float		m_StateTime[(UINT)(PlayerState::END)] = {};

	Vec2		m_MoveDir;
    float       m_MoveSpeed;

public:
    virtual void begin() override;
    virtual void tick() override;

    virtual void BeginOverlap(CCollider2D* _Other) override;

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
	// 미래의 위치를 계산하여 그곳의 RGB값을 체크하고, 이동 가능한 곳이면 이동한다.
	void MapCollisionCheckMoveGround();
	void MapCollisionCheckMoveAir();
};

