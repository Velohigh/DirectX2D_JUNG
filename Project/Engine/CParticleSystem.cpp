#include "pch.h"
#include "CParticleSystem.h"

#include "CDevice.h"
#include "CStructuredBuffer.h"

#include "CResMgr.h"
#include "CTransform.h"

CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_ParticleBuffer(nullptr)
	, m_iMaxParticleCount(100)
{
	// 입자 메쉬
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));

	// 파티클 전용 재질
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"));

	// 파티클 업데이트 컴퓨트 쉐이더	
	m_UpdateCS = (CParticleUpdateShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"ParticleUpdateCS").Get();

	// 파티클 버퍼 초기 데이터
	tParticle arrParticle[100] = { };
	float fAngle = XM_2PI / 100.f;
	float fRadius = 20.f;
	float fSpeed = 100.f;

	for (UINT i = 0; i < 100; ++i)
	{
		arrParticle[i].vWorldPos = Vec3(fRadius * cosf(fAngle * (float)i), fRadius * sinf(fAngle * (float)i), 100.f);
		arrParticle[i].vVelocity = arrParticle[i].vWorldPos;
		arrParticle[i].vVelocity.z = 0.f;
		arrParticle[i].vVelocity.Normalize();
		arrParticle[i].vVelocity *= fSpeed;
		arrParticle[i].vWorldScale = Vec3(10.f, 10.f, 1.f);

		if (i < 50)
			arrParticle[i].Age = -1.f;
	}

	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_iMaxParticleCount, SB_TYPE::READ_WRITE, false, arrParticle);
}

CParticleSystem::~CParticleSystem()
{
	if (nullptr != m_ParticleBuffer)
		delete m_ParticleBuffer;
}


void CParticleSystem::finaltick()
{
	// 파티클 업데이트
	m_UpdateCS->SetParticleBuffer(m_ParticleBuffer);
	m_UpdateCS->Execute();
}

void CParticleSystem::render()
{
	Transform()->UpdateData();

	m_ParticleBuffer->UpdateData(20, PIPELINE_STAGE::PS_ALL);

	// Particle Render	
	GetMaterial()->UpdateData();
	GetMesh()->render_particle(m_iMaxParticleCount);
}