#pragma once
#include "CComputeShader.h"

class CStructuredBuffer;

class CParticleUpdateShader :
    public CComputeShader
{
private:
    CStructuredBuffer* m_ParticleBuffer;

public:
    void SetParticleBuffer(CStructuredBuffer* _Buffer);

public:
    virtual void UpdateData() override;
    virtual void Clear() override;

public:
    CParticleUpdateShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ);
    ~CParticleUpdateShader();
};