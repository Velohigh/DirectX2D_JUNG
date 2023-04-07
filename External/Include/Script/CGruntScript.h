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
	ObjDir		m_PreDir;	// ������ �ٶ󺸰��ִ� ����
	ObjDir		m_CurDir;	// ���� �ٶ󺸴� ����
	ObjState	m_CurState;	// ���� ����
	CLevel* m_Level;	// ���� ����
	float		m_StateTime[static_cast<int>(ObjState::END)];	// �ش� ���°� �ǰ� ���� �ð�

	float		m_MoveSpeed;	// �̵��ӵ�


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
	void SetSize2x();	// ����� ���� �̹����� 2��� ����

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

