#include "pch.h"
#include "CScriptMgr.h"

#include "CCameraMoveScript.h"
#include "CDustCloudScript.h"
#include "CGravityScript.h"
#include "CGruntScript.h"
#include "CJumpCloudScript.h"
#include "CLandCloudScript.h"
#include "CMainCameraScript.h"
#include "CMissileScript.h"
#include "CMonsterScript.h"
#include "CMouseScript.h"
#include "CPlayerScript.h"
#include "CSlashScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CDustCloudScript");
	_vec.push_back(L"CGravityScript");
	_vec.push_back(L"CGruntScript");
	_vec.push_back(L"CJumpCloudScript");
	_vec.push_back(L"CLandCloudScript");
	_vec.push_back(L"CMainCameraScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CMouseScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CSlashScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CDustCloudScript" == _strScriptName)
		return new CDustCloudScript;
	if (L"CGravityScript" == _strScriptName)
		return new CGravityScript;
	if (L"CGruntScript" == _strScriptName)
		return new CGruntScript;
	if (L"CJumpCloudScript" == _strScriptName)
		return new CJumpCloudScript;
	if (L"CLandCloudScript" == _strScriptName)
		return new CLandCloudScript;
	if (L"CMainCameraScript" == _strScriptName)
		return new CMainCameraScript;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"CMouseScript" == _strScriptName)
		return new CMouseScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CSlashScript" == _strScriptName)
		return new CSlashScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::DUSTCLOUDSCRIPT:
		return new CDustCloudScript;
		break;
	case (UINT)SCRIPT_TYPE::GRAVITYSCRIPT:
		return new CGravityScript;
		break;
	case (UINT)SCRIPT_TYPE::GRUNTSCRIPT:
		return new CGruntScript;
		break;
	case (UINT)SCRIPT_TYPE::JUMPCLOUDSCRIPT:
		return new CJumpCloudScript;
		break;
	case (UINT)SCRIPT_TYPE::LANDCLOUDSCRIPT:
		return new CLandCloudScript;
		break;
	case (UINT)SCRIPT_TYPE::MAINCAMERASCRIPT:
		return new CMainCameraScript;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSCRIPT:
		return new CMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::MOUSESCRIPT:
		return new CMouseScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::SLASHSCRIPT:
		return new CSlashScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
		break;

	case SCRIPT_TYPE::DUSTCLOUDSCRIPT:
		return L"CDustCloudScript";
		break;

	case SCRIPT_TYPE::GRAVITYSCRIPT:
		return L"CGravityScript";
		break;

	case SCRIPT_TYPE::GRUNTSCRIPT:
		return L"CGruntScript";
		break;

	case SCRIPT_TYPE::JUMPCLOUDSCRIPT:
		return L"CJumpCloudScript";
		break;

	case SCRIPT_TYPE::LANDCLOUDSCRIPT:
		return L"CLandCloudScript";
		break;

	case SCRIPT_TYPE::MAINCAMERASCRIPT:
		return L"CMainCameraScript";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"CMonsterScript";
		break;

	case SCRIPT_TYPE::MOUSESCRIPT:
		return L"CMouseScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::SLASHSCRIPT:
		return L"CSlashScript";
		break;

	}
	return nullptr;
}