#pragma once
#include <Engine\CScript.h>

class CTitleScript :
	public CScript
{
private:
	CSound* m_pBgm;
	bool	m_bEnterPush;
	bool	m_bEnd;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	CSound* GetBgm() { return m_pBgm; }

	void SetBgm(CSound* _pBgm) { m_pBgm = _pBgm; }



private:


private:
	CLONE(CTitleScript);

public:
	CTitleScript();
	~CTitleScript();
};

