#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	ATTACKRANGESCRIPT,
	BLOODANIMATION2SCRIPT,
	BLOODANIMATIONSCRIPT,
	CAMERAMOVESCRIPT,
	DUSTCLOUDSCRIPT,
	ENEMY_FOLLOWSCRIPT,
	GRAVITYSCRIPT,
	GRUNTSCRIPT,
	GRUNTSLASHSCRIPT,
	JUMPCLOUDSCRIPT,
	LANDCLOUDSCRIPT,
	MAINCAMERASCRIPT,
	MISSILESCRIPT,
	MONSTERSCRIPT,
	MOUSESCRIPT,
	PLAYERSCRIPT,
	SLASHSCRIPT,
	VIEWSCRIPT,
	END,
};

using namespace std;

class CScript;

class CScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static CScript * GetScript(const wstring& _strScriptName);
	static CScript * GetScript(UINT _iScriptType);
	static const wchar_t * GetScriptName(CScript * _pScript);
};
