#pragma once
#include <Engine\CScript.h>

enum class ObjDir
{
	Left,
	Right,
	End
};

enum class ObjState
{
	Idle,
	Walk,
	Turn,
	Run,
	Attack,
	HurtGround,
	HurtFly,
	KnockDown,
	Dead,
	END
};

class CGruntScript :
	public CScript
{
private:
	ObjDir		m_PreDir;	// 이전에 바라보고있던 방향
	ObjDir		m_CurDir;	// 현재 바라보는 방향
	ObjState	m_CurState;	// 현재 상태
	CLevel* m_Level;	// 속한 레벨
	float		m_StateTime[static_cast<int>(ObjState::END)];	// 해당 상태가 되고 지난 시간

	float		m_MoveSpeed;	// 이동속도


public:
	virtual void begin() override;
	virtual void tick() override;

public:
	void DirAnimationCheck();
	void StateChange(ObjState _State);
	void StateUpdate();

public:
	void SetSpeed(float _Speed) { m_MoveSpeed = _Speed; }

private:
	CLONE(CGruntScript);
public:
	CGruntScript();
	~CGruntScript();

private:
	void SetSize2x();	// 사이즈를 원본 이미지의 2배로 지정

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


};

