#include "pch.h"
#include "CBulletScript.h"
#include "CPlayerScript.h"
#include "CReflectScript.h"
#include "CMonsterScript.h"


CBulletScript::CBulletScript()
	: CScript((UINT)SCRIPT_TYPE::BULLETSCRIPT)
{
}

CBulletScript::~CBulletScript()
{
}


void CBulletScript::begin()
{
	m_Level = CLevelMgr::GetInst()->GetCurLevel();

	m_MoveSpeed = 1100.f;

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


}

void CBulletScript::tick()
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	Vec2 m_PosyReverse = Vec2(m_Pos3.x, -m_Pos3.y);


	MoveValue(m_MoveDir * DT * m_MoveSpeed);
	m_AttackDir = m_MoveDir * m_MoveSpeed + Vector2{ 0.f, 400.f };

	// ���� �ε����� �Ѿ� �ı�.
	CTexture* ColMap = GetOwner()->GetColMapTexture();
	int Color = ColMap->GetPixelColor(m_PosyReverse);

	if (RGB(0, 0, 0) == Color)
	{
		Destroy();
	}

	Vec3 CameraPos = m_Level->FindParentObjectByName(L"MainCamera")->Transform()->GetRelativePos();
	// �浹ü ��ġ ����
	if(Collider2D() != nullptr)
	Collider2D()->SetOffsetPos(Vec2(-CameraPos.x, -CameraPos.y));

	// �̵��������� �̹��� ȸ��
	Vec2 vRight = { 1.f , 0.f };
	float Angle = m_MoveDir.Dot(vRight);
	Angle = acosf(Angle);

	if(m_MoveDir.y <0.f)
	Angle = 2 * XM_PI - Angle;

	Vec3 m_Rot = Transform()->GetRelativeRot();
	Transform()->SetRelativeRot(m_Rot.x, m_Rot.y, Angle);

}


void CBulletScript::MoveDir(const Vec2& Dir)
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);
	m_Pos += Dir * m_MoveSpeed * DT;

	Transform()->SetRelativePos(m_Pos.x, m_Pos.y, m_Pos3.z);

}

void CBulletScript::MoveValue(const Vector2& MoveValue)
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);

	m_Pos += MoveValue /*m_TimeScale*/;

	Transform()->SetRelativePos(m_Pos.x, m_Pos.y, m_Pos3.z);
}

void CBulletScript::BeginOverlap(CCollider2D* _Other)
{
	Vec3 m_Pos3 = Transform()->GetRelativePos();
	Vec2 m_Pos = Vec2(m_Pos3.x, m_Pos3.y);


	// �÷��̾�� �浹�� ���
	if (_Other->GetOwner()->GetName() == L"Player")
	{
		// �÷��̾ ��� ���°� �ƴϰ� , ������ ������ �ƴҋ� ��Ʈ����
		PlayerState m_PState = _Other->GetOwner()->GetScript<CPlayerScript>()->GetState();
		if (m_PState != PlayerState::HurtFlyLoop &&
			m_PState != PlayerState::HurtGround &&
			m_PState != PlayerState::Dead &&
			m_PState != PlayerState::Dodge)
		{
			Destroy();
			if (m_CurDir == ObjDir::Right)
			{
				_Other->GetOwner()->GetScript<CPlayerScript>()->SetEnemyAttackDir(Vector2{ 1.f , 0.5f } *800);
			}
			else if (m_CurDir == ObjDir::Left)
			{
				_Other->GetOwner()->GetScript<CPlayerScript>()->SetEnemyAttackDir(Vector2{ -1.f , 0.5f } *800);
			}

			_Other->GetOwner()->GetScript<CPlayerScript>()->StateChange(PlayerState::HurtFlyLoop);
			return;
		}




	}

	// �÷��̾��� ���ݰ� �浹�� ��� �÷��̾� �Ѿ��� �ȴ�.
	else if (_Other->GetOwner()->GetName() == L"Slash")
	{
		m_MoveDir *= -1.f;
		Destroy();

		// �÷��̾� �Ѿ� ����
		CGameObject* pBullet = new CGameObject;
		pBullet->SetName(L"Bullet");
		pBullet->AddComponent(new CTransform);
		pBullet->AddComponent(new CMeshRender);
		pBullet->AddComponent(new CCollider2D);
		pBullet->AddComponent(new CAnimator2D);
		pBullet->AddComponent(new CBulletScript);

		pBullet->Transform()->SetRelativeScale(48.f, 2.f, 1.f);
		pBullet->Transform()->SetRelativePos(m_Pos3);

		pBullet->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pBullet->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"BulletMtrl"));

		pBullet->Collider2D()->SetAbsolute(true);
		pBullet->Collider2D()->SetOffsetScale(Vec2(10.f, 10.f));

		pBullet->Animator2D()->Create_Effect_Animation();
		pBullet->Animator2D()->Play(L"texture\\effect\\spr_bullet", false);
		pBullet->GetScript<CBulletScript>()->SetMoveDir(m_MoveDir);

		SpawnGameObject(pBullet, Transform()->GetRelativePos(), L"PlayerProjectile");


		// Reflect ����Ʈ
		CGameObject* pReflect = new CGameObject;
		pReflect->SetName(L"Reflect");
		pReflect->AddComponent(new CTransform);
		pReflect->AddComponent(new CMeshRender);
		pReflect->AddComponent(new CAnimator2D);
		pReflect->AddComponent(new CReflectScript);

		pReflect->Transform()->SetRelativeScale(140.f, 128.f, 1.f);

		pReflect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pReflect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ReflectMtrl"));

		pReflect->Animator2D()->Create_Effect_Animation();
		pReflect->Animator2D()->Play(L"texture\\effect\\spr_reflect", false);
		pReflect->GetScript<CReflectScript>()->SetOwner(GetOwner());
		if (m_CurDir == ObjDir::Right)
			pReflect->GetScript<CReflectScript>()->SetDir(ObjDir::Right);
		else if (m_CurDir == ObjDir::Left)
			pReflect->GetScript<CReflectScript>()->SetDir(ObjDir::Left);

		Vec3 MyPos3 = Transform()->GetRelativePos();

		SpawnGameObject(pReflect, MyPos3, L"Default");

		// Reflect ����
		Ptr<CSound> pReflectSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\reflect.wav");
		pReflectSound->Play(1, 1.f, true);
	}

	// �÷��̾��� �Ѿ˰� ���� �Ѿ��� �浹�� ���
	else if (_Other->GetOwner()->GetName() == L"Bullet")
	{

		// Reflect ����Ʈ
		CGameObject* pReflect = new CGameObject;
		pReflect->SetName(L"Reflect");
		pReflect->AddComponent(new CTransform);
		pReflect->AddComponent(new CMeshRender);
		pReflect->AddComponent(new CAnimator2D);
		pReflect->AddComponent(new CReflectScript);

		pReflect->Transform()->SetRelativeScale(140.f, 128.f, 1.f);

		pReflect->MeshRender()->SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
		pReflect->MeshRender()->SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ReflectMtrl"));

		pReflect->Animator2D()->Create_Effect_Animation();
		pReflect->Animator2D()->Play(L"texture\\effect\\spr_reflect", false);
		pReflect->GetScript<CReflectScript>()->SetOwner(GetOwner());
		if (m_CurDir == ObjDir::Right)
			pReflect->GetScript<CReflectScript>()->SetDir(ObjDir::Right);
		else if (m_CurDir == ObjDir::Left)
			pReflect->GetScript<CReflectScript>()->SetDir(ObjDir::Left);

		Vec3 MyPos3 = Transform()->GetRelativePos();

		SpawnGameObject(pReflect, MyPos3, L"Default");

		// Reflect ����
		Ptr<CSound> pReflectSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\reflect.wav");
		pReflectSound->Play(1, 1.f, true);
		
		Destroy();
	}


	// �÷��̾� �Ѿ��� ���Ϳ� �浹�� ���
	else if (_Other->GetOwner()->GetScript<CMonsterScript>()->GetState() != ObjState::Dead &&
		_Other->GetOwner()->GetScript<CMonsterScript>()->GetState() != ObjState::HurtFly &&
		_Other->GetOwner()->GetScript<CMonsterScript>()->GetState() != ObjState::HurtGround &&
		_Other->GetOwner()->GetName() != L"Player" &&
		_Other->GetOwner()->GetName() != L"Bullet")
	{
		_Other->GetOwner()->GetScript<CMonsterScript>()->SetEnemyAttackDir(m_MoveDir * m_MoveSpeed + Vector2{ 0.f, -400.f });
		_Other->GetOwner()->GetScript<CMonsterScript>()->StateChange(ObjState::HurtFly);
		Destroy();

		// DeathBullet ����
		Ptr<CSound> pDeathBulletSound = CResMgr::GetInst()->FindRes<CSound>(L"sound\\death_bullet.wav");
		pDeathBulletSound->Play(1, 1.f, true);

	}

	

}

void CBulletScript::OnOverlap(CCollider2D* _Other)
{
}

void CBulletScript::EndOverlap(CCollider2D* _Other)
{
}