#pragma once
#include <Engine\CScript.h>

class CLandCloudScript :
	public CScript
{
private:

public:
	virtual void begin() override;
	virtual void tick() override;

private:
	CLONE(CLandCloudScript);

public:
	CLandCloudScript();
	~CLandCloudScript();
};

