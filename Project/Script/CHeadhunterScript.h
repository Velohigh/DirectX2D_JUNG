#pragma once
#include <Engine\CScript.h>
#include "CMonsterScript.h"

class CHeadhunterScript :
    public CMonsterScript
{
private:
	BossState	m_BeginState;

	BossState	m_PreState;
	BossState	m_CurState;
	float		m_StateTime[(UINT)(BossState::End)] = {};

	float		m_Gravity;
	float		m_GravityAccel;

	float		m_JumpPower;


public:
    virtual void begin() override;
    virtual void tick() override;

public:
	virtual void DirAnimationCheck() override;
	virtual void StateChange(BossState _State);
	virtual void StateUpdate() override;


private:
    CLONE(CHeadhunterScript);
public:
    CHeadhunterScript();
    ~CHeadhunterScript();

	virtual void BeginOverlap(CCollider2D* _Other) override;

	void SetBeginState(BossState _State) { m_BeginState = _State; }

private:
	////	FSM Start
	virtual void IdleS();	// 대기
	virtual void TakeoutgunS();		// 권총 꺼내는 모션
	virtual void PutbackgunS();		// 권총 넣는 모션
	virtual void ShootS();			// 권총 쏘는 모션
	virtual void TakeoutrifleS();	// 라이플 꺼내는 모션
	virtual void PutbackrifleS();	// 라이플 넣는 모션
	virtual void Aim_rifleS();		// 라이플 조준하는 모션
	virtual void HurtS();			// 한대맞고 한바퀴 돌면서 날아가는 모션
	virtual void RecoverS();		// 한대맞고나서 그로기상태에서S(); 사라지는 모습.
	virtual void WallidleS();		// 벽에 매달려서 대기
	virtual void WalljumpS();		// 벽에서 점프하여 지상으로 착지하는 모션
	virtual void Walljump_landS();	// 착지모션
	virtual void PredashS();				// 칼 꺼내서 대쉬하기직전 모션
	virtual void DashS();					// 칼꺼내고 대쉬하는 모션
	virtual void Dash_end_groundS();		// 대쉬 끝나고 베는 모션
	virtual void PrejumpS();				// 점프 진입모션
	virtual void JumpS();					// 점프
	virtual void Teleport_inS();			// 라이플 아래향한채로S(); 나타나는 모션
	virtual void Teleport_outS();			// 라이플 아래향한채로S(); 사라지는 모션
	virtual void Teleport_in_groundS();		// 땅에서 라이플 앞 향한채로S(); 나타나는 모션
	virtual void Teleport_out_groundS();	// 땅에서 라이플 앞 향한채로S(); 사라지는 모션
	virtual void Teleport_in_sweepS();		// sweep 하기전에 공중에 나타나는 모션
	virtual void SweepS();					// 공중에서 총을 한바퀴 회전하면서S(); 총알을 흩뿌리는 모션
	virtual void Teleport_out_sweepS();		// sweep 후S(); 공중에서 사라지는 모션
	virtual void DodgerollS();				// 굴러서 회피 모션
	virtual void HurtflyS();				// 막타 맞았을때 날아가는 모션
	virtual void HurtgroundS();				// 막타 맞고 날아가다 땅에 닿았을때 모션



	////	FSM Update
	virtual void IdleU();	// 대기
	virtual void TakeoutgunU();		// 권총 꺼내는 모션
	virtual void PutbackgunU();		// 권총 넣는 모션
	virtual void ShootU();			// 권총 쏘는 모션
	virtual void TakeoutrifleU();	// 라이플 꺼내는 모션
	virtual void PutbackrifleU();	// 라이플 넣는 모션
	virtual void Aim_rifleU();		// 라이플 조준하는 모션
	virtual void HurtU();			// 한대맞고 한바퀴 돌면서 날아가는 모션
	virtual void RecoverU();		// 한대맞고나서 그로기상태에서U(); 사라지는 모습.
	virtual void WallidleU();		// 벽에 매달려서 대기
	virtual void WalljumpU();		// 벽에서 점프하여 지상으로 착지하는 모션
	virtual void Walljump_landU();	// 착지모션
	virtual void PredashU();				// 칼 꺼내서 대쉬하기직전 모션
	virtual void DashU();					// 칼꺼내고 대쉬하는 모션
	virtual void Dash_end_groundU();		// 대쉬 끝나고 베는 모션
	virtual void PrejumpU();				// 점프 진입모션
	virtual void JumpU();					// 점프
	virtual void Teleport_inU();			// 라이플 아래향한채로U(); 나타나는 모션
	virtual void Teleport_outU();			// 라이플 아래향한채로U(); 사라지는 모션
	virtual void Teleport_in_groundU();		// 땅에서 라이플 앞 향한채로U(); 나타나는 모션
	virtual void Teleport_out_groundU();	// 땅에서 라이플 앞 향한채로U(); 사라지는 모션
	virtual void Teleport_in_sweepU();		// sweep 하기전에 공중에 나타나는 모션
	virtual void SweepU();					// 공중에서 총을 한바퀴 회전하면서U(); 총알을 흩뿌리는 모션
	virtual void Teleport_out_sweepU();		// sweep 후U(); 공중에서 사라지는 모션
	virtual void DodgerollU();				// 굴러서 회피 모션
	virtual void HurtflyU();				// 막타 맞았을때 날아가는 모션
	virtual void HurtgroundU();				// 막타 맞고 날아가다 땅에 닿았을때 모션





};

