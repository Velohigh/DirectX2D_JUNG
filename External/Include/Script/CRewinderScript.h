#pragma once
#include <Engine\CScript.h>

class CRewinderScript :
	public CScript
{
private:
	vector<Ptr<CTexture>>	m_vecRewinder;
	vector<Ptr<CTexture>>	m_vecRewinderReverse;
	float	m_Time;

	bool	m_bSaveOn;
	bool	m_bReverseOn;
	bool	m_bReplayOn;

public:
	virtual void begin() override;
	virtual void tick() override;

public:
	void SetAnimationSaveOn(bool _bool) { m_bSaveOn = _bool; }
	void SetReverseOn(bool _bool) { m_bReverseOn = _bool; }
	void SetReplayOn(bool _bool) { m_bReplayOn = _bool; }
	void CreateFolderAnim_Rewinder();

private:

private:
	CLONE(CRewinderScript);

public:
	CRewinderScript();
	~CRewinderScript();
};

