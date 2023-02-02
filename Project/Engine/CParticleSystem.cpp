#include "pch.h"
#include "CParticleSystem.h"

#include "CDevice.h"
#include "CStructuredBuffer.h"

#include "CResMgr.h"

CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_iMaxParticleCount(5)
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"RectMesh"));
	SetMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"ParticleRenderMtrl"));


	tParticle arrParticle[5] = { };

	float fStep = 100.f;

	arrParticle[0].vWorldPos = Vec3(-2.f * fStep, 0.f, 100.f);
	arrParticle[1].vWorldPos = Vec3(-1.f * fStep, 0.f, 100.f);
	arrParticle[2].vWorldPos = Vec3(0.f, 0.f, 100.f);
	arrParticle[3].vWorldPos = Vec3(1.f * fStep, 0.f, 100.f);
	arrParticle[4].vWorldPos = Vec3(2.f * fStep, 0.f, 100.f);

	arrParticle[0].vWorldScale = Vec3(10.f, 10.f, 1.f);
	arrParticle[1].vWorldScale = Vec3(10.f, 10.f, 1.f);
	arrParticle[2].vWorldScale = Vec3(10.f, 10.f, 1.f);
	arrParticle[3].vWorldScale = Vec3(10.f, 10.f, 1.f);
	arrParticle[4].vWorldScale = Vec3(10.f, 10.f, 1.f);

	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_iMaxParticleCount, SB_TYPE::READ_ONLY, arrParticle);
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
	m_ParticleBuffer->UpdateData(20, PIPELINE_STAGE::PS_ALL);

	for (int i = 0; i < m_iMaxParticleCount; ++i)
	{
		// Particle Render
		GetMaterial()->SetScalarParam(INT_0, &i);
		GetMaterial()->UpdateData();
		GetMesh()->render();
	}
}