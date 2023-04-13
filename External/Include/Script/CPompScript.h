#pragma once
#include <Engine\CScript.h>
#include "CMonsterScript.h"


class CPompScript :
	public CMonsterScript
{
private:

public:
	virtual void begin() override;
	virtual void tick() override;

private:
	CLONE(CPompScript);
public:
	CPompScript();
	~CPompScript();

protected:
	///////////////////////////////
	////	FSM
	///////////////////////////////
	virtual void IdleStart();
	virtual void WalkStart();
	virtual void TurnStart();
	virtual void RunStart();
	virtual void AttackStart();
	virtual void HurtGroundStart();
	virtual void HurtFlyStart();
	virtual void KnockDownStart();
	virtual void DeadStart();


	virtual void IdleUpdate();
	virtual void WalkUpdate();
	virtual void TurnUpdate();
	virtual void RunUpdate();
	virtual void AttackUpdate();
	virtual void HurtGroundUpdate();
	virtual void HurtFlyUpdate();
	virtual void KnockDownUpdate();
	virtual void DeadUpdate();
};

