#include "pch.h"
#include "CScriptMgr.h"

#include "CAttackRangeScript.h"
#include "CBloodAnimation2Script.h"
#include "CBloodAnimationScript.h"
#include "CBloodSplaterScript.h"
#include "CCameraMoveScript.h"
#include "CDustCloudScript.h"
#include "CEnemy_FollowScript.h"
#include "CGravityScript.h"
#include "CGruntScript.h"
#include "CGruntSlashScript.h"
#include "CJumpCloudScript.h"
#include "CLandCloudScript.h"
#include "CMainCameraScript.h"
#include "CMissileScript.h"
#include "CMonsterScript.h"
#include "CMouseScript.h"
#include "CPlayerScript.h"
#include "CSlashScript.h"
#include "CViewScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CAttackRangeScript");
	_vec.push_back(L"CBloodAnimation2Script");
	_vec.push_back(L"CBloodAnimationScript");
	_vec.push_back(L"CBloodSplaterScript");
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CDustCloudScript");
	_vec.push_back(L"CEnemy_FollowScript");
	_vec.push_back(L"CGravityScript");
	_vec.push_back(L"CGruntScript");
	_vec.push_back(L"CGruntSlashScript");
	_vec.push_back(L"CJumpCloudScript");
	_vec.push_back(L"CLandCloudScript");
	_vec.push_back(L"CMainCameraScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CMouseScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CSlashScript");
	_vec.push_back(L"CViewScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CAttackRangeScript" == _strScriptName)
		return new CAttackRangeScript;
	if (L"CBloodAnimation2Script" == _strScriptName)
		return new CBloodAnimation2Script;
	if (L"CBloodAnimationScript" == _strScriptName)
		return new CBloodAnimationScript;
	if (L"CBloodSplaterScript" == _strScriptName)
		return new CBloodSplaterScript;
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CDustCloudScript" == _strScriptName)
		return new CDustCloudScript;
	if (L"CEnemy_FollowScript" == _strScriptName)
		return new CEnemy_FollowScript;
	if (L"CGravityScript" == _strScriptName)
		return new CGravityScript;
	if (L"CGruntScript" == _strScriptName)
		return new CGruntScript;
	if (L"CGruntSlashScript" == _strScriptName)
		return new CGruntSlashScript;
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
	if (L"CViewScript" == _strScriptName)
		return new CViewScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::ATTACKRANGESCRIPT:
		return new CAttackRangeScript;
		break;
	case (UINT)SCRIPT_TYPE::BLOODANIMATION2SCRIPT:
		return new CBloodAnimation2Script;
		break;
	case (UINT)SCRIPT_TYPE::BLOODANIMATIONSCRIPT:
		return new CBloodAnimationScript;
		break;
	case (UINT)SCRIPT_TYPE::BLOODSPLATERSCRIPT:
		return new CBloodSplaterScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::DUSTCLOUDSCRIPT:
		return new CDustCloudScript;
		break;
	case (UINT)SCRIPT_TYPE::ENEMY_FOLLOWSCRIPT:
		return new CEnemy_FollowScript;
		break;
	case (UINT)SCRIPT_TYPE::GRAVITYSCRIPT:
		return new CGravityScript;
		break;
	case (UINT)SCRIPT_TYPE::GRUNTSCRIPT:
		return new CGruntScript;
		break;
	case (UINT)SCRIPT_TYPE::GRUNTSLASHSCRIPT:
		return new CGruntSlashScript;
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
	case (UINT)SCRIPT_TYPE::VIEWSCRIPT:
		return new CViewScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::ATTACKRANGESCRIPT:
		return L"CAttackRangeScript";
		break;

	case SCRIPT_TYPE::BLOODANIMATION2SCRIPT:
		return L"CBloodAnimation2Script";
		break;

	case SCRIPT_TYPE::BLOODANIMATIONSCRIPT:
		return L"CBloodAnimationScript";
		break;

	case SCRIPT_TYPE::BLOODSPLATERSCRIPT:
		return L"CBloodSplaterScript";
		break;

	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
		break;

	case SCRIPT_TYPE::DUSTCLOUDSCRIPT:
		return L"CDustCloudScript";
		break;

	case SCRIPT_TYPE::ENEMY_FOLLOWSCRIPT:
		return L"CEnemy_FollowScript";
		break;

	case SCRIPT_TYPE::GRAVITYSCRIPT:
		return L"CGravityScript";
		break;

	case SCRIPT_TYPE::GRUNTSCRIPT:
		return L"CGruntScript";
		break;

	case SCRIPT_TYPE::GRUNTSLASHSCRIPT:
		return L"CGruntSlashScript";
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

	case SCRIPT_TYPE::VIEWSCRIPT:
		return L"CViewScript";
		break;

	}
	return nullptr;
}