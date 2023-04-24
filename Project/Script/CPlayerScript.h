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

	float		m_fBattery;		// 배터리 양
	float		m_BatteryRechargeTime;	// 배터리 재충전 시간
	float		m_BatteryPushTime;		// 누른 시간

	float		m_Timer;		// 제한시간


	float		m_Gravity;			// 플레이어 중력 계수
	float		m_GravityAccel;		// 중력가속도

	float		m_JumpPower;
	float		m_LongJumpPower;
	bool		m_IsLongJump;

	int			m_AttackCount;

	Vec2		m_Move;			// 이동량
	Vec3		m_PrePos;		// 이전 프레임 위치
	Vec3		m_CurPos;

	Vec2		m_MoveDir;		// 이동 방향 벡터
	float       m_MoveSpeed;	// 이동 속도
	Vec2		m_AttackDir;	// 내 공격 방향
	Vec2		m_EnemyAttackDir;	// 적의 공격 방향

	bool		m_bRun1SoundOn;	// Run 사운드 재생여부
	bool		m_bRun2SoundOn;	// Run 사운드 재생여부
	bool		m_bHitOn;				// 공격 받았는지 여부

	bool		m_IsSlowMode;	// 슬로우 모드인지 여부
	bool		m_bReplayStage;	// 클리어후 리플레이 재생여부

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
	// 미래의 위치를 계산하여 그곳의 RGB값을 체크하고, 이동 가능한 곳이면 이동한다.
	void MapCollisionCheckMoveGround();
	void MapCollisionCheckMoveAir();
};

// 리셋 스테이지, 엔딩
void ResetStage_1();
void ResetStage_2();
void ResetStage_3();
void ResetStage_Ending();

CGameObject* CreateGrunt();
CGameObject* CreatePomp();
CGameObject* CreateGangster();