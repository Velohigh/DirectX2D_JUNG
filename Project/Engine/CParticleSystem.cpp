#include "pch.h"
#include "CParticleSystem.h"

#include "CDevice.h"
#include "CStructuredBuffer.h"

CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_iMaxParticleCount(100)
{
	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_iMaxParticleCount, SB_TYPE::READ_WRITE);


}

CParticleSystem::~CParticleSystem()
{
}


void CParticleSystem::finaltick()
{

}

void CParticleSystem::render()
{

}
