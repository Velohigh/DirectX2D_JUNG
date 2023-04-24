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
	virtual void IdleS();	// ���
	virtual void TakeoutgunS();		// ���� ������ ���
	virtual void PutbackgunS();		// ���� �ִ� ���
	virtual void ShootS();			// ���� ��� ���
	virtual void TakeoutrifleS();	// ������ ������ ���
	virtual void PutbackrifleS();	// ������ �ִ� ���
	virtual void Aim_rifleS();		// ������ �����ϴ� ���
	virtual void HurtS();			// �Ѵ�°� �ѹ��� ���鼭 ���ư��� ���
	virtual void RecoverS();		// �Ѵ�°��� �׷α���¿���S(); ������� ���.
	virtual void WallidleS();		// ���� �Ŵ޷��� ���
	virtual void WalljumpS();		// ������ �����Ͽ� �������� �����ϴ� ���
	virtual void Walljump_landS();	// �������
	virtual void PredashS();				// Į ������ �뽬�ϱ����� ���
	virtual void DashS();					// Į������ �뽬�ϴ� ���
	virtual void Dash_end_groundS();		// �뽬 ������ ���� ���
	virtual void PrejumpS();				// ���� ���Ը��
	virtual void JumpS();					// ����
	virtual void Teleport_inS();			// ������ �Ʒ�����ä��S(); ��Ÿ���� ���
	virtual void Teleport_outS();			// ������ �Ʒ�����ä��S(); ������� ���
	virtual void Teleport_in_groundS();		// ������ ������ �� ����ä��S(); ��Ÿ���� ���
	virtual void Teleport_out_groundS();	// ������ ������ �� ����ä��S(); ������� ���
	virtual void Teleport_in_sweepS();		// sweep �ϱ����� ���߿� ��Ÿ���� ���
	virtual void SweepS();					// ���߿��� ���� �ѹ��� ȸ���ϸ鼭S(); �Ѿ��� ��Ѹ��� ���
	virtual void Teleport_out_sweepS();		// sweep ��S(); ���߿��� ������� ���
	virtual void DodgerollS();				// ������ ȸ�� ���
	virtual void HurtflyS();				// ��Ÿ �¾����� ���ư��� ���
	virtual void HurtgroundS();				// ��Ÿ �°� ���ư��� ���� ������� ���



	////	FSM Update
	virtual void IdleU();	// ���
	virtual void TakeoutgunU();		// ���� ������ ���
	virtual void PutbackgunU();		// ���� �ִ� ���
	virtual void ShootU();			// ���� ��� ���
	virtual void TakeoutrifleU();	// ������ ������ ���
	virtual void PutbackrifleU();	// ������ �ִ� ���
	virtual void Aim_rifleU();		// ������ �����ϴ� ���
	virtual void HurtU();			// �Ѵ�°� �ѹ��� ���鼭 ���ư��� ���
	virtual void RecoverU();		// �Ѵ�°��� �׷α���¿���U(); ������� ���.
	virtual void WallidleU();		// ���� �Ŵ޷��� ���
	virtual void WalljumpU();		// ������ �����Ͽ� �������� �����ϴ� ���
	virtual void Walljump_landU();	// �������
	virtual void PredashU();				// Į ������ �뽬�ϱ����� ���
	virtual void DashU();					// Į������ �뽬�ϴ� ���
	virtual void Dash_end_groundU();		// �뽬 ������ ���� ���
	virtual void PrejumpU();				// ���� ���Ը��
	virtual void JumpU();					// ����
	virtual void Teleport_inU();			// ������ �Ʒ�����ä��U(); ��Ÿ���� ���
	virtual void Teleport_outU();			// ������ �Ʒ�����ä��U(); ������� ���
	virtual void Teleport_in_groundU();		// ������ ������ �� ����ä��U(); ��Ÿ���� ���
	virtual void Teleport_out_groundU();	// ������ ������ �� ����ä��U(); ������� ���
	virtual void Teleport_in_sweepU();		// sweep �ϱ����� ���߿� ��Ÿ���� ���
	virtual void SweepU();					// ���߿��� ���� �ѹ��� ȸ���ϸ鼭U(); �Ѿ��� ��Ѹ��� ���
	virtual void Teleport_out_sweepU();		// sweep ��U(); ���߿��� ������� ���
	virtual void DodgerollU();				// ������ ȸ�� ���
	virtual void HurtflyU();				// ��Ÿ �¾����� ���ư��� ���
	virtual void HurtgroundU();				// ��Ÿ �°� ���ư��� ���� ������� ���





};

