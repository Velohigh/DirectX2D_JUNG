#pragma once
#include "CRenderComponent.h"

class CStructuredBuffer;

class CParticleSystem :
    public CRenderComponent
{
private:
    CStructuredBuffer*  m_ParticleBuffer;

    UINT                m_iMaxParticleCount;

public:
    virtual void finaltick() override;
    virtual void render() override;


    CLONE(CParticleSystem);
public:
    CParticleSystem();
    ~CParticleSystem();
};

