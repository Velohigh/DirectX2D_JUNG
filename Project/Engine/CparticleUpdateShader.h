#pragma once
#include "CComputeShader.h"

class CStructuredBuffer;

class CParticleUpdateShader :
    public CComputeShader
{
private:
    CStructuredBuffer* m_ParticleBuffer;
    CStructuredBuffer* m_RWBuffer;
    CStructuredBuffer* m_ModuleData;


public:
    void SetParticleBuffer(CStructuredBuffer* _Buffer);
    void SetRWParticleBuffer(CStructuredBuffer* _Buffer) { m_RWBuffer = _Buffer; }
    void SetModuleData(CStructuredBuffer* _Buffer) { m_ModuleData = _Buffer; }

public:
    virtual void UpdateData() override;
    virtual void Clear() override;

public:
    CParticleUpdateShader(UINT _iGroupPerThreadX, UINT _iGroupPerThreadY, UINT _iGroupPerThreadZ);
    ~CParticleUpdateShader();
};