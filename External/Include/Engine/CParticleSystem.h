#pragma once
#include "CRenderComponent.h"

#include "ptr.h"
#include "CParticleUpdateShader.h"

class CStructuredBuffer;

class CParticleSystem :
    public CRenderComponent
{
private:
    CStructuredBuffer*          m_ParticleBuffer;
    UINT                        m_iMaxParticleCount;

    Ptr<CParticleUpdateShader>  m_UpdateCS;





public:
    virtual void finaltick() override;
    virtual void render() override;


    CLONE(CParticleSystem);
public:
    CParticleSystem();
    ~CParticleSystem();
};

