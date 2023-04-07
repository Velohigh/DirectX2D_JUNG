#pragma once
#include <Engine\CScript.h>

class CJumpCloudScript :
	public CScript
{
private:

public:
	virtual void begin() override;
	virtual void tick() override;

private:
	CLONE(CJumpCloudScript);

public:
	CJumpCloudScript();
	~CJumpCloudScript();
};

