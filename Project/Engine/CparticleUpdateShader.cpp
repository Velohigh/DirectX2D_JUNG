#include "pch.h"
#include "CParticleUpdateShader.h"

#include "CStructuredBuffer.h"

CParticleUpdateShader::CParticleUpdateShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ)
	: m_ParticleBuffer(nullptr)
{
	m_iGroupPerThreadX = _iGroupPerThreadX;
	m_iGroupPerThreadY = _iGroupPerThreadY;
	m_iGroupPerThreadZ = _iGroupPerThreadZ;
}

CParticleUpdateShader::~CParticleUpdateShader()
{

}

void CParticleUpdateShader::SetParticleBuffer(CStructuredBuffer* _Buffer)
{
	m_ParticleBuffer = _Buffer;
	m_Const.arrInt[0] = m_ParticleBuffer->GetElementCount();
}

void CParticleUpdateShader::UpdateData()
{
	m_ParticleBuffer->UpdateData_CS(0);

	// �׷� ��
	m_iGroupX = (m_ParticleBuffer->GetElementCount() / m_iGroupPerThreadX) + 1;
}

void CParticleUpdateShader::Clear()
{
	m_ParticleBuffer->Clear_CS();
}