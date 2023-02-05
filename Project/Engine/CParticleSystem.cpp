#include "pch.h"
#include "CParticleSystem.h"

#include "CDevice.h"
#include "CStructuredBuffer.h"

#include "CResMgr.h"
#include "CTransform.h"

CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_iMaxParticleCount(5)
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"));

	tParticle arrParticle[1000] = { };

	float fStep = 100.f;

	arrParticle[0].vWorldPos = Vec3(-2.f * fStep, 0.f, 10.f);
	arrParticle[1].vWorldPos = Vec3(-1.f * fStep, 0.f, 10.f);
	arrParticle[2].vWorldPos = Vec3(0.f, 0.f, 10.f);
	arrParticle[3].vWorldPos = Vec3(1.f * fStep, 0.f, 10.f);
	arrParticle[4].vWorldPos = Vec3(2.f * fStep, 0.f, 10.f);

	arrParticle[0].vWorldScale = Vec3(10.f, 10.f, 1.f);
	arrParticle[1].vWorldScale = Vec3(10.f, 10.f, 1.f);
	arrParticle[2].vWorldScale = Vec3(10.f, 10.f, 1.f);
	arrParticle[3].vWorldScale = Vec3(10.f, 10.f, 1.f);
	arrParticle[4].vWorldScale = Vec3(10.f, 10.f, 1.f);

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

}

void CParticleSystem::render()
{
	Transform()->UpdateData();

	m_ParticleBuffer->UpdateData(20, PIPELINE_STAGE::PS_ALL);

	// Particle Render	
	GetMaterial()->UpdateData();
	GetMesh()->render_particle(m_iMaxParticleCount);
}