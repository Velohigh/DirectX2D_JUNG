#include "pch.h"
#include "CPlayerScript.h"
#include "CGruntScript.h"
#include "CGangsterScript.h"
#include "CPompScript.h"
#include "CMouseScript.h"
#include "CUIScript.h"
#include "CBatteryScript.h"
#include "CTimerScript.h"
#include <random>

#include <Engine\CMeshRender.h>
#include <Engine\CMaterial.h>
#include <Engine\CLevelMgr.h>
#include <Engine\CCollisionMgr.h>

#include "CMissileScript.h"
#include "CDustCloudScript.h"
#include "CJumpCloudScript.h"
#include "CLandCloudScript.h"
#include "CSlashScript.h"
#include "CRewinderScript.h"

CPlayerScript::CPlayerScript()
	: CScript((UINT)SCRIPT_TYPE::PLAYERSCRIPT)
	, m_MoveSpeed(50.f)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_MoveSpeed, "Speed");
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_JumpPower, "JumpPower");
	AddScriptParam(SCRIPT_PARAM::FLOAT, &m_LongJumpPower, "LongJumpPower");
}

CPlayerScript::~CPlayerScript()
{

}



void CPlayerScript::begin()
{
	m_Level = CLevelMgr::GetInst()->GetCurLevel();
	MeshRender()->GetDynamicMaterial();

	StateChange(PlayerState::Idle);


	// �浹�� ����
	wstring LevelName = m_Level->GetName();
	if (LevelName == L"Stage_1")
	{
		Ptr<CTexture> Colmap = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\map\\room_factory_2_ColMap.png");
		GetOwner()->SetColMapTexture(Colmap.Get());
	}
	else if (LevelName == L"Stage_2")
	{
		Ptr<CTexture> Colmap = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\map\\stage2_bg_collision.png");
		GetOwner()->SetColMapTexture(Colmap.Get());
	}
	else if (LevelName == L"Stage_3")
	{
		Ptr<CTexture> Colmap = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\map\\stage3_bg_collision.png");
		GetOwner()->SetColMapTexture(Colmap.Get());
	}


	m_Gravity = 10.f;		// �÷��̾� �߷� ���
	m_GravityAccel = 2000.f;		// �߷°��ӵ�

	m_fBattery = 1;				// ���͸� �� 1�� �ִ�
	m_BatteryRechargeTime = 0;	// ���͸� ������ �ð�
	m_BatteryPushTime = 0;		// ���� �ð�

	m_Timer = 60.f;				// ���ѽð�


	m_JumpPower = 330.f;
	m_LongJumpPower = 2050.f;
	m_IsLongJump = false;

	m_AttackCount = 0;


	m_bRun1SoundOn = false;		// Run ���� �������
	m_bRun2SoundOn = false;		// Run ���� �������
	m_bHitOn = false;			// ���� �޾Ҵ��� ����

}

void CPlayerScript::tick()
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	m_PrePos = Transform()->GetRelativePos();


	DirAnimationCheck();
	StateUpdate();
	m_CurPos = Transform()->GetRelativePos();

	// �̵���
	m_Move = Vec2(m_PrePos.x - m_CurPos.x, m_PrePos.y - m_CurPos.y);


	Vec3 CameraPos = m_Level->FindParentObjectByName(L"MainCamera")->Transform()->GetRelativePos();
	Collider2D()->SetOffsetPos(Vec2(-CameraPos.x, -CameraPos.y + 35.f));

	SlowModeIn();
	//SlowModeOut();

	// ���ѽð� ����
	if(m_Timer > 0.f)
		m_Timer -= DT;


	//Transform()->SetRelativePos(vCurPos);

	//if (KEY_TAP(KEY::SPACE))
	//{
	//	DrawDebugCircle(Transform()->GetWorldPos(), 500.f, Vec4(0.f, 0.f, 1.f, 1.f), Vec3(0.f, 0.f, 0.f), 2.f);

	//	Shoot();
	//}

	// ����� ��, �ش� ���� �ٽ� ����.
	if (KEY_TAP(KEY::R))
	{
		// ����� ����
		Ptr<CSound> pBGM = CLevelMgr::GetInst()->GetCurLevel()->GetBgm();
		pBGM->Stop();

		// ����� ���� ���
		pBGM = CResMgr::GetInst()->FindRes<CSound>(L"sound\\Rewind.wav");
		pBGM->Play(1, 0.9f, true);

		// ����� ���� ���
		pBGM = CResMgr::GetInst()->FindRes<CSound>(L"sound\\replay_rew.wav");
		pBGM->Play(999, 0.9f, true);

		CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
		pCurLevel->SetBgm(pBGM.Get());


		CGameObject* pRewinder = CLevelMgr::GetInst()->GetCurLevel()->FindParentObjectByName(L"Rewinder");

		if (nullptr != pRewinder)
		{
			pRewinder->GetScript<CRewinderScript>()->SetAnimationSaveOn(false);
			pRewinder->Transform()->SetRelativeScale(1280.f, 720.f, 1.f);
			pRewinder->GetScript<CRewinderScript>()->CreateFolderAnim_Rewinder();
			pRewinder->Animator2D()->Play(L"RewinderReverseAnimation", true);

			m_Level->FindParentObjectByName(L"MainCamera")->Camera()->SetLayerMaskAll(false);

			CLevelMgr::GetInst()->GetCurLevel()->DestroyAllObject();

			CGameObject* pPostProcess = new CGameObject;
			pPostProcess->SetName(L"PostProcess");
			pPostProcess->AddComponent(new CTransform);
			pPostProcess->AddComponent(new CMeshRender);
			pPostProcess->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			pPostProcess->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"GrayMtrl"));
			SpawnGameObject(pPostProcess, Vec3(0.f, 0.f, 0.f), 31);

			CGameObject* pDistortion = new CGameObject;
			pDistortion->SetName(L"Distortion");
			pDistortion->AddComponent(new CTransform);
			pDistortion->AddComponent(new CMeshRender);
			pDistortion->Transform()->SetRelativeScale(1280, 720.f, 1.f);
			pDistortion->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
			pDistortion->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DistortionMtrl"));
			SpawnGameObject(pDistortion, Vec3(0.f, 0.f, 20.f), 31);

		}
	}
}

void CPlayerScript::Shoot()
{
	// �̻��� ������ ����
	Ptr<CPrefab> pMissilePrefab = CResMgr::GetInst()->FindRes<CPrefab>(L"MissilePrefab");
	Vec3 vMissilePos = Transform()->GetRelativePos() + Vec3(0.f, 0.5f, 0.f) * Transform()->GetRelativeScale();
	CGameObject* pCloneMissile = pMissilePrefab->Instantiate();

	// ������ �߰�
	SpawnGameObject(pCloneMissile, vMissilePos, L"PlayerProjectile");
}

void CPlayerScript::DirAnimationCheck()
{
	PlayerDir CheckDir = m_CurDir;

	Vec3 Rot = Transform()->GetRelativeRot();


	if (m_CurState != PlayerState::Attack && m_CurState != PlayerState::Dodge &&
		m_CurState != PlayerState::Dead && m_CurState != PlayerState::HurtFlyLoop &&
		m_CurState != PlayerState::HurtGround &&
		m_CurState != PlayerState::WallGrab &&
		m_CurState != PlayerState::Flip)
	{

		if (KEY_PRESSED(KEY::D))
		{
			CheckDir = PlayerDir::Right;
		}

		else if (KEY_PRESSED(KEY::A))
		{
			CheckDir = PlayerDir::Left;
		}

		if (CheckDir != m_CurDir)
		{
			m_CurDir = CheckDir;
		}
	}

	if (m_CurDir == PlayerDir::Right)
	{
		Rot = Vec3(Rot.x, 0.f, Rot.z);

	}
	else if (m_CurDir == PlayerDir::Left)
	{
		Rot = Vec3(Rot.x, XM_PI, Rot.z);
		//Vec3 OffsetPos = Collider2D()->GetOffsetPos();
		//Collider2D()->SetOffsetPos(Vec2(OffsetPos.x, OffsetPos.y));
		//int a = 0;
		//Collider2D()->SetOffsetPos()
	}


	Transform()->SetRelativeRot(Rot);
}

void CPlayerScript::StateChange(PlayerState _State)
{
	if (m_CurState != _State)	// ���°� �ٲ� �ѹ��� ��������ش�.
	{
		switch (_State)
		{
		case PlayerState::Idle:
			IdleStart();
			break;
		case PlayerState::IdleToRun:
			IdleToRunStart();
			break;
		case PlayerState::Run:
			RunStart();
			break;
		case PlayerState::RunToIdle:
			RunToIdleStart();
			break;
		case PlayerState::Jump:
			JumpStart();
			break;
		case PlayerState::Landing:
			LandingStart();
			break;
		case PlayerState::Attack:
			AttackStart();
			break;
		case PlayerState::Fall:
			FallStart();
			break;
		case PlayerState::Dodge:
			DodgeStart();
			break;
		case PlayerState::PlaySong:
			PlaySongStart();
			break;
		case PlayerState::HurtFlyLoop:
			HurtFlyLoopStart();
			break;
		case PlayerState::HurtGround:
			HurtGroundStart();
			break;
		case PlayerState::WallGrab:
			WallGrabStart();
			break;
		case PlayerState::Flip:
			FlipStart();
			break;
		case PlayerState::Dead:
			DeadStart();
			break;
		}
		m_CurState = _State;
	}
}

void CPlayerScript::StateUpdate()
{
	switch (m_CurState)
	{
	case PlayerState::Idle:
		IdleUpdate();
		break;
	case PlayerState::IdleToRun:
		IdleToRunUpdate();
		break;
	case PlayerState::Run:
		RunUpdate();
		break;
	case PlayerState::RunToIdle:
		RunToIdleUpdate();
		break;
	case PlayerState::Jump:
		JumpUpdate();
		break;
	case PlayerState::Landing:
		LandingUpdate();
		break;
	case PlayerState::Attack:
		AttackUpdate();
		break;
	case PlayerState::Fall:
		FallUpdate();
		break;
	case PlayerState::Dodge:
		DodgeUpdate();
		break;
	case PlayerState::PlaySong:
		PlaySongUpdate();
		break;
	case PlayerState::HurtFlyLoop:
		HurtFlyLoopUpdate();
		break;
	case PlayerState::HurtGround:
		HurtGroundUpdate();
		break;
	case PlayerState::WallGrab:
		WallGrabUpdate();
		break;
	case PlayerState::Flip:
		FlipUpdate();
		break;
	case PlayerState::Dead:
		DeadUpdate();
		break;

	}
}

bool CPlayerScript::IsMoveKey()
{
	// ����Ű �����ٸ� true ����
	if (KEY_PRESSED(KEY::A) ||
		KEY_PRESSED(KEY::D))
	{
		return true;
	}
	return false;
}

void CPlayerScript::SetSize2x()
{
	vector<Ptr<CTexture>> VecFolderTex = Animator2D()->GetCurAnim()->GetvecFolderTex();
	int CurFrmCount = Animator2D()->GetCurAnim()->GetCurFrmCount();

	float Width = VecFolderTex[CurFrmCount]->Width();
	float Height = VecFolderTex[CurFrmCount]->Height();
	Transform()->SetRelativeScale(Width * 2, Height * 2, 1);
}

void CPlayerScript::MoveDir(const Vec2& Dir)
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	m_Pos += Dir * m_MoveSpeed * DT;

	Transform()->SetRelativePos(m_Pos.x, m_Pos.y, m_Pos3.z);
}

void CPlayerScript::MoveValue(const Vector2& MoveValue)
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);

	m_Pos += MoveValue /*m_TimeScale*/;

	if (MoveValue.Length() <= 0.001f)
		int ab = 0;

	Transform()->SetRelativePos(m_Pos.x, m_Pos.y, m_Pos3.z);

}

void CPlayerScript::SetPos(const Vector2& Pos)
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Transform()->SetRelativePos(Pos.x, Pos.y, m_Pos3.z);

}


void CPlayerScript::SlowModeIn()
{

	if (KEY_TAP(KEY::Q))
	{
		// ���͸� �ܷ��� 0 ���϶�� �ٷ� ����.
		if (m_fBattery <= 0)
		{
			SlowModeOut();
			return;
		}
		
		// ���͸� �ܷ��� ������� �۵�
		else if (m_fBattery > 0.1f)
		{

			CTimeMgr::GetInst()->SetTimeScale(0.2f);

			// SlowMode �� ��� �ؽ��� ����
			if (m_Level->GetName() == L"Stage_1")
			{
				m_Level->FindParentObjectByName(L"BackGround")->MeshRender()->GetMaterial()->
					SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\map\\room_factory_2_slow.png"));
			}
			else if (m_Level->GetName() == L"Stage_2")
			{
				m_Level->FindParentObjectByName(L"BackGround")->MeshRender()->GetMaterial()->
					SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\map\\stage2_bg_render_slow.png"));
			}
			else if (m_Level->GetName() == L"Stage_3")
			{
				m_Level->FindParentObjectByName(L"BackGround")->MeshRender()->GetMaterial()->
					SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\map\\stage3_bg_render_slow.png"));
			}


			// SlowModeIn ���� ���
			Ptr<CSound> pSlowModeSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\slow_in.mp3");
			pSlowModeSound->Play(1, 1.f, true);

			m_IsSlowMode = true;
		}
	}

	if (KEY_PRESSED(KEY::Q))
	{
		// ���͸� �ܷ��� 0 ���϶�� �ٷ� ����.
		if (m_fBattery <= 0)
		{
			SlowModeOut();
			return;
		}


		m_BatteryPushTime += DT;
		m_fBattery -= DT;
	}
	else
	{
		// ���͸� ������
		if(m_fBattery < 1.f)
			m_fBattery += DT * 0.1f;
	}

	if (KEY_RELEASE(KEY::Q))
	{
		SlowModeOut();
	}
}

void CPlayerScript::SlowModeOut()
{
		if(m_BatteryPushTime != 0.f)
			m_BatteryPushTime = 0.f;

		CTimeMgr::GetInst()->SetTimeScale(1.f);

		// SlowMode �� ��� �ؽ��� ����
		if (m_Level->GetName() == L"Stage_1")
		{
			m_Level->FindParentObjectByName(L"BackGround")->MeshRender()->GetMaterial()->
				SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\map\\room_factory_2.png"));
		}
		else if (m_Level->GetName() == L"Stage_2")
		{
			m_Level->FindParentObjectByName(L"BackGround")->MeshRender()->GetMaterial()->
				SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\map\\stage2_bg_render.png"));
		}
		else if (m_Level->GetName() == L"Stage_3")
		{
			m_Level->FindParentObjectByName(L"BackGround")->MeshRender()->GetMaterial()->
				SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\map\\stage3_bg_render.png"));
		}


		// SlowModeOut ���� ���
		Ptr<CSound> pSlowModeSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\slow_out.mp3");
		pSlowModeSound->Play(1, 1.f, false);

		m_IsSlowMode = false;
}


void CPlayerScript::BeginOverlap(CCollider2D* _Other)
{

	if (m_bHitOn == true)
	{
		// �÷��̾ ��� ���°� �ƴϰ� , ������ ������ �ƴҋ� ��Ʈ����
		if (m_CurState != PlayerState::HurtFlyLoop &&
			m_CurState != PlayerState::HurtGround &&
			m_CurState != PlayerState::Dead &&
			m_CurState != PlayerState::Dodge)
		{
			StateChange(PlayerState::HurtFlyLoop);
			return;
		}
	}

}





void CPlayerScript::SaveToLevelFile(FILE* _File)
{
	fwrite(&m_MoveSpeed, sizeof(float), 1, _File);
}

void CPlayerScript::LoadFromLevelFile(FILE* _File)
{
	fread(&m_MoveSpeed, sizeof(float), 1, _File);
}


///////////////////////////////////////////
	// State Update 
///////////////////////////////////////////

void CPlayerScript::IdleUpdate()
{
	// �̵�Ű�� ������ IdleToRun ���·�
	if (true == IsMoveKey())
	{
		StateChange(PlayerState::IdleToRun);
		return;
	}


	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	Vec2 m_PosYReverse = Vec2(m_Pos3.x, -m_Pos3.y);
	CTexture* m_MapColTexture = GetOwner()->GetColMapTexture();

	// �Ʒ��ʿ� ������ ���ٸ� Fall���·�
	int color = m_MapColTexture->GetPixelColor(m_PosYReverse + Vec2{ 0,10 });
	int Rcolor = m_MapColTexture->GetPixelColor(m_PosYReverse + Vec2{ 0,1 });
	if (color != RGB(0, 0, 0) && m_CurState != PlayerState::Jump &&
		Rcolor != RGB(255, 0, 0) &&
		Rcolor != RGB(0, 0, 0))
	{
		StateChange(PlayerState::Fall);
		return;
	}

	// �浹�� �������̸� �Ʒ��� �̵� ����
	if (Rcolor == RGB(255, 0, 0) &&
		KEY_TAP(KEY::S))
	{
		SetPos(m_Pos + Vector2{ 0, -4 });
	}

	// ����Ű�� ������ Jump ���·�
	if (KEY_TAP(KEY::SPACE))		// @@@ ���� �߰�.
	{
		StateChange(PlayerState::Jump);
		return;
	}

	// ����Ű�� ������ ���ݻ��·�
	if (KEY_TAP(KEY::LBTN))
	{
		StateChange(PlayerState::Attack);
		return;
	}

	// ȸ��
	if (KEY_TAP(KEY::LSHIFT))	// @@@ ȸ�� �߰�.
	{
		StateChange(PlayerState::Dodge);
		return;
	}



}

void CPlayerScript::IdleToRunUpdate()
{
	// �ִϸ��̼� ������ Run �������
	if (true == Animator2D()->IsEndAnimation())
	{
		StateChange(PlayerState::Run);
		return;
	}

	// �̵�Ű�� �ȴ����ٸ� Idle ���·�
	if (false == IsMoveKey())				// �̵�Ű�� �ȴ����ٸ�
	{
		StateChange(PlayerState::Idle);
		return;
	}

	// ����Ű�� ������ Jump ���·�
	if (KEY_TAP(KEY::SPACE))		// @@@ ���� �߰�.
	{
		StateChange(PlayerState::Jump);
		return;
	}

	// ȸ��Ű�� ������ Dodge ���·�
	if (KEY_TAP(KEY::LSHIFT))	// @@@ ȸ�� �߰�.
	{
		StateChange(PlayerState::Dodge);
		return;
	}

	// ����
	if (KEY_TAP(KEY::LBTN))
	{
		StateChange(PlayerState::Attack);
		return;
	}

	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	Vec2 m_PosYReverse = Vec2(m_Pos3.x, -m_Pos3.y);
	CTexture* m_MapColTexture = GetOwner()->GetColMapTexture();


	// �Ʒ��ʿ� ������ ���ٸ� Fall���·�
	int color[5] = {};
	for (int i = 1; i <= 5; ++i)
	{
		color[i - 1] = m_MapColTexture->GetPixelColor(m_PosYReverse + Vector2{ 0.f,(float)i });
	}
	if (color[0] != RGB(0, 0, 0) && color[0] != RGB(255, 0, 0) &&
		color[1] != RGB(0, 0, 0) &&
		color[2] != RGB(0, 0, 0) &&
		color[3] != RGB(0, 0, 0) &&
		color[4] != RGB(0, 0, 0) &&
		m_CurState != PlayerState::Jump)
	{
		StateChange(PlayerState::Fall);
		return;
	}

	// �浹�� �������̸� �Ʒ��� �̵� ����
	if (color[0] == RGB(255, 0, 0) &&
		KEY_TAP(KEY::S))
	{
		SetPos(m_Pos + Vector2{ 0, -4 });
	}

	m_MoveDir = Vector2{ 0,0 };

	if (KEY_PRESSED(KEY::A))
	{
		m_MoveDir = Vector2{ -1.f, 0.f };
	}

	if (KEY_PRESSED(KEY::D))
	{
		m_MoveDir = Vector2{ 1.f, 0.f };
	}

	MapCollisionCheckMoveGround();
}

void CPlayerScript::RunUpdate()
{
	m_StateTime[static_cast<int>(PlayerState::Run)] += DT;

	// �̵�Ű�� �ȴ����� Idle ���·�
	if (false == IsMoveKey())
	{
		StateChange(PlayerState::RunToIdle);
		return;
	}

	// ����Ű�� ������ Jump ���·�
	if (KEY_TAP(KEY::SPACE))		// @@@ ���� �߰�.
	{
		StateChange(PlayerState::Jump);
		return;
	}

	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	Vec2 m_PosYReverse = Vec2(m_Pos3.x, -m_Pos3.y);
	CTexture* m_MapColTexture = GetOwner()->GetColMapTexture();


	// �Ʒ��ʿ� ������ ���ٸ� Fall���·�
	int color[10] = {};
	for (int i = 1; i <= 10; ++i)
	{
		color[i - 1] = m_MapColTexture->GetPixelColor(m_PosYReverse + Vector2{ 0.f,(float)i });
	}
	if (color[0] != RGB(0, 0, 0) && color[0] != RGB(255, 0, 0) &&
		color[1] != RGB(0, 0, 0) &&
		color[2] != RGB(0, 0, 0) &&
		color[3] != RGB(0, 0, 0) &&
		color[4] != RGB(0, 0, 0) &&
		color[5] != RGB(0, 0, 0) &&
		color[6] != RGB(0, 0, 0) &&
		color[7] != RGB(0, 0, 0) &&
		color[8] != RGB(0, 0, 0) &&
		color[9] != RGB(0, 0, 0) &&
		m_CurState != PlayerState::Jump)
	{
		StateChange(PlayerState::Fall);
		return;
	}

	// �浹�� �������̸� �Ʒ��� �̵� ����
	if (color[0] == RGB(255, 0, 0) &&
		KEY_TAP(KEY::S))
	{
		SetPos(m_Pos + Vector2{ 0, -4 });
	}

	// ȸ��Ű�� ������ Dodge ���·�
	if (KEY_TAP(KEY::LSHIFT))	// @@@ ȸ�� �߰�.
	{
		StateChange(PlayerState::Dodge);
		return;
	}

	// ����
	if (KEY_TAP(KEY::LBTN))
	{
		StateChange(PlayerState::Attack);
		return;
	}


	m_MoveDir = Vector2{ 0.f, 0.f };

	if (KEY_PRESSED(KEY::A))
	{
		m_MoveDir = Vector2{ -1.f, 0.f };
	}

	if (KEY_PRESSED(KEY::D))
	{
		m_MoveDir = Vector2{ 1.f, 0.f };
	}

	// �޸��� �Ҹ�
	if (3 == Animator2D()->GetCurAnim()->GetCurFrmCount()
		&& m_bRun1SoundOn == false)
	{
		Ptr<CSound> pRunSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_player_running_2.wav");
		pRunSound->Play(1, 0.6f, true);
		m_bRun1SoundOn = true;
	}
	if (8 == Animator2D()->GetCurAnim()->GetCurFrmCount()
		&& m_bRun2SoundOn == false)
	{
		Ptr<CSound> pRunSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_player_running_2.wav");
		pRunSound->Play(1, 0.6f, true);
		m_bRun2SoundOn = true;
	}

	if (true == Animator2D()->IsEndAnimation())
	{
		m_bRun1SoundOn = false;
		m_bRun2SoundOn = false;
	}




	MapCollisionCheckMoveGround();

}

void CPlayerScript::RunToIdleUpdate()
{
	// �̵�Ű�� ������ Run ���·�
	if (true == IsMoveKey())
	{
		StateChange(PlayerState::IdleToRun);
		return;
	}

	// ����Ű�� ������ Jump ���·�
	if (KEY_TAP(KEY::SPACE))		// @@@ ���� �߰�.
	{
		StateChange(PlayerState::Jump);
		return;
	}

	// �̵�Ű�� �ȴ�����, �ִϸ��̼��� ������ ����Ǹ� Idle ���·�
	if (false == IsMoveKey() &&
		true == Animator2D()->IsEndAnimation())
	{
		StateChange(PlayerState::Idle);
		return;
	}

	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	Vec2 m_PosYReverse = Vec2(m_Pos3.x, -m_Pos3.y);
	CTexture* m_MapColTexture = GetOwner()->GetColMapTexture();


	// �Ʒ��ʿ� ������ ���ٸ� Fall���·�
	int color[10] = {};
	for (int i = 1; i <= 10; ++i)
	{
		color[i - 1] = m_MapColTexture->GetPixelColor(m_PosYReverse + Vector2{ 0.f,(float)i });
	}
	if (color[0] != RGB(0, 0, 0) && color[0] != RGB(255, 0, 0) &&
		color[1] != RGB(0, 0, 0) &&
		color[2] != RGB(0, 0, 0) &&
		color[3] != RGB(0, 0, 0) &&
		color[4] != RGB(0, 0, 0) &&
		color[5] != RGB(0, 0, 0) &&
		color[6] != RGB(0, 0, 0) &&
		color[7] != RGB(0, 0, 0) &&
		color[8] != RGB(0, 0, 0) &&
		color[9] != RGB(0, 0, 0) &&
		m_CurState != PlayerState::Jump)
	{
		StateChange(PlayerState::Fall);
		return;
	}

	// �浹�� �������̸� �Ʒ��� �̵� ����
	if (color[0] == RGB(255, 0, 0) &&
		KEY_TAP(KEY::S))
	{
		SetPos(m_Pos + Vector2{ 0, -4 });
	}

	// ȸ��Ű�� ������ Dodge ���·�
	if (KEY_TAP(KEY::LSHIFT))	// @@@ ȸ�� �߰�.
	{
		StateChange(PlayerState::Dodge);
		return;
	}

	// ����
	if (KEY_TAP(KEY::LBTN))
	{
		StateChange(PlayerState::Attack);
		return;
	}

	// �̵�Ű�� �ȴ�����, �ִϸ��̼��� ������ ����Ǹ� Idle ���·�
	if (false == IsMoveKey() &&
		true == Animator2D()->IsEndAnimation())
	{
		StateChange(PlayerState::Idle);
		return;
	}
	else if (false == IsMoveKey() &&
		false == Animator2D()->IsEndAnimation())
	{
		Vector2 MoveDir = Vector2{ 0.f,0.f };

		if (m_CurDir == PlayerDir::Left)
		{
			MoveDir = Vector2{ -1.f,0.f };
		}

		else if (m_CurDir == PlayerDir::Right)
		{
			MoveDir = Vector2{ 1.f,0.f };
		}

		MapCollisionCheckMoveGround();
	}

	// ���ߴ��߿� �ٽ� �̵�Ű�� ������
	if (true == IsMoveKey())
	{
		StateChange(PlayerState::IdleToRun);
		return;
	}

}

void CPlayerScript::JumpUpdate()
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	Vec2 m_PosYReverse = Vec2(m_Pos3.x, -m_Pos3.y);
	CTexture* m_MapColTexture = GetOwner()->GetColMapTexture();


	// ����Ű�� ���� ������ �� ���� �ڴ�.
	m_StateTime[static_cast<int>(PlayerState::Jump)] += DT;

	if (m_StateTime[static_cast<int>(PlayerState::Jump)] > 0.f &&
		m_StateTime[static_cast<int>(PlayerState::Jump)] <= 0.15f &&
		KEY_PRESSED(KEY::SPACE))
	{
		m_MoveDir += Vector2{ 0.f, 1.f } *DT * m_LongJumpPower;
	}


	m_MoveDir += Vector2{ 0.f, -1.f } *DT * 1500.f;

	// y�̵����� �������Ϸ� �������� Fall ���·�
	Vector2 TempY = { 0.f ,m_MoveDir.y };
	if (30.0f >= TempY.Length())
	{
		m_MoveDir.y = 0;
		//MoveDir.Normal2D();
		StateChange(PlayerState::Fall);
		return;
	}

	// ����
	if (KEY_TAP(KEY::LBTN))
	{
		StateChange(PlayerState::Attack);
		return;
	}


	if (KEY_TAP(KEY::S))
	{
		StateChange(PlayerState::Fall);
		return;
	}

	// ���߿��� Ű������ �ش�������� ���ӵ�
	if (KEY_PRESSED(KEY::A))
	{
		m_MoveDir += Vector2{ -1.f, 0.f } *DT * 2000.f;

		// ���߿��� �ִ� ���ӵ� ����
		Vector2 TempX = { m_MoveDir.x,0.f };
		if (TempX.Length() >= 450.f)
		{
			TempX.Normalize();
			TempX *= 450.f;
			m_MoveDir.x = TempX.x;
		}
	}
	if (KEY_PRESSED(KEY::D))
	{
		m_MoveDir += Vector2{ 1.f, 0.f } *DT * 2000.f;
		Vector2 TempX = { m_MoveDir.x,0.f };

		if (TempX.Length() >= 450.f)
		{
			TempX.Normalize();
			TempX *= 450.f;
			m_MoveDir.x = TempX.x;
		}
	}

	// ���� ���� �����ʰ� ���� �ε������ wallgrab
	int Color = m_MapColTexture->GetPixelColor(m_PosYReverse + Vector2{ 0.f,1.f });
	int LCColor = m_MapColTexture->GetPixelColor(m_PosYReverse + Vector2{ -20,-35 });
	int RCColor = m_MapColTexture->GetPixelColor(m_PosYReverse + Vector2{ 20,-35 });

	if (Color != RGB(0, 0, 0))
	{
		if (LCColor == RGB(255, 0, 255))
		{
			m_CurDir = PlayerDir::Left;
			m_Gravity = 10.0f;
			StateChange(PlayerState::WallGrab);
			return;
		}
		else if (RCColor == RGB(255, 0, 255))
		{
			m_CurDir = PlayerDir::Right;
			m_Gravity = 10.0f;
			StateChange(PlayerState::WallGrab);
			return;
		}
	}



	// ���� ���� ���� ��� ����
	Color = m_MapColTexture->GetPixelColor(m_PosYReverse + Vector2{ 0,1 });
	if (RGB(0, 0, 0) == Color)
	{
		m_Gravity = 10.0f;
		m_MoveDir.Normalize();

		//Effect_LandCloud* NewEffect = GetLevel()->CreateActor<Effect_LandCloud>((int)ORDER::UI);
		//NewEffect->SetPosition(GetPosition());

		StateChange(PlayerState::Landing);
		return;
	}


	MapCollisionCheckMoveAir();

}

void CPlayerScript::LandingUpdate()
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	Vec2 m_PosYReverse = Vec2(m_Pos3.x, -m_Pos3.y);
	CTexture* m_MapColTexture = GetOwner()->GetColMapTexture();

	// �浹�� �̹��� ���� ����׿�
	//int temp = m_MapColTexture->GetPixelColor(Vec2(1785, 254));

	if (true == Animator2D()->IsEndAnimation())
	{
		StateChange(PlayerState::Idle);
		return;
	}

	// �̵�Ű�� ������ IdleToRun ���·�
	if (true == IsMoveKey())
	{
		StateChange(PlayerState::IdleToRun);
		return;
	}

	// �Ʒ��ʿ� ������ ���ٸ� Fall���·�
	int color[5] = {};
	for (int i = 1; i <= 5; ++i)
	{
		color[i - 1] = m_MapColTexture->GetPixelColor(m_PosYReverse + Vector2{ 0.f,(float)i });
	}
	if (color[0] != RGB(0, 0, 0) && color[0] != RGB(255, 0, 0) &&
		color[1] != RGB(0, 0, 0) &&
		color[2] != RGB(0, 0, 0) &&
		color[3] != RGB(0, 0, 0) &&
		color[4] != RGB(0, 0, 0) &&
		m_CurState != PlayerState::Jump)
	{
		StateChange(PlayerState::Fall);
		return;
	}

	// �浹�� �������̸� �Ʒ��� �̵� ����
	if (color[0] == RGB(255, 0, 0) &&
		KEY_TAP(KEY::S))
	{
		SetPos(m_Pos + Vector2{ 0, -4 });
	}

	// ȸ��Ű�� ������ Dodge ���·�
	if (KEY_TAP(KEY::LSHIFT))	// @@@ ȸ�� �߰�.
	{
		StateChange(PlayerState::Dodge);
		return;
	}

	// ����
	if (KEY_TAP(KEY::LBTN))
	{
		StateChange(PlayerState::Attack);
		return;
	}


	// ����Ű�� ������ Jump ���·�
	if (KEY_TAP(KEY::SPACE))
	{
		StateChange(PlayerState::Jump);
		return;
	}



}

void CPlayerScript::AttackUpdate()
{
	//// �÷��̾� ������ ������ �÷��̾� ���� �浹ü ����

	// ���� ������ Fall ���·�
	if (true == Animator2D()->IsEndAnimation())
	{
		StateChange(PlayerState::Fall);
		return;
	}

	MapCollisionCheckMoveAir();
}

void CPlayerScript::FallUpdate()
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	Vec2 m_PosYReverse = Vec2(m_Pos3.x, -m_Pos3.y);
	CTexture* m_MapColTexture = GetOwner()->GetColMapTexture();


	// ���� ���� �����ʰ� ��ȫ���� �ε������ wallgrab
	int Color = m_MapColTexture->GetPixelColor(m_PosYReverse + Vector2{ 0,1 });
	int LCColor = m_MapColTexture->GetPixelColor(m_PosYReverse + Vector2{ -20,-35 });
	int RCColor = m_MapColTexture->GetPixelColor(m_PosYReverse + Vector2{ 20,-35 });

	if (Color != RGB(0, 0, 0))
	{
		if (LCColor == RGB(255, 0, 255))
		{
			m_CurDir = PlayerDir::Left;
			m_Gravity = 10.0f;
			StateChange(PlayerState::WallGrab);
			return;
		}
		else if (RCColor == RGB(255, 0, 255))
		{
			m_CurDir = PlayerDir::Right;
			m_Gravity = 10.0f;
			StateChange(PlayerState::WallGrab);
			return;
		}
	}


	// ���� ������� �������·�
	// ���߿� �� �����ϰ�� �߷¿����� �޴´�.
	// �߷� ���ӵ��� ���� ���� �ӵ�.
	{
		// �������ǿ��� ���ϴ� ��ġ�� �ȼ��� ������ ���� �� �ִ�.
		int Color = m_MapColTexture->GetPixelColor(m_PosYReverse + Vector2{ 0.f,1.f });
		m_Gravity += m_GravityAccel * DT;
		if (RGB(0, 0, 0) == Color || RGB(255, 0, 0) == Color)	// ���� ���� ��� 
		{
			m_Gravity = 10.0f;
			m_MoveDir.Normalize();


			StateChange(PlayerState::Landing);
			return;
		}
		MoveValue(Vector2{ 0.f, -1.f } *m_Gravity * DT);
	}



	// ����
	if (KEY_TAP(KEY::LBTN))
	{
		StateChange(PlayerState::Attack);
		return;
	}

	if (KEY_PRESSED(KEY::A))
	{
		m_MoveDir += Vector2{ -1.f, 0.f } *DT * 2000.f;
		Vector2 TempX = { m_MoveDir.x, 0.f };

		if (TempX.Length() >= 450.f)
		{
			TempX.Normalize();
			TempX *= 450.f;
			m_MoveDir.x = TempX.x;
		}
	}
	if (KEY_PRESSED(KEY::D))
	{
		m_MoveDir += Vector2{ 1.f, 0.f } *DT * 2000.f;
		Vector2 TempX = { m_MoveDir.x, 0.f };

		if (TempX.Length() >= 450.f)
		{
			TempX.Normalize();
			TempX *= 450.f;
			m_MoveDir.x = TempX.x;
		}
	}

	if (KEY_PRESSED(KEY::S))
	{
		m_MoveDir += Vector2{ 0.f , -1.f } *DT * 4000;
	}

	MapCollisionCheckMoveAir();

}

void CPlayerScript::DodgeUpdate()
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	Vec2 m_PosYReverse = Vec2(m_Pos3.x, -m_Pos3.y);
	CTexture* m_MapColTexture = GetOwner()->GetColMapTexture();


	// ���� ����Ʈ ����
	m_StateTime[(UINT)PlayerState::Dodge] += DT;
	if (0.01f <= m_StateTime[(UINT)PlayerState::Dodge])
	{
		m_StateTime[static_cast<int>(PlayerState::Dodge)] = 0.f;

		CGameObject* pDustCloud = new CGameObject;
		pDustCloud->SetName(L"DustCloud");
		pDustCloud->AddComponent(new CTransform);
		pDustCloud->AddComponent(new CMeshRender);
		pDustCloud->AddComponent(new CAnimator2D);
		pDustCloud->AddComponent(new CDustCloudScript);

		pDustCloud->Transform()->SetRelativeScale(38.f, 38.f, 1.f);

		pDustCloud->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pDustCloud->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DustCloudMtrl"));

		pDustCloud->Animator2D()->Create_Effect_Animation();
		pDustCloud->Animator2D()->Play(L"texture\\effect\\spr_dustcloud", false);


		SpawnGameObject(pDustCloud, Transform()->GetRelativePos(), L"Default");

		if (m_CurDir == PlayerDir::Right)
			pDustCloud->GetScript<CDustCloudScript>()->SetDir(ObjDir::Left);
		else if (m_CurDir == PlayerDir::Left)
			pDustCloud->GetScript<CDustCloudScript>()->SetDir(ObjDir::Right);

	}

	// ���� ����� RunToIdle ���·�
	if (true == Animator2D()->IsEndAnimation())
	{
		StateChange(PlayerState::RunToIdle);
		return;
	}

	// �Ʒ��ʿ� ������ ���ٸ� Fall���·�
	int color[20] = {};
	for (int i = 1; i <= 20; ++i)
	{
		color[i - 1] = m_MapColTexture->GetPixelColor(m_PosYReverse + Vector2{ 0.f,(float)i });
	}
	if (color[0] != RGB(0, 0, 0) && color[0] != RGB(255, 0, 0) &&
		color[1] != RGB(0, 0, 0) &&
		color[2] != RGB(0, 0, 0) &&
		color[3] != RGB(0, 0, 0) &&
		color[4] != RGB(0, 0, 0) &&
		color[5] != RGB(0, 0, 0) &&
		color[6] != RGB(0, 0, 0) &&
		color[7] != RGB(0, 0, 0) &&
		color[8] != RGB(0, 0, 0) &&
		color[9] != RGB(0, 0, 0) &&
		color[10] != RGB(0, 0, 0) &&
		color[11] != RGB(0, 0, 0) &&
		color[12] != RGB(0, 0, 0) &&
		color[13] != RGB(0, 0, 0) &&
		color[14] != RGB(0, 0, 0) &&
		color[15] != RGB(0, 0, 0) &&
		color[16] != RGB(0, 0, 0) &&
		color[17] != RGB(0, 0, 0) &&
		color[18] != RGB(0, 0, 0) &&
		color[19] != RGB(0, 0, 0) &&
		m_CurState != PlayerState::Jump)
	{
		StateChange(PlayerState::Fall);
		return;
	}

	// ����Ű�� ������ Jump ���·�
	if (KEY_TAP(KEY::SPACE))
	{
		StateChange(PlayerState::Jump);
		return;
	}

	// ����
	if (KEY_TAP(KEY::LBTN))
	{
		StateChange(PlayerState::Attack);
		return;
	}

	MapCollisionCheckMoveGround();

}

void CPlayerScript::PlaySongUpdate()
{
}

void CPlayerScript::HurtFlyLoopUpdate()
{
}

void CPlayerScript::HurtGroundUpdate()
{
}


void CPlayerScript::WallGrabUpdate()
{
	// ����Ű�� ������ �ø����·�
	if (KEY_TAP(KEY::SPACE))
	{
		if (m_CurDir == PlayerDir::Left)
			m_CurDir = PlayerDir::Right;
		else if (m_CurDir == PlayerDir::Right)
			m_CurDir = PlayerDir::Left;

		StateChange(PlayerState::Flip);
		return;
	}

	// ����
	if (KEY_TAP(KEY::LBTN))
	{
		StateChange(PlayerState::Attack);
		return;
	}

	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	Vec2 m_PosyReverse = Vec2(m_Pos3.x, -m_Pos3.y);
	CTexture* m_MapColTexture = GetOwner()->GetColMapTexture();


	// ���� ������� �������·�
	// ���߿� �� �����ϰ�� �߷¿����� �޴´�.
	// �߷� ���ӵ��� ���� ���� �ӵ�.
	{
		// �������ǿ��� ���ϴ� ��ġ�� �ȼ��� ������ ���� �� �ִ�.
		int Color = m_MapColTexture->GetPixelColor(m_PosyReverse + Vector2{ 0.f,1.f });
		m_Gravity += (m_GravityAccel / 3.f) * DT;
		if (RGB(0, 0, 0) == Color || RGB(255, 0, 0) == Color)	// ���� ���� ��� 
		{
			m_Gravity = 10.0f;
			m_MoveDir.Normalize();


			StateChange(PlayerState::Landing);
			return;
		}
		MoveValue(Vector2{ 0.f, -1.f } *m_Gravity * DT);
	}

	// ���׷� ���� ����Ʈ ����
	m_StateTime[static_cast<int>(PlayerState::WallGrab)] += DT;
	if (0.02f <= m_StateTime[static_cast<int>(PlayerState::WallGrab)] &&
		m_Move.Length() >= 0.48f)
	{
		CGameObject* pDustCloud = new CGameObject;
		pDustCloud->SetName(L"DustCloud");
		pDustCloud->AddComponent(new CTransform);
		pDustCloud->AddComponent(new CMeshRender);
		pDustCloud->AddComponent(new CAnimator2D);
		pDustCloud->AddComponent(new CDustCloudScript);

		pDustCloud->Transform()->SetRelativeScale(38.f, 38.f, 1.f);

		pDustCloud->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pDustCloud->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DustCloudMtrl"));

		pDustCloud->Animator2D()->Create_Effect_Animation();
		pDustCloud->Animator2D()->Play(L"texture\\effect\\spr_dustcloud", false);

		if (m_CurDir == PlayerDir::Right)
		{
			pDustCloud->GetScript<CDustCloudScript>()->SetDir(ObjDir::Left);
			SpawnGameObject(pDustCloud, Transform()->GetRelativePos() + Vec3(18.f, +65.f, 0.f), L"Default");
		}
		else if (m_CurDir == PlayerDir::Left)
		{
			pDustCloud->GetScript<CDustCloudScript>()->SetDir(ObjDir::Right);
			SpawnGameObject(pDustCloud, Transform()->GetRelativePos() + Vec3(-18.f, +65.f, 0.f), L"Default");
		}

		m_StateTime[static_cast<int>(PlayerState::WallGrab)] = 0.f;
	}


	// ���� �����ʰ� ���� �Ⱥε������ Fall
	int Color = m_MapColTexture->GetPixelColor(m_PosyReverse + Vector2{ 0,1 });
	int LCColor = m_MapColTexture->GetPixelColor(m_PosyReverse + Vector2{ -20,-35 });
	int RCColor = m_MapColTexture->GetPixelColor(m_PosyReverse + Vector2{ 20,-35 });

	if (Color != RGB(0, 0, 0))
	{
		if (LCColor != RGB(255, 0, 255) &&
			RCColor != RGB(255, 0, 255))
		{
			//m_Gravity = 10.0f;
			StateChange(PlayerState::Fall);
			return;
		}
	}
	MapCollisionCheckMoveAir();

}



void CPlayerScript::FlipUpdate()
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	Vec2 m_PosyReverse = Vec2(m_Pos3.x, -m_Pos3.y);
	CTexture* m_MapColTexture = GetOwner()->GetColMapTexture();


	m_StateTime[(int)PlayerState::Flip] += DT;

	// ����
	if (KEY_TAP(KEY::LBTN))
	{
		StateChange(PlayerState::Attack);
		return;
	}

	// ���� ���� �����ʰ� ���� �ε������ Flip
	int Color = m_MapColTexture->GetPixelColor(m_PosyReverse + Vector2{ 0,1 });
	int LCColor = m_MapColTexture->GetPixelColor(m_PosyReverse + Vector2{ -20,-35 });
	int RCColor = m_MapColTexture->GetPixelColor(m_PosyReverse + Vector2{ 20,-35 });

	if (m_StateTime[(int)PlayerState::Flip] >= 0.1f)
	{
		if (Color != RGB(0, 0, 0) &&
			LCColor == RGB(255, 0, 255) ||
			RCColor == RGB(255, 0, 255))
		{
			m_Gravity = 10.0f;

			StateChange(PlayerState::WallGrab);
			return;
		}
	}

	// ���� ������� �������·�
	// ���߿� �� �����ϰ�� �߷¿����� �޴´�.
	// �߷� ���ӵ��� ���� ���� �ӵ�.
	{
		// �������ǿ��� ���ϴ� ��ġ�� �ȼ��� ������ ���� �� �ִ�.
		int Color = m_MapColTexture->GetPixelColor(m_PosyReverse + Vector2{ 0.f,1.f });
		m_Gravity += m_GravityAccel * DT;
		if (RGB(0, 0, 0) == Color || RGB(255, 0, 0) == Color)	// ���� ���� ��� 
		{
			m_Gravity = 10.0f;
			m_MoveDir.Normalize();


			StateChange(PlayerState::Landing);
			return;
		}
		MoveValue(Vector2{ 0.f, -1.f } *m_Gravity * DT);
	}

	// �ִϸ��̼� ������ Fall���·�
	if (true == Animator2D()->IsEndAnimation())
	{
		StateChange(PlayerState::Fall);
		return;
	}

	MapCollisionCheckMoveAir();
}

void CPlayerScript::DeadUpdate()
{
}

// State Start
void CPlayerScript::IdleStart()
{
	Animator2D()->Play(L"texture\\player\\spr_idle", true);
	m_MoveSpeed = 0.f;

	SetSize2x();
}

void CPlayerScript::IdleToRunStart()
{
	Animator2D()->Play(L"texture\\player\\spr_idle_to_run", true);
	m_MoveSpeed = 200.f;

	SetSize2x();

}

void CPlayerScript::RunStart()
{
	Animator2D()->Play(L"texture\\player\\spr_run", true);
	m_MoveSpeed = 450.f;
	SetSize2x();

	// �� ��ŸƮ ���� ����Ʈ �߰�
	for (int i = 0; i < 7; ++i)
	{
		CGameObject* pDustCloud = new CGameObject;
		pDustCloud->SetName(L"DustCloud");
		pDustCloud->AddComponent(new CTransform);
		pDustCloud->AddComponent(new CMeshRender);
		pDustCloud->AddComponent(new CAnimator2D);
		pDustCloud->AddComponent(new CDustCloudScript);

		pDustCloud->Transform()->SetRelativeScale(38.f, 38.f, 1.f);

		pDustCloud->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pDustCloud->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"DustCloudMtrl"));

		pDustCloud->Animator2D()->Create_Effect_Animation();
		pDustCloud->Animator2D()->Play(L"texture\\effect\\spr_dustcloud", false);
		

		SpawnGameObject(pDustCloud, Transform()->GetRelativePos(), L"Default");

		if (m_CurDir == PlayerDir::Right)
			pDustCloud->GetScript<CDustCloudScript>()->SetDir(ObjDir::Left);
		else if (m_CurDir == PlayerDir::Left)
			pDustCloud->GetScript<CDustCloudScript>()->SetDir(ObjDir::Right);
	}
	 
	// �� ��ŸƮ ���� �߰�
	Ptr<CSound> pRunSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_player_prerun.wav");
	pRunSound->Play(1, 0.35f, true);


	m_bRun1SoundOn = false;
	m_bRun1SoundOn = false;

}

void CPlayerScript::RunToIdleStart()
{
	Animator2D()->Play(L"texture\\player\\spr_run_to_idle", true);
	m_MoveSpeed = 100.f;

	SetSize2x();
}

void CPlayerScript::JumpStart()
{
	// ���� ����Ʈ �߰�
	CGameObject* pJumpCloud = new CGameObject;
	pJumpCloud->SetName(L"JumpCloud");
	pJumpCloud->AddComponent(new CTransform);
	pJumpCloud->AddComponent(new CMeshRender);
	pJumpCloud->AddComponent(new CAnimator2D);
	pJumpCloud->AddComponent(new CJumpCloudScript);

	pJumpCloud->Transform()->SetRelativeScale(64.f, 102.f, 1.f);

	pJumpCloud->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_Pivot"));
	pJumpCloud->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"JumpCloudMtrl"));

	pJumpCloud->Animator2D()->Create_Effect_Animation();
	pJumpCloud->Animator2D()->Play(L"texture\\effect\\spr_jumpcloud", false);

	SpawnGameObject(pJumpCloud, Transform()->GetRelativePos(), L"Default");
	
	// ���� ���� �߰�
	Ptr<CSound> pJumpSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_player_jump.wav");
	pJumpSound->Play(1, 0.65f, true);

	Animator2D()->Play(L"texture\\player\\spr_jump", true);
	SetSize2x();

	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Transform()->SetRelativePos(m_Pos3.x, m_Pos3.y + 4.f, m_Pos3.z);

	m_MoveDir *= m_MoveSpeed;
	m_MoveDir += Vec2(0.f, 1.f) * m_JumpPower;

	m_StateTime[(UINT)PlayerState::Jump] = 0.f;
	m_IsLongJump = false;

}

void CPlayerScript::LandingStart()
{
	{
		// ���� ����Ʈ �߰�
		CGameObject* pLandCloud = new CGameObject;
		pLandCloud->SetName(L"LandCloud");
		pLandCloud->AddComponent(new CTransform);
		pLandCloud->AddComponent(new CMeshRender);
		pLandCloud->AddComponent(new CAnimator2D);
		pLandCloud->AddComponent(new CLandCloudScript);

		pLandCloud->Transform()->SetRelativeScale(100.f, 28.f, 1.f);

		pLandCloud->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_Pivot"));
		pLandCloud->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"LandCloudMtrl"));

		pLandCloud->Animator2D()->Create_Effect_Animation();
		pLandCloud->Animator2D()->Play(L"texture\\effect\\spr_landcloud", false);

		SpawnGameObject(pLandCloud, Transform()->GetRelativePos(), L"Default");
	}

	
	// ���� ���� �߰�
	Ptr<CSound> pLandingSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_player_land.wav");
	pLandingSound->Play(1, 1.f, true);


	Animator2D()->Play(L"texture\\player\\spr_landing", true);
	SetSize2x();
	m_MoveDir = Vec2(0.f, 0.f);
	m_MoveSpeed = 0.f;
	m_AttackCount = 0;

}

void CPlayerScript::AttackStart()
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	Vec2 m_PosYReverse = Vec2(m_Pos3.x, -m_Pos3.y);
	CTexture* m_MapColTexture = GetOwner()->GetColMapTexture();
	CLevel* CurLevel = CLevelMgr::GetInst()->GetCurLevel();

	Vec3 MousePos = CurLevel->FindParentObjectByName(L"Mouse")->Transform()->GetRelativePos();
	Vec3 MainCamPos = CurLevel->FindParentObjectByName(L"MainCamera")->Transform()->GetRelativePos();
	Vec3 MouseWorldPos3 = MousePos + MainCamPos;
	Vec2 MouseWorldPos = Vec2(MouseWorldPos3.x, MouseWorldPos3.y);

	// ���� ���� �߰�
	std::random_device rd;  // �õ尪�� ��� ���� random_device ����.
	std::mt19937 gen(rd());  // random_device �� ���� ���� ���� ������ �ʱ�ȭ �Ѵ�.
	std::uniform_int_distribution<int> IntRange(0, 2);  // 0 ���� 99 ���� �յ��ϰ� ��Ÿ���� �������� �����ϱ� ���� �յ� ���� ����.
	int Num = IntRange(gen);

	// ���� ���� ���� ���
	if (Num == 0)
	{
		Ptr<CSound> pAttackSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_player_slash_1.wav");
		pAttackSound->Play(1, 0.9f, true);
	}
	else if (Num == 1)
	{
		Ptr<CSound> pAttackSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_player_slash_2.wav");
		pAttackSound->Play(1, 0.9f, true);
	}
	else if (Num == 2)
	{
		Ptr<CSound> pAttackSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_player_slash_3.wav");
		pAttackSound->Play(1, 0.9f, true);
	}


	// ���� ����Ʈ �߰�
	{
		CGameObject* pAttackSlash = new CGameObject;
		pAttackSlash->SetName(L"Slash");
		pAttackSlash->AddComponent(new CTransform);
		pAttackSlash->AddComponent(new CMeshRender);
		pAttackSlash->AddComponent(new CAnimator2D);
		pAttackSlash->AddComponent(new CCollider2D);
		pAttackSlash->AddComponent(new CSlashScript);

		pAttackSlash->Transform()->SetRelativeScale(212.f, 64.f, 1.f);

		pAttackSlash->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pAttackSlash->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"SlashMtrl"));

		pAttackSlash->Animator2D()->Create_Effect_Animation();
		pAttackSlash->Animator2D()->Play(L"texture\\effect\\spr_slash", false);

		SpawnGameObject(pAttackSlash, Transform()->GetRelativePos(), L"PlayerProjectile");
	}

	// ���� ������ ���콺 ���� ����
	if (MouseWorldPos.x >= (m_Pos + Vector2{ 0.f, -35.f }).x)
	{
		m_CurDir = PlayerDir::Right;
	}
	else if (MouseWorldPos.x < (m_Pos + Vector2{ 0.f, -35.f }).x)
	{
		m_CurDir = PlayerDir::Left;
	}

	Animator2D()->Play(L"texture\\player\\spr_attack", true);
	SetSize2x();

	// �÷��̾�->���콺 ���� ���� ȹ��
	Vector2 AttackDir = MouseWorldPos - (m_Pos + Vector2{ 0, 35 });
	AttackDir.Normalize();

	// ���ݹ��� ����.
	m_AttackDir = AttackDir;

	// ���� ���� �浹ü �߰� (SlashScript�� �߰�)

	m_MoveDir = Vector2{ 0.f, 0.f };
	// ���߿��� ���� �ѹ��� �����϶��� y�� �������� �ο��Ѵ�.
	if (m_AttackCount <= 0)
	{
		m_MoveDir = AttackDir * 540.f;
		++m_AttackCount;
	}
	else if (m_AttackCount >= 1)
	{
		// �÷��̾�� 2ȸ �������� y�� �̵� ����, ���� ���� ���� ������
		if (AttackDir.y > 0)
		{
			m_MoveDir = Vector2{ AttackDir.x, 0.f } *540.f;
		}
		else
		{
			m_MoveDir = Vector2{ AttackDir.x, AttackDir.y } *540.f;
		}
	}
	m_Gravity = 10.f;	// ���� �� �߷� �ʱ�ȭ


}

void CPlayerScript::FallStart()
{
	Animator2D()->Play(L"texture\\player\\spr_fall", true);
	SetSize2x();


}

void CPlayerScript::DodgeStart()
{
	// ���� ����
	{
		Ptr<CSound> pDodgeSound_Real = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_player_roll_real.wav");
		pDodgeSound_Real->Play(1, 0.74f, true);

		Ptr<CSound> pDodgeSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\sound_player_roll.wav");
		pDodgeSound->Play(1, 0.4f, true);
	}


	m_StateTime[(UINT)PlayerState::Dodge] = 0.f;


	// ���� �� ������ ���⺤�Ͱ� 0�� �Ǵ¼����� �������ҽ� ���ڸ����� ������ ����.
	if (m_CurDir == PlayerDir::Right)
		m_MoveDir = Vec2{ 1.f, 0.f };
	else if (m_CurDir == PlayerDir::Left)
		m_MoveDir = Vec2{ -1.f, 0.f };


	Animator2D()->Play(L"texture\\player\\spr_roll", true);
	m_MoveSpeed = 680.f;
	SetSize2x();

}

void CPlayerScript::PlaySongStart()
{
	Animator2D()->Play(L"texture\\player\\spr_player_playsong", true);


	SetSize2x();

}

void CPlayerScript::HurtFlyLoopStart()
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);

	// �� ���� ���⿡ ���� ���ư� �غ�
	m_MoveDir = m_EnemyAttackDir;

	if (m_EnemyAttackDir.x > 0)
	{
		m_CurDir = PlayerDir::Left;
	}
	else if (m_EnemyAttackDir.x < 0)
	{
		m_CurDir = PlayerDir::Right;
	}

	SetPos(m_Pos + Vector2{ 0.f, 3.f });

	// ���� ������ ���������� ����� ���� (SlashScript �� ����)

	// ��Ʈ�� ȭ�� ��鸲

	// ��� �Ҹ�
	Ptr<CSound> pDeadSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\dead.wav");
	pDeadSound->Play(1, 1.f, true);

	Animator2D()->Play(L"texture\\player\\spr_hurtfly_begin", true);
	SetSize2x();
}

void CPlayerScript::HurtGroundStart()
{
	Animator2D()->Play(L"texture\\player\\spr_hurtground", true);
	SetSize2x();

	m_MoveSpeed = 0.f;

}

void CPlayerScript::WallGrabStart()
{



	// �׷� �� ���� ���
	Ptr<CSound> pWallGrabSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\grabwall.wav");
	pWallGrabSound->Play(1, 1.f, true);


	m_StateTime[static_cast<int>(PlayerState::WallGrab)] = 0.f;
	Animator2D()->Play(L"texture\\player\\spr_wallgrab", true);
	SetSize2x();


	m_MoveDir /= 2.f;
	m_AttackCount = 0;
}

void CPlayerScript::FlipStart()
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);



	// ������ ����
	Ptr<CSound> pWallJumpSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\walljump.wav");
	pWallJumpSound->Play(1, 1.f, true);

	// �ø��� �߷°� �ʱ�ȭ
	m_Gravity = 10.f;

	m_StateTime[static_cast<int>(PlayerState::Flip)] = 0.f;
	if (m_CurDir == PlayerDir::Left)
	{
		SetPos(m_Pos + Vector2{ -3.f, 0.f });
		m_CurDir = PlayerDir::Left;
	}
	else if (m_CurDir == PlayerDir::Right)
	{
		SetPos(m_Pos + Vector2{ 3.f, 0.f });
		m_CurDir = PlayerDir::Right;
	}
	Animator2D()->Play(L"texture\\player\\spr_player_flip", true);
	SetSize2x();


	if (m_CurDir == PlayerDir::Right)
		m_MoveDir = Vector2{ 1.f, 0.7f } * 750;
	else if (m_CurDir == PlayerDir::Left)
		m_MoveDir = Vector2{ -1.f, 0.7f } * 750;



}

void CPlayerScript::DeadStart()
{
	SetSize2x();

}


void CPlayerScript::MapCollisionCheckMoveGround()
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	Vec2 m_PosyReverse = Vec2(m_Pos3.x, -m_Pos3.y);
	CTexture* m_MapColTexture = GetOwner()->GetColMapTexture();

	{
		// �̷��� ��ġ�� ����Ͽ� �װ��� RGB���� üũ�ϰ�, �̵� ������ ���̸� �̵��Ѵ�.
		Vector2 NextPos = m_PosyReverse + (Vec2{ 0.f,-m_MoveDir.y } *DT * m_MoveSpeed);
		Vector2 CheckPos = NextPos + Vector2{ 0,0 };	// �̷� ��ġ�� �߱��� ����
		Vector2 CheckPosTopRight = NextPos + Vector2{ 18,-70 };	// �̷� ��ġ�� �Ӹ����� ����
		Vector2 CheckPosTopLeft = NextPos + Vector2{ -18,-70 };	// �̷� ��ġ�� �Ӹ����� ����

		int Color = m_MapColTexture->GetPixelColor(CheckPos);
		int TopRightColor = m_MapColTexture->GetPixelColor(CheckPosTopRight);
		int TopLeftColor = m_MapColTexture->GetPixelColor(CheckPosTopLeft);

		if (RGB(0, 0, 0) != Color &&
			RGB(0, 0, 0) != TopRightColor &&
			RGB(0, 0, 0) != TopLeftColor &&
			RGB(255, 0, 255) != Color &&
			RGB(255, 0, 255) != TopRightColor &&
			RGB(255, 0, 255) != TopLeftColor)
		{
			MoveDir(Vector2{ 0.f, m_MoveDir.y });
		}
	}

	{
		// �̷��� ��ġ�� ����Ͽ� �װ��� RGB���� üũ�ϰ�, �̵� ������ ���̸� �̵��Ѵ�.
		Vector2 NextPos = m_PosyReverse + (Vec2{ m_MoveDir.x,0.f } *DT * m_MoveSpeed);
		Vector2 CheckPos = NextPos + Vec2{ 0.f,0.f };	// �̷� ��ġ�� �߱��� ����
		Vector2 CheckPosTopRight = NextPos + Vec2{ 18,-70 };	// �̷� ��ġ�� �Ӹ����� ����
		Vector2 CheckPosTopLeft = NextPos + Vec2{ -18,-70 };	// �̷� ��ġ�� �Ӹ����� ����
		Vector2 ForDownPos = m_PosyReverse + Vec2{ 0.f, 1.f };	// �� �Ʒ� ����

		int CurColor = m_MapColTexture->GetPixelColor(m_PosyReverse);
		int ForDownColor = m_MapColTexture->GetPixelColor(ForDownPos);	// �� �Ʒ� ����
		int Color = m_MapColTexture->GetPixelColor(CheckPos);	// �̷� ��ġ�� �߱��� ����
		int TopRightColor = m_MapColTexture->GetPixelColor(CheckPosTopRight);
		int TopLeftColor = m_MapColTexture->GetPixelColor(CheckPosTopLeft);


		//// �׻� ���� �پ��ֱ�_���� �ƹ��� �ָ��������־ �ݺ������� ����.
		//while (1)
		//{
		//	if (RGB(0, 0, 0) != ForDownColor && RGB(255, 0, 0) != ForDownColor)
		//	{
		//		SetPos(Vector2{ m_Pos.x, m_Pos.y - 1.f });
		//		m_Pos3 = Transform()->GetRelativePos();
		//		m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
		//		ForDownPos.y += 1.f;
		//		ForDownColor = m_MapColTexture->GetPixelColor(ForDownPos);
		//	}
		//	else
		//		break;
		//}

		// �׻� �������� �߷�
		if (RGB(0, 0, 0) != ForDownColor && RGB(255, 0, 0) != ForDownColor)
		{
			SetPos(Vector2{ m_Pos.x, m_Pos.y - 3.f });
		}


		// ��� �ö󰡱�
		while (RGB(0, 0, 0) == Color &&
			TopRightColor != RGB(0, 0, 0) && TopLeftColor != RGB(0, 0, 0))
		{
			CheckPos.y -= 1.0f;
			Color = m_MapColTexture->GetPixelColor(CheckPos);
			SetPos(Vector2{ m_Pos.x, m_Pos.y + 1.0f });
			m_Pos3 = Transform()->GetRelativePos();
			m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
			ForDownPos.y += 1.f;

		}


		if (RGB(0, 0, 0) != Color &&
			RGB(0, 0, 0) != TopRightColor &&
			RGB(0, 0, 0) != TopLeftColor &&
			RGB(255, 0, 255) != Color &&
			RGB(255, 0, 255) != TopRightColor &&
			RGB(255, 0, 255) != TopLeftColor)
		{
			MoveDir(Vector2{ m_MoveDir.x,0.f });
		}
	}


}

void CPlayerScript::MapCollisionCheckMoveAir()
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	Vec2 m_PosyReverse = Vec2(m_Pos3.x, -m_Pos3.y);
	CTexture* m_MapColTexture = GetOwner()->GetColMapTexture();

	{
		// �̷��� ��ġ�� ����Ͽ� �װ��� RGB���� üũ�ϰ�, �̵� ������ ���̸� �̵��Ѵ�.
		Vector2 NextPos = m_PosyReverse + (Vector2{ 0.f, -m_MoveDir.y } *DT);
		//Vector2 NextPos = m_PosyReverse + (m_MoveDir * DT);
		Vector2 CheckPos = NextPos + Vector2{ 0,0 };	// �̷� ��ġ�� �߱��� ����
		Vector2 CheckPosTopRight = NextPos + Vector2{ 18,-70 };	// �̷� ��ġ�� �Ӹ����� ����
		Vector2 CheckPosTopLeft = NextPos + Vector2{ -18,-70 };	// �̷� ��ġ�� �Ӹ����� ����
		Vector2 CheckPosCenterRight = NextPos + Vector2{ 18,-20 };	// ���߾� ����
		Vector2 CheckPosCenterLeft = NextPos + Vector2{ -18,-20 };	// ���߾� ����

		int Color = m_MapColTexture->GetPixelColor(CheckPos);
		int TopRightColor = m_MapColTexture->GetPixelColor(CheckPosTopRight);
		int TopLeftColor = m_MapColTexture->GetPixelColor(CheckPosTopLeft);
		int CenterRightColor = m_MapColTexture->GetPixelColor(CheckPosCenterRight);
		int CenterLeftColor = m_MapColTexture->GetPixelColor(CheckPosCenterLeft);


		if (RGB(0, 0, 0) != Color &&
			RGB(0, 0, 0) != TopRightColor &&
			RGB(0, 0, 0) != TopLeftColor &&
			RGB(0, 0, 0) != CenterRightColor &&
			RGB(0, 0, 0) != CenterLeftColor &&
			RGB(255, 0, 255) != Color &&
			RGB(255, 0, 255) != TopRightColor &&
			RGB(255, 0, 255) != TopLeftColor &&
			RGB(255, 0, 255) != CenterRightColor &&
			RGB(255, 0, 255) != CenterLeftColor)
		{
			MoveValue(Vector2(0.f, m_MoveDir.y) * DT);
		}
	}

	{
		// �̷��� ��ġ�� ����Ͽ� �װ��� RGB���� üũ�ϰ�, �̵� ������ ���̸� �̵��Ѵ�.
		Vector2 NextPos = m_PosyReverse + (Vector2{ m_MoveDir.x,0.f } *DT);
		Vector2 CheckPos = NextPos + Vector2{ 0.f, 0.f };	// �̷� ��ġ�� �߱��� ����
		Vector2 CheckPosTopRight = NextPos + Vector2{ 18,-70 };	// �̷� ��ġ�� �Ӹ����� ����
		Vector2 CheckPosTopLeft = NextPos + Vector2{ -18,-70 };	// �̷� ��ġ�� �Ӹ����� ����
		Vector2 CheckPosCenterRight = NextPos + Vector2{ 18,-20 };	// ���߾� ����
		Vector2 CheckPosCenterLeft = NextPos + Vector2{ -18,-20 };	// ���߾� ����

		int Color = m_MapColTexture->GetPixelColor(CheckPos);
		int TopRightColor = m_MapColTexture->GetPixelColor(CheckPosTopRight);
		int TopLeftColor = m_MapColTexture->GetPixelColor(CheckPosTopLeft);
		int CenterRightColor = m_MapColTexture->GetPixelColor(CheckPosCenterRight);
		int CenterLeftColor = m_MapColTexture->GetPixelColor(CheckPosCenterLeft);

		if (RGB(0, 0, 0) != Color &&
			RGB(0, 0, 0) != TopRightColor &&
			RGB(0, 0, 0) != TopLeftColor &&
			RGB(0, 0, 0) != CenterRightColor &&
			RGB(0, 0, 0) != CenterLeftColor &&
			RGB(255, 0, 255) != Color &&
			RGB(255, 0, 255) != TopRightColor &&
			RGB(255, 0, 255) != TopLeftColor &&
			RGB(255, 0, 255) != CenterRightColor &&
			RGB(255, 0, 255) != CenterLeftColor)
		{
			MoveValue(Vector2(m_MoveDir.x, 0.f) * DT);
		}
	}


}

void ResetStage_1()
{
	// ����� ����
	Ptr<CSound> pBGM = CLevelMgr::GetInst()->GetCurLevel()->GetBgm();
	pBGM->Stop();

	// ����� ���
	pBGM = CResMgr::GetInst()->FindRes<CSound>(L"sound\\song_youwillneverknow.ogg");
	pBGM->Play(999, 0.9f, true);

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	pCurLevel->SetName(L"Stage_1");
	pCurLevel->ChangeState(LEVEL_STATE::PLAY);
	pCurLevel->SetBgm(pBGM.Get());

	// Layer �̸�����
	pCurLevel->GetLayer(0)->SetName(L"Default");
	pCurLevel->GetLayer(1)->SetName(L"Tile");
	pCurLevel->GetLayer(2)->SetName(L"PlayerHitBox");
	pCurLevel->GetLayer(3)->SetName(L"MonsterHitBox");
	pCurLevel->GetLayer(4)->SetName(L"PlayerProjectile");
	pCurLevel->GetLayer(5)->SetName(L"MonsterProjectile");
	pCurLevel->GetLayer(6)->SetName(L"MonsterView");
	pCurLevel->GetLayer(7)->SetName(L"MonsterAttackRange");
	pCurLevel->GetLayer(31)->SetName(L"ViewPort UI");

	// ī�޶� visible mask �ٽ� all true
	pCurLevel->FindParentObjectByName(L"MainCamera")->Camera()->SetLayerMaskAll(true);

	// ������Ʈ ����
	CGameObject* pParent = new CGameObject;
	pParent->SetName(L"Player");
	pParent->AddComponent(new CTransform);
	pParent->AddComponent(new CMeshRender);
	pParent->AddComponent(new CCollider2D);
	pParent->AddComponent(new CAnimator2D);
	pParent->AddComponent(new CPlayerScript);

	pParent->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));

	pParent->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_Pivot"));
	pParent->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimLightMtrl"));

	Vec2 _Resolution = GlobalData.Resolution;
	pParent->Collider2D()->SetAbsolute(true);
	pParent->Collider2D()->SetOffsetScale(Vec2(36.f, 70.f));
	//pParent->Collider2D()->SetOffsetPos(Vec2( -_Resolution.x/2.f, _Resolution.y/2.f + 35.f));

	Ptr<CTexture> pAnimAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\link.png");
	pParent->Animator2D()->CreateAnimation(L"WalkDown", pAnimAtlas, Vec2(0.f, 520.f), Vec2(120.f, 130.f), Vec2(300.f, 300.f), 10, 16);
	pParent->Animator2D()->Play(L"WalkDown", true);
	pParent->Animator2D()->Create_Player_Animation();
	SpawnGameObject(pParent, Vec3(230.f, -671.f, 500.f), L"PlayerHitBox");

	// Mouse
	CGameObject* pMouse = new CGameObject;
	pMouse->SetName(L"Mouse");

	pMouse->AddComponent(new CTransform);
	pMouse->AddComponent(new CMeshRender);
	pMouse->AddComponent(new CCollider2D);
	pMouse->AddComponent(new CMouseScript);

	pMouse->Transform()->SetRelativePos(Vec3(0.f, 250.f, 100.f));
	pMouse->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));


	pMouse->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMouse->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"MouseDMtrl"));
	pMouse->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\spr_cursor.png"));

	pMouse->Collider2D()->SetAbsolute(true);
	pMouse->Collider2D()->SetOffsetScale(Vec2(35.f, 35.f));

	SpawnGameObject(pMouse, Vec3(0.f, 0.f, 100.f), L"ViewPort UI");
	{
		// Grunt_1	2�� ����
		CGameObject* pGrunt = CreateGrunt();
		pGrunt->GetScript<CGruntScript>()->SetBeginDir(ObjDir::Right);
		pGrunt->GetScript<CGruntScript>()->SetBeginState(ObjState::Walk);
		pGrunt->GetScript<CGruntScript>()->SetPatrol(true, 4.f);
		SpawnGameObject(pGrunt, Vec3(1054, -383, 500.f), L"MonsterHitBox");

		// Grunt_2	2�� ���
		pGrunt = CreateGrunt();
		pGrunt->GetScript<CGruntScript>()->SetBeginDir(ObjDir::Right);
		pGrunt->GetScript<CGruntScript>()->SetBeginState(ObjState::Idle);
		SpawnGameObject(pGrunt, Vec3(338, -383, 500.f), L"MonsterHitBox");
	}
	{
		// ������ 2�� ���
		CGameObject* pGangster = CreateGangster();
		pGangster->GetScript<CGangsterScript>()->SetBeginDir(ObjDir::Left);
		pGangster->GetScript<CGangsterScript>()->SetBeginState(ObjState::Idle);
		SpawnGameObject(pGangster, Vec3(545, -383, 500.f), L"MonsterHitBox");

		// ���� 1��
		CGameObject* pPomp = CreatePomp();
		pPomp->GetScript<CPompScript>()->SetBeginDir(ObjDir::Right);
		pPomp->GetScript<CPompScript>()->SetBeginState(ObjState::Idle);
		SpawnGameObject(pPomp, Vec3(530, -671, 500.f), L"MonsterHitBox");

	}
	// BackGround Object
	CGameObject* pBackGround = new CGameObject;
	pBackGround->SetName(L"BackGround");

	pBackGround->AddComponent(new CTransform);
	pBackGround->AddComponent(new CMeshRender);

	pBackGround->Transform()->SetRelativeScale(Vec3(1800.f, 784.f, 1.f));

	pBackGround->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pBackGround->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	pBackGround->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\map\\room_factory_2.png"));

	SpawnGameObject(pBackGround, Vec3(900.f, -392.f, 1000.f), L"Default");	//��������1 ������ġ

	// UI
	CGameObject* pUI = new CGameObject;
	pUI->SetName(L"UI");

	pUI->AddComponent(new CTransform);
	pUI->AddComponent(new CMeshRender);
	pUI->AddComponent(new CUIScript);

	pUI->Transform()->SetRelativePos(Vec3(GlobalData.Resolution.x / 2.f, GlobalData.Resolution.y / 2.f, 100.f));
	pUI->Transform()->SetRelativeScale(Vec3(1280.f, 46.f, 1.f));

	pUI->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pUI->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UIMtrl"));
	pUI->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\hud_collapse.png"));

	SpawnGameObject(pUI, Vec3(0, 360.f - 42.f, 200.f), L"ViewPort UI");

	// UI_Battery
	CGameObject* pBattery = new CGameObject;
	pBattery->SetName(L"UI_Battery");

	pBattery->AddComponent(new CTransform);
	pBattery->AddComponent(new CMeshRender);
	pBattery->AddComponent(new CBatteryScript);

	pBattery->Transform()->SetRelativeScale(Vec3(110.f, 20.f, 1.f));

	pBattery->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_PivotRight"));
	pBattery->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BatteryMtrl"));
	pBattery->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\effect_battery.png"));

	SpawnGameObject(pBattery, Vec3(-617.f, 318.f, 100.f), L"ViewPort UI");

	// UI_Timer
	CGameObject* pTimer = new CGameObject;
	pTimer->SetName(L"UI_Timer");

	pTimer->AddComponent(new CTransform);
	pTimer->AddComponent(new CMeshRender);
	pTimer->AddComponent(new CTimerScript);

	pTimer->Transform()->SetRelativeScale(Vec3(188.f, 22.f, 1.f));

	pTimer->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_PivotRight"));
	pTimer->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TimerMtrl"));
	pTimer->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\effect_timer.png"));

	SpawnGameObject(pTimer, Vec3(-89.f, 324.f, 100.f), L"ViewPort UI");

	// UI_Katana
	CGameObject* pKatana = new CGameObject;
	pKatana->SetName(L"UI_Katana");

	pKatana->AddComponent(new CTransform);
	pKatana->AddComponent(new CMeshRender);

	pKatana->Transform()->SetRelativeScale(Vec3(40.f, 40.f, 1.f));

	pKatana->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_PivotRight"));
	pKatana->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"KatanaMtrl"));
	pKatana->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\spr_katanaicons.png"));

	SpawnGameObject(pKatana, Vec3(507.f, 318.f, 100.f), L"ViewPort UI");


	// �����δ� �ִϸ��̼�
	CGameObject* pRewinder = new CGameObject;
	pRewinder->SetName(L"Rewinder");
	pRewinder->AddComponent(new CTransform);
	pRewinder->AddComponent(new CMeshRender);
	pRewinder->AddComponent(new CAnimator2D);
	pRewinder->AddComponent(new CRewinderScript);

	pRewinder->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pRewinder->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimLightMtrl"));

	// �ӽ÷� ���
	pRewinder->Animator2D()->Create_Grunt_Animation();
	pRewinder->Animator2D()->Play(L"texture\\grunt\\spr_grunt_idle", true);

	SpawnGameObject(pRewinder, Vec3(0.f, 0.f, 30.f), L"ViewPort UI");


	// �浹 ��ų ���̾� ¦ ����
	CCollisionMgr::GetInst()->LayerCheck(L"PlayerProjectile", L"MonsterHitBox");
	CCollisionMgr::GetInst()->LayerCheck(L"MonsterProjectile", L"PlayerHitBox");
	CCollisionMgr::GetInst()->LayerCheck(L"MonsterProjectile", L"PlayerProjectile");
	CCollisionMgr::GetInst()->LayerCheck(L"MonsterView", L"PlayerHitBox");
	CCollisionMgr::GetInst()->LayerCheck(L"MonsterView", L"MonsterHitBox");
	CCollisionMgr::GetInst()->LayerCheck(L"MonsterAttackRange", L"PlayerHitBox");
}

void ResetStage_2()
{
	// ����� ����
	Ptr<CSound> pBGM = CLevelMgr::GetInst()->GetCurLevel()->GetBgm();
	pBGM->Stop();

	// ����� ���
	pBGM = CResMgr::GetInst()->FindRes<CSound>(L"sound\\bgm_bunker.mp3");
	pBGM->Play(999, 0.9f, true);

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	pCurLevel->SetName(L"Stage_2");
	pCurLevel->ChangeState(LEVEL_STATE::PLAY);
	pCurLevel->SetBgm(pBGM.Get());

	// Layer �̸�����
	pCurLevel->GetLayer(0)->SetName(L"Default");
	pCurLevel->GetLayer(1)->SetName(L"Tile");
	pCurLevel->GetLayer(2)->SetName(L"PlayerHitBox");
	pCurLevel->GetLayer(3)->SetName(L"MonsterHitBox");
	pCurLevel->GetLayer(4)->SetName(L"PlayerProjectile");
	pCurLevel->GetLayer(5)->SetName(L"MonsterProjectile");
	pCurLevel->GetLayer(6)->SetName(L"MonsterView");
	pCurLevel->GetLayer(7)->SetName(L"MonsterAttackRange");
	pCurLevel->GetLayer(31)->SetName(L"ViewPort UI");

	// ī�޶� visible mask �ٽ� all true
	pCurLevel->FindParentObjectByName(L"MainCamera")->Camera()->SetLayerMaskAll(true);

	// ������Ʈ ����
	CGameObject* pParent = new CGameObject;
	pParent->SetName(L"Player");
	pParent->AddComponent(new CTransform);
	pParent->AddComponent(new CMeshRender);
	pParent->AddComponent(new CCollider2D);
	pParent->AddComponent(new CAnimator2D);
	pParent->AddComponent(new CPlayerScript);

	pParent->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));

	pParent->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_Pivot"));
	pParent->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimLightMtrl"));

	Vec2 _Resolution = GlobalData.Resolution;
	pParent->Collider2D()->SetAbsolute(true);
	pParent->Collider2D()->SetOffsetScale(Vec2(36.f, 70.f));
	//pParent->Collider2D()->SetOffsetPos(Vec2( -_Resolution.x/2.f, _Resolution.y/2.f + 35.f));

	Ptr<CTexture> pAnimAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\link.png");
	pParent->Animator2D()->CreateAnimation(L"WalkDown", pAnimAtlas, Vec2(0.f, 520.f), Vec2(120.f, 130.f), Vec2(300.f, 300.f), 10, 16);
	pParent->Animator2D()->Play(L"WalkDown", true);
	pParent->Animator2D()->Create_Player_Animation();
	SpawnGameObject(pParent, Vec3(375.f, -300.f, 500.f), L"PlayerHitBox");

	// Mouse
	CGameObject* pMouse = new CGameObject;
	pMouse->SetName(L"Mouse");

	pMouse->AddComponent(new CTransform);
	pMouse->AddComponent(new CMeshRender);
	pMouse->AddComponent(new CCollider2D);
	pMouse->AddComponent(new CMouseScript);

	pMouse->Transform()->SetRelativePos(Vec3(0.f, 250.f, 100.f));
	pMouse->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));


	pMouse->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMouse->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"MouseDMtrl"));
	pMouse->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\spr_cursor.png"));

	pMouse->Collider2D()->SetAbsolute(true);
	pMouse->Collider2D()->SetOffsetScale(Vec2(35.f, 35.f));

	SpawnGameObject(pMouse, Vec3(0.f, 0.f, 100.f), L"ViewPort UI");

	{
		// �׷�Ʈ	8�� ����
		CGameObject* pGrunt = CreateGrunt();
		pGrunt->GetScript<CGruntScript>()->SetBeginDir(ObjDir::Right);
		pGrunt->GetScript<CGruntScript>()->SetBeginState(ObjState::Walk);
		pGrunt->GetScript<CGruntScript>()->SetPatrol(true, 3.f);
		SpawnGameObject(pGrunt, Vec3(642, -863, 500.f), L"MonsterHitBox");

		// �׷�Ʈ	6�� ����
		pGrunt = CreateGrunt();
		pGrunt->GetScript<CGruntScript>()->SetBeginDir(ObjDir::Left);
		pGrunt->GetScript<CGruntScript>()->SetBeginState(ObjState::Walk);
		pGrunt->GetScript<CGruntScript>()->SetPatrol(true, 2.f);
		SpawnGameObject(pGrunt, Vec3(924, -1311, 500.f), L"MonsterHitBox");

		// �׷�Ʈ	6�� Idle
		pGrunt = CreateGrunt();
		pGrunt->GetScript<CGruntScript>()->SetBeginDir(ObjDir::Right);
		pGrunt->GetScript<CGruntScript>()->SetBeginState(ObjState::Idle);
		pGrunt->GetScript<CGruntScript>()->SetPatrol(false, 4.f);
		SpawnGameObject(pGrunt, Vec3(596, -1311, 500.f), L"MonsterHitBox");

		// �׷�Ʈ	5�� Patrol
		pGrunt = CreateGrunt();
		pGrunt->GetScript<CGruntScript>()->SetBeginDir(ObjDir::Right);
		pGrunt->GetScript<CGruntScript>()->SetBeginState(ObjState::Walk);
		pGrunt->GetScript<CGruntScript>()->SetPatrol(true, 3.f);
		SpawnGameObject(pGrunt, Vec3(871, -1535, 500.f), L"MonsterHitBox");

		// �׷�Ʈ	3�� ��
		pGrunt = CreateGrunt();
		pGrunt->GetScript<CGruntScript>()->SetBeginDir(ObjDir::Left);
		pGrunt->GetScript<CGruntScript>()->SetBeginState(ObjState::Idle);
		pGrunt->GetScript<CGruntScript>()->SetPatrol(false, 3.f);
		SpawnGameObject(pGrunt, Vec3(1220, -1983, 500.f), L"MonsterHitBox");

		// �׷�Ʈ	1�� ��� ����
		pGrunt = CreateGrunt();
		pGrunt->GetScript<CGruntScript>()->SetBeginDir(ObjDir::Left);
		pGrunt->GetScript<CGruntScript>()->SetBeginState(ObjState::Walk);
		pGrunt->GetScript<CGruntScript>()->SetPatrol(true, 5.f);
		SpawnGameObject(pGrunt, Vec3(1155, -2754, 500.f), L"MonsterHitBox");

		// �׷�Ʈ	1�� Idle
		pGrunt = CreateGrunt();
		pGrunt->GetScript<CGruntScript>()->SetBeginDir(ObjDir::Left);
		pGrunt->GetScript<CGruntScript>()->SetBeginState(ObjState::Idle);
		pGrunt->GetScript<CGruntScript>()->SetPatrol(false, 5.f);
		SpawnGameObject(pGrunt, Vec3(1741, -2943, 500.f), L"MonsterHitBox");
	}

	{

		// ������	9�� ������
		CGameObject* CGangster = CreateGangster();
		CGangster->GetScript<CGangsterScript>()->SetBeginDir(ObjDir::Left);
		CGangster->GetScript<CGangsterScript>()->SetBeginState(ObjState::Idle);
		CGangster->GetScript<CGangsterScript>()->SetPatrol(false, 5.f);
		SpawnGameObject(CGangster, Vec3(1212, -639, 500.f), L"MonsterHitBox");

		// ������	8�� ������
		CGangster = CreateGangster();
		CGangster->GetScript<CGangsterScript>()->SetBeginDir(ObjDir::Right);
		CGangster->GetScript<CGangsterScript>()->SetBeginState(ObjState::Idle);
		CGangster->GetScript<CGangsterScript>()->SetPatrol(false, 5.f);
		SpawnGameObject(CGangster, Vec3(574, -863, 500.f), L"MonsterHitBox");

		// ������	7�� ��
		CGangster = CreateGangster();
		CGangster->GetScript<CGangsterScript>()->SetBeginDir(ObjDir::Left);
		CGangster->GetScript<CGangsterScript>()->SetBeginState(ObjState::Idle);
		CGangster->GetScript<CGangsterScript>()->SetPatrol(false, 5.f);
		SpawnGameObject(CGangster, Vec3(1216, -1087, 500.f), L"MonsterHitBox");

		// ������	5�� ��
		CGangster = CreateGangster();
		CGangster->GetScript<CGangsterScript>()->SetBeginDir(ObjDir::Left);
		CGangster->GetScript<CGangsterScript>()->SetBeginState(ObjState::Idle);
		CGangster->GetScript<CGangsterScript>()->SetPatrol(false, 5.f);
		SpawnGameObject(CGangster, Vec3(1220, -1535, 500.f), L"MonsterHitBox");

		// ������	5��
		CGangster = CreateGangster();
		CGangster->GetScript<CGangsterScript>()->SetBeginDir(ObjDir::Right);
		CGangster->GetScript<CGangsterScript>()->SetBeginState(ObjState::Idle);
		CGangster->GetScript<CGangsterScript>()->SetPatrol(false, 5.f);
		SpawnGameObject(CGangster, Vec3(1150, -1535, 500.f), L"MonsterHitBox");

		// ������	4��
		CGangster = CreateGangster();
		CGangster->GetScript<CGangsterScript>()->SetBeginDir(ObjDir::Right);
		CGangster->GetScript<CGangsterScript>()->SetBeginState(ObjState::Idle);
		CGangster->GetScript<CGangsterScript>()->SetPatrol(false, 5.f);
		SpawnGameObject(CGangster, Vec3(575, -1759, 500.f), L"MonsterHitBox");


		// ������	4�� ���
		CGangster = CreateGangster();
		CGangster->GetScript<CGangsterScript>()->SetBeginDir(ObjDir::Left);
		CGangster->GetScript<CGangsterScript>()->SetBeginState(ObjState::Idle);
		CGangster->GetScript<CGangsterScript>()->SetPatrol(false, 5.f);
		SpawnGameObject(CGangster, Vec3(986, -1731, 500.f), L"MonsterHitBox");

		// ������	2�� ��
		CGangster = CreateGangster();
		CGangster->GetScript<CGangsterScript>()->SetBeginDir(ObjDir::Right);
		CGangster->GetScript<CGangsterScript>()->SetBeginState(ObjState::Idle);
		CGangster->GetScript<CGangsterScript>()->SetPatrol(false, 5.f);
		SpawnGameObject(CGangster, Vec3(583, -2207, 500.f), L"MonsterHitBox");

		// ������	1�� ��
		CGangster = CreateGangster();
		CGangster->GetScript<CGangsterScript>()->SetBeginDir(ObjDir::Left);
		CGangster->GetScript<CGangsterScript>()->SetBeginState(ObjState::Idle);
		CGangster->GetScript<CGangsterScript>()->SetPatrol(false, 5.f);
		SpawnGameObject(CGangster, Vec3(1900, -2943, 500.f), L"MonsterHitBox");
	}

	{
		// ����	10��
		CGameObject* pPomp = CreatePomp();
		pPomp->GetScript<CPompScript>()->SetBeginDir(ObjDir::Right);
		pPomp->GetScript<CPompScript>()->SetBeginState(ObjState::Walk);
		pPomp->GetScript<CPompScript>()->SetPatrol(true, 3.f);
		SpawnGameObject(pPomp, Vec3(550, -415, 500.f), L"MonsterHitBox");

		// ����	3��
		pPomp = CreatePomp();
		pPomp->GetScript<CPompScript>()->SetBeginDir(ObjDir::Left);
		pPomp->GetScript<CPompScript>()->SetBeginState(ObjState::Idle);
		pPomp->GetScript<CPompScript>()->SetPatrol(false, 3.f);
		SpawnGameObject(pPomp, Vec3(1120, -1983, 500.f), L"MonsterHitBox");

		// ����	2�� Idle
		pPomp = CreatePomp();
		pPomp->GetScript<CPompScript>()->SetBeginDir(ObjDir::Left);
		pPomp->GetScript<CPompScript>()->SetBeginState(ObjState::Idle);
		pPomp->GetScript<CPompScript>()->SetPatrol(false, 3.f);
		SpawnGameObject(pPomp, Vec3(630, -2207, 500.f), L"MonsterHitBox");

		// ����	1�� Idle
		pPomp = CreatePomp();
		pPomp->GetScript<CPompScript>()->SetBeginDir(ObjDir::Left);
		pPomp->GetScript<CPompScript>()->SetBeginState(ObjState::Idle);
		pPomp->GetScript<CPompScript>()->SetPatrol(false, 3.f);
		SpawnGameObject(pPomp, Vec3(1638, -2943, 500.f), L"MonsterHitBox");
	}

	// BackGround Object
	CGameObject* pBackGround = new CGameObject;
	pBackGround->SetName(L"BackGround");

	pBackGround->AddComponent(new CTransform);
	pBackGround->AddComponent(new CMeshRender);

	pBackGround->Transform()->SetRelativeScale(Vec3(2176.f, 3500.f, 1.f));

	pBackGround->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pBackGround->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	//pBackGround->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\map\\room_factory_2.png"));
	pBackGround->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\map\\stage2_bg_render.png"));

	//SpawnGameObject(pBackGround, Vec3(900.f, -392.f, 1000.f), L"Default");	//��������1 ������ġ
	SpawnGameObject(pBackGround, Vec3(2176.f / 2.f, -3500.f / 2.f, 1000.f), L"Default");	// ��������2 ������ġ

	// UI
	CGameObject* pUI = new CGameObject;
	pUI->SetName(L"UI");

	pUI->AddComponent(new CTransform);
	pUI->AddComponent(new CMeshRender);
	pUI->AddComponent(new CUIScript);

	pUI->Transform()->SetRelativePos(Vec3(GlobalData.Resolution.x / 2.f, GlobalData.Resolution.y / 2.f, 100.f));
	pUI->Transform()->SetRelativeScale(Vec3(1280.f, 46.f, 1.f));

	pUI->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pUI->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UIMtrl"));
	pUI->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\hud_collapse.png"));

	SpawnGameObject(pUI, Vec3(0, 360.f - 42.f, 200.f), L"ViewPort UI");

	// UI_Battery
	CGameObject* pBattery = new CGameObject;
	pBattery->SetName(L"UI_Battery");

	pBattery->AddComponent(new CTransform);
	pBattery->AddComponent(new CMeshRender);
	pBattery->AddComponent(new CBatteryScript);

	pBattery->Transform()->SetRelativeScale(Vec3(110.f, 20.f, 1.f));

	pBattery->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_PivotRight"));
	pBattery->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BatteryMtrl"));
	pBattery->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\effect_battery.png"));

	SpawnGameObject(pBattery, Vec3(-617.f, 318.f, 100.f), L"ViewPort UI");

	// UI_Timer
	CGameObject* pTimer = new CGameObject;
	pTimer->SetName(L"UI_Timer");

	pTimer->AddComponent(new CTransform);
	pTimer->AddComponent(new CMeshRender);
	pTimer->AddComponent(new CTimerScript);

	pTimer->Transform()->SetRelativeScale(Vec3(188.f, 22.f, 1.f));

	pTimer->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_PivotRight"));
	pTimer->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TimerMtrl"));
	pTimer->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\effect_timer.png"));

	SpawnGameObject(pTimer, Vec3(-89.f, 324.f, 100.f), L"ViewPort UI");

	// UI_Katana
	CGameObject* pKatana = new CGameObject;
	pKatana->SetName(L"UI_Katana");

	pKatana->AddComponent(new CTransform);
	pKatana->AddComponent(new CMeshRender);

	pKatana->Transform()->SetRelativeScale(Vec3(40.f, 40.f, 1.f));

	pKatana->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_PivotRight"));
	pKatana->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"KatanaMtrl"));
	pKatana->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\spr_katanaicons.png"));

	SpawnGameObject(pKatana, Vec3(507.f, 318.f, 100.f), L"ViewPort UI");

	// �����δ� �ִϸ��̼�
	CGameObject* pRewinder = new CGameObject;
	pRewinder->SetName(L"Rewinder");
	pRewinder->AddComponent(new CTransform);
	pRewinder->AddComponent(new CMeshRender);
	pRewinder->AddComponent(new CAnimator2D);
	pRewinder->AddComponent(new CRewinderScript);

	pRewinder->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pRewinder->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimLightMtrl"));

	// �ӽ÷� ���
	pRewinder->Animator2D()->Create_Grunt_Animation();
	pRewinder->Animator2D()->Play(L"texture\\grunt\\spr_grunt_idle", true);

	SpawnGameObject(pRewinder, Vec3(0.f, 0.f, 30.f), L"ViewPort UI");

	// �浹 ��ų ���̾� ¦ ����
	CCollisionMgr::GetInst()->LayerCheck(L"PlayerProjectile", L"MonsterHitBox");
	CCollisionMgr::GetInst()->LayerCheck(L"MonsterProjectile", L"PlayerHitBox");
	CCollisionMgr::GetInst()->LayerCheck(L"MonsterProjectile", L"PlayerProjectile");
	CCollisionMgr::GetInst()->LayerCheck(L"MonsterView", L"PlayerHitBox");
	CCollisionMgr::GetInst()->LayerCheck(L"MonsterView", L"MonsterHitBox");
	CCollisionMgr::GetInst()->LayerCheck(L"MonsterAttackRange", L"PlayerHitBox");
}

void ResetStage_3()
{
	// ����� ����
	Ptr<CSound> pBGM = CLevelMgr::GetInst()->GetCurLevel()->GetBgm();
	pBGM->Stop();

	// ����� ���
	pBGM = CResMgr::GetInst()->FindRes<CSound>(L"sound\\song_sneaky_driver.ogg");
	pBGM->Play(999, 0.9f, true);

	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurLevel();
	pCurLevel->SetName(L"Stage_3");
	pCurLevel->ChangeState(LEVEL_STATE::PLAY);
	pCurLevel->SetBgm(pBGM.Get());

	// Layer �̸�����
	pCurLevel->GetLayer(0)->SetName(L"Default");
	pCurLevel->GetLayer(1)->SetName(L"Tile");
	pCurLevel->GetLayer(2)->SetName(L"PlayerHitBox");
	pCurLevel->GetLayer(3)->SetName(L"MonsterHitBox");
	pCurLevel->GetLayer(4)->SetName(L"PlayerProjectile");
	pCurLevel->GetLayer(5)->SetName(L"MonsterProjectile");
	pCurLevel->GetLayer(6)->SetName(L"MonsterView");
	pCurLevel->GetLayer(7)->SetName(L"MonsterAttackRange");
	pCurLevel->GetLayer(31)->SetName(L"ViewPort UI");

	// ī�޶� visible mask �ٽ� all true
	pCurLevel->FindParentObjectByName(L"MainCamera")->Camera()->SetLayerMaskAll(true);

	// ������Ʈ ����
	CGameObject* pParent = new CGameObject;
	pParent->SetName(L"Player");
	pParent->AddComponent(new CTransform);
	pParent->AddComponent(new CMeshRender);
	pParent->AddComponent(new CCollider2D);
	pParent->AddComponent(new CAnimator2D);
	pParent->AddComponent(new CPlayerScript);

	pParent->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));

	pParent->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_Pivot"));
	pParent->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimLightMtrl"));

	Vec2 _Resolution = GlobalData.Resolution;
	pParent->Collider2D()->SetAbsolute(true);
	pParent->Collider2D()->SetOffsetScale(Vec2(36.f, 70.f));
	//pParent->Collider2D()->SetOffsetPos(Vec2( -_Resolution.x/2.f, _Resolution.y/2.f + 35.f));

	Ptr<CTexture> pAnimAtlas = CResMgr::GetInst()->FindRes<CTexture>(L"texture\\link.png");
	pParent->Animator2D()->CreateAnimation(L"WalkDown", pAnimAtlas, Vec2(0.f, 520.f), Vec2(120.f, 130.f), Vec2(300.f, 300.f), 10, 16);
	pParent->Animator2D()->Play(L"WalkDown", true);
	pParent->Animator2D()->Create_Player_Animation();
	SpawnGameObject(pParent, Vec3(60.f, -755.f, 500.f), L"PlayerHitBox");

	// Mouse
	CGameObject* pMouse = new CGameObject;
	pMouse->SetName(L"Mouse");

	pMouse->AddComponent(new CTransform);
	pMouse->AddComponent(new CMeshRender);
	pMouse->AddComponent(new CCollider2D);
	pMouse->AddComponent(new CMouseScript);

	pMouse->Transform()->SetRelativePos(Vec3(0.f, 250.f, 100.f));
	pMouse->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 1.f));


	pMouse->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pMouse->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"MouseDMtrl"));
	pMouse->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\spr_cursor.png"));

	pMouse->Collider2D()->SetAbsolute(true);
	pMouse->Collider2D()->SetOffsetScale(Vec2(35.f, 35.f));

	SpawnGameObject(pMouse, Vec3(0.f, 0.f, 100.f), L"ViewPort UI");

	{
		// Grunt_1	�� 2�� ����
		CGameObject* pGrunt = CreateGrunt();
		pGrunt->GetScript<CGruntScript>()->SetBeginDir(ObjDir::Right);
		pGrunt->GetScript<CGruntScript>()->SetBeginState(ObjState::Idle);
		pGrunt->GetScript<CGruntScript>()->SetPatrol(false, 3.f);
		SpawnGameObject(pGrunt, Vec3(611, -387.f, 500.f), L"MonsterHitBox");

		// Grunt_1	�� 2�� ������
		pGrunt = CreateGrunt();
		pGrunt->GetScript<CGruntScript>()->SetBeginDir(ObjDir::Left);
		pGrunt->GetScript<CGruntScript>()->SetBeginState(ObjState::Idle);
		pGrunt->GetScript<CGruntScript>()->SetPatrol(false, 3.f);
		SpawnGameObject(pGrunt, Vec3(737, -387.f, 500.f), L"MonsterHitBox");

		// Grunt_1	�߾� ����
		pGrunt = CreateGrunt();
		pGrunt->GetScript<CGruntScript>()->SetBeginDir(ObjDir::Right);
		pGrunt->GetScript<CGruntScript>()->SetBeginState(ObjState::Walk);
		pGrunt->GetScript<CGruntScript>()->SetPatrol(true, 3.f);
		SpawnGameObject(pGrunt, Vec3(2272, -515, 500.f), L"MonsterHitBox");

		// Grunt_1	�߾� ����
		pGrunt = CreateGrunt();
		pGrunt->GetScript<CGruntScript>()->SetBeginDir(ObjDir::Left);
		pGrunt->GetScript<CGruntScript>()->SetBeginState(ObjState::Idle);
		pGrunt->GetScript<CGruntScript>()->SetPatrol(false, 3.f);
		SpawnGameObject(pGrunt, Vec3(3422, -515, 500.f), L"MonsterHitBox");

		// Grunt_1	�� 1��
		pGrunt = CreateGrunt();
		pGrunt->GetScript<CGruntScript>()->SetBeginDir(ObjDir::Right);
		pGrunt->GetScript<CGruntScript>()->SetBeginState(ObjState::Idle);
		pGrunt->GetScript<CGruntScript>()->SetPatrol(false, 3.f);
		SpawnGameObject(pGrunt, Vec3(1155, -771, 500.f), L"MonsterHitBox");

		// Grunt_1	�� 1��
		pGrunt = CreateGrunt();
		pGrunt->GetScript<CGruntScript>()->SetBeginDir(ObjDir::Right);
		pGrunt->GetScript<CGruntScript>()->SetBeginState(ObjState::Walk);
		pGrunt->GetScript<CGruntScript>()->SetPatrol(true, 3.f);
		SpawnGameObject(pGrunt, Vec3(3110, -771, 500.f), L"MonsterHitBox");
	}

	{
		// ������ �� 2�� ������
		CGameObject* pGangster = CreateGangster();
		pGangster->GetScript<CGangsterScript>()->SetBeginDir(ObjDir::Left);
		pGangster->GetScript<CGangsterScript>()->SetBeginState(ObjState::Idle);
		pGangster->GetScript<CGangsterScript>()->SetPatrol(false, 3.f);
		SpawnGameObject(pGangster, Vec3(1483, -387.f, 500.f), L"MonsterHitBox");


		// ������ �߾� ������
		pGangster = CreateGangster();
		pGangster->GetScript<CGangsterScript>()->SetBeginDir(ObjDir::Right);
		pGangster->GetScript<CGangsterScript>()->SetBeginState(ObjState::Idle);
		pGangster->GetScript<CGangsterScript>()->SetPatrol(false, 3.f);
		SpawnGameObject(pGangster, Vec3(1840, -515.f, 500.f), L"MonsterHitBox");

		// ������ �߾� ����
		pGangster = CreateGangster();
		pGangster->GetScript<CGangsterScript>()->SetBeginDir(ObjDir::Left);
		pGangster->GetScript<CGangsterScript>()->SetBeginState(ObjState::Walk);
		pGangster->GetScript<CGangsterScript>()->SetPatrol(true, 3.f);
		SpawnGameObject(pGangster, Vec3(2788, -515.f, 500.f), L"MonsterHitBox");

		// ������ �߾� ����
		pGangster = CreateGangster();
		pGangster->GetScript<CGangsterScript>()->SetBeginDir(ObjDir::Left);
		pGangster->GetScript<CGangsterScript>()->SetBeginState(ObjState::Walk);
		pGangster->GetScript<CGangsterScript>()->SetPatrol(true, 3.f);
		SpawnGameObject(pGangster, Vec3(3567, -515.f, 500.f), L"MonsterHitBox");

		// ������ �߾� ���� ��
		pGangster = CreateGangster();
		pGangster->GetScript<CGangsterScript>()->SetBeginDir(ObjDir::Left);
		pGangster->GetScript<CGangsterScript>()->SetBeginState(ObjState::Idle);
		pGangster->GetScript<CGangsterScript>()->SetPatrol(false, 3.f);
		SpawnGameObject(pGangster, Vec3(3684, -515.f, 500.f), L"MonsterHitBox");

		// ������ �� 1�� ���� ��
		pGangster = CreateGangster();
		pGangster->GetScript<CGangsterScript>()->SetBeginDir(ObjDir::Left);
		pGangster->GetScript<CGangsterScript>()->SetBeginState(ObjState::Idle);
		pGangster->GetScript<CGangsterScript>()->SetPatrol(false, 3.f);
		SpawnGameObject(pGangster, Vec3(1600, -771.f, 500.f), L"MonsterHitBox");

		// ������ �� 1�� ���� ��
		pGangster = CreateGangster();
		pGangster->GetScript<CGangsterScript>()->SetBeginDir(ObjDir::Left);
		pGangster->GetScript<CGangsterScript>()->SetBeginState(ObjState::Idle);
		pGangster->GetScript<CGangsterScript>()->SetPatrol(false, 3.f);
		SpawnGameObject(pGangster, Vec3(4000, -771.f, 500.f), L"MonsterHitBox");

		// ������ �� 1��
		pGangster = CreateGangster();
		pGangster->GetScript<CGangsterScript>()->SetBeginDir(ObjDir::Left);
		pGangster->GetScript<CGangsterScript>()->SetBeginState(ObjState::Idle);
		pGangster->GetScript<CGangsterScript>()->SetPatrol(false, 3.f);
		SpawnGameObject(pGangster, Vec3(3425, -771.f, 500.f), L"MonsterHitBox");



		// ���� �� 2��
		CGameObject* pPomp = CreatePomp();
		pPomp->GetScript<CPompScript>()->SetBeginDir(ObjDir::Left);
		pPomp->GetScript<CPompScript>()->SetBeginState(ObjState::Walk);
		pPomp->GetScript<CPompScript>()->SetPatrol(true, 3.f);
		SpawnGameObject(pPomp, Vec3(1420, -387.f, 500.f), L"MonsterHitBox");

		// ���� �߾� 2��
		pPomp = CreatePomp();
		pPomp->GetScript<CPompScript>()->SetBeginDir(ObjDir::Left);
		pPomp->GetScript<CPompScript>()->SetBeginState(ObjState::Idle);
		pPomp->GetScript<CPompScript>()->SetPatrol(false, 3.f);
		SpawnGameObject(pPomp, Vec3(3220, -515.f, 500.f), L"MonsterHitBox");

		// ���� �� 1�� ����
		pPomp = CreatePomp();
		pPomp->GetScript<CPompScript>()->SetBeginDir(ObjDir::Left);
		pPomp->GetScript<CPompScript>()->SetBeginState(ObjState::Walk);
		pPomp->GetScript<CPompScript>()->SetPatrol(true, 4.f);
		SpawnGameObject(pPomp, Vec3(1530, -771.f, 500.f), L"MonsterHitBox");

		// ���� �� 1�� �߾�
		pPomp = CreatePomp();
		pPomp->GetScript<CPompScript>()->SetBeginDir(ObjDir::Right);
		pPomp->GetScript<CPompScript>()->SetBeginState(ObjState::Idle);
		pPomp->GetScript<CPompScript>()->SetPatrol(false, 4.f);
		SpawnGameObject(pPomp, Vec3(3645, -771.f, 500.f), L"MonsterHitBox");

		// ���� �� 1�� �߾�
		pPomp = CreatePomp();
		pPomp->GetScript<CPompScript>()->SetBeginDir(ObjDir::Left);
		pPomp->GetScript<CPompScript>()->SetBeginState(ObjState::Idle);
		pPomp->GetScript<CPompScript>()->SetPatrol(false, 4.f);
		SpawnGameObject(pPomp, Vec3(3710, -771.f, 500.f), L"MonsterHitBox");

	}

	// BackGround Object
	CGameObject* pBackGround = new CGameObject;
	pBackGround->SetName(L"BackGround");

	pBackGround->AddComponent(new CTransform);
	pBackGround->AddComponent(new CMeshRender);

	pBackGround->Transform()->SetRelativeScale(Vec3(4288.f, 1200.f, 1.f));

	pBackGround->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pBackGround->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DMtrl"));
	pBackGround->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\map\\stage3_bg_render.png"));

	SpawnGameObject(pBackGround, Vec3(2144, -600.f, 1000.f), L"Default");	//��������1 ������ġ

	// UI
	CGameObject* pUI = new CGameObject;
	pUI->SetName(L"UI");

	pUI->AddComponent(new CTransform);
	pUI->AddComponent(new CMeshRender);
	pUI->AddComponent(new CUIScript);

	pUI->Transform()->SetRelativePos(Vec3(GlobalData.Resolution.x / 2.f, GlobalData.Resolution.y / 2.f, 100.f));
	pUI->Transform()->SetRelativeScale(Vec3(1280.f, 46.f, 1.f));

	pUI->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pUI->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"UIMtrl"));
	pUI->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\hud_collapse.png"));

	SpawnGameObject(pUI, Vec3(0, 360.f - 42.f, 200.f), L"ViewPort UI");

	// UI_Battery
	CGameObject* pBattery = new CGameObject;
	pBattery->SetName(L"UI_Battery");

	pBattery->AddComponent(new CTransform);
	pBattery->AddComponent(new CMeshRender);
	pBattery->AddComponent(new CBatteryScript);

	pBattery->Transform()->SetRelativeScale(Vec3(110.f, 20.f, 1.f));

	pBattery->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_PivotRight"));
	pBattery->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BatteryMtrl"));
	pBattery->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\effect_battery.png"));

	SpawnGameObject(pBattery, Vec3(-617.f, 318.f, 100.f), L"ViewPort UI");

	// UI_Timer
	CGameObject* pTimer = new CGameObject;
	pTimer->SetName(L"UI_Timer");

	pTimer->AddComponent(new CTransform);
	pTimer->AddComponent(new CMeshRender);
	pTimer->AddComponent(new CTimerScript);

	pTimer->Transform()->SetRelativeScale(Vec3(188.f, 22.f, 1.f));

	pTimer->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_PivotRight"));
	pTimer->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"TimerMtrl"));
	pTimer->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\effect_timer.png"));

	SpawnGameObject(pTimer, Vec3(-89.f, 324.f, 100.f), L"ViewPort UI");

	// UI_Katana
	CGameObject* pKatana = new CGameObject;
	pKatana->SetName(L"UI_Katana");

	pKatana->AddComponent(new CTransform);
	pKatana->AddComponent(new CMeshRender);

	pKatana->Transform()->SetRelativeScale(Vec3(40.f, 40.f, 1.f));

	pKatana->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_PivotRight"));
	pKatana->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"KatanaMtrl"));
	pKatana->MeshRender()->GetMaterial()->SetTexParam(TEX_0, CResMgr::GetInst()->FindRes<CTexture>(L"texture\\UI\\spr_katanaicons.png"));

	SpawnGameObject(pKatana, Vec3(507.f, 318.f, 100.f), L"ViewPort UI");

	// �����δ� �ִϸ��̼�
	CGameObject* pRewinder = new CGameObject;
	pRewinder->SetName(L"Rewinder");
	pRewinder->AddComponent(new CTransform);
	pRewinder->AddComponent(new CMeshRender);
	pRewinder->AddComponent(new CAnimator2D);
	pRewinder->AddComponent(new CRewinderScript);

	pRewinder->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	pRewinder->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"Std2DAnimLightMtrl"));

	// �ӽ÷� ���
	pRewinder->Animator2D()->Create_Grunt_Animation();
	pRewinder->Animator2D()->Play(L"texture\\grunt\\spr_grunt_idle", true);

	SpawnGameObject(pRewinder, Vec3(0.f, 0.f, 30.f), L"ViewPort UI");

	// �浹 ��ų ���̾� ¦ ����
	CCollisionMgr::GetInst()->LayerCheck(L"PlayerProjectile", L"MonsterHitBox");
	CCollisionMgr::GetInst()->LayerCheck(L"MonsterProjectile", L"PlayerHitBox");
	CCollisionMgr::GetInst()->LayerCheck(L"MonsterProjectile", L"PlayerProjectile");
	CCollisionMgr::GetInst()->LayerCheck(L"MonsterView", L"PlayerHitBox");
	CCollisionMgr::GetInst()->LayerCheck(L"MonsterView", L"MonsterHitBox");
	CCollisionMgr::GetInst()->LayerCheck(L"MonsterAttackRange", L"PlayerHitBox");
}


CGameObject* CreateGrunt()
{
	// �׷�Ʈ ũ������Ʈ
	CGameObject* pGrunt = new CGameObject;
	pGrunt->SetName(L"Grunt");
	pGrunt->AddComponent(new CTransform);
	pGrunt->AddComponent(new CMeshRender);
	pGrunt->AddComponent(new CCollider2D);
	pGrunt->AddComponent(new CAnimator2D);
	pGrunt->AddComponent(new CGruntScript);

	pGrunt->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));

	pGrunt->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_Pivot"));
	pGrunt->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"GruntMtrl"));

	pGrunt->Collider2D()->SetAbsolute(true);
	pGrunt->Collider2D()->SetOffsetScale(Vec2(36.f, 70.f));

	pGrunt->Animator2D()->Create_Grunt_Animation();
	pGrunt->Animator2D()->Play(L"texture\\grunt\\spr_grunt_idle", true);
	//SpawnGameObject(pGrunt, Vec3(230.f, -671.f, 500.f), L"MonsterHitBox");

	return pGrunt;
}

CGameObject* CreatePomp()
{
	// Pomp ũ������Ʈ
	CGameObject* pPomp = new CGameObject;
	pPomp->SetName(L"Pomp");
	pPomp->AddComponent(new CTransform);
	pPomp->AddComponent(new CMeshRender);
	pPomp->AddComponent(new CCollider2D);
	pPomp->AddComponent(new CAnimator2D);
	pPomp->AddComponent(new CPompScript);

	pPomp->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));

	pPomp->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_Pivot"));
	pPomp->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"PompMtrl"));

	pPomp->Collider2D()->SetAbsolute(true);
	pPomp->Collider2D()->SetOffsetScale(Vec2(36.f, 70.f));

	pPomp->Animator2D()->Create_Pomp_Animation();
	pPomp->Animator2D()->Play(L"texture\\pomp\\spr_pomp_idle", true);

	return pPomp;
}

CGameObject* CreateGangster()
{
	// Gangster ũ������Ʈ
	CGameObject* pGangster = new CGameObject;
	pGangster->SetName(L"Gangster");
	pGangster->AddComponent(new CTransform);
	pGangster->AddComponent(new CMeshRender);
	pGangster->AddComponent(new CCollider2D);
	pGangster->AddComponent(new CAnimator2D);
	pGangster->AddComponent(new CGangsterScript);

	pGangster->Transform()->SetRelativeScale(Vec3(200.f, 200.f, 1.f));

	pGangster->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh_Pivot"));
	pGangster->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"GangsterMtrl"));

	pGangster->Collider2D()->SetAbsolute(true);
	pGangster->Collider2D()->SetOffsetScale(Vec2(36.f, 70.f));

	pGangster->Animator2D()->Create_Gangster_Animation();
	pGangster->Animator2D()->Play(L"texture\\gangster\\spr_gangsteridle", true);

	return pGangster;
}